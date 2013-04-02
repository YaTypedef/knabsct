#include <vector>
#include <iostream>
#include <cmath>

#include "calculator.h"

using std::vector;
using std::cerr;
using std::cout;
using std::endl;

const float EPS = 0.000001;

void TFactorsCalculator::CalculateFactors(
    const std::vector<TAccount>& accounts,
    std::vector<float>* factors) const
{
    factors->clear();

    factors->push_back(CalculateAccountsCount(accounts));
    
    factors->push_back(CalculateContractTypeCount(accounts, CT_UNKNOWN));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CAR));
    factors->push_back(CalculateContractTypeCount(accounts, CT_LEASING));
    factors->push_back(CalculateContractTypeCount(accounts, CT_HYPOTHEC));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CREDIT_CARD));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CONSUMER_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_BUSINESS_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_CIRCULATING_ASSESTS_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_EQUIPMENT_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_REALTY_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_STOCK_CREDIT));
    factors->push_back(CalculateContractTypeCount(accounts, CT_OTHER));

    factors->push_back(CalculateContractStatusCount(accounts, CS_UNKNOWN));
    factors->push_back(CalculateContractStatusCount(accounts, CS_ACTIVE));
    factors->push_back(CalculateContractStatusCount(accounts, CS_PAYED));
    factors->push_back(CalculateContractStatusCount(accounts, CS_CLOSED));
    factors->push_back(CalculateContractStatusCount(accounts, CS_SENT_TO_OTHER_BANK));
    factors->push_back(CalculateContractStatusCount(accounts, CS_CONTROVERSY));
    factors->push_back(CalculateContractStatusCount(accounts, CS_OVERDUE));
    factors->push_back(CalculateContractStatusCount(accounts, CS_PROBLEMS));

    factors->push_back(CalculateHasUSD(accounts));

    AppendNumericMinimums(accounts, factors);
    AppendNumericMaximums(accounts, factors);
    AppendNumericMeans(accounts, factors);
    AppendNumericDispersions(accounts, factors);
}


float TFactorsCalculator::CalculateAccountsCount(const std::vector<TAccount>& accounts) const {
    return accounts.size();
}

float TFactorsCalculator::CalculateContractTypeCount(
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

float TFactorsCalculator::CalculateContractStatusCount(
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

float TFactorsCalculator::CalculateHasUSD(const std::vector<TAccount>& accounts) const {
    for (size_t index = 0; index < accounts.size(); ++index) {
        if (accounts[index].Currency == C_USD) {
            return 1.0f;
        }
    }
    return 0.0f;
}

void TFactorsCalculator::AppendNumericMeans(
    const std::vector<TAccount>& accounts,
    std::vector<float>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        float sum = 0.0;
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
    std::vector<float>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        float sum = 0.0;
        float squaresSum = 0.0;
        size_t counter = 0;
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            float item = accounts[accountIndex].NumericFields[fieldIndex];
            if (item > -EPS) {
                sum += item;
                squaresSum += item * item;
                ++counter;
            }
        }
        sum /= (0.01 + counter);
        squaresSum /= (0.01 + counter);
        factors->push_back(pow(squaresSum - sum * sum, 0.5f));
    }
}

void TFactorsCalculator::AppendNumericMaximums(
    const std::vector<TAccount>& accounts,
    std::vector<float>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        float max = -1.0;
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
    std::vector<float>* factors) const
{
    if (accounts.empty()) {
        return;
    }

    for (size_t fieldIndex = 0;
         fieldIndex < accounts[0].NumericFields.size();
         ++fieldIndex)
    {
        float min = 100000; // sorry
        for (size_t accountIndex = 0;
             accountIndex < accounts.size();
             ++accountIndex) 
        {
            if (accounts[accountIndex].NumericFields[fieldIndex] < min &&
                accounts[accountIndex].NumericFields[fieldIndex] > -EPS)
            {
                min = accounts[accountIndex].NumericFields[fieldIndex];
            }
        }
        factors->push_back(min);
    }
}

void TFactorsCalculator::AppendNumericUnknownCount(
    const std::vector<TAccount>& accounts,
    std::vector<float>* factors) const
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

void TFactorsCalculator::PrintPool(const TData& data, EPoolType poolType) const {
    const std::unordered_map<size_t, float>& targets =
        (poolType == PT_LEARN) ? data.LearnTargets : data.TestTargets;

    for (std::unordered_map<size_t, float>::const_iterator it = targets.begin();
         it != targets.end();
         ++it)
    {
        size_t id = it->first;
        float target = it->second;

        vector<float> factors;
        TData::THashData::const_iterator dataIt = data.Data.find(id);
        if (dataIt == data.Data.end()) {
            cerr << "Wrong id: " << id << endl;
            return;
        }
        CalculateFactors(dataIt->second, &factors);

        cout << id << '\t' << target << "\tempty\t" << id;
        for (size_t index = 0; index < factors.size(); ++index) {
            cout << '\t' << factors[index];
        }
        cout << endl;
    }
}
