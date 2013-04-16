#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>

#include "calculator.h"

using std::ofstream;
using std::vector;
using std::set;
using std::cerr;
using std::cout;
using std::endl;
using std::sort;

const double EPS = 0.000001;
const double MAX = 100000000;

void TFactorsCalculator::CalculateFactors(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    factors->clear();

    factors->push_back(CalculateAccountsCount(accounts));
    
//    factors->push_back(CalculateBureauCdCount(accounts, 1));
//    factors->push_back(CalculateBureauCdCount(accounts, 2));
//    factors->push_back(CalculateBureauCdCount(accounts, 3));

    factors->push_back(CalculateContractTypeCount(accounts, CT_UNKNOWN) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CAR) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_LEASING) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_HYPOTHEC) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CREDIT_CARD) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CONSUMER_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_BUSINESS_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CIRCULATING_ASSESTS_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_EQUIPMENT_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_REALTY_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_STOCK_CREDIT) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractTypeCount(accounts, CT_OTHER) / (0.01 + accounts.size()));

    factors->push_back(CalculateContractStatusCount(accounts, CS_UNKNOWN) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_ACTIVE) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_PAYED) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_CLOSED) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_SENT_TO_OTHER_BANK) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_CONTROVERSY) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_OVERDUE) / (0.01 + accounts.size()));
    factors->push_back(CalculateContractStatusCount(accounts, CS_PROBLEMS) / (0.01 + accounts.size()));

    factors->push_back(CalculateNotRUB(accounts) / (0.01 + accounts.size()));

    AppendNumericMinimums(accounts, factors);
    AppendNumericMaximums(accounts, factors);
    AppendNumericMeans(accounts, factors);
//    AppendNumericDispersions(accounts, factors);
    AppendNumericUnknownCount(accounts, factors);
    AppendNumericSums(accounts, factors);
    AppendNumericMedians(accounts, factors);
    AppendNumericLogs(accounts, factors);
//    AppendZerosCount(accounts, factors);

    AppendInTimeReturns(accounts, factors);

//	AppendDifferentValuesCount(accounts, factors);
}

void TFactorsCalculator::AppendZerosCount(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        size_t counter = 0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS &&
                accounts[accountIndex].NumericFields[fieldIndex] < EPS)
            {
                ++counter;
            }
        }
        factors->push_back(counter);
    }
}


void TFactorsCalculator::AppendInTimeReturns(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    double beforeCounter = 0.0f;
    double inTimeCounter = 0.0f;
    double afterCounter = 0.0f;
    double unknownCounter = 0.0f;

    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].FinalPaymentDate.IsUnknown ||
            accounts[index].FactCloseDate.IsUnknown)
        {
            ++unknownCounter;
            continue;
        }

        size_t finalDate = accounts[index].FinalPaymentDate.AsNumber();
        size_t factDate = accounts[index].FactCloseDate.AsNumber();

        if (finalDate > factDate) {
            ++beforeCounter;
        } else if (finalDate == factDate) {
            ++inTimeCounter;
        } else {
            ++afterCounter;
        }
    }
    factors->push_back(beforeCounter / accounts.size());
    factors->push_back(inTimeCounter / accounts.size());
    factors->push_back(afterCounter / accounts.size());
    factors->push_back(unknownCounter / accounts.size());
}

void TFactorsCalculator::CalculateFactorsAlternative(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    factors->clear();

    for (size_t index = 0; index < accounts.size(); ++index) {
        const TAccount& account = accounts[index];
        
        factors->push_back(account.BureauCd);
        factors->push_back(account.BkiRequestDate.AsNumber());
        factors->push_back(account.InfConfirmDate.AsNumber());
        factors->push_back(static_cast<size_t>(account.ContractType));
        factors->push_back(static_cast<size_t>(account.ContractStatus));
        factors->push_back(account.OpenDate.AsNumber());
        factors->push_back(account.FinalPaymentDate.AsNumber());
        factors->push_back(account.FactCloseDate.AsNumber());
        factors->push_back(account.CreditLimit);
        factors->push_back(static_cast<size_t>(account.Currency));
        factors->push_back(account.Outstanding);
        factors->push_back(account.NextPayment);
        factors->push_back(account.CurrentBalanceAmount);
        factors->push_back(account.CurrentDelq);
        factors->push_back(account.TtlDelq_5);
        factors->push_back(account.TtlDelq_5_29);
        factors->push_back(account.TtlDelq_30_59);
        factors->push_back(account.TtlDelq_60_89);
        factors->push_back(account.TtlDelq_30);
        factors->push_back(account.TtlDelq_90_Plus);
        factors->push_back(account.PaymentStringStart.AsNumber());
        factors->push_back(account.DelqBalance);
        factors->push_back(account.MaxDelqBalance);
        factors->push_back(account.InterestRate);
        factors->push_back(static_cast<size_t>(account.PaymentFrequency));
        factors->push_back(static_cast<size_t>(account.Relationship));
        
        factors->push_back(account.ContractDuration);
        factors->push_back(account.ContractFactDuration);
        factors->push_back(account.PaymentString0Count);
        factors->push_back(account.PaymentString1Count);
        factors->push_back(account.PaymentString2Count);
        factors->push_back(account.PaymentString3Count);
        factors->push_back(account.PaymentString4Count);
        factors->push_back(account.PaymentString5Count);
        factors->push_back(account.PaymentString7Count);
        factors->push_back(account.PaymentString8Count);
        factors->push_back(account.PaymentString9Count);
        factors->push_back(account.PaymentStringACount);
        factors->push_back(account.PaymentStringXCount);
    }
    factors->resize(1000);
}



double TFactorsCalculator::CalculateAccountsCount(const std::vector<TAccount>& accounts) const {
    return accounts.size();
}

double TFactorsCalculator::CalculateBureauCdCount(
    const std::vector<TAccount>& accounts,
    size_t bureauCd) const
{
    size_t counter = 0;
    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].BureauCd == bureauCd) {
            ++counter;
        }
    }
    return counter;
}


double TFactorsCalculator::CalculateContractTypeCount(
    const std::vector<TAccount>& accounts,
    EContractType type) const
{
    size_t counter = 0;
    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].ContractType == type) {
            ++counter;
        }
    }
    return counter;
}

double TFactorsCalculator::CalculateContractStatusCount(
    const std::vector<TAccount>& accounts,
    EContractStatus status) const
{
    size_t counter = 0;
    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].ContractStatus == status) {
            ++counter;
        }
    }
    return counter;    
}

double TFactorsCalculator::CalculateNotRUB(const std::vector<TAccount>& accounts) const {
    size_t counter = 0;
    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].Currency != C_RUB) {
            ++counter;
        }
    }
    return counter;
}

void TFactorsCalculator::AppendDifferentValuesCount(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        vector<double> values;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS) {
				values.push_back(accounts[accountIndex].NumericFields[fieldIndex]);
            }
        }
		sort(values.begin(), values.end());
		
		size_t counter = 0;
		for (size_t i = 1; i < values.size(); ++i) {
			if (abs(values[i] - values[i - 1]) > EPS) {
				++counter;
			}
		}
        factors->push_back(counter);
    }
}

void TFactorsCalculator::AppendNumericMeans(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double sum = 0.0;
        size_t counter = 0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS) {
                sum += accounts[accountIndex].NumericFields[fieldIndex];
                ++counter;
            }
        }
        factors->push_back(sum / (0.01 + counter));
    }
}

void TFactorsCalculator::AppendNumericDispersions(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double sum = 0.0;
        double squaresSum = 0.0;
        size_t counter = 0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            double item = accounts[accountIndex].NumericFields[fieldIndex];
            if (item > -EPS) {
                sum += item;
                squaresSum += item * item;
                ++counter;
            }
        }
        sum /= (0.01 + counter);
        squaresSum /= (0.01 + counter);
        factors->push_back(pow(squaresSum - sum * sum, 0.5));
    }
}

void TFactorsCalculator::AppendNumericMaximums(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double max = -1.0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > max &&
                accounts[accountIndex].NumericFields[fieldIndex] > -EPS)
            {
                max = accounts[accountIndex].NumericFields[fieldIndex];
            }
        }
        factors->push_back(max);
    }
}

void TFactorsCalculator::AppendNumericMinimums(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double min = MAX;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] < min &&
                accounts[accountIndex].NumericFields[fieldIndex] > EPS)
            {
                min = accounts[accountIndex].NumericFields[fieldIndex];
            }
        }
        factors->push_back(min);
    }
}

void TFactorsCalculator::AppendNumericUnknownCount(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        size_t counter = 0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] < -EPS) {
                ++counter;
            }
        }
        factors->push_back(counter);
    }
}


void TFactorsCalculator::AppendNumericSums(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double sum = 0.0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS) {
                sum += accounts[accountIndex].NumericFields[fieldIndex];
            }
        }
        factors->push_back(sum);
    }
}


void TFactorsCalculator::AppendNumericMedians(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        vector<double> values;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS) {
                values.push_back(accounts[accountIndex].NumericFields[fieldIndex]);
            }
        }
        sort(values.begin(), values.end());
        size_t median = values.size() / 2;

        if (!values.empty()) { 
            factors->push_back(values[median]);
            factors->push_back(values[median / 2]);
            factors->push_back(values[3 * median / 2]);
        } else {
            factors->push_back(-1.0f);
            factors->push_back(-1.0f);
            factors->push_back(-1.0f);
        }
    }
}

void TFactorsCalculator::AppendNumericLogs(
    const std::vector<TAccount>& accounts,
    std::vector<double>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        double logsSum = 0.0f;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] > -EPS) {
                logsSum += log(1.0 + accounts[accountIndex].NumericFields[fieldIndex]);
            }
        }
        factors->push_back(logsSum);
    }
}

void TFactorsCalculator::PrintPool(const TData& data, EPoolType poolType) const {
    const std::unordered_map<size_t, double>& targets =
        (poolType == PT_LEARN) ? data.LearnTargets : data.TestTargets;
	const std::string& outputFileName = (poolType == PT_LEARN) ? "learn.txt" : "test.txt";

	std::ofstream ofs(outputFileName.c_str());

    for (std::unordered_map<size_t, double>::const_iterator it = targets.begin();
         it != targets.end();
         ++it)
    {
        size_t id = it->first;
        double target = it->second;

        vector<double> factors;
        TData::THashData::const_iterator dataIt = data.Data.find(id);
        if (dataIt == data.Data.end()) {
            cerr << "Wrong id: " << id << endl;
            return;
        }
        CalculateFactors(dataIt->second, &factors);

        ofs << id << '\t' << target << "\tempty\t" << id;
        for (size_t index = 0; index < factors.size(); ++index) {
            ofs << '\t' << factors[index];
        }
        ofs << endl;
    }
}
