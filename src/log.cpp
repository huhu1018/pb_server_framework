#include "log.h"

namespace pb{

LogEvent::LogEvent(LogLevel::Level level, const char *file, int32_t m_line, uint32_t thread_id, uint32_t fiber_id, uint64_t time)
: m_level(level)
, m_file(file)
, m_line(m_line)
, m_threadId(thread_id)
, m_fiberId(fiber_id)
, m_time(time)
{}

// 默认全部输出
Logger::Logger(const std::string &name) : m_name(name), m_level(LogLevel::DEBUG)
{}

void Logger::log(LogEvent::ptr event)
{
    event->setLogger(this->getName());
    //如果想要查看的级别大于等于当前日志器能查看的级别，那么才进行输出
    if(event->getLevel() >= m_level){
        for(auto &i : m_appenders){
            if(event->getLevel() >= i->getLevel()){
                i->log(event);
            }
        } 
    }
}

void Logger::addAppender(LogAppender::ptr appender)
{
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender)
{
    for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}

void StdoutLogAppender::log(LogEvent::ptr event)
{
    static const std::string format = "%Y-%m-%d %H:%M:%S";

    std::time_t timeStamp = static_cast<std::time_t>(event->getTime());
    std::tm* localTime = std::localtime(&timeStamp);


    std::cout
    << "["
    << std::setw(5) << std::setiosflags(std::ios::right) << std::setfill(' ') << LogLevel::ToString(event->getLevel())
    << "] "
    << std::put_time(localTime, format.c_str()) << " "
    << event->getThreadId() << " "
    << event->getFiberId() << " "
    << "["
    << event->getLogName()
    << "] "
    << event->getFile() << ":" << event->getLine() << " "
    << std::setw(40) << std::setiosflags(std::ios::right) << std::setfill(' ') << event->getContent()
    << std::endl;
}

FileLogAppender::FileLogAppender(const std::string &filename) : m_filename(filename)
{
    if(m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    if(!m_filestream) {
        std::cout << "open file " << m_filename << " failed" << std::endl;
        exit(1);
    }
}

void FileLogAppender::log(LogEvent::ptr event)
{
    static const std::string format = "%Y-%m-%d %H:%M:%S";
    std::time_t timeStamp = static_cast<std::time_t>(event->getTime());
    std::tm* localTime = std::localtime(&timeStamp);
    m_filestream
    << "["
    << std::setw(5) << std::setiosflags(std::ios::right) << std::setfill(' ') << LogLevel::ToString(event->getLevel())
    << "] "
    << std::put_time(localTime, format.c_str()) << " "
    << event->getThreadId() << " "
    << event->getFiberId() << " "
    << "["
    << event->getLogName()
    << "] "
    << event->getFile() << ":" << event->getLine() << " "
    << std::setw(40) << std::setiosflags(std::ios::right) << std::setfill(' ') << event->getContent()
    << std::endl;
}

std::string LogLevel::ToString(LogLevel::Level level)
{
    switch (level)
    {
    case (LogLevel::DEBUG):
        return "DEBUG";
        break;
    case (LogLevel::INFO):
        return "INFO";
        break;
    case (LogLevel::WARN):
        return "WARN";
        break;
    case (LogLevel::ERROR):
        return "ERROR";
        break;
    case (LogLevel::FATAL):
        return "FATAL";
        break;
    default:
        return "UNKNOW";
        break;
    }
}

}
