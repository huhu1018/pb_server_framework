#include "log.h"
using namespace pb;

int main(int argc,char** argv){

    LogEvent::ptr event(new LogEvent(LogLevel::WARN,0, 1, 2, 3,4, time(0)));
    Logger::ptr lg(new Logger("XYZ"));

    lg->log(event); //日志输出模拟
    lg->setLevel(LogLevel::ERROR);
    lg->log(event);
    return 0;
}
