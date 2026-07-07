/*******************************************************************************
 *
 * Copyright (c) 2026 Yihang Yang
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 ******************************************************************************/
#include "logging.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace phydb {
namespace {

std::ofstream g_log_file;
std::mutex g_log_mutex;
severity g_min_severity = severity::info;
bool g_disable_log_prefix = false;

bool ShouldLog(severity level) {
  return static_cast<int>(level) >= static_cast<int>(g_min_severity);
}

std::string FindAvailableLogFileName() {
  constexpr int kUpperLimit = 2048;
  for (int i = 0; i < kUpperLimit; ++i) {
    std::string file_name = "phydb" + std::to_string(i) + ".log";
    if (!std::filesystem::exists(file_name)) {
      return file_name;
    }
  }
  return "phydb_out_of_bounds.log";
}

std::string CurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t current_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_time{};
  localtime_r(&current_time, &local_time);

  std::ostringstream timestamp;
  timestamp << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
  return timestamp.str();
}

std::string LogPrefix(severity level) {
  std::ostringstream prefix;
  prefix << "[" << CurrentTimestamp() << "] [" << std::this_thread::get_id()
         << "] [" << SeverityName(level) << "] ";
  return prefix.str();
}

}  // namespace

const char* SeverityName(severity level) {
  switch (level) {
    case severity::trace:
      return "trace";
    case severity::debug:
      return "debug";
    case severity::info:
      return "info";
    case severity::warning:
      return "warning";
    case severity::error:
      return "error";
    case severity::fatal:
      return "fatal";
  }
  return "unknown";
}

LogMessage::LogMessage(severity level) : level_(level) {}

LogMessage::~LogMessage() {
  if (!ShouldLog(level_)) {
    return;
  }

  std::lock_guard<std::mutex> lock(g_log_mutex);
  const std::string message = stream_.str();
  std::cout << message;
  std::cout.flush();

  if (g_log_file.is_open()) {
    if (!g_disable_log_prefix) {
      g_log_file << LogPrefix(level_);
    }
    g_log_file << message;
    g_log_file.flush();
  }
}

std::ostream& LogMessage::Stream() { return stream_; }

severity IntToLoggingLevel(int level) {
  switch (level) {
    case 0:
      return severity::fatal;
    case 1:
      return severity::error;
    case 2:
      return severity::warning;
    case 3:
      return severity::info;
    case 4:
      return severity::debug;
    case 5:
      return severity::trace;
    default:
      std::cout << "Invalid phydb verbosity level (0-5)!\n";
      exit(1);
  }
}

severity StrToLoggingLevel(const std::string& severity_level_str) {
  int level;
  try {
    level = std::stoi(severity_level_str);
  } catch (...) {
    std::cout << "Invalid phydb verbosity level (0-5)!\n";
    exit(1);
  }

  return IntToLoggingLevel(level);
}

void InitLogging(const std::string& log_file_name, severity severity_level,
                 bool disable_log_prefix) {
  CloseLogging();

  g_min_severity = severity_level;
  g_disable_log_prefix = disable_log_prefix;

  const std::string file_name =
      log_file_name.empty() ? FindAvailableLogFileName() : log_file_name;
  g_log_file.open(file_name, std::ios::out);
  if (!g_log_file.is_open()) {
    std::cerr << "Failed to open log file: " << file_name << "\n";
  }
}

void CloseLogging() {
  std::lock_guard<std::mutex> lock(g_log_mutex);
  if (g_log_file.is_open()) {
    g_log_file.close();
  }
}

}  // namespace phydb
