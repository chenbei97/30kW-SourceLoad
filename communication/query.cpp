#include "query.h"

ResponseProcessor * Query::mProcessor = new ResponseProcessor;
ResponseProcessor::Controller Query::mController  = ResponseProcessor::Controller::MonitorQuery;

Query::Query(QObject * parent) : QObject(parent)
{
        mDevice = Q_NULLPTR;
        mProcessor->setController(mController);
        connect(this,&Query::setMonitorRole,mProcessor,&ResponseProcessor::updateMonitorRole);
        connect(mProcessor,&ResponseProcessor::fixedQuery,this,&Query::fixedQueryData);
        connect(mProcessor,SIGNAL(monitorQuery(MONITORROLE,bool)),this,SIGNAL(monitorQueryData(MONITORROLE,bool)));
        connect(mProcessor,SIGNAL(monitorQuery(MONITORROLE,const QByteArray&,const QByteArray&)),
                this,SIGNAL(monitorQueryData(MONITORROLE,const QByteArray&,const QByteArray&)));
        connect(mProcessor,&ResponseProcessor::terminalQuery,this,&Query::terminalQueryData);
}

Query::~Query()
{
     if (mProcessor != Q_NULLPTR){
         delete mProcessor;
         mProcessor = Q_NULLPTR;
     }
     if (mDevice != Q_NULLPTR) {
         if (!mDevice->isOpen())
             mDevice->close();
         delete mDevice;
         mDevice = Q_NULLPTR;
     }
}

void Query::fixedQuery()
{
    if (mDevice == Q_NULLPTR) return;
    if (!isOpen()) return;
    //mDevice->write(SPICCOMAND::MEAS_VCPR);
    //QMutexLocker locker(&mMutex); // 这里也不要加锁,否则set和fixed相互竞争导致界面卡死
    mDevice->write("FETCH:VOLT?;:FETCH:CURR?;:FETCH:POW?\n");
    if (mDevice->waitForBytesWritten(mQueryInterval/3)) // 请求正常
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents); // 解决监控界面轻微卡顿现象
        emit errorCode(TIMEOUTCODE::RequestNormal);
        if (mDevice->waitForReadyRead(mQueryInterval)) //  回复正常
        {
              QCoreApplication::processEvents(QEventLoop::AllEvents); // 解决监控界面轻微卡顿现象
              emit errorCode(TIMEOUTCODE::ReceiveNormal);
        }
        else {emit errorCode(TIMEOUTCODE::ReceiveTimeout);}// 回复超时
    }
    else
    {
        emit errorCode(TIMEOUTCODE::RequestTimeout); // 请求超时
        emit errorCode(TIMEOUTCODE::ReceiveTimeout);
    }
}

void Query::monitorQuery(MONITORROLE role, const QByteArray & value)
{
    if (mDevice == Q_NULLPTR) return;
    if (!isOpen()) return;
    QByteArray c = getSetQueryCommand(role,value);
    //QMutexLocker locker(&mMutex); // 这里也不要加锁,否则disconnect时候异常退出
    mDevice->write(c);
    mProcessor->addRequestCount(ResponseProcessor::Controller::MonitorQuery); // 请求次数增加1次
}

void Query::terminalQuery(const QByteArray & c)
{
    if (mDevice == Q_NULLPTR) return;
    if (!isOpen()) return;
    mProcessor->setTerminalCommand(c); // 为了把terminal的命令和返回值合并到一起作为terminalQueryData发出去
    mDevice->write(c+"\n");//这个传来的c没有换行符
    mProcessor->addRequestCount(ResponseProcessor::Controller::TerminalQuery); // 请求次数增加1次
}

bool Query::isOpen()
{
    if (!mDevice->isOpen()) {
        return false;
    }
    return true;
}

void Query::setDevice(QIODevice*device)
{
    mDevice = device;
}

QIODevice* Query::device() const
{
    return mDevice;
}

void Query::setController(ResponseProcessor::Controller controller)
{
    mController = controller;
    mProcessor->setController(mController);//也要同步更新
}

ResponseProcessor::Controller Query::controller()
{
    return mController;
}
