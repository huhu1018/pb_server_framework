#include "log.h"
using namespace pb;

void test(){
    LOG_DEBUG_ROOT << "DEBUG!";
}


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
    
    pb::root->addAppender(stdApd);
    pb::root->addAppender(fileApd);
    test();
    LOG_DEBUG_ROOT << "DEBUG!";
    LOG_INFO_ROOT << "INFO!";
    LOG_WARN_ROOT << "WARN!";
    LOG_ERROR_ROOT << "ERROR!";
    LOG_FATAL_ROOT << "FATAL!";
    return 0;
}
