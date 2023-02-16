#include "logfixedprocessor.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LogFixedProcessor::LogFixedProcessor(LogProcessor*parent):LogProcessor(parent)
{
    mHeader <<"DateTime"<<"Meas-V" <<"Meas-A"<<"Meas-P"<<"Meas-R";

    mLogPath = setFileHeader(mHeader,"fixedQuery"); // 子类必须初始化和随时更新mLogPath
    connect(this,static_cast<void (LogFixedProcessor::*)(FILEFORMAT)>(&LogFixedProcessor::logFileFormat),
            this,[=](FILEFORMAT format){update(format);mLogPath = setFileHeader(mHeader,"fixedQuery");});
}


void LogFixedProcessor::generateLogFile(const QStringList &text)
{
    if (!QDir().exists(mLogPath)){ // 正常会存在,除非应用启动后创建的文件被人为删除
           setFileHeader(mHeader,"fixedQuery"); // 那么重新创建文件/表头
    }
    setFileContent(text);
}
