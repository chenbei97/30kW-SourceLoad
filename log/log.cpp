#include "log.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Log::Log(QObject *parent) : QObject(parent)
{
    mCounter = 0;
    mExecute = false;
    mLogEnabled = false;
    mPeriodMultiple = 30; // 9s/300ms=9000ms/300ms=30

    mFixedLogProcessor = new LogFixedProcessor;
    mMonitorLogProcessor = new LogMonitorProcessor;
    mTerminalLogProcessor = new LogTerminalProcessor;

    //QThread * mFixedLogThread = new QThread; // 跨线程的话下方的信号和槽无法立即反应
    //mFixedLogProcessor->moveToThread(mFixedLogThread);
    connect(this,&Log::logFileFormat,mFixedLogProcessor,&LogFixedProcessor::logFileFormat);
    connect(this,&Log::logFileFormat,mMonitorLogProcessor,&LogMonitorProcessor::logFileFormat);
    connect(this,&Log::logFileFormat,mTerminalLogProcessor,&LogTerminalProcessor::logFileFormat);
}

void Log::setEnabled(bool isLogEnabled)
{
    mLogEnabled = isLogEnabled;
}

bool Log::isEnabled() const
{
    return mLogEnabled;
}

void Log::increaseCounter()
{
     ++mCounter;
    checkCounter();
}

void Log::checkCounter()
{
    if (mCounter == mPeriodMultiple) // 一个日志周期记录1次
    {
        mCounter = 0;
        setExecute(true);
    }
}

void Log::setExecute(bool ret)
{
    mExecute = ret;
}

bool Log::isExecute() const
{
    return mExecute;
}

void Log::setInterval(quint32 ms)
{
    mPeriodMultiple = ms / 300;
}

QString Log::logDir() const
{
    return mFixedLogProcessor->logDir(); // 任何一个均可,都是一样的
}

void Log::reset() // disconnect中使用
{
    mCounter = 0;
    mExecute = false;
}

void Log::generateFixedQueryLog(const QList<QByteArray> & data)
{
    // data = t + v + i + p +r
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh::mm::ss::zzz");
    QStringList logData;
    logData << now;
    foreach(QByteArray d, data) logData<<QString::fromLocal8Bit(d);
    //qDebug()<<"fixed log data = " << logData;// fixed log data =  ("2022/12/26-14::47::34::030", "0.334", "2.78", "45.3")
    mFixedLogProcessor->generateLogFile(logData);
}

void Log::generateMonitorQueryLog(const QStringList& data)
{
   // data = t + role + preset + actual
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh::mm::ss::zzz");
    QStringList logData;
    logData << now << data;
    //qDebug()<<"monitor log data = " << logData;//monitor log data =  ("2022/12/27-11::53::59::110", "源内阻", "0.900", "0.9")
    mMonitorLogProcessor->generateLogFile(logData);
}

void Log::generateTerminalQueryLog(const QStringList& data)
{
    // data = t + command + response
    QString now = QDateTime::currentDateTime().toString("yyyy/MM/dd-hh::mm::ss::zzz");
    QStringList logData;
    logData << now << data;
    //qDebug()<<"terminal log data = " << logData;
    mTerminalLogProcessor->generateLogFile(logData);
}
