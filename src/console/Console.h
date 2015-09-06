#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <sstream>
#include <tuple>
#include <mutex>
#include <atomic>

#include "Command.h"
#include "Utils.h"

using namespace std;

class Console {

public:

	static std::shared_ptr<Console> getInstance();

	~Console();

	void execute(const char* args) const;

	void add(const char* name, std::function<void(const char*)> function);


	template<class C, typename... Ps>
	void add(const char* name, C* c, void (C::*f)(Ps...)) {
		addCommandItem(Command(name, [&, name, c, f](const char* args){ 	
			std::istringstream is(Utils::reverseArgs(args));
			std::tuple<Ps...> arguments = std::tuple<Ps...>{ extract<Ps>(is)... };
			call(typename generate<sizeof...(Ps)>::sequence(), c, f, arguments);
			// cout << name << " : CALLED" << endl;
		}));	
	}


	template<class C, typename R, typename... Ps>
	void add(const char* name, C* c, R (C::*f)(Ps...)) {
		addCommandItem(Command(name, [&, name, c, f](const char* args){ 	
			std::istringstream is(Utils::reverseArgs(args));
			std::tuple<Ps...> arguments = std::tuple<Ps...>{ extract<Ps>(is)... };
			R r = call(typename generate<sizeof...(Ps)>::sequence(), c, f, arguments);
			// cout << name << " : " << r << endl;
		}));	
	}

 	bool getIsRunning();
 	void close();

protected:
private:
	mutable std::recursive_mutex commandItemsMutex_;
	std::unordered_map<std::string, Command> commandItems_;
	std::atomic<bool> isRunning_;
  std::thread consoleThread_;

	Console();

	void suggestions(std::string& cmd) const;
	void addCommandItem(const Command& command);


	template<typename P>
	P extract(std::istringstream& is) {
		P p;
		is >> p;
		return p;
	}


	template<int ...>
	struct seq { };


	template<int N, int ...S>
	struct generate : generate<N-1, N-1, S...> { };


	template<int ...S>
	struct generate<0, S...> {
	  typedef seq<S...> sequence;
	};


	template<int ...S, class C, typename... Ps, typename... As>
	void call(seq<S...>, C* c, void (C::*f)(Ps...), std::tuple<As...> arguments) {
		(c->*f)(std::get<S>(arguments)...);
	}


	template<int ...S, class C, typename R, typename... Ps, typename... As>
	R call(seq<S...>, C* c, R (C::*f)(Ps...), std::tuple<As...> arguments) {
		return (c->*f)(std::get<S>(arguments)...);
	}

};

#endif