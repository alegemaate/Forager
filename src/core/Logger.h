
#pragma once

#include <string>

class Logger {
 public:
  static void heading(const std::string& msg);
  static void point(const std::string& msg);
  static void log(const std::string& msg);
};
