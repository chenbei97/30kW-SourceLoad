#include "logmonitorprocessor.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LogMonitorProcessor::LogMonitorProcessor(LogProcessor*parent):LogProcessor(parent)
{
    mHeader <<"DateTime"<<"Variable" <<"SetValue"<<"ActualValue";

    mLogPath = setFileHeader(mHeader,"monitorQuery"); // 创建当天日期的日志文件
    connect(this,static_cast<void (LogMonitorProcessor::*)(FILEFORMAT)>(&LogMonitorProcessor::logFileFormat),
            this,[=](FILEFORMAT format){update(format);mLogPath = setFileHeader(mHeader,"monitorQuery");}); // 更新3个变量和logpath
}

void LogMonitorProcessor::generateLogFile(const QStringList &text)
{
    if (!QDir().exists(mLogPath)){ // 正常会存在,除非应用启动后创建的文件被人为删除
           setFileHeader(mHeader,"monitorQuery"); // 那么重新创建文件/表头
    }
    setFileContent(text);
}
