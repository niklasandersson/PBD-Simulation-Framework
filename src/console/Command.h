#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>

class Command {

public:
	Command(const char* name, std::function<void(const char*)> function);

	void execute(const char* args) const;
	const char* getName(void) const;
	
protected:
private:
	const char* name_;
	const std::function<void(const char*)> function_;

};

#endif