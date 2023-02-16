#ifndef LOGMONITORPROCESSOR_H
#define LOGMONITORPROCESSOR_H

#include "logprocessor.h"
class LogMonitorProcessor : public LogProcessor
{
    Q_OBJECT
public:
    explicit LogMonitorProcessor(LogProcessor*parent = Q_NULLPTR);
    void generateLogFile(const QStringList &) override;
private:
    QStringList mHeader;
};
#endif // LOGMONITORPROCESSOR_H
