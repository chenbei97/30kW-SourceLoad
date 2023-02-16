#ifndef DATA_PROCESSING_H
#define DATA_PROCESSING_H

#include <QDebug>
#include <QPair>
#include "enum.h"


class ResponseProcessor : public QObject
{
      Q_OBJECT
public:
        explicit ResponseProcessor(QObject * parent = Q_NULLPTR): QObject(parent)
        {
            mData = Q_NULLPTR;
            mMonitorRole = MONITORROLE::None;
            mMonitorValue = "";
            mController = Controller::MonitorQuery;
            mMonitorCounter = mTerminalCounter = {0,0};
        };
        typedef QPair<quint32,quint32> RequestResponseCount;
        enum class Controller {MonitorQuery,TerminalQuery}; // only used in single reply
        void setData(const QByteArray&);
        void setTerminalCommand(const QByteArray&);
        void setController(Controller);
        void addRequestCount(Controller);
        void run();
        void updateMonitorRole(MONITORROLE ,const QByteArray&);
        void updateTerminalRole(TERMINALROLE);
private:
      QByteArray mData;
      QByteArray mCommand;
      MONITORROLE mMonitorRole;
      QByteArray mMonitorValue;
      Controller mController;
      RequestResponseCount mMonitorCounter;
      RequestResponseCount mTerminalCounter;
      void addResponseCount(Controller);
      void processMultiReply(const QByteArray&);
      void processSingleReply(const QByteArray&);
      void processMonitorQuery(const QByteArray&);
      void processTerminalQuery(const QByteArray&);
 signals:
      void fixedQuery(const QList<QByteArray>&);
      void monitorQuery(MONITORROLE,bool);
      void monitorQuery(MONITORROLE,const QByteArray&,const QByteArray&);
      void terminalQuery(const QByteArray&,const QByteArray&);
};

inline void ResponseProcessor::setData(const QByteArray &data)
{
     mData = data;
}

inline void ResponseProcessor::setTerminalCommand(const QByteArray & c)
{
    mCommand = c;
}

inline void ResponseProcessor::addRequestCount(Controller c)
{
    switch (c) {
        case Controller::MonitorQuery: ++mMonitorCounter.first;break;
        case Controller::TerminalQuery: ++mTerminalCounter.first;break;
    }
}

inline void ResponseProcessor::addResponseCount(Controller c)
{
    switch (c) {
        case Controller::MonitorQuery: ++mMonitorCounter.second;break;
        case Controller::TerminalQuery: ++mTerminalCounter.second;break;
    }
}

inline void ResponseProcessor::setController(Controller controller)
{
    mController = controller;
}

inline void ResponseProcessor::updateMonitorRole(MONITORROLE role, const QByteArray& value)
{
     mMonitorRole = role;
     mMonitorValue = value;
     //qDebug()<<"is called"; // 经过测试有个bug在于,不仅点击monitor's spinbox会触发,双击communication和mainwindow都能触发
     // 默认触发的角色是source power, 似乎与monitor中SetPowerSource最先初始化有关,之后双击会和上次spinbox点击的角色一致
     // 这个bug呢解决不了,因为不知道为何双击窗口能触发spinbox的信号,这是Qt本身的问题
     // 但是影响也不大,如果用户双击窗口,就是无意做了一次设定查询的过程,和有意一样本身不会有什么错误
}

inline void ResponseProcessor::run()
{
        while (!mData.isEmpty() && mData.contains('\n')) // 由于线程的缘故很有可能同时读到查询和设定线程回复的数据
        {
            int currEnd = mData.indexOf('\n'); // 返回第1个\n位置
            if(currEnd+1 == 0) mData.clear(); //这一步很重要,没有\n的数据直接抛弃
            // 如果不做处理，while一直卡住,因为remove总是(0,0),那么mData不会为空永远不会跳出
            // 那么外部的IO设备(串口或套接字)内部缓存会累积不能在下次被读取给mData,这样内存就会爆掉
            QByteArray mResponseValue = mData.left(currEnd+1); // 使用left来获取最早的数据,包含\n字符,此时就可以依据是否带分号来区分来自定时查询还是设定查询
            mData.remove(0,currEnd+1);// 拿到一条数据后清理消息队列的对应消息
            //qDebug()<< "处理后的数据 = "<<mResponseValue; // "15.000; 2.000; 75.000\n", "1.3895\n", 分出了2类消息
            if (mResponseValue.contains(';'))  //只有定时查询查询的回复数据才带有分号
                processMultiReply(mResponseValue); // 拆分数据发出去
            else processSingleReply(mResponseValue); // 可能来自monitor也可能terminal
        }
}

inline void ResponseProcessor::processMultiReply(const QByteArray& data)
{
      // 数据形式一般是"-0.00195313; 0.000434875; -1.0049e-06\n", 分号后边有个空格
      // 有时候可能出现"15.000; 2.000; 75.000\n15.000; 2.000; 75.000\n"连发的情况
      // 要把单独的QByteArray格式拆分成QList<QByteArray>格式
      if (data.endsWith('\n')) // 说明是一条完整的命令(一般一定满足)
      {
              QList<QByteArray> datas = data.split('\n'); // 这里是按\n拆分,会多出个空字符串  ("15.000; 2.000; 75.000", "")
              datas.pop_back(); // 把空字符串删掉

              // 处理连发的情况,这是因为测试时发现有时候可能发的比较快导致收到的消息是多条
              if (datas.count() > 1) // 刨除空字符串剩下的消息数量如果比1大说明有多条消息,类似("15.000;2.000;75.000", "15.000;2.000;76.000", "15.000;2.000;77.000", "")
              {
                    // 连发消息只要最新的,其它舍弃
                    QByteArray value = datas.back(); // 因为队列back()是最新的消息
                    QList<QByteArray> values = value.split(';');
                    emit fixedQuery(values); // 最后1条消息拆分的VIPR4个值发出去,给监控界面显示
                    return;
              }
               //  如果只有1条消息,没有连发,
              QByteArray value = datas[0]; // 其实value就是datas,之所以要这样一步,因为datas是const类型不能后边split
              QList<QByteArray> values = value.split(';'); // 不会有空字符串 ("15.000", "2.000", "75.000")
              emit fixedQuery(values);
       }
}

inline void ResponseProcessor::processSingleReply(const QByteArray&data)
{
        addResponseCount(mController);
        switch (mController)
        {
                case Controller::MonitorQuery : processMonitorQuery(data);break;
                case Controller::TerminalQuery : processTerminalQuery(data);break;
                // ... 未来更多功能
        }
}

inline void ResponseProcessor::processMonitorQuery(const QByteArray&data)
{
    if (mMonitorRole == MONITORROLE::None) return;

    qDebug()<<QString("mMonitor => request[%1] response[%2]") // 监控请求和回复的次数是否一致
              .arg(mMonitorCounter.first).arg(mMonitorCounter.second);

    QByteArray d = data; // data="5.000\n" 去除"\n"
    d.chop(1);
    bool ret = qAbs(d.toDouble() - mMonitorValue.toDouble()) < 0.0005; // 浮点数的相等确定方法

    emit monitorQuery(mMonitorRole,ret);
    emit monitorQuery(mMonitorRole,mMonitorValue,d); // 角色+设定值,实际值

    mMonitorRole = MONITORROLE::None; // 这步比较重要,重新设置为None避免无意义的回复被发出去
}

inline void ResponseProcessor::processTerminalQuery(const QByteArray& data)
{
    if (mCommand.isEmpty()) return; // 没有查询就回复的值是没有意义的不要发出去

    qDebug()<<QString("mTerminal=> request[%1] response[%2]") // 监控请求和回复的次数是否一致
              .arg(mTerminalCounter.first).arg(mTerminalCounter.second);

    emit terminalQuery(mCommand,data); // 合并command/response,注意data是带\n的

    mCommand.clear(); // 本次轮询结束清空上次的命令
}
#endif // DATA_PROCESSING_H

