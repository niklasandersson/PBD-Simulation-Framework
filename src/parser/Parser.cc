#include "parser/Parser.h"


std::string Parser::getFileName() const {
  if( !fileNameSet_ ) {
    throw std::runtime_error{"The file name is not set"};
  }
  return fileName_;
}


std::string Parser::getFilePath() const {
  if( !filePathSet_ ) {
    throw std::runtime_error{"The file path is not set"};
  }
  return filePath_;
}


unsigned int Parser::getLineNumber() const {
  return lineNumber_;
}


std::string Parser::parseFile(const std::string& pathAndName) {

  try {
    size_t end_notation = pathAndName.rfind('/');
    filePath_ = pathAndName.substr(0, end_notation+1);   
    filePathSet_ = true;
    fileName_ = pathAndName.substr(end_notation+1, pathAndName.size());
    fileNameSet_ = true;
  } catch(...) {
    filePath_ = "";
    fileName_ = pathAndName;
  }

  std::ifstream input{(filePath_ + fileName_).c_str()};

  if( !input ) {
    std::ostringstream os;
    os << "Could not open file '" << filePath_ + fileName_ << "'";
    throw std::invalid_argument{os.str()};
  }

  std::string parsedFile = parseStream(input);

  // filePathSet_= false;
  // fileNameSet_= false;

  return parsedFile;
}


std::string Parser::parseText(const std::string& text) {
  std::istringstream input{text};
  return parseStream(input);
}


std::string Parser::parseStream(std::istream& input) {
  std::ostringstream output;
  std::string line = "";
  char c;
  lineNumber_ = 1;

   while( input >> std::noskipws >> c ) {
    
    if( c == '\n' ) {
      output << parseLineImp2(line) << std::endl;
      lineNumber_++;
      line = "";
    } else {
      line += c;
    }

  }

  output << parseLineImp2(std::move(line));

  return output.str();
}


std::string Parser::parseLine(const std::string& line) {
  return line + "parser";
}
