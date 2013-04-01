#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "data.h"
#include "common.h"

using std::ifstream;
using std::vector;
using std::string;
using std::stringstream;
using std::getline;
using std::cerr;
using std::endl;

TDate::TDate() 
    : IsUnknown(true)
{
}

TDate::TDate(const std::string& str) {
    if (str.size() != 9 ||
        !(str[0] >= '0' && str[0] <= '9') ||
        !(str[1] >= '0' && str[1] <= '9') ||
        !(str[2] >= 'A' && str[2] <= 'Z') ||
        !(str[3] >= 'a' && str[3] <= 'z') ||
        !(str[4] >= 'a' && str[4] <= 'z') ||
        !(str[5] >= '0' && str[5] <= '9') ||
        !(str[6] >= '0' && str[6] <= '9') ||
        !(str[7] >= '0' && str[7] <= '9') ||
        !(str[8] >= '0' && str[8] <= '9')) {
        IsUnknown = true;
        
        if (!str.empty() && str != "-1") {
            cerr << "Unknown date: " << str << endl;
        }
        return;
    }

    Day = atoi(str.substr(0, 2).c_str());

    string month = str.substr(2, 3);
    if (month == "Jan") {
        Month = 1;
    } else if (month == "Feb") {
        Month = 2;
    } else if (month == "Mar") {
        Month = 3;
    } else if (month == "Apr") {
        Month = 4;
    } else if (month == "May") {
        Month = 5;
    } else if (month == "Jun") {
        Month = 6;
    } else if (month == "Jul") {
        Month = 7;
    } else if (month == "Aug") {
        Month = 8;
    } else if (month == "Sep") {
        Month = 9;
    } else if (month == "Oct") {
        Month = 10;
    } else if (month == "Nov") {
        Month = 11;
    } else if (month == "Dec") {
        Month = 12;
    } else {
        cerr << "Unknown date: " << str << endl;
        IsUnknown = true;
        return;
    }

    Year = atoi(str.substr(5, 4).c_str());
}


TAccount::TAccount() {
}

TAccount::TAccount(const string& str) {
    vector<string> items;
    SplitString(str, ';', &items);

    if (items.size() != INPUT_FIELDS_COUNT) {
        cerr << "Wrong input line: " << str << endl;
        return;
    }
    for (size_t index = 0; index < items.size(); ++index) {
        if (items[index].empty()) {
            items[index] = "-1";
        }
    }

    Id = atoi(items[0].c_str());
    BureauCd = atoi(items[1].c_str());

    BkiRequestDate = TDate(items[2]);
    InfConfirmDate = TDate(items[3]);
    
    ContractType = ContractTypeFromString(items[4]);
    ContractStatus = ContractStatusFromString(items[5]);

    OpenDate = TDate(items[6]);
    FinalPaymentDate = TDate(items[7]);
    FactCloseDate = TDate(items[8]);

    CreditLimit = atof(items[9].c_str());
    Currency = CurrencyFromString(items[10]);

    Outstanding = atof(items[11].c_str()); 
    NextPayment = atof(items[12].c_str());
    CurrentBalanceAmount = atof(items[13].c_str());

    CurrentDelq = atoi(items[14].c_str());
    TtlDelq_5 = atoi(items[15].c_str());
    TtlDelq_5_29 = atoi(items[16].c_str());
    TtlDelq_30 = atoi(items[17].c_str());
    TtlDelq_30_59 = atoi(items[18].c_str());
    TtlDelq_60_89 = atoi(items[19].c_str());
    TtlDelq_90_Plus = atoi(items[20].c_str());

    PaymentStringStart = TDate(items[21]);
    PaymentString_84M = items[22];

    DelqBalance = atof(items[23].c_str());
    MaxDelqBalance = atof(items[24].c_str());
    InterestRate = atof(items[25].c_str());

    PaymentFrequency = PaymentFrequencyFromString(items[26]);
    Relationship = RelationshipFromString(items[27]);

    NumericFields.push_back(BureauCd);
    NumericFields.push_back(CreditLimit);
    NumericFields.push_back(Outstanding);
    NumericFields.push_back(NextPayment);
    NumericFields.push_back(CurrentBalanceAmount);
    NumericFields.push_back(CurrentDelq);
    NumericFields.push_back(TtlDelq_5);
    NumericFields.push_back(TtlDelq_5_29);
    NumericFields.push_back(TtlDelq_30);
    NumericFields.push_back(TtlDelq_30_59);
    NumericFields.push_back(TtlDelq_60_89);
    NumericFields.push_back(TtlDelq_90_Plus);
    NumericFields.push_back(DelqBalance);
    NumericFields.push_back(MaxDelqBalance);
    NumericFields.push_back(InterestRate);
}

EContractType TAccount::ContractTypeFromString(const std::string& str) const {
    if (str == "") {
        return CT_UNKNOWN;
    }

    return static_cast<EContractType>(atoi(str.c_str()));
}

EContractStatus TAccount::ContractStatusFromString(const std::string& str) const {
    if (str == "") {
        return CS_UNKNOWN;
    }

    return static_cast<EContractStatus>(atoi(str.c_str()));
}

ECurrency TAccount::CurrencyFromString(const std::string& str) const {
    if (str == "RUB") {
        return C_RUB;
    }
    if (str == "USD") {
        return C_USD;
    }
    return C_UNKNOWN;
}

EPaymentFrequency TAccount::PaymentFrequencyFromString(const std::string& str) const {
    if (str == "1") {
        return PF_DAY;
    }
    if (str == "2") {
        return PF_TWO_WEEKS;
    }
    if (str == "3") {
        return PF_MONTH;
    }
    if (str == "A") {
        return PF_TWO_MONTHS;
    }
    if (str == "4") {
        return PF_THREE_MONTHS;
    }
    if (str == "B") {
        return PF_FOUR_MONTHS;
    }
    if (str == "5") {
        return PF_SIX_MONTHS;
    }
    if (str == "6") {
        return PF_YEAR;
    }
    return PF_OTHER;
}

ERelationship TAccount::RelationshipFromString(const std::string& str) const {
    if (str == "") {
        return R_UNKNOWN;
    }

    return static_cast<ERelationship>(atoi(str.c_str()));
}

void TData::LoadFromCSVFiles(const std::string& accountsFile, const std::string& customersFile) {
    ifstream accountsIn(accountsFile.c_str());

    string line;
    getline(accountsIn, line); // skip first line
    while (getline(accountsIn, line)) {
        TAccount account(line);
        Data[account.Id].push_back(account);
    }

    ifstream customersIn(customersFile.c_str());
    getline(customersIn, line); // skip first line
    while (getline(customersIn, line)) {
        vector<string> items;
        SplitString(line, ';', &items);
        
        if (items.size() != 3) {
            cerr << "Wrong input line: " << line << endl;
            continue;
        }
        float id = atoi(items[0].c_str());
        if (items[2] == "train") {
            float target = atoi(items[1].c_str());
            LearnTargets[id] = target;
        } else if (items[2] == "test") {
            TestTargets[id] = 0.0;
        } else {
            cerr << "Wrong input line: " << line << endl;
            continue;
        }
    }
}