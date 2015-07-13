#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H

#include <vector>
#include <algorithm>
#include <sstream>

class Regular_Expression {

public:
  Regular_Expression(const std::string& regex);
  ~Regular_Expression() = default;

  bool is_match(const char c) const;
  bool is_match(const std::string& string_to_check) const;
  std::string& clean(std::string& str) const;

protected:

private:
  void build(const std::string& regex);

  std::vector<std::pair<char, char> > ranges_;
  std::vector<char> lonelys_;

};


#endif // REGULAR_EXPRESSION_H