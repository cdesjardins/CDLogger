/*
    CDLogger
    Copyright (C) 2015  Chris Desjardins
    http://blog.chrisd.info cjd@chrisd.info

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#define cdLog(level) if (level < Logger::getLogger().getMinLogLevel()) {} else Logger::getLogger()(level, LOG_TAG)

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
        : _logger(other._logger),
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
    void addStream(std::ostream* stream);

    LogStream operator()(LogLevel level, const std::string& tag);

private:
    void getLocalTime(tm* localTime);

    static Logger _logger;
    std::mutex _mutex;
    std::vector<std::shared_ptr<LoggerData> > _outStreams;
    LogLevel _minLogLevel;
    std::map<LogLevel, std::string> _levelNames;
};

#endif
