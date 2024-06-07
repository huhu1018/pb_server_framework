#include "log.h"
using namespace pb;

int main(int argc,char** argv){
    
    Logger::ptr lg(new Logger("PB"));

    StdoutLogAppender::ptr stdApd(new StdoutLogAppender());
    //控制台只输出警告以上日志
    stdApd->setLevel(LogLevel::WARN);
    FileLogAppender::ptr fileApd(new FileLogAppender("../log/log.txt"));
    //文件中输出所有日志
    fileApd->setLevel(LogLevel::DEBUG);

    lg->addAppender(stdApd);
    lg->addAppender(fileApd);
    
    LOG_DEBUG(lg) << "DEBUG!";
    LOG_INFO(lg) << "INFO!";
    LOG_WARN(lg) << "WARN!";
    LOG_ERROR(lg) << "ERROR!";
    LOG_FATAL(lg) << "FATAL!";
    return 0;
}
