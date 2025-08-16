#include "Logger.h"

#include <iostream>

void Logger::heading(const std::string& msg) {
  std::cout << "----------" << msg << "-----------" << std::endl;
}

void Logger::point(const std::string& msg) {
  std::cout << " -> " << msg << std::endl;
}

void Logger::log(const std::string& msg) {
  std::cout << msg << std::endl;
}