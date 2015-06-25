#include "test/Batch.h"


void Batch::add(Test* test) {
  tests_.push_back(test);
}


void Batch::remove(Test* test) {
  for(unsigned int i=0; i<tests_.size(); i++) {
    if( tests_[i] == test ) {
      tests_.erase(tests_.begin() + i);
    }
  }
}


std::vector<std::pair<Test*, bool> > Batch::getTestResults() const {
  return allTestResults_;
}


unsigned int Batch::getNumberOfTests() const {
  return tests_.size();
}


void Batch::setDynamicIndent(const unsigned int dynamicIndent) {
  dynamicIndent_ = dynamicIndent;
}


unsigned int Batch::getDynamicIndent() const {
  return dynamicIndent_;
}


bool Batch::containsTest(Test* test) const {
  if( std::find(tests_.begin(), tests_.end(), test) != tests_.end() ) {
    return true;
  }

  for(auto& test : tests_) {
    if( typeid(*test) == typeid(Batch) ) {
      if( static_cast<Batch*>(test)->containsTest(test) ) {
        return true;
      }
    }
  }

  return false;
}


void Batch::clear() {
  tests_.clear();
}


bool Batch::test() {
  return execute();
}


std::string Batch::colorTestLine(const std::string& line, Test* test) const {
  std::string newLine;
  std::string word = "";
  std::istringstream is{line};
  bool first = true;
  while( true ) {
    bool spaceBeforeWord = false;

    if( first ) {
      first = false;
    } else {
      char c = is.peek();
      if( c == ' ' ) {
        spaceBeforeWord = true;
      }
    }

    if( !(is >> word) ) {
      break;
    }

    if( word == "Error" ) {
      word = Ansi::color("Error", Ansi::Color::RED, Ansi::Type::NORMAL);
    } if( word == "Error:" ) {
      word = Ansi::color("Error", Ansi::Color::RED, Ansi::Type::NORMAL) + ":";
    } else if( word == "Error," ) {
      word = Ansi::color("Error", Ansi::Color::RED, Ansi::Type::NORMAL) + ",";
    } else if( word == "Warning" ) {
      test->setHasWarning(true);
      word = Ansi::color("Warning", Ansi::Color::YELLOW, Ansi::Type::NORMAL);
    } else if( word == "Warning:" ) {
      test->setHasWarning(true);
      word = Ansi::color("Warning", Ansi::Color::YELLOW, Ansi::Type::NORMAL) + ":";
    } else if( word == "Warning," ) {
      test->setHasWarning(true);
      word = Ansi::color("Warning", Ansi::Color::YELLOW, Ansi::Type::NORMAL) + ",";
    } else if( word == "PASS" ) {
      if( test->hasWarning() ) {
        word = Ansi::color("PASS", Ansi::Color::YELLOW, Ansi::Type::BOLD);
      } else {
        word = Ansi::color("PASS", Ansi::Color::GREEN, Ansi::Type::BOLD);
      }
    } else if( word == "FAIL" ) {
      word = Ansi::color("FAIL", Ansi::Color::RED, Ansi::Type::BOLD);
    }

    if( spaceBeforeWord ) {
      newLine += " ";
    }

    newLine += word;

  }

  return newLine;
}


bool Batch::execute() {
  const auto start = std::chrono::high_resolution_clock::now();

  myTestResults_.clear();
  includedBatch_ = false;

  log_ << "[" << log_.getBatchName(getName()) << "]" << std::endl;

  std::string sign = "\033[1;32m|\033[0m";
  log_ << sign << std::endl;

  bool batchResult = true;
  bool batchWarnings = false;

  for(auto& test : tests_) {

    if( !test ) {
      std::string testName = typeid(test).name();
      log_ << "Test " << testName << " is undefined." << std::endl;
      batchResult = false;
      continue;
    }

    test->setIndent(4);
    if( typeid(*test) == typeid(Batch) ) {
      static_cast<Batch*>(test)->setDynamicIndent(dynamicIndent_ + 4);
    }

    test->clearLog();
    test->disablePrintOut();
    test->resetDuration();
    test->setHasWarning(false);

    bool testResult = false;
    std::string testLog;

    std::ostringstream throwOs;
    bool exceptionThrown = false;

    try {
      testResult = test->test();
    } catch( const std::exception& e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << typeid(e).name() << std::endl;
      throwOs << e.what() << std::endl;
    } catch( const unsigned int& e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: unsigned int" << std::endl;
      throwOs << e << std::endl;
    } catch( const int& e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << "int" << std::endl;
      throwOs << e << std::endl;
    } catch( const char* e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << "c-string" << std::endl;
      throwOs << e << std::endl;
    } catch( const std::string& e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << "std::string" << std::endl;
      throwOs << e << std::endl;
    } catch( const char& e ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << "char" << std::endl;
      throwOs << e << std::endl;
    } catch( ... ) {
      exceptionThrown = true;
      throwOs  << "Error, uncaught exception: " << "unknown" << std::endl;
    }

    if( exceptionThrown ) {
      testResult = false;;
      throwOs << log_.getResult(testResult, test->hasWarning()) << std::endl << std::endl;
      test->log(throwOs.str());
    }

    testLog = test->getLog();

    if( !testResult ) {
      batchResult = false;
    }

    if( typeid(*test) == typeid(*this) ) {
      includedBatch_ = true;
    }  else {
      unsigned int length = 80 - dynamicIndent_ - 4;
      std::ostringstream os;
      std::string line;
      std::istringstream is{testLog};
      while( getline(is, line) ) {
        while( line.size() > length ) {
          std::string add = line.substr(0, length);

          add = colorTestLine(add, test);
          os << add << std::endl;
          line = line.substr(length, line.size());
        }

        line = colorTestLine(line, test);
        os << line << std::endl;
      }
      testLog = os.str();
    }

    if( test->hasWarning() ) {
      batchWarnings = true;
      setHasWarning(true);
    }


    for(unsigned int i=0; i<testLog.size(); i++) {
      if( i == 0 ) {
        for(unsigned int j=0; j<test->getIndent(); j++) {
          if( j == 0 ) {
            if( batchResult ) {
              if( batchWarnings ) {
                std::string sign = "\033[1;33m|\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              } else {
                std::string sign = "\033[1;32m|\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              }
            } else {
              std::string sign = "\033[1;31m|\033[0m";
              testLog.insert(i, sign);
              i += sign.size() - 1;
            }

          } else if( j == 1 ) {
            if( testResult ) {
              if( test->hasWarning() ) {
                std::string sign = "\033[1;33m\\\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              } else {
                std::string sign = "\033[1;32m\\\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              }
            } else {
              std::string sign = "\033[1;31m\\\033[0m";
              testLog.insert(i, sign);
              i += sign.size() - 1;
            }

          } else {
            if( testResult ) {
              if( test->hasWarning() ) {
                std::string sign = "\033[1;33m_\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              } else {
                std::string sign = "\033[1;32m_\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              }
            } else {
              std::string sign = "\033[1;31m_\033[0m";
              testLog.insert(i, sign);
              i += sign.size() - 1;
            }

          }
          i++;
        }
      } else 

      if( testLog[i] == '\n' && i != testLog.size()-1 ) {
        i++;
        for(unsigned int j=0; j<test->getIndent(); j++) {
          if( j == 0 ) {
            if( batchResult ) {
              if( batchWarnings ) {
                std::string sign = "\033[1;33m|\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              } else {
                std::string sign = "\033[1;32m|\033[0m";
                testLog.insert(i, sign);
                i += sign.size() - 1;
              }
            } else {
              std::string sign = "\033[1;31m|\033[0m";
              testLog.insert(i, sign);
              i += sign.size() - 1;
            }

          } else {
            testLog.insert(testLog.begin() + i, ' ');
          }
          
          i++;
        }
      }
    }

    log_ << testLog;

    if( typeid(*test) == typeid(*this) ) {
      if( batchResult ) {
        if( batchWarnings ) {
          std::string sign = "\033[1;33m|\033[0m";
          log_ << sign;
        } else {
          std::string sign = "\033[1;32m|\033[0m";
          log_ << sign;
        }
      } else {
        std::string sign = "\033[1;31m|\033[0m";
        log_ << sign;
      }
      log_ << std::endl;
    }
    
    myTestResults_.push_back(std::make_pair(test, testResult));

    if( typeid(*test) != typeid(*this) ) {
      allTestResults_.push_back(std::make_pair(test, testResult));
    } else {
      std::vector<std::pair<Test*, bool> > results = static_cast<Batch*>(test)->getTestResults();
      for(auto& result : results) {
        if( typeid(*result.first) != typeid(*this) ) {
          allTestResults_.push_back(result);
        }
      }
    }
    
  }

  log_ << "[" << log_.getResult(batchResult, hasWarning()) << "]" << std::endl;

  const auto end = std::chrono::high_resolution_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
  setDuration(duration);
  return batchResult;
}


bool Batch::run() {

  dynamicIndent_ = 0;

  log_.begin();

  log_.putLine();

  log_ << log_.getBatchName(getName()) << ": " << log_.getDateAndTime(log_.getStartDateAndTime());
  if( hasFileName() ) {
    log_ << " @ " << getFileName();
    if( hasLine() ) {
      log_ << " " << getLine();
    }
  } 
  log_ << std::endl;
  

  log_.putLine();

  const bool batchResult = execute();

  if( includedBatch_ ) {
    log_.putLine();

    log_ << "All " << allTestResults_.size() << " tests:" << std::endl;

    unsigned int allMaxLength = 0;
    for(const auto& testResult : allTestResults_) {
      unsigned int length;
      if( typeid(*testResult.first) == typeid(*this) ) {
        length = log_.getBatchName(testResult.first->getName()).size();
      } else {
        length = log_.getTestName(testResult.first->getName()).size();
      }
      if( length > allMaxLength ) {
        allMaxLength = length;
      }
    }

    std::sort(allTestResults_.begin(), allTestResults_.end(), [](const std::pair<Test*, bool>& p1, const std::pair<Test*, bool>& p2) -> bool {
      std::string str1 = p1.first->getName();
      std::string str2 = p2.first->getName();
      std::transform(str1.begin(), str1.end(),str1.begin(), ::toupper);
      std::transform(str2.begin(), str2.end(),str2.begin(), ::toupper);
      return str1 < str2;
    });

    for(const auto& testResult : allTestResults_) {
      std::ostringstream os;
      if( testResult.first->hasSetDuration() ) {
        os << testResult.first->getDuration() << " ms";
      } else {
        os << "no duration set";
      }
      
      if( typeid(*testResult.first) == typeid(*this) ) {
        log_ << std::setfill(' ') << std::setw(allMaxLength + 1) << std::left << log_.getBatchName(testResult.first->getName()) << std::left << ": " 
        << log_.getResult(testResult.second, testResult.first->hasWarning()) << " " << log_.getDateAndTime(os.str());
        if( !testResult.second ) {
          if( testResult.first->hasFileName() && testResult.first->hasLine() ) {
            log_ << " @ " << testResult.first->getFileName() << " " << testResult.first->getLine() << std::endl;
          } else {
            log_ << " has not file or line set" << std::endl;
          }
        } else {
          log_ << std::endl;
        }
      } else {
        log_ << std::setfill(' ') << std::setw(allMaxLength + 1) << std::left << log_.getTestName(testResult.first->getName()) << std::left << ": " 
        << log_.getResult(testResult.second, testResult.first->hasWarning()) << " " << log_.getDateAndTime(os.str());
        if( !testResult.second ) {
          if( testResult.first->hasFileName() && testResult.first->hasLine() ) {
            log_ << " @ " << testResult.first->getFileName() << " " << testResult.first->getLine() << std::endl;
          } else {
            log_ << " has not file or line set" << std::endl;
          }
        } else {
          log_ << std::endl;
        }
      }
    }
  }

  log_.putLine();

  log_ << "This batch:" << std::endl;

  unsigned int myMaxLength = 0;
  for(const auto& testResult : myTestResults_) {
    unsigned int length;
    if( typeid(*testResult.first) == typeid(*this) ) {
      length = log_.getBatchName(testResult.first->getName()).size();
    } else {
      length = log_.getTestName(testResult.first->getName()).size();
    }
    if( length > myMaxLength ) {
      myMaxLength = length;
    }
  }

  for(const auto& testResult : myTestResults_) {
    std::ostringstream os;
    if( testResult.first->hasSetDuration() ) {
      os << testResult.first->getDuration() << " ms";
    } else {
      os << "no duration set";
    }

    if( typeid(*testResult.first) == typeid(*this) ) {
      log_ << std::setfill(' ') << std::setw(myMaxLength + 1) << std::left << log_.getBatchName(testResult.first->getName()) << std::left << ": " 
      << log_.getResult(testResult.second, testResult.first->hasWarning()) << " " << log_.getDateAndTime(os.str());
      if( !testResult.second ) {
        if( testResult.first->hasFileName() && testResult.first->hasLine() ) {
          log_ << " @ " << testResult.first->getFileName() << " " << testResult.first->getLine() << std::endl;
        } else {
          log_ << " has not file or line set" << std::endl;
        }
      } else {
        log_ << std::endl;
      }
    } else {
      log_ << std::setfill(' ') << std::setw(myMaxLength + 1) << std::left << log_.getTestName(testResult.first->getName()) << std::left << ": " 
      << log_.getResult(testResult.second, testResult.first->hasWarning()) << " " << log_.getDateAndTime(os.str());
      if( !testResult.second ) {
        if( testResult.first->hasFileName() && testResult.first->hasLine() ) {
          log_ << " @ " << testResult.first->getFileName() << " " << testResult.first->getLine() << std::endl;
        } else {
          log_ << " has not file or line set" << std::endl;
        }
      } else {
        log_ << std::endl;
      }
    }
  }

  log_.putLine();

  std::ostringstream os;
  if( hasSetDuration() ) {
    os << getDuration() << " ms";
  } else {
    os << "no duration set";
  }
  
  log_ << "Time elapsed: " << log_.getDateAndTime(os.str()) << std::endl;
  log_ << log_.getBatchName(getName()) << ": " << log_.getResult(batchResult, hasWarning()) << std::endl;

  log_.putLine();

  std::string fail = 
  // "////////////////////////////////////////////////////////////////////////////////\n"
  // "0000000   00      00  00\n"  
  // "00       0000     00  00\n"
  // "00000   00  00    00  00\n"
  // "00     00000000   00  00\n"
  // "00    00      00  00  000000\n";

// "      ______  ___               __  \n"
// "     / ____/ /   \\             /_/ \n"
// "    /_/     /_/| |  _         /_/   \n"
// "   /_/___  /_/_| | | |       /_/    \n"
// "  /_/~~~´ /_,--| | | |  _   / /____ \n"
// " /_/     /_/   |_| |_| (_) /____/_/ \n\n";

Ansi::color("      ______  ___               __  ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n" +
Ansi::color("     / ____/ /   \\             /_/ ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n" +
Ansi::color("    /_/     /_/| |  _         /_/   ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n" +
Ansi::color("   /_/___  /_/_| | | |       /_/    ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n" +
Ansi::color("  /_/~~~´ /_,--| | | |  _   / /____ ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n" +
Ansi::color(" /_/     /_/   |_| |_| (_) /____/_/ ", Ansi::Color::RED, Ansi::Type::BOLD) + "\n\n";

  // "////////////////////////////////////////////////////////////////////////////////\n";
 //    __ _
 //   / _` |
 //  | |_| |
 //   \__,_|

 //   P  ________  ___     _____   _____
 //     / ____ o/ /o o\   /o___/  /o___/   
 //    /_/   /o/ /_/|o|  /o(__   / (__  
 //   /_/___/o/ /_/_| |  \___ \  \___ \  
 //  /_/~~~~~´ /_,--| |  ____) ) ____) )  
 // /_/       /_/   |_| /_____/ /_____/   
  
 //      ______  ___               __
 //     / ____/ /   \             /_/
 //    /_/     /_/| |  _         /_/
 //   /_/___  /_/_| | | |       /_/
 //  /_/~~~´ /_,--| | | |  _   / /____
 // /_/     /_/   |_| |_| (_) /____/_/

 //     /_-\
 //    /_/\-\
 //   /_/__\-\
 //  /_,----.-\
 // /_/      \_\ 


 // ,.` ´ ±±\}$£¡@£}$]€¥¥{[]¡¡@£€€${¥[¥{[]{[@£¡£¡@£$€¥[]}"#¤%&/()=?`**ÄÄ"]}}]}}
 // S
 //    ___
 //   /   \
 //  (___
 //      )
 // \___/
   
  std::string passWarnings = 
  // "////////////////////////////////////////////////////////////////////////////////\n"
  // "111111     11       111111   111111\n"
  // "11  11    1111     11       11\n"
  // "111111   11  11    111111   111111\n"
  // "11      11111111        11       11\n"      
  // "11     11      11  111111   111111\n";
 // "      ________  ___     _____   _____ \n"
 // "     / ____  / /   \\   / ___/  / ___/ \n"   
 // "    /_/   / / /_/| |  / (__   / (__   \n"  
 // "   /_/___/ / /_/_| |  \\___ \\  \\___ \\  \n"  
 // "  /_/~~~~~´ /_,--| |  ____) ) ____) ) \n"  
 // " /_/       /_/   |_| /_____/ /_____/  \n\n";  

  Ansi::color("      ________  ___     _____   _____ ", Ansi::Color::YELLOW, Ansi::Type::BOLD)     + "\n" +
  Ansi::color("     / ____  / /   \\   / ___/  / ___/ ", Ansi::Color::YELLOW, Ansi::Type::BOLD)    + "\n" +   
  Ansi::color("    /_/   / / /_/| |  / (__   / (__   ", Ansi::Color::YELLOW, Ansi::Type::BOLD)     + "\n" +  
  Ansi::color("   /_/___/ / /_/_| |  \\___ \\  \\___ \\  ", Ansi::Color::YELLOW, Ansi::Type::BOLD) + "\n" +  
  Ansi::color("  /_/~~~~~´ /_,--| |  ____) ) ____) ) ", Ansi::Color::YELLOW, Ansi::Type::BOLD)     + "\n" + 
  Ansi::color(" /_/       /_/   |_| /_____/ /_____/  ", Ansi::Color::YELLOW, Ansi::Type::BOLD)     + "\n\n";   


  std::string pass = 
  Ansi::color("      ________  ___     _____   _____ ", Ansi::Color::GREEN, Ansi::Type::BOLD)     + "\n" +
  Ansi::color("     / ____  / /   \\   / ___/  / ___/ ", Ansi::Color::GREEN, Ansi::Type::BOLD)    + "\n" +   
  Ansi::color("    /_/   / / /_/| |  / (__   / (__   ", Ansi::Color::GREEN, Ansi::Type::BOLD)     + "\n" +  
  Ansi::color("   /_/___/ / /_/_| |  \\___ \\  \\___ \\  ", Ansi::Color::GREEN, Ansi::Type::BOLD) + "\n" +  
  Ansi::color("  /_/~~~~~´ /_,--| |  ____) ) ____) ) ", Ansi::Color::GREEN, Ansi::Type::BOLD)     + "\n" + 
  Ansi::color(" /_/       /_/   |_| /_____/ /_____/  ", Ansi::Color::GREEN, Ansi::Type::BOLD)     + "\n\n";   

  // "////////////////////////////////////////////////////////////////////////////////\n";

  if( batchResult ) {
    if( hasWarning() ) {
      // log_ << Ansi::color(pass, Ansi::Color::YELLOW, Ansi::Type::BOLD);
      log_ << passWarnings;
    } else {
      // log_ << Ansi::color(pass, Ansi::Color::GREEN, Ansi::Type::BOLD);
      log_ << pass;
    }
  } else {
    // log_ << Ansi::color(fail, Ansi::Color::RED, Ansi::Type::BOLD);
    log_ << fail;
  }

  log_.end();

  return batchResult;
}
