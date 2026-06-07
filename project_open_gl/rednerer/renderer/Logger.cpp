//
// Created by Andrii Krenevych on 12.06.2025.
//

#include "Logger.h"
#include <iostream>

namespace Renderer {

    Logger& Logger::log() {
        static Logger instance;
        return instance;
    }

#ifndef NDEBUG // в debug-режимі

    void Logger::d(const std::string& message) {
        std::lock_guard lock(mMutex);

        std::time_t now = std::time(nullptr);
        char timebuf[32];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        mOut << "[" << timebuf << "] " << message << std::endl;
    }

    void Logger::d(std::string&& message) {
        std::lock_guard lock(mMutex);

        std::time_t now = std::time(nullptr);
        char timebuf[32];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        mOut << "[" << timebuf << "] " << message << std::endl;
    }

    void Logger::e(const std::string& message) {
        std::lock_guard lock(mMutex);

        std::time_t now = std::time(nullptr);
        char timebuf[32];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        mOut << "ERROR:"<< std::endl;
        mOut << "[" << timebuf << "] " << message << std::endl;
    }

    void Logger::e(std::string&& message) {
        std::lock_guard lock(mMutex);

        std::time_t now = std::time(nullptr);
        char timebuf[32];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        mOut << "ERROR:"<< std::endl;
        mOut << "[" << timebuf << "] " << message << std::endl;
    }

    Logger::Logger() {
        mOut.open("log.txt", std::ios::app);
        if (!mOut.is_open()) {
            std::cerr << "Не вдалося відкрити log.txt\n";
        }
    }

    Logger::~Logger() {
        if (mOut.is_open()) {
            mOut.close();
        }
    }

#else

    Logger::Logger() = default;

    Logger::~Logger() = default;

    void Logger::d(const std::string& message) {
    }

    void Logger::d(std::string&& message) {
    }

    void Logger::e(const std::string& message) {
    }

    void Logger::e(std::string&& message) {
    }

#endif

} // Renderer
