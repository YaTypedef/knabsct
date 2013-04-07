#include "stat_calculator.h"
#include "common.h"

#include <string>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using std::endl;
using std::string;
using std::vector;

namespace NMerchants {

typedef std::unordered_map<string, size_t> TStringDistribution;
typedef std::unordered_map<size_t, size_t> TNumberDistribution;
typedef std::unordered_set<size_t> TClientSet;

const size_t OVERALL_MCC_CODE = 11111;
const TMccDescriptionTable EMPTY_DESCRIPTION_TABLE = TMccDescriptionTable();

TMccDescriptionTable ReadMccDescriptionTableFromFile(const std::string& filename) {
    std::ifstream ifs(filename);
    
    TMccDescriptionTable descriptionTable;
    string line;
    vector<string> fields;
    while (std::getline(ifs, line)) {
        SplitString(line, '\t', &fields);
        descriptionTable[atoi(fields[0].c_str())] = fields[1];
    }

    return descriptionTable;
}

template <class T>
class TDistributionComparator {
public:
    bool operator ()(const std::pair<T, size_t>& first, const std::pair<T, size_t>& second) {
        return first.second > second.second;
    }
};

template <class T>
void PrintDistributionToStream(const std::unordered_map<T, size_t>& distribution, std::ostream& os) {
    std::vector<std::pair<T, size_t>> sortedDistribution(distribution.begin(), distribution.end());
    std::sort(sortedDistribution.begin(), sortedDistribution.end(), TDistributionComparator<T>());

    for (std::vector<std::pair<T, size_t>>::const_iterator it = sortedDistribution.begin();
         it != sortedDistribution.end(); ++ it) {
        os << it->first << "\t";
    }
    os << std::endl;

    for (std::vector<std::pair<T, size_t>>::const_iterator it = sortedDistribution.begin();
         it != sortedDistribution.end(); ++ it) {
        os << it->second << "\t";
    }
    os << std::endl;
}

//void IMerchantStatCalculator::Initialize() {
//}

///////////////////////////////////////////////////////////////////////////////
// Merchant Category Code Calculator

//struct TMccStat {
//    size_t MerchantCategoryCode;
//    size_t TransactionCount;
//    size_t OverallTransactionAmount;
//    size_t MaximumTransactionAmount;
//    size_t MinimumTransactionAmount;
//
//    TMccStat()
//    {}
//
//    TMccStat(size_t merchantCategoryCode)
//        : MerchantCategoryCode(merchantCategoryCode)
//        , TransactionCount(0)
//        , OverallTransactionAmount(0)
//        , MaximumTransactionAmount(0)
//        , MinimumTransactionAmount(UINT_MAX)
//    {}
//};

struct TTransactionStat {
    size_t Count;
    size_t OnlineCount;
    size_t OverallAmount;
    size_t MaximumAmount;
    size_t MinimumAmount;
    TStringDistribution CountryDistribution;
    TStringDistribution CityDistribution;

    TTransactionStat() 
        : Count(0)
        , OnlineCount(0)
        , OverallAmount(0)
        , MaximumAmount(0)
        , MinimumAmount(UINT_MAX)
    {}
};

struct TClientStat {
    size_t Count;
    size_t FemaleCount;
    size_t MaleCount;
    TStringDistribution RegionDistribution;
    TNumberDistribution AgeDistribution;

    TClientStat()
        : Count(0)
        , FemaleCount(0)
        , MaleCount(0)
    {}
};

class TMccCalculator : public IMerchantStatCalculator {
public:
    TMccCalculator(size_t mccCode, const TMccDescriptionTable& mccDescriptionTable);
    
    void AddRecord(const TTransaction& transaction);
    void PrintShortToStream(std::ostream& os) const;
    void PrintLongToStream(std::ostream& os) const;
    size_t GetComparisonValue() const;
    //void PrintClientStatToStream(std::ostream& os) const;
    
    virtual std::string GetMccDescription() const;

private:
    void CalcTransactionStat(const TTransaction& transaction);
    void CalcClientStat(const TTransaction& transaction);

private:
    const TMccDescriptionTable& DescriptionTable;
    size_t MccCode;
    TTransactionStat TransactionStat;
    TClientSet ClientSet;
    TClientStat ClientStat;
};

TMccCalculator::TMccCalculator(size_t mccCode, const TMccDescriptionTable& mccDescriptionTable) 
    : MccCode(mccCode)
    , DescriptionTable(mccDescriptionTable)
{}

void TMccCalculator::AddRecord(const TTransaction& transaction) {
    CalcTransactionStat(transaction);
    CalcClientStat(transaction);
}

void TMccCalculator::CalcTransactionStat(const TTransaction& transaction) {
    TransactionStat.Count += 1;
    if (transaction.IsOnline) {
        TransactionStat.OnlineCount += 1;
    }

    TransactionStat.OverallAmount += transaction.Amount;
    if (transaction.Amount > TransactionStat.MaximumAmount) {
        TransactionStat.MaximumAmount = transaction.Amount;
    }
    if (transaction.Amount < TransactionStat.MinimumAmount) {
        TransactionStat.MinimumAmount = transaction.Amount;
    }

    TransactionStat.CountryDistribution[transaction.CountryCode] += 1;
    TransactionStat.CityDistribution[transaction.City] += 1;
}

void TMccCalculator::CalcClientStat(const TTransaction& transaction) {
    if (ClientSet.find(transaction.ClientId) != ClientSet.end()) {
        return;
    }

    ClientSet.insert(transaction.ClientId);
    ClientStat.Count += 1;
    if (transaction.ClientGender == G_Female) {
        ClientStat.FemaleCount += 1;
    } else {
        ClientStat.MaleCount += 1;
    }
    ClientStat.AgeDistribution[transaction.ClientAge] += 1;
    ClientStat.RegionDistribution[transaction.ClientRegion] += 1;
}

void TMccCalculator::PrintShortToStream(std::ostream& os) const {
    os << MccCode << "\t" 
        << TransactionStat.Count << "\t" << TransactionStat.OverallAmount << "\t"
        << static_cast<double>(TransactionStat.OverallAmount) / TransactionStat.Count << "\t"
        << TransactionStat.MinimumAmount << "\t" << TransactionStat.MaximumAmount << "\t"
        << static_cast<double>(TransactionStat.OnlineCount) / TransactionStat.Count << "\t"
        << GetMccDescription() << std::endl;
}

void TMccCalculator::PrintLongToStream(std::ostream& os) const {
    os << "MCC" << "\t" << MccCode << "\t" << GetMccDescription() << std::endl;

    os << "Transaction country distribution" << std::endl;
    PrintDistributionToStream(TransactionStat.CountryDistribution, os);

    os << "Transaction city distribution" << std::endl;
    PrintDistributionToStream(TransactionStat.CityDistribution, os);

    os << "Clients gender distribution" << std::endl;
    os << "Count" << "\t" << "Male count" << "\t" << "Female count" << std::endl;
    os << ClientStat.Count << "\t" << ClientStat.MaleCount << "\t" << ClientStat.FemaleCount << std::endl;

    os << "Client age distribution" << std::endl;
    PrintDistributionToStream(ClientStat.AgeDistribution, os);

    os << "Client region distribution" << std::endl;
    PrintDistributionToStream(ClientStat.RegionDistribution, os);
}

std::string TMccCalculator::GetMccDescription() const {
    TMccDescriptionTable::const_iterator descriptionIt = DescriptionTable.find(MccCode);
    return descriptionIt != DescriptionTable.end()
        ? descriptionIt->second
        : "-----";
}

size_t TMccCalculator::GetComparisonValue() const {
    return TransactionStat.Count;
}


class TOverallMccCalculator : public TMccCalculator {
public:
    TOverallMccCalculator();
    std::string GetMccDescription() const;
};

TOverallMccCalculator::TOverallMccCalculator()
    : TMccCalculator(OVERALL_MCC_CODE, EMPTY_DESCRIPTION_TABLE)
{}

std::string TOverallMccCalculator::GetMccDescription() const {
    return "Overall";
}




//typedef std::unordered_map<size_t, TMccStat> TMccStatDistribution;
//
//class TMccDistributionCalculator : public IMerchantStatCalculator {
//public:
//    TMccDistributionCalculator(const TMccDescriptionTable& mccDescriptionTable);
//    void AddRecord(const TTransaction& transaction);
//    void PrintToStream(std::ostream& os);
//private:
//    TMccStatDistribution StatDistribution;
//    const TMccDescriptionTable& DescriptionTable;
//};
//
//TMccDistributionCalculator::TMccDistributionCalculator(const TMccDescriptionTable& mccDescriptionTable)
//    : DescriptionTable(mccDescriptionTable)
//{}
//
//void TMccDistributionCalculator::AddRecord(const TTransaction& transaction) {
//    TMccStatDistribution::iterator it = StatDistribution.find(transaction.MerchantCategoryCode);
//    if (it == StatDistribution.end()) {
//        StatDistribution.insert(
//            std::make_pair(transaction.MerchantCategoryCode, TMccStat(transaction.MerchantCategoryCode)));
//    }
//    TMccStat& stat = StatDistribution[transaction.MerchantCategoryCode];
//    stat.TransactionCount += 1;
//    stat.OverallTransactionAmount += transaction.Amount;
//    if (transaction.Amount > stat.MaximumTransactionAmount) {
//        stat.MaximumTransactionAmount = transaction.Amount;
//    }
//    if (transaction.Amount < stat.MinimumTransactionAmount) {
//        stat.MinimumTransactionAmount = transaction.Amount;
//    }
//}
//
//class TMccStatAmountComparator {
//public:
//    bool operator ()(const TMccStat& first, const TMccStat& second) {
//        return first.OverallTransactionAmount > second.OverallTransactionAmount;
//    }
//};
//
//void TMccDistributionCalculator::PrintToStream(std::ostream& os) {
//    os << "MCC" << "\t" << "Count" << "\t" << "OverallAmount" 
//        << "\t" << "AverageAmount" << "\t" << "MinAmount" << "\t" << "MaxAmount" 
//        << "\t" << "Description" << std::endl;
//    vector<TMccStat> statSortedByAmount;
//    for (TMccStatDistribution::const_iterator it = StatDistribution.begin();
//         it != StatDistribution.end(); ++it) {
//        statSortedByAmount.push_back(it->second);
//    }
//    std::sort(statSortedByAmount.begin(), statSortedByAmount.end(), TMccStatAmountComparator());
//
//    for (vector<TMccStat>::const_iterator mccStatIt = statSortedByAmount.begin();
//         mccStatIt != statSortedByAmount.end(); ++mccStatIt) {
//        const TMccStat& stat = *mccStatIt;
//        os << stat.MerchantCategoryCode << "\t"
//            << stat.TransactionCount << "\t" << stat.OverallTransactionAmount << "\t"
//            << static_cast<double>(stat.OverallTransactionAmount) / stat.TransactionCount << "\t"
//            << stat.MinimumTransactionAmount << "\t" << stat.MaximumTransactionAmount << "\t";
//        TMccDescriptionTable::const_iterator descriptionIt = DescriptionTable.find(stat.MerchantCategoryCode);
//        if (descriptionIt != DescriptionTable.end()) {
//            os << descriptionIt->second;
//        } else {
//            os << "-----";
//        }
//        os << endl;
//    }
//}

///////////////////////////////////////////////////////////////////////////////
// Transaction distribution calculators

//class TAmountDistributionCalculator : public IMerchantStatCalculator {
//};
//
//class TCountryDistribuionCalculator : public IMerchantStatCalculator {
//};

///////////////////////////////////////////////////////////////////////////////
// Transaction distribution calculators

//class TClientAgeDistributionCalculator : public IMerchantStatCalculator {
//};
//
//class TClientGenderDistributionCalculator : public IMerchantStatCalculator {
//};
//
//class TTransactionStatCalculator : public IMerchantStatCalculator {
//public:
//    void AddRecord(const TTransaction& transaction);
//    void PrintToStream(std::ostream& os);
//private:
//};
//
//class TClientStatCalculator : public IMerchantStatCalculator {
//};

///////////////////////////////////////////////////////////////////////////////
// Bundle

TMerchantStatBundle::TMerchantStatBundle(const TMccDescriptionTable& mccDescriptionTable)
    : MccDescriptionTable(mccDescriptionTable)
    , OverallCalculator(new TOverallMccCalculator)
{
    //Calculators.push_back(TCalculatorPtr(new TMccDistributionCalculator(MccDescriptionTable)));
}

//void TMerchantStatBundle::Initialize() {
    //for (vector<TCalculatorPtr>::iterator it = Calculators.begin(); it != Calculators.end(); ++it) {
    //    (*it)->Initialize();
    //}
//}

void TMerchantStatBundle::AddRecord(const TTransaction& transaction) {
    size_t mcc = transaction.MerchantCategoryCode;
    if (Calculators.find(mcc) == Calculators.end()) {
        Calculators.insert(std::make_pair(mcc, new TMccCalculator(mcc, MccDescriptionTable)));
    }
    Calculators[mcc]->AddRecord(transaction);
    OverallCalculator->AddRecord(transaction);
}

void PrintShortHeader(std::ostream& os) {
    os << "MCC" << "\t" << "Count" << "\t" << "OverallAmount" 
        << "\t" << "AverageAmount" << "\t" << "MinAmount" << "\t" << "MaxAmount" 
        << "\t" << "OnlinePercent"
        << "\t" << "Description" << std::endl;
}

class TCaclulatorComparer {
public:
    bool operator ()(const TCalculatorPtr& first, const TCalculatorPtr& second) {
        return first->GetComparisonValue() > second->GetComparisonValue();
    }
};

void TMerchantStatBundle::PrintToStream(std::ostream& os) const {
    PrintShortHeader(os);

    vector<TCalculatorPtr> sortedCalculators;
    for (std::unordered_map<size_t, TCalculatorPtr>::const_iterator it = Calculators.begin(); 
         it != Calculators.end(); ++it) {
        sortedCalculators.push_back(it->second);
    }
    std::sort(sortedCalculators.begin(), sortedCalculators.end(), TCaclulatorComparer());
    
    OverallCalculator->PrintShortToStream(os);
    for (vector<TCalculatorPtr>::const_iterator calculatorIt = sortedCalculators.begin();
         calculatorIt != sortedCalculators.end(); ++calculatorIt) {
        (*calculatorIt)->PrintShortToStream(os);
    }
    os << std::endl;

    OverallCalculator->PrintLongToStream(os);
    os << std::endl;
    for (vector<TCalculatorPtr>::const_iterator calculatorIt = sortedCalculators.begin();
         calculatorIt != sortedCalculators.end(); ++calculatorIt) {
        (*calculatorIt)->PrintLongToStream(os);
        os << std::endl;
    }
    os << std::endl;
}

}