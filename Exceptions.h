#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include<string>
#include <stdexcept>
struct LexicalError final : std::runtime_error {
	//Single-argument constructors must be marked explicit to avoid unintentional implicit conversions 
	explicit LexicalError(const char* msg) : std::runtime_error(msg){}
	explicit LexicalError(const std::string& msg) : std::runtime_error(msg){}
};
struct SintaticError final : std::runtime_error {
	explicit SintaticError(const char* msg) : std::runtime_error(msg) {}
	explicit SintaticError(const std::string& msg) : std::runtime_error(msg) {}
};
struct FileError final : std::runtime_error {
	explicit FileError(const char* msg) : std::runtime_error(msg) {}
	explicit FileError(const std::string& msg) : std::runtime_error(msg) {}
};

struct SemanticError  final : std::runtime_error {
	explicit SemanticError(const char* msg) : std::runtime_error(msg) {}
	explicit SemanticError(const std::string& msg) : std::runtime_error(msg) {}
};
#endif
