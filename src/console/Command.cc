#include "Command.h"

Command::Command(const char* name, std::function<void(const char*)> function) 
	: name_(name), function_(function)
{ 
}

void Command::execute(const char* args) const {
	function_(args);
}

const char* Command::getName(void) const {
	return name_;
}

