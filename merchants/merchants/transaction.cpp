#include "transaction.h"
#include "common.h"

#include <iostream>
#include <sstream>
#include <fstream>

using std::cerr;
using std::endl;
using std::string;
using std::vector;

namespace NMerchants {

TDate::TDate()
{
}

TDate::TDate(const std::string& str) {
    if (str.size() != 7 ||
        !(str[0] >= '0' && str[0] <= '9') ||
        !(str[1] >= '0' && str[1] <= '9') ||
        !(str[2] >= 'A' && str[2] <= 'Z') ||
        !(str[3] >= 'A' && str[3] <= 'Z') ||
        !(str[4] >= 'A' && str[4] <= 'Z') ||
        !(str[5] >= '0' && str[5] <= '9') ||
        !(str[6] >= '0' && str[6] <= '9')) {
        IsUnknown = true;
        
        if (!str.empty() && str != "-1") {
            cerr << "Unknown date: " << str << endl;
        }
        return;
    }

    Day = atoi(str.substr(0, 2).c_str());

    string month = str.substr(2, 3);
    if (month == "Jan" || month == "JAN") {
        Month = 1;
    } else if (month == "Feb" || month == "FEB") {
        Month = 2;
    } else if (month == "Mar" || month == "MAR") {
        Month = 3;
    } else if (month == "Apr" || month == "APR") {
        Month = 4;
    } else if (month == "May" || month == "MAY") {
        Month = 5;
    } else if (month == "Jun" || month == "JUN") {
        Month = 6;
    } else if (month == "Jul" || month == "JUL") {
        Month = 7;
    } else if (month == "Aug" || month == "AUG") {
        Month = 8;
    } else if (month == "Sep" || month == "SEP") {
        Month = 9;
    } else if (month == "Oct" || month == "OCT") {
        Month = 10;
    } else if (month == "Nov" || month == "NOV") {
        Month = 11;
    } else if (month == "Dec" || month == "DEC") {
        Month = 12;
    } else {
        cerr << "Unknown date: " << str << endl;
        IsUnknown = true;
        return;
    }

    Year = atoi(str.substr(5, 2).c_str());
}

std::ostream& operator <<(std::ostream& os, const TDate& date) {
    os << date.Day << "." << date.Month << "." << date.Year;
    return os;
}

TTransaction::TTransaction(const std::string& str) {
    vector<string> fields;
    SplitString(str, '\t', &fields);

    ClientId = atoi(fields[0].c_str());
    Amount = atoi(fields[1].c_str());
    Date = TDate(fields[2].c_str());
    IsOnline = fields[3] == "Y";
    MerchantCategoryCode = atoi(fields[4].c_str());
    CountryCode = fields[5];
    City = fields[6];
    ClientRegion = fields[7];
    ClientGender = fields[8] == "F" ? G_Female : G_Male;
    ClientAge = atoi(fields[9].c_str());
    MerchantId = atoi(fields[10].c_str());
}

void TTransaction::DebugPrint() const {
    std::cout << "ClientId = " << ClientId << endl
        << "Amount = " << Amount << endl 
        << "Date = " << Date << endl
        << "IsOnline = " << IsOnline << endl
        << "MerchantCategoryCode = " << MerchantCategoryCode << endl
        << "CountryCode = " << CountryCode << endl
        << "City = " << City << endl
        << "ClientRegion = " << ClientRegion << endl
        << "ClientGender = " << ClientGender << endl
        << "ClientAge = " << ClientAge << endl
        << "MerchantId = " << MerchantId << endl;
}


TTransactions ReadTransactionsFromFile(const std::string& filename) {
    std::ifstream ifs(filename);

    TTransactions transactions;

    string line;
    while (std::getline(ifs, line)) {
        TTransaction transaction(line);
        transactions.push_back(transaction);
    }

    return transactions;
}

}