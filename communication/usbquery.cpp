#include "usbquery.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

UsbQuery::UsbQuery(Query * parent):Query(parent)
{

}

void UsbQuery::setDevice(QIODevice * device)
{
        mPort = static_cast<QSerialPort*>(device);
        mDevice = mPort;
        connect(mPort,&QSerialPort::readyRead,this,[=]{
            emit errorCode(USBERROR::UsbNoError);
            if (mPort->canReadLine())
            {
               QByteArray data = mPort->readAll();
               mProcessor->setData(data);
               mProcessor->run();
            }
        });
}

void UsbQuery::fixedQuery()
{
        if (!isOpen()) return;
        Query::fixedQuery();
}

void UsbQuery::monitorQuery(MONITORROLE role ,const QByteArray& value)
{
        if (!isOpen()) return;
        Query::monitorQuery(role,value);
}

void UsbQuery::terminalQuery(const QByteArray & c)
{
        if (!isOpen()) return;
        Query::terminalQuery(c);
}

bool UsbQuery::isOpen()
{
    if (!mPort->isOpen()) {
        quint32 index = static_cast<quint32>(mPort->error());
        emit errorCode(USBERROR(index));
        return false;
    }
    return true;
}
