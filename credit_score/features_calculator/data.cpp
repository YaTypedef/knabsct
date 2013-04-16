#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

#include "data.h"
#include "common.h"

using std::ifstream;
using std::vector;
using std::string;
using std::stringstream;
using std::getline;
using std::reverse;
using std::count;
using std::cerr;
using std::endl;

TDate::TDate() 
    : IsUnknown(true)
{
}

TDate::TDate(const std::string& str) {
    IsUnknown = false;
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
        
        if (!str.empty() && str != "-1000000") {
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

size_t  TDate::AsNumber() const {
    if (IsUnknown) {
        return 0;
    }
    size_t answer = Day;

    if (Month == 2) {
        answer += 31;
    }
    if (Month == 3) {
        answer += 28;
    }
    if (Month == 4) {
        answer += 31;
    }
    if (Month == 5) {
        answer += 30;
    }
    if (Month == 6) {
        answer += 31;
    }
    if (Month == 7) {
        answer += 30;
    }
    if (Month == 8) {
        answer += 31;
    }
    if (Month == 9) {
        answer += 31;
    }
    if (Month == 10) {
        answer += 30;
    }
    if (Month == 11) {
        answer += 31;
    }
    if (Month == 12) {
        answer += 30;
    }
    answer += Year * 365; // Забиваем на високосные
    return answer; 
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

	UnknownFieldsCount = 0;
    for (size_t index = 0; index < items.size(); ++index) {
        if (items[index].empty()) {
            items[index] = "-1000000";
			++UnknownFieldsCount;
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

    // Дополнительные поля
    ContractDuration = FinalPaymentDate.AsNumber() - OpenDate.AsNumber();
    ContractFactDuration = FactCloseDate.AsNumber() - OpenDate.AsNumber();
    PaymentStringSize = PaymentString_84M.size();
    PaymentString0Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '0');
    PaymentString1Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '1');
    PaymentString2Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '2');
    PaymentString3Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '3');
    PaymentString4Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '4');
    PaymentString5Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '5');
    PaymentString7Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '7');
    PaymentString8Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '8');
    PaymentString9Count = count(PaymentString_84M.begin(), PaymentString_84M.end(), '9');
    PaymentStringACount = count(PaymentString_84M.begin(), PaymentString_84M.end(), 'A');
    PaymentStringXCount = count(PaymentString_84M.begin(), PaymentString_84M.end(), 'X');

    PaymentString0Pos = 0;
    PaymentString1Pos = 0;
    PaymentString2Pos = 0;
    PaymentString3Pos = 0;
    PaymentString4Pos = 0;
    PaymentString5Pos = 0;
    PaymentString7Pos = 0;
    PaymentString8Pos = 0;
    PaymentString9Pos = 0;
    PaymentStringAPos = 0;
    PaymentStringXPos = 0;
    for (size_t i = 0; i < PaymentString_84M.size(); ++i) {
        char c = PaymentString_84M[i];
        if (c == '0') {
            PaymentString0Pos += i / (1.0f + PaymentString0Count);
        } else if (c == '1') {
            PaymentString1Pos += i / (1.0f + PaymentString1Count);
        } else if (c == '2') {
            PaymentString2Pos += i / (1.0f + PaymentString2Count);
        } else if (c == '3') {
            PaymentString3Pos += i / (1.0f + PaymentString3Count);
        } else if (c == '4') {
            PaymentString4Pos += i / (1.0f + PaymentString4Count);
        } else if (c == '5') {
            PaymentString5Pos += i / (1.0f + PaymentString5Count);
        } else if (c == '7') {
            PaymentString7Pos += i / (1.0f + PaymentString7Count);
        } else if (c == '8') {
            PaymentString8Pos += i / (1.0f + PaymentString8Count);
        } else if (c == '9') {
            PaymentString9Pos += i / (1.0f + PaymentString9Count);
        } else if (c == 'A') {
            PaymentStringAPos += i / (1.0f + PaymentStringACount);
        } else if (c == 'X') {
            PaymentStringXPos += i / (1.0f + PaymentStringXCount);
        }
    }

    const static double EPS = 0.000001;

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
    NumericFields.push_back(PaymentStringSize);
    NumericFields.push_back(PaymentString0Count);
    NumericFields.push_back(PaymentString1Count);
    NumericFields.push_back(PaymentString2Count);
    NumericFields.push_back(PaymentString3Count);
    NumericFields.push_back(PaymentString4Count);
    NumericFields.push_back(PaymentString5Count);
    NumericFields.push_back(PaymentString7Count);
    NumericFields.push_back(PaymentString8Count);
    NumericFields.push_back(PaymentString9Count);
    NumericFields.push_back(PaymentStringACount);
    NumericFields.push_back(PaymentStringXCount);

    NumericFields.push_back(PaymentString0Pos);
    NumericFields.push_back(PaymentString1Pos);
    NumericFields.push_back(PaymentString2Pos);
    NumericFields.push_back(PaymentString3Pos);
    NumericFields.push_back(PaymentString4Pos);
    NumericFields.push_back(PaymentString5Pos);
    NumericFields.push_back(PaymentString7Pos);
    NumericFields.push_back(PaymentString8Pos);
    NumericFields.push_back(PaymentString9Pos);
    NumericFields.push_back(PaymentStringAPos);
    NumericFields.push_back(PaymentStringXPos);

    NumericFields.push_back(BkiRequestDate.AsNumber());
    NumericFields.push_back(InfConfirmDate.AsNumber());
    NumericFields.push_back(OpenDate.AsNumber());
    NumericFields.push_back(FinalPaymentDate.AsNumber());
    NumericFields.push_back(FactCloseDate.AsNumber());
    NumericFields.push_back(PaymentStringStart.AsNumber());

    NumericFields.push_back(ContractDuration);
    NumericFields.push_back(ContractFactDuration);

    // string covering
    double days = 1.0f;
    if (PaymentFrequency == PF_TWO_WEEKS) {
        days = 14;
    } else if (PaymentFrequency == PF_MONTH) {
        days = 30;
    } else if (PaymentFrequency == PF_TWO_MONTHS) {
        days = 60;
    } else if (PaymentFrequency == PF_THREE_MONTHS) {
        days = 90;
    } else if (PaymentFrequency == PF_FOUR_MONTHS) {
        days = 120;
    } else if (PaymentFrequency == PF_SIX_MONTHS) {
        days = 185;
    } else if (PaymentFrequency == PF_YEAR) {
        days = 365;
    }
    NumericFields.push_back(PaymentString_84M.size() * days / (1.0 + FactCloseDate.AsNumber() - OpenDate.AsNumber()));
	NumericFields.push_back(UnknownFieldsCount);
	//NumericFields.push_back(NextPayment / (1.0 + Outstanding));
	//NumericFields.push_back(DelqBalance / (1.0 + CurrentBalanceAmount));
	//NumericFields.push_back(CreditLimit / (1.0 + FinalPaymentDate.AsNumber() - OpenDate.AsNumber()));
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
        double id = atoi(items[0].c_str());

        if (items[2] == "train") {
            double target = atoi(items[1].c_str());
            LearnTargets[id] = target;
        } else if (items[2] == "test") {
            TestTargets[id] = 0.0;
        } else {
            cerr << "Wrong input line: " << line << endl;
            continue;
        }
    }
}