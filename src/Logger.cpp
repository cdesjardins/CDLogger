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

#include "Logger.h"
#include <iomanip>
#include <cassert>
#include <chrono>

#ifdef WIN32
#define localtime_r(_Time, _Tm) localtime_s(_Tm, _Time)
#endif

Logger Logger::_logger;
bool s_cdLogging = false;

class LoggerData
{
public:
    LoggerData() = delete;
    LoggerData(const LoggerData&) = delete;

    LoggerData(const std::string& filename)
        : _stream(new std::ofstream(filename))
    {
    }

    LoggerData(const std::shared_ptr<std::ostream>& stream)
        : _stream(stream)
    {
    }

    ~LoggerData()
    {
        _stream->flush();
        _stream.reset();
    }

    void log(const std::string levelName, const std::string& tag, const std::string& msg)
    {
        tm localTime;
        std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
        time_t now = std::chrono::system_clock::to_time_t(t);
        localtime_r(&now, &localTime);

        const std::chrono::duration<double> tse = t.time_since_epoch();
        std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() %
                                                 1000;

        *_stream << (1900 + localTime.tm_year) << '-'
                 << std::setfill('0') << std::setw(2) << (localTime.tm_mon + 1) << '-'
                 << std::setfill('0') << std::setw(2) << localTime.tm_mday << ' '
                 << std::setfill('0') << std::setw(2) << localTime.tm_hour << ':'
                 << std::setfill('0') << std::setw(2) << localTime.tm_min << ':'
                 << std::setfill('0') << std::setw(2) << localTime.tm_sec << '.'
                 << std::setfill('0') << std::setw(3) << milliseconds << " ("
                 << levelName << "/" << tag << ") " << msg << std::endl;
    }

    void flush()
    {
        _stream->flush();
    }

private:
    std::shared_ptr<std::ostream> _stream;
};

Logger::Logger()
    : _minLogLevel(LogLevel::Info)
{
    _levelNames[LogLevel::Debug] = "Debug";
    _levelNames[LogLevel::Info] = "Info";
    _levelNames[LogLevel::Error] = "Error";
    assert(_levelNames.size() == (size_t)LogLevel::maxlevels);
    s_cdLogging = true;
}

Logger::~Logger()
{
    s_cdLogging = false;
    stopLogging();
}

LogStream Logger::operator()(LogLevel level, const std::string& tag)
{
    std::string t = tag;
    size_t index = tag.find_last_of("/\\");
    if (index != std::string::npos)
    {
        t = tag.substr(index + 1);
    }
    return LogStream(*this, level, t);
}

void Logger::log(LogLevel level, const std::string& tag, const std::string& msg)
{
    std::unique_lock<std::mutex> lock(_mutex);
    for (std::shared_ptr<LoggerData> it : _outStreams)
    {
        it->log(_levelNames[level], tag, msg);
    }
}

void Logger::addStream(const std::string& filename)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _outStreams.push_back(std::shared_ptr<LoggerData>(new LoggerData(filename)));
}

void Logger::addStream(const std::shared_ptr<std::ostream>& stream)
{
    std::unique_lock<std::mutex> lock(_mutex);
    _outStreams.push_back(std::shared_ptr<LoggerData>(new LoggerData(stream)));
}

void Logger::stopLogging()
{
    for (std::shared_ptr<LoggerData> it : _outStreams)
    {
        it->flush();
    }
    _outStreams.clear();
}

bool Logger::isLogging(LogLevel level)
{
    return ((s_cdLogging == true) && (level >= Logger::getLogger().getMinLogLevel())) ? true : false;
}

LogStream::~LogStream()
{
    _logger.log(_level, _tag, str());
}
