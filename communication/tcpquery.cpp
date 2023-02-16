#include "tcpquery.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

TcpQuery::TcpQuery(Query *parent) : Query(parent)
{

}

void TcpQuery::setDevice(QIODevice * device)
{
        mTcpSocket = static_cast<QTcpSocket*>(device);
        mDevice = mTcpSocket;

        connect(mTcpSocket,&QTcpSocket::readyRead,this,[=]{
            emit errorCode(TCPERROR::TcpNoError);
           if (mTcpSocket->canReadLine()) // 确保读取1行
           {
               QByteArray data = mTcpSocket->readAll();
               mProcessor->setData(data);
               mProcessor->run();
           }
        });
}

void TcpQuery::fixedQuery()
{
        if (!isOpen()) return;
        Query::fixedQuery();
}

void TcpQuery::monitorQuery(MONITORROLE role,const QByteArray& value)
{
        if (!isOpen()) return;
        Query::monitorQuery(role,value);
}

void TcpQuery::terminalQuery(const QByteArray & c)
{
        if (!isOpen()) return;
        Query::terminalQuery(c);
}

bool TcpQuery::isOpen()
{
        if (!mTcpSocket->isOpen()){
            // 要注意,TCPERROR比QAbstractSocket::SocketError多1个TcpNpError,错位对应关系
            quint32 index = static_cast<quint32>(mTcpSocket->error());
            emit errorCode(TCPERROR(index+1)); // index+1
            return false;
        }
        return true;
}
