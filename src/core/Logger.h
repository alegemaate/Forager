
#pragma once

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

enum class AnsiCode : int {
  Reset = 0,
  BoldOn = 1,
  FaintOff = 2,
  ItalicOn = 3,
  UnderlineOn = 4,
  SlowBlinkOn = 5,
  RapidBlinkOn = 6,
  ReverseVideoOn = 7,
  ConcealOn = 8,
  CrossedOutOn = 9,
  BoldOff = 22,
  ItalicOff = 23,
  UnderlineOff = 24,
  BlinkOff = 25,
  ReverseVideoOff = 27,
  ConcealOff = 28,
  CrossedOutOff = 29,

  // Text color
  TextBlack = 30,
  TextRed = 31,
  TextGreen = 32,
  TextYellow = 33,
  TextBlue = 34,
  TextMagenta = 35,
  TextCyan = 36,
  TextWhite = 37,
  TextDefault = 39,

  // Background color
  BgBlack = 40,
  BgRed = 41,
  BgGreen = 42,
  BgYellow = 43,
  BgBlue = 44,
  BgMagenta = 45,
  BgCyan = 46,
  BgWhite = 47,
  BgDefault = 49
};

inline std::string ansiCodeToString(AnsiCode code) {
  return "\033[" + std::to_string(static_cast<int>(code)) + "m";
}

class Logger {
 public:
  static void heading(const std::string& msg);
  static void log(const std::string& msg);
  static void warn(const std::string& msg);
  static void error(const std::string& msg);
  static void progress(const std::string& msg, float progress);

 private:
  template <typename... Args>
  static void writeMessage(const std::string& msg, const Args&... modifiers) {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // Write logs
    std::cout << "[" << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "] ";
    (void)std::initializer_list<int>{
        (std::cout << ansiCodeToString(modifiers), 0)...};

    std::cout << msg;
    std::cout << ansiCodeToString(AnsiCode::Reset);
    std::cout << '\n';
  }
};
