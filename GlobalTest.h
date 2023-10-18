#ifndef GLOBALTEST_H
#define GLOBALTEST_H
// We need a singelton class as the global variable
#include <unordered_map>
#include <string>
#include <iostream>
#include "Exceptions.h"

class Global {
public:

	Global(const Global&) = delete;
	Global& operator=(const Global&) = delete;

	static Global& Get() {

		static Global s_instance;
		return s_instance;
	}

	static std::unordered_map<std::string, long> returnVariables() {
		return Get().returnVariablesInternal();
	}

	static bool check_varible(const std::string& name) {
		return Get().check_varible_internal(name);
	}

	static long GetVarNumeric(const std::string& name) {
		return Get().get_var_numeric_internal(name);
	}

	static void addvariable(const std::string& name, long num) {
		return Get().addvariable_internal(name, num);
	}

private:

	Global() {}

	std::unordered_map<std::string, long> VariablesMap;

	std::unordered_map<std::string, long> returnVariablesInternal() {
		return VariablesMap;
	}

	bool check_varible_internal(const std::string& name) const
	{
		for (const auto& x : VariablesMap) {
			if (x.first == name) {
				return true;
			}
		}
		return false;
	}


	long get_var_numeric_internal(const std::string& name) const
	{
		if (check_varible_internal(name) == true) {
			for (const auto& x : VariablesMap) {
				if (x.first == name) {
					return x.second;
				}
			}
		}
		return 4762847;
	}


	void addvariable_internal(const std::string& name, long num) {
		if (check_varible_internal(name)) {
			VariablesMap[name] = num;
		}
		else {
			std::pair<std::string, long > pair(name, num);
			VariablesMap.insert(pair);
		}
	}

};

#endif
