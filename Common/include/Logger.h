#pragma once
#include <string>

namespace Cocorium {
    class Logger {
    public:
        static void Init(const std::string& filename);
        static void Info(const std::string& context, const std::string& message);
        static void Warn(const std::string& context, const std::string& message);
        static void Error(const std::string& context, const std::string& message);
        static void Critical(const std::string& context, const std::string& message);
    };
}
