#include "Console.h"


std::shared_ptr<Console> Console::getInstance() {
    static std::shared_ptr<Console> instance(new Console());
	return instance;
}


Console::Console() {
	// std::cout << "CONSOLE CREATED" << std::endl;
	std::lock_guard<std::recursive_mutex> lock(commandItemsMutex_);
	addCommandItem(Command("ls", [&](const char* args){
		for_each(commandItems_.begin(), commandItems_.end(), [](std::pair<std::string, Command> item) {
    		std::cout << "\t" << item.first << std::endl;
    	});
	})); 

  isRunning_ = true;
  
  consoleThread_ = std::thread([](Console* console) {

    while( console->getIsRunning() ) {
      std::string line;
      std::cout << "Console: ";
      std::getline(std::cin, line);
      if( line != "" ) {
        console->execute(line.c_str());
      }
    }
    // std::cout << "consoleThread_ CLOSES!!!" << std::endl;
  }, this);

}


Console::~Console() {
	// std::cout << "CONSOLE DELETED" << std::endl;
  isRunning_ = false;
  consoleThread_.join();
}


void Console::execute(const char* args) const {
	std::istringstream is(args);
	std::string cmd;
	is >> cmd;

	std::lock_guard<std::recursive_mutex> lock(commandItemsMutex_);
	std::unordered_map<std::string, Command>::const_iterator it = commandItems_.find(cmd);
	
  	if (it == commandItems_.end()) {
    	std::cout << cmd << ": command not found " << endl;
    	suggestions(cmd);
 	} else {
 		std::string rest;
		std::getline(is, rest);
 		try {
 			it->second.execute(rest.c_str());
 		} catch(...) {
 			cout << "ERROR: failed to execute command: " << cmd << endl;
 		}
 	}	
}


void Console::suggestions(std::string& cmd) const {
	const float LIKENESS = 0.5f;
	std::lock_guard<std::recursive_mutex> lock(commandItemsMutex_);
	for_each(commandItems_.begin(), commandItems_.end(), [&cmd, &LIKENESS](std::pair<std::string, Command> item) {
		if(Utils::Like(cmd, item.first) >= LIKENESS) {
			cout << " - perhaps you meant: " << item.first << endl;
		}
	});
}


void Console::add(const char* name, std::function<void(const char*)> function) {
	addCommandItem(Command(name, function));
}


void Console::addCommandItem(const Command& command) {
	std::lock_guard<std::recursive_mutex> lock(commandItemsMutex_);
	std::unordered_map<std::string, Command>::iterator it = commandItems_.find(command.getName());
  	if (it == commandItems_.end()) {
		commandItems_.insert({command.getName(), command});
		// std::cout << "Added command: " << command.getName() << std::endl;
  	} else {
  		std::cout << command.getName() << ": already added" << std::endl;
  	}
}


bool Console::getIsRunning() {
  return isRunning_;
}

void Console::close() {
  isRunning_ = false;
}

