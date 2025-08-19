#include "Logger.h"

void Logger::heading(const std::string& msg) {
  writeMessage(msg, AnsiCode::BoldOn, AnsiCode::TextGreen,
               AnsiCode::UnderlineOn);
}

void Logger::log(const std::string& msg) {
  writeMessage(msg);
}

void Logger::warn(const std::string& msg) {
  writeMessage(msg, AnsiCode::TextYellow);
}

void Logger::error(const std::string& msg) {
  writeMessage(msg, AnsiCode::TextRed, AnsiCode::BoldOn);
}

void Logger::progress(const std::string& msg, float progress) {
  std::string paddedProgress = std::to_string(static_cast<int>(progress * 100));

  while (paddedProgress.size() < 3) {
    paddedProgress = " " + paddedProgress;  // Pad with spaces
  }

  const std::string progressBar = " [" + paddedProgress + "%] " + msg;
  writeMessage(progressBar, AnsiCode::TextWhite);
}