#ifndef LOGFIXEDPROCESSOR_H
#define LOGFIXEDPROCESSOR_H

#include "logprocessor.h"

class LogFixedProcessor : public LogProcessor
{
    Q_OBJECT
public:
    explicit LogFixedProcessor(LogProcessor*parent = Q_NULLPTR);
    void generateLogFile(const QStringList &) override;
private:
    QStringList mHeader;
};
#endif // LOGFIXEDPROCESSOR_H
