//
// Created by entropy on 10/1/18.
//

#include "Logger.h"

Logger *Logger::_instance = nullptr;

Logger::Logger()
{

}

Logger::Logger(const Logger &logger)
{

}

Logger* Logger::get()
{
    if (_instance == nullptr)
        _instance = new Logger();
    return _instance;
}

void Logger::debug(const BabelString &message)
{
    std::cout << "[DEBUG] " << message << std::endl;
}

void Logger::info(const BabelString &message)
{
    std::cout << "[INFO] " << message << std::endl;
}

void Logger::warn(const BabelString &message)
{
    std::cout << "[WARN] " << message << std::endl;
}

void Logger::error(const BabelString &message)
{
    std::cout << "[ERROR] " << message << std::endl;
}

Logger& Logger::operator=(const Logger &)
{
    return *this;
}