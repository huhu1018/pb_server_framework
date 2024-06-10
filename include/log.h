#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>


#define ROOT ::pb::root

#define LOG_LEVEL(logger, level)                                               \
  		if (logger->getLevel() <= level)                                       \
 			 LogEventWrap(logger, LogEvent::ptr(new LogEvent(                  \
                           level, __FILE__, __LINE__,                          \
                           syscall(SYS_gettid), 0, time(0))))                  \
      					   .getSS()


#define LOG_DEBUG(logger) LOG_LEVEL(logger, LogLevel::DEBUG)
#define LOG_INFO(logger) LOG_LEVEL(logger, LogLevel::INFO)
#define LOG_WARN(logger) LOG_LEVEL(logger, LogLevel::WARN)
#define LOG_ERROR(logger) LOG_LEVEL(logger, LogLevel::ERROR)
#define LOG_FATAL(logger) LOG_LEVEL(logger, LogLevel::FATAL)


#define LOG_DEBUG_ROOT LOG_DEBUG(ROOT)
#define LOG_INFO_ROOT LOG_INFO(ROOT)
#define LOG_WARN_ROOT LOG_WARN(ROOT)
#define LOG_ERROR_ROOT LOG_ERROR(ROOT)
#define LOG_FATAL_ROOT LOG_FATAL(ROOT)



namespace pb{

class LogLevel{
public:
    enum Level{
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    static std::string ToString(LogLevel::Level level);
};


class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(LogLevel::Level level
            ,const char* file, int32_t m_line, 
            uint32_t thread_id, uint32_t fiber_id, uint64_t time);
    void setLogger(std::string logName) { m_logName = logName;}
    const char* getFile() const { return m_file;}
    int32_t getLine() const { return m_line;}
    uint32_t getThreadId() const { return m_threadId;}
    uint32_t getFiberId() const { return m_fiberId;}
    uint64_t getTime() const { return m_time;}
    LogLevel::Level getLevel() const { return m_level;}
    std::string getContent() const { return m_ss.str();}
    std::stringstream& getSS() { return m_ss;}
    const std::string& getLogName() const { return m_logName;}
private:
    const char* m_file = nullptr;  //文件名
    int32_t m_line = 0;            //行号
    uint32_t m_threadId = 0;       //线程id
    uint32_t m_fiberId = 0;        //协程id
    uint64_t m_time = 0;           //时间戳
    LogLevel::Level m_level;	   //日志级别	
    std::stringstream m_ss;        //字符流
    std::string m_logName;         //日志器名称
};


class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}
	virtual void log(LogEvent::ptr event) = 0;
    virtual LogLevel::Level getLevel() = 0;
};


//输出到控制台
class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogEvent::ptr event) override;
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() override { return m_level; }
private:
    LogLevel::Level m_level = LogLevel::DEBUG;
};


//输出到文件
class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(LogEvent::ptr event) override;
    void setLevel(LogLevel::Level level) { m_level = level; }
    LogLevel::Level getLevel() override { return m_level; }

    bool open();
private:
    std::string m_filename;
    std::ofstream m_filestream;
    LogLevel::Level m_level = LogLevel::DEBUG;
};



class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string& name = "root");
    const std::string& getName() const { return m_name; }
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) {m_level = val;}
    void log(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
private:
    std::string m_name;
    LogLevel::Level m_level; // 大于等于该日志级别才能输出
    std::list<LogAppender::ptr> m_appenders;
};


class LogEventWrap {
public:
    LogEventWrap(Logger::ptr logger, LogEvent::ptr e)
        :m_logger(logger), m_event(e) {}
    ~LogEventWrap() {
        m_logger->log(m_event);
    }
    std::stringstream& getSS() { return m_event->getSS(); }
    LogEvent::ptr getEvent() const {return m_event;}
private:
    Logger::ptr m_logger;
    LogEvent::ptr m_event;
};


extern Logger::ptr root;

} // namespace pb