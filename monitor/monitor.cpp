#include "monitor.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Monitor::Monitor(QWidget *parent) : QMainWindow(parent)
{
        resize(1200,800);
        setFont(QFont("Times New Roman",12)); //格式先初始化再初始化布局才有效果
        setWindowIcon(QIcon(":/images/monitorInterface.png"));
        setWindowFlag(Qt::Window);
}

void Monitor::paintEvent(QPaintEvent *e)
{
      QPainter p;
      p.begin(this);
      p.setOpacity(1.0);
      p.setRenderHint(QPainter::SmoothPixmapTransform);
      QPixmap pix(":/images/serial.jpg");
      pix.scaled(QSize(1200,800));
      p.drawPixmap(QPoint(0,0),pix);
      p.end();
      Q_UNUSED(e);
}

void Monitor::setWorkMode(WORKMODE mode)
{
    mWorkMode = mode;
    QString title; // 延迟初始化,外部必须在初始化后确定工作模式才初始化剩下的工作
    if (mWorkMode == WORKMODE::CV)
    {
          initCVUI();
          title = tr("监控界面(CV模式)");
    }
    else {
          initCCUI();
          title = tr("监控界面(CC模式)");
    }
    setWindowTitle(title);
}

WORKMODE Monitor::workMode() const
{
      return mWorkMode;
}

void Monitor::setMessageState(TIMEOUTCODE code)
{
      QString errorString = translateErrorCode(code);
      if (code == TIMEOUTCODE::RequestNormal || code == TIMEOUTCODE::RequestTimeout) // 字符串变量需要使用QT_TR_NOOP标记翻译
          MessageRequestState->setText(tr("消息请求状态：%1").arg(QT_TR_NOOP(errorString))); // 请求状态
      else MessageResponseState->setText(tr("消息回复状态：%1").arg(QT_TR_NOOP(errorString))); // 回复状态
}

void Monitor::setConnectState(USBERROR error)
{
      QString errorString = translateErrorCode(error);
      ConnectState->setText(tr("串口连接状态：%1").arg(QT_TR_NOOP(errorString)));
}

void Monitor::setConnectState(TCPERROR error)
{
      QString errorString = translateErrorCode(error);
      ConnectState->setText(tr("TCP连接状态：%1").arg(QT_TR_NOOP(errorString)));
}

void Monitor::showMeasureState(const QList<QByteArray> &data)
{
      //Q_ASSERT(data.size() == 3);
      //qDebug()<<"data = "<<data;
      if (data.size() != 3)
      {
          //QMessageBox::critical(this,tr("错误"),tr("串口/TCP数据格式解析错误"));
          qDebug()<<"串口/TCP数据格式解析错误 => "<<data;
          return;
      }
      ShowVoltage->setText(data.at(0)+QStringLiteral(" V"));
      ShowCurrent->setText(data.at(1)+QStringLiteral(" A"));
      ShowPower->setText(data.at(2)+QStringLiteral(" W"));
}

void Monitor::showSetState(MONITORROLE role,bool ret)
{
      QString roleString = translateErrorCode(role);
      QString retString = ret ? tr("设定成功"):tr("设定失败");
      MessageSetState->setText(tr("设定状态：%1%2").arg(QT_TR_NOOP(roleString)).arg(QT_TR_NOOP(retString)));
}

QString Monitor::translateErrorCode(USBERROR code)
{
      QString s = tr("没有错误");
      switch (code)
      {
            case  USBERROR::OpenError: s = tr("打开错误");break;
            case  USBERROR::ReadError: s = tr("读取错误");break;
            case  USBERROR::WriteError: s = tr("写入错误");break;
            case  USBERROR::ParityError: s = tr("奇偶校验错误");break;
            case  USBERROR::FramingError: s = tr("帧错误");break;
            case  USBERROR::NotOpenError: s = tr("未打开错误");break;
            case  USBERROR::TimeoutError: s = tr("超时错误");break;
            case  USBERROR::ResourceError: s = tr("资源错误");break;
            case  USBERROR::PermissionError: s = tr("占用错误");break;
            case  USBERROR::BreakConditionError: s = tr("中断错误");break;
            case  USBERROR::DeviceNotFoundError: s = tr("设备未找到错误");break;
            case  USBERROR::UnsupportedOperationError: s = tr("不支持的操作错误");break;
            case  USBERROR::UnknownError: s = tr("未知错误");break;
            default: break;
      }
      return s;
}

QString Monitor::translateErrorCode(TCPERROR code)
{
      QString s =tr("没有错误") ;
      switch (code)
      {
            case TCPERROR::ConnectionRefusedError:  s = tr("连接被拒绝"); break;
            case TCPERROR::RemoteHostClosedError:s = tr("远程服务器关闭");break;
            case TCPERROR::HostNotFoundError:s = tr("未找到服务器");break;
            case TCPERROR::SocketAccessError:s = tr("套接字没有权限");break;
            case TCPERROR::SocketResourceError:s = tr("套接字资源错误");break;
            case TCPERROR::SocketTimeoutError:s = tr("套接字连接超时");break;
            case TCPERROR::DatagramTooLargeError:s = tr("数据报太大");break;
            case TCPERROR::NetworkError:s = tr("网络错误");break;
            case TCPERROR::AddressInUseError:s = tr("远程服务器已被占用");break;
            case TCPERROR::SocketAddressNotAvailableError:s = tr("套接字地址无效");break;
            case TCPERROR::UnsupportedSocketOperationError:s = tr("不支持的套接字操作");break;
            case TCPERROR::ProxyAuthenticationRequiredError:s = tr("套接字代理需要身份验证");break;
            case TCPERROR::SslHandshakeFailedError:s = tr("SSL/TLS 握手失败");break;
            case TCPERROR::UnfinishedSocketOperationError:s = tr("未完成的套接字操作");break;
            case TCPERROR::ProxyConnectionRefusedError:s = tr("连接代理服务器被拒绝");break;
            case TCPERROR::ProxyConnectionClosedError:s = tr("代理服务器意外关闭");break;
            case TCPERROR::ProxyConnectionTimeoutError:s = tr("连接代理服务器超时");break;
            case TCPERROR::ProxyNotFoundError:s = tr("未找到代理服务器");break;
            case TCPERROR::ProxyProtocolError:s = tr("连接代理服务器失败");break;
            case TCPERROR::OperationError:s = tr("套接字处于不允许的操作");break;
            case TCPERROR::SslInternalError:s = tr("SSL库内部错误");break;
            case TCPERROR::SslInvalidUserDataError:s = tr("无效的SSL证明");break;
            case TCPERROR::TemporaryError:s = tr("临时错误");break;
            case TCPERROR::UnknownSocketError:s = tr("未知错误");break;
            default:break;
      }
      return s;
}

QString Monitor::translateErrorCode(TIMEOUTCODE code)
{
      QString ret = tr("请求超时");
      switch (code)
      {
            case TIMEOUTCODE::ReceiveNormal: ret = tr("回复正常");break;
            case TIMEOUTCODE::RequestNormal: ret = tr("请求正常");break;
            case TIMEOUTCODE::ReceiveTimeout:ret = tr("回复超时");break;
            default: break;
      }
      return ret;
}

QString Monitor::translateErrorCode(MONITORROLE role)
{
      QString ret;
      switch (role)
      {
            case MONITORROLE::PowerSource: ret = tr("源功率");break;
            case MONITORROLE::PowerLoad: ret = tr("载功率");break;
            case MONITORROLE::ResistanceSource:ret = tr("源内阻");break;
            case MONITORROLE::ResistanceLoad: ret = tr("载内阻");break;
            case MONITORROLE::Voltage: ret = tr("电压");break;
            case MONITORROLE::CurrentSource: ret = tr("源电流");break;
            case MONITORROLE::CurrentLoad: ret = tr("载电流");break;
            case MONITORROLE::Current: ret = tr("电流");break;
            case MONITORROLE::VoltageSource: ret = tr("源电压");break;
            case MONITORROLE::VoltageLoad: ret = tr("载电压");break;
            default: break;
      }
      return ret;
}
