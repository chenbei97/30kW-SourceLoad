#ifndef QUERY_H
#define QUERY_H

#include <QIODevice>
#include <QCoreApplication>
#include <QDebug>
#include "../core/data_processing.h"
#include "../core/spic_command.h"

using namespace SPICCOMAND;

class Query : public QObject
{
    Q_OBJECT
public:
    explicit Query(QObject*parent = Q_NULLPTR);
    virtual ~Query();

    virtual void fixedQuery() ;
    virtual void monitorQuery(MONITORROLE,const QByteArray&);
    virtual void terminalQuery(const QByteArray&);

    virtual void setDevice(QIODevice*);
    virtual QIODevice* device() const;

    virtual bool isOpen();

    static void setController(ResponseProcessor::Controller);
    static ResponseProcessor::Controller controller();

protected:
    QIODevice * mDevice;
    static ResponseProcessor * mProcessor;
    static const quint32 mQueryInterval = 300;
    static ResponseProcessor::Controller mController;
signals:
    void setMonitorRole(MONITORROLE,const QByteArray&);
    void errorCode(TIMEOUTCODE);
    void fixedQueryData(const QList<QByteArray>&);
    void monitorQueryData(MONITORROLE,bool);
    void monitorQueryData(MONITORROLE,const QByteArray&,const QByteArray&);
    void terminalQueryData(const QByteArray&,const QByteArray&);
};

#endif // QUERY_H
