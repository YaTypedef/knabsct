#include <iostream>

#include "data.h"
#include "calculator.h"

int main() {
    TData data;
    data.LoadFromCSVFiles("SAMPLE_ACCOUNTS.csv", "SAMPLE_CUSTOMERS.csv");
    
    TFactorsCalculator calculator;
    calculator.PrintPool(data, PT_LEARN);
    calculator.PrintPool(data, PT_TEST);
	return 0;
}