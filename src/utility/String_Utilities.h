#ifndef STRING_UTILITIES_H
#define STRING_UTILITIES_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "regex/Regular_Expression.h"

#include "exception/Error.h"


bool Equal_beginning(const std::string& s1, const std::string& s2);
std::vector<unsigned int> Equal_middle(const std::string& s1, const std::string& s2);
bool Equal_ending(const std::string& s1, const std::string& s2);
std::string Replace_occurences(const std::string& s1, const std::string& s2, const std::string& s3);
unsigned int read_unsigned_int(const std::string& line);


#endif // STRING_UTILITIES_H