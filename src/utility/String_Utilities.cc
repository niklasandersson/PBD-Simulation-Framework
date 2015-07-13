#include "utility/String_Utilities.h"


bool Equal_beginning(const std::string& s1, const std::string& s2) {
  const unsigned int N1 = s1.size();
  const unsigned int N2 = s2.size();
  
  unsigned int equals = 0;
  for(unsigned int i=0; i<N1 && i<N2; i++) {
    if(s1[i] == s2[i]) {
      equals++;
    }
  }

  return equals == N2;
}


std::vector<unsigned int> Equal_middle(const std::string& s1, const std::string& s2) {
  const unsigned int N1 = s1.size();
  const unsigned int N2 = s2.size();
  const int j = N1 - N2;
  
  if( j == 0 ) {
    if( s1 == s2 ) {
      return std::move(std::vector<unsigned int>{0});
    } else {
      return std::move(std::vector<unsigned int>{});
    }
  } else if( j < 0 ) {
    return std::move(std::vector<unsigned int>{});
  }

  std::vector<unsigned int> result{};

  unsigned int equals;
  unsigned int i;
  unsigned int k;

  for(i=0; i<j+1; i++) {
    equals = 0;

    for(k=0; k<N2; k++) {
      if(s1[i + k] == s2[k]) {
        equals++;
        continue;
      } 
      break;
    }

    if(equals == N2) {
      result.push_back(i);
      i += N2-1;
    }

  }

  return std::move(result);
}


bool Equal_ending(const std::string& s1, const std::string& s2) {
  const unsigned int N1 = s1.size();
  const unsigned int N2 = s2.size();
  
  unsigned int equals = 0;
  for(unsigned int i=0; i<N1 && i<N2; i++) {
    if(s1[N1-1-i] == s2[N2-1-i]) {
      equals++;
    }
  }

  return equals == N2;
}


std::string Replace_occurences(const std::string& s1, const std::string& s2, const std::string& s3)
{ 
    std::string result{s1};
    size_t it;
    while( (it = result.find(s2)) != std::string::npos) {
      result.replace(it, s2.length(), s3);
    }
    return std::move(result);

}


unsigned int read_unsigned_int(const std::string& line) {

  Regular_Expression regex{"0-9"};

  if( !regex.is_match(line) ) {
    throw std::invalid_argument{ report_error( "A non integer string was given." ) };
  }

  std::istringstream is{line};
  unsigned int value;
  is >> value;

  return value;

}