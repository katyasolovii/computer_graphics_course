//
// Created by Andrii Krenevych on 12.06.2025.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#ifndef NDEBUG // в debug-режимі

#include <fstream>
#include <mutex>

#endif

namespace Renderer {
    class Logger {
    public:
        static Logger& log();

        Logger(const Logger&) = delete;

        Logger& operator=(const Logger&) = delete;

        void d(const std::string& message);

        void d(std::string&& message);

        void e(const std::string& message);

        void e(std::string&& message);

    private:
        Logger();

        ~Logger();


#ifndef NDEBUG // в debug-режимі
        std::ofstream mOut;
        std::mutex mMutex;
#endif

    };
} // Renderer

#endif //LOGGER_H
