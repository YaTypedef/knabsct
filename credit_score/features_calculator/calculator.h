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
    void CalculateFactors(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void CalculateFactorsAlternative(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;

private:
    double CalculateAccountsCount(const std::vector<TAccount>& accounts) const;
    
    double CalculateBureauCdCount(const std::vector<TAccount>& accounts, size_t bureauCd) const;

    double CalculateContractTypeCount(const std::vector<TAccount>& accounts, EContractType type) const;
    double CalculateContractStatusCount(const std::vector<TAccount>& accounts, EContractStatus status) const;

    double CalculateNotRUB(const std::vector<TAccount>& accounts) const;

    void AppendInTimeReturns(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    
    void AppendNumericMeans(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericDispersions(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericMaximums(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericMinimums(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericUnknownCount(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericSums(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericMedians(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendNumericLogs(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendZerosCount(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
    void AppendDifferentValuesCount(const std::vector<TAccount>& accounts, std::vector<double>* factors) const;
};