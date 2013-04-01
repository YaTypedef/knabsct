#include <iostream>

#include "data.h"
#include "calculator.h"

int main() {
    TData data;
    data.LoadFromCSVFiles(
        "c:\\Users\\typedef\\Documents\\GitHub\\knabsct\\credit_score\\data\\SAMPLE_ACCOUNTS.csv",
        "c:\\Users\\typedef\\Documents\\GitHub\\knabsct\\credit_score\\data\\SAMPLE_CUSTOMERS.csv");
    
    TFactorsCalculator calculator;
    calculator.PrintPool(data, PT_LEARN);
	return 0;
}