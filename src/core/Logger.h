
#ifndef FORAGER_LOGGER_H
#define FORAGER_LOGGER_H

#include <string>

class Logger {
 public:
  static void heading(const std::string& msg);
  static void point(const std::string& msg);
  static void log(const std::string& msg);
};

#endif  // FORAGER_LOGGER_H
