#include "transaction.h"
#include "stat_calculator.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace NMerchants;

int main(int argc, const char** argv) {
    if (argc != 4) {
        std::cout << "Usage: merchants <path_to_transactions> <path_to_mcc_codes> <output_file>" << std::endl;
        return 0;
    }

    const std::string pathTransactions = argv[1];
    const std::string pathMccCodes = argv[2];
    const std::string pathOutput = argv[3];

    TTransactions transactions = ReadTransactionsFromFile(pathTransactions);
    TMccDescriptionTable mccDescriptionTable = ReadMccDescriptionTableFromFile(pathMccCodes);
    
    TMerchantStats stats;
    for (size_t merchantIndex = 0; merchantIndex < FIRST_MERCHANT_INDEX + MERCHANT_COUNT; ++merchantIndex) {
        stats.push_back(TMerchantStatBundle(mccDescriptionTable));
    }

    for (TTransactions::const_iterator it = transactions.begin(); it != transactions.end(); ++it) {
        size_t merchantIndex = it->MerchantId;
        if (merchantIndex < FIRST_MERCHANT_INDEX || merchantIndex > stats.size()) {
            std::cerr << "Incorrect merchant index = " << merchantIndex << std::endl;
            continue;
        }
        stats[merchantIndex].AddRecord(*it);
    }

    std::ofstream output(pathOutput);

    for (size_t merchantIndex = FIRST_MERCHANT_INDEX; merchantIndex < stats.size(); ++merchantIndex) {
        output << "MerchantId" << "\t" << merchantIndex << std::endl;
        stats[merchantIndex].PrintToStream(output);
    }
    
    return 0;
}