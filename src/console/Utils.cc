#include "Utils.h"

std::string Utils::reverseArgs(std::string args) {
	std::reverse(std::begin(args), std::end(args));
	std::istringstream is(args);
	std::string arg;
	std::string final;

	while(is >> arg) {
		std::reverse(std::begin(arg), std::end(arg));
		final += arg + ' ';
	}

	return final;
}


float Utils::Like(const std::string& s1 , const std::string& s2) {

	const unsigned int N1 = s1.size();
	const unsigned int N2 = s2.size();
	
	unsigned int equals = 0;
	for(int i=0; i<N1 && i<N2; i++) {
		if(s1[i] == s2[i]) {
			equals++;
		}
	}

	return static_cast<float>(equals / static_cast<float>(N2));
}


void Utils::DrawProgressBar(int length, double percent) {
  	std::string progress;
  	for(int i=0; i<length; i++) {
    	progress += i < static_cast<int>(length * percent) ? "=" : " ";
	}
  	std::cout << "\r[" << progress << "] " << static_cast<int>(100 * percent) << '%';
	flush(std::cout);
}
