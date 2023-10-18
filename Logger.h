#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <string>
#include <fstream>
#include <string>
#include "Token.h"
class Logger {
public:
	Logger(const std::string& filename) : m_output(filename, std::ios::app) {
		if (!m_output) {
			throw std::runtime_error("Failed to open log file");
		}
	}
	~Logger() {
		if (m_output) {
			m_output.close();
		}
	}

	void log(const std::string& message, const std::string& other) {
		m_output <<  " : " << message <<"   "<<other << std::endl;
	}

	void log(const std::string& message, long other) {
		m_output <<  " : " << message << "   " << other << std::endl;
	}

	void log(const std::string& message, bool other) {
		m_output <<  " : " << message << "   " << other << std::endl;
	}

	void log(const std::string& message) {
		m_output <<  " : " << message << std::endl;
	}

	void log(const std::string& message, int other) {
		m_output <<  " : " << message << "   " << other << std::endl;
	}

	void log(const Token& other) {
		m_output <<  " : " << other << std::endl;
	}

private:
	std::ofstream m_output;
};


#endif

