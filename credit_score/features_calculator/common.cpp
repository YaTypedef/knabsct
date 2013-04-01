#include <sstream>
#include <algorithm>

#include "common.h"

using std::stringstream;
using std::string;
using std::getline;

void SplitString(const std::string& str, char delim, std::vector<std::string>* answer) {
    answer->clear();

    stringstream ss(str);
    string item;

    while (getline(ss, item, ';')) {
        answer->push_back(item);
    }
}