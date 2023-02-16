#ifndef LOGTERMINALPROCESSOR_H
#define LOGTERMINALPROCESSOR_H

#include "logprocessor.h"

class LogTerminalProcessor : public LogProcessor
{
    Q_OBJECT
public:
    explicit LogTerminalProcessor(LogProcessor*parent = Q_NULLPTR);
    void generateLogFile(const QStringList &) override;
private:
    QStringList mHeader;
};

#endif // LOGTERMINALPROCESSOR_H
