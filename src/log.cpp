#include "log.h"

namespace pb{

LogEvent::LogEvent(LogLevel::Level level, const char *file, int32_t m_line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id, uint64_t time)
: m_level(level), m_file(file), m_line(m_line), m_elapse(elapse), m_threadId(thread_id), m_fiberId(fiber_id), m_time(time) {}

// 默认全部输出
Logger::Logger(const std::string &name) : m_name(name), m_level(LogLevel::DEBUG)
{}

void Logger::log(LogEvent::ptr event)
{
    //如果想要查看的级别大于等于当前日志器能查看的级别，那么才进行输出
    if(event->getLevel() >= m_level){
        std::cout << "日志输出模拟" << std::endl; 
    }
}

}





