#pragma once

#include <string>
#include <vector>

namespace NMerchants {

enum EGender {
    G_Male,
    G_Female
};

struct TDate {
    size_t Day;
    size_t Month;
    size_t Year;

    bool IsUnknown;

public:
    TDate();
    TDate(const std::string& str);
};

std::ostream& operator <<(std::ostream& os, const TDate& date);

struct TTransaction {
    size_t MerchantId;
    size_t MerchantCategoryCode;

    size_t ClientId;
    std::string ClientRegion;
    EGender ClientGender;
    size_t ClientAge;

    size_t Amount;
    TDate Date;

    std::string CountryCode;
    std::string City;
    
    bool IsOnline;

public:
    TTransaction(const std::string& str);

    void DebugPrint() const;
};

typedef std::vector<TTransaction> TTransactions;

TTransactions ReadTransactionsFromFile(const std::string& filename);

}