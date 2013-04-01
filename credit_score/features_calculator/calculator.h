#pragma once

#include <vector>

#include "data.h"

enum EPoolType {
    PT_LEARN,
    PT_TEST
};

class TFactorsCalculator {
public:
    void PrintPool(const TData& data, EPoolType poolType) const;

public:
    void CalculateFactors(const std::vector<TAccount>& accounts, std::vector<float>* factors) const;

private:
    float CalculateAccountsCount(const std::vector<TAccount>& accounts) const;

    float CalculateContractTypeCount(const std::vector<TAccount>& accounts, EContractType type) const;
    float CalculateContractStatusCount(const std::vector<TAccount>& accounts, EContractStatus status) const;

    float CalculateHasUSD(const std::vector<TAccount>& accounts) const;
    
    void AppendNumericMeans(const std::vector<TAccount>& accounts, std::vector<float>* factors) const;
    void AppendNumericDispersions(const std::vector<TAccount>& accounts, std::vector<float>* factors) const;
    void AppendNumericMaximums(const std::vector<TAccount>& accounts, std::vector<float>* factors) const;
    void AppendNumericMinimums(const std::vector<TAccount>& accounts, std::vector<float>* factors) const;
};