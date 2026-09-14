/*
    CDLogger
    Copyright (c) 2015-2026, Chris Desjardins
    https://github.com/cdesjardins/ComBomb cjd@chrisd.info

    SPDX-License-Identifier: BSD-3-Clause
    See the LICENSE file at the project root for the full license text.
*/

/*
** The core idea for this logger was from:
** http://vilipetek.com/2014/04/17/thread-safe-simple-logger-in-c11/
*/
#ifndef CD_LOGGER_Hxx
#define CD_LOGGER_Hxx

#include <sstream>
#include <mutex>
#include <memory>
#include <fstream>
#include <map>
#include <vector>

#define cdLog(level) if (Logger::isLogging(level) == false) {} else Logger::getLogger()(level, __FILE__)

enum class LogLevel { Debug, Info, Error, maxlevels };
class Logger;
class LoggerData;

class LogStream : public std::ostringstream
{
public:
    ~LogStream();
private:
    LogStream(Logger& logger, LogLevel level, const std::string& tag)
        : _logger(logger),
        _level(level),
        _tag(tag)
    {
    }

    LogStream(const LogStream& other)
        : std::ios(), std::ostringstream(),
        _logger(other._logger),
        _level(other._level),
        _tag(other._tag)
    {
    }

    friend class Logger;
    Logger& _logger;
    LogLevel _level;
    std::string _tag;
};

class Logger
{
public:
    static Logger& getLogger()
    {
        return _logger;
    }

    Logger();
    virtual ~Logger();

    void log(LogLevel level, const std::string& tag, const std::string& msg);

    LogLevel getMinLogLevel()
    {
        return _minLogLevel;
    }

    void setMinLogLevel(LogLevel minLogLevel)
    {
        _minLogLevel = minLogLevel;
    }

    void addStream(const std::string& filename);
    // To add stdout as a stream:
    // addStream(std::shared_ptr<std::ostream>(&std::cout, [](void*) {}))
    void addStream(const std::shared_ptr<std::ostream>& stream);
    void stopLogging();

    LogStream operator()(LogLevel level, const std::string& tag);
    static bool isLogging(LogLevel level);
private:

    static Logger _logger;
    std::mutex _mutex;
    std::vector<std::shared_ptr<LoggerData> > _outStreams;
    LogLevel _minLogLevel;
    std::map<LogLevel, std::string> _levelNames;
};

#endif
