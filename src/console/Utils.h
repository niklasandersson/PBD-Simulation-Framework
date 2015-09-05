#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

class Utils {

public:
	static std::string reverseArgs(std::string args);
	static float Like(const std::string& s1 , const std::string& s2);
	static void DrawProgressBar(int length, double percent);

protected:
private:

};

#endif