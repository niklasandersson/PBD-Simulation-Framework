#include "Regular_Expression.h"


Regular_Expression::Regular_Expression(const std::string& regex) {
  build(regex);
}


bool Regular_Expression::is_match(const char c) const {

  for(const auto& p : ranges_) {
    if( c >= p.first && c <= p.second ) {
      return true;
    }
  }

  for(const auto& p : lonelys_) {
    if( c == p ) {
      return true;
    }
  }

  return false;

}


bool Regular_Expression::is_match(const std::string& string_to_check) const {

  for(const char& c : string_to_check) {
    if( !is_match(c) ) {
      return false;
    }
  }

  return true;

}


void Regular_Expression::build(const std::string& regex) {
  std::istringstream is{regex};
  char be;
  char at;
  char en;

  do {
    is >> be;
    at = is.peek();

    if( at == '-' ) {
      is >> at;
      if( is >> en ) {
        ranges_.push_back(std::pair<char, char>{be, en});
      }
    } else {
      lonelys_.push_back(be);
    }

  } while( !is.eof() );

} 


std::string& Regular_Expression::clean(std::string& str) const {
  for(unsigned int i=0; i<str.size(); i++) {
    if( !is_match(str[i]) ) {
      str.erase(str.begin() + i);
      i--;
    }
  }
  return str;
}
