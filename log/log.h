#ifndef LOG_H
#define LOG_H

#include "logfixedprocessor.h"
#include "logmonitorprocessor.h"
#include "logterminalprocessor.h"

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = Q_NULLPTR);

    void setEnabled(bool);
    bool isEnabled() const;

    void setExecute(bool);
    bool isExecute() const;

    void increaseCounter();

    void setInterval(quint32);

    QString logDir() const;

    void reset();

    void generateFixedQueryLog(const QList<QByteArray>&);
    void generateMonitorQueryLog(const QStringList&);
    void generateTerminalQueryLog(const QStringList&);

private:
    void checkCounter();
    bool mLogEnabled;
    bool mExecute;
    quint32 mCounter;
    quint32 mPeriodMultiple;
    LogFixedProcessor * mFixedLogProcessor;
    LogMonitorProcessor * mMonitorLogProcessor;
    LogTerminalProcessor * mTerminalLogProcessor;
signals:
    void logFileFormat(FILEFORMAT);
};

#endif // LOG_H
