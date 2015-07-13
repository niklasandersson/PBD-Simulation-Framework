#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <memory>

#include "exception/Error.h"

class ConfigObject {

public:
  ConfigObject(const std::string& name) : name_{name} {}

  ~ConfigObject() = default;

  std::string getName() const {
    return name_;
  }

  void addChild(std::shared_ptr<ConfigObject> configObjectPtr) {
    children_[configObjectPtr->getName()] = configObjectPtr;
  }

  void removeChild(const std::string& name) {
    children_.erase(name);
  }

  std::shared_ptr<ConfigObject> getChild(const std::string& name) {
    if( children_.find(name) == std::end(children_) ) {
      throw std::runtime_error{ report_error( "Could not find child '" << name << "' in scope '" << name_ << "'" ) };
    }
    return children_[name];
  }

  void addDefine(const std::string& name, const std::string& value) {
    defines_[name] = value;
  }

  void removeDefine(const std::string& name) {
    defines_.erase(name);
  }

  std::string getDefine(const std::string& name) {
    if( defines_.find(name) == std::end(defines_) ) {
      throw std::runtime_error{ report_error( "Could not find variable '" << name << "' in scope '" << name_ << "'" ) };
    }
    return defines_[name];
  }

protected:

private:
  const std::string name_;
  std::unordered_map<std::string, std::shared_ptr<ConfigObject> > children_;
  std::unordered_map<std::string, std::string> defines_;

};



#endif