#pragma once

#include "transaction.h"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace NMerchants {

typedef std::unordered_map<size_t, std::string> TMccDescriptionTable;

TMccDescriptionTable ReadMccDescriptionTableFromFile(const std::string& filename);

class IMerchantStatCalculator {
public:
    //virtual void Initialize();
    virtual void AddRecord(const TTransaction& transaction) = 0;
    virtual void PrintShortToStream(std::ostream& os) const = 0;
    virtual void PrintLongToStream(std::ostream& os) const = 0;
    virtual size_t GetComparisonValue() const = 0;
};

typedef std::shared_ptr<IMerchantStatCalculator> TCalculatorPtr;


class TMerchantStatBundle { //: public IMerchantStatCalculator {
public:
    TMerchantStatBundle(const TMccDescriptionTable& mccDescriptionTable);
    //void Initialize();
    void AddRecord(const TTransaction& transaction);
    void PrintToStream(std::ostream& os) const;
private:
    std::unordered_map<size_t, TCalculatorPtr> Calculators;
    TCalculatorPtr OverallCalculator;
    const TMccDescriptionTable& MccDescriptionTable;
};

const size_t MERCHANT_COUNT = 50;
const size_t FIRST_MERCHANT_INDEX = 1;

typedef std::vector<TMerchantStatBundle> TMerchantStats;

}