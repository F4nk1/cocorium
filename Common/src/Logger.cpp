#include "Logger.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>
#include <iomanip>

namespace Cocorium {

    static std::ofstream logFile;
    static std::mutex logMutex;

    void Logger::Init(const std::string& filename) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.close();
        }
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    static void LogInternal(const std::string& level, const std::string& context, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        
        // Formatear la fecha/hora
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        if (logFile.is_open()) {
            logFile << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
                    << "[" << level << "] "
                    << "[" << context << "] " << message << "\n";
            logFile.flush();
        }
        
        // También imprimir a la consola para debug rápido
        if (level == "ERROR" || level == "CRITICAL") {
            std::cerr << "[" << level << "] [" << context << "] " << message << std::endl;
        } else {
            std::cout << "[" << level << "] [" << context << "] " << message << std::endl;
        }
    }

    void Logger::Info(const std::string& context, const std::string& message) {
        LogInternal("INFO", context, message);
    }

    void Logger::Warn(const std::string& context, const std::string& message) {
        LogInternal("WARN", context, message);
    }

    void Logger::Error(const std::string& context, const std::string& message) {
        LogInternal("ERROR", context, message);
    }

    void Logger::Critical(const std::string& context, const std::string& message) {
        LogInternal("CRITICAL", context, message);
    }

}
