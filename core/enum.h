#ifndef ENUM_H
#define ENUM_H
#include <QMetaType>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

enum class LANUAGE
{
        CN,
        EN
};

enum class WORKMODE
{
        CC,
        CV
};

enum class FILEFORMAT
{
        CSV,
        TXT
};

enum class TIMEOUTCODE
{
    ReceiveTimeout,
    RequestTimeout,
    ReceiveNormal,
    RequestNormal
};

enum class USBERROR // 和QSerialPort::SerialPortError保持一致
{
      UsbNoError = 0,
      DeviceNotFoundError = 1,
      PermissionError = 2,
      OpenError = 3,
      NotOpenError = 13,
      ParityError = 4,
      FramingError = 5,
      BreakConditionError = 6,
      WriteError = 7,
      ReadError = 8,
      ResourceError = 9,
      UnsupportedOperationError = 10,
      TimeoutError = 12,
      UnknownError = 11
};

enum class TCPERROR // 和QAbstractSocket::SocketError一致
{
     TcpNoError, // 没有错误

     ConnectionRefusedError , // 连接被拒绝
     RemoteHostClosedError, // 远程服务器关闭
     HostNotFoundError , // 未找到服务器
     SocketAccessError, // 套接字没有权限
     SocketResourceError, // 套接字资源错误

     SocketTimeoutError , // 套接字连接超时错误
     DatagramTooLargeError, // 数据报太大
     NetworkError, // 网络错误
     AddressInUseError, // 远程服务器已被占用
     SocketAddressNotAvailableError, // IP地址错误

     UnsupportedSocketOperationError , // 不支持的套接字操作(例如缺乏IPV6支持)
     UnfinishedSocketOperationError , // 未完成的套接字操作（在QAbstractSocket::SocketError中的值11）
     ProxyAuthenticationRequiredError , // 套接字代理需要身份验证
     SslHandshakeFailedError , // SSL/TLS 握手失败
     ProxyConnectionRefusedError , // 连接代理服务器被拒绝

     ProxyConnectionClosedError, // 与代理服务器的意外关闭
     ProxyConnectionTimeoutError, // 与代理服务器连接超时
     ProxyNotFoundError, // 未找到代理
     ProxyProtocolError, // 与代理服务器的连接协商失败
     OperationError, // 套接字处于不允许的状态

     SslInternalError , // SSL库内部错误
     SslInvalidUserDataError, // 无效的SSL证明
     TemporaryError, // 临时错误
     UnknownSocketError, // 未知错误
};

enum class MONITORROLE
{
    PowerSource,
    PowerLoad,

    ResistanceSource,
    ResistanceLoad,

    Voltage,
    CurrentSource,
    CurrentLoad,

    Current,
    VoltageSource,
    VoltageLoad,

    None
};

enum class TERMINALROLE // 和terminal的支持的命令项保持一致,前缀_表示非询问,共188个
{
    _RST, // 16个IEEE通用命令
    _CLS,
    _TRG,
    _WAL,
    _IDN,
    _TST,
    _ESR,
    _STB,
    SRE,
    PSC,
    ESE,
    _OPC,
    OPC,
    _FORM_ASC,
    _FORM_BIN,
    FORM,
    _SYST_POS_SAV0,// System子系统 18
    _SYST_POS_RST,
    SYST_POS,
    SYST_VERS,
    SYST_ERR,
    _SYST_CLE,
    _SYST_REM,
    _SYST_LOC,
    _SYST_RWL,
    SYST_DATE, // 10
    SYST_TIME,
    SYST_BEEP,
    SYST_COMM_SEL,
    SYST_COMM_SER_BAUD,
    SYST_COMM_LAN_IP,
    SYST_COMM_LAN_IP_MODE,
    SYST_COMM_LAN_SMAS,
    SYST_COMM_LAN_DGAT, //18
    SOUR_FUNC, // SOURce子系统 105
    SOUR_CURR_LIM,
    SOUR_CURR_LIM_DEF,
    SOUR_CURR_LIM_MIN,
    SOUR_CURR_LIM_MAX,
    SOUR_CURR_LIM_NEG,
    SOUR_CURR_LIM_NEG_DEF,
    SOUR_CURR_LIM_NEG_MIN,
    SOUR_CURR_LIM_NEG_MAX,
    SOUR_VOLT, // 10
    SOUR_VOLT_DEF,
    SOUR_VOLT_MIN,
    SOUR_VOLT_MAX,
    SOUR_VOLT_LIM,// cc模式的2V+A
    SOUR_VOLT_LIM_DEF,
    SOUR_VOLT_LIM_MIN,
    SOUR_VOLT_LIM_MAX,
    SOUR_VOLT_LIM_NEG,
    SOUR_VOLT_LIM_NEG_DEF,
    SOUR_VOLT_LIM_NEG_MIN,//20
    SOUR_VOLT_LIM_NEG_MAX,
    SOUR_CURR,
    SOUR_CURR_DEF,
    SOUR_CURR_MIN,
    SOUR_CURR_MAX,
    SOUR_POW_LIM, // P+R 180row
    SOUR_POW_LIM_DEF,
    SOUR_POW_LIM_MIN,
    SOUR_POW_LIM_MAX,
    SOUR_POW_LIM_NEG,//30
    SOUR_POW_LIM_NEG_DEF,
    SOUR_POW_LIM_NEG_MIN,
    SOUR_POW_LIM_NEG_MAX,
    SOUR_RES_LIM,
    SOUR_RES_LIM_DEF,
    SOUR_RES_LIM_MIN,
    SOUR_RES_LIM_MAX,
    SOUR_RES_LIM_NEG,
    SOUR_RES_LIM_NEG_DEF,
    SOUR_RES_LIM_NEG_MIN,//40
    SOUR_RES_LIM_NEG_MAX,
    SOUR_VOLT_SLEW_DEF,
    SOUR_VOLT_SLEW_MIN,
    SOUR_VOLT_SLEW_MAX,
    SOUR_VOLT_SLEW_POS_DEF,
    SOUR_VOLT_SLEW_POS_MIN,
    SOUR_VOLT_SLEW_POS_MAX,
    SOUR_VOLT_SLEW_NEG_DEF,
    SOUR_VOLT_SLEW_NEG_MIN,
    SOUR_VOLT_SLEW_NEG_MAX,//50
    SOUR_CURR_SLEW_DEF,
    SOUR_CURR_SLEW_MIN,
    SOUR_CURR_SLEW_MAX,
    SOUR_CURR_SLEW_POS_DEF,
    SOUR_CURR_SLEW_POS_MIN,
    SOUR_CURR_SLEW_POS_MAX,
    SOUR_CURR_SLEW_NEG_DEF,
    SOUR_CURR_SLEW_NEG_MIN,
    SOUR_CURR_SLEW_NEG_MAX,
    _SOUR_VOLT_PROT_STAT_ON,//60
    _SOUR_VOLT_PROT_STAT_OFF,
    SOUR_VOLT_PROT_STAT,
    SOUR_VOLT_PROT,
    SOUR_VOLT_PROT_DEL_DEF,
    SOUR_VOLT_PROT_DEL_MIN,
    SOUR_VOLT_PROT_DEL_MAX,
    _SOUR_VOLT_UND_PROT_STAT_ON,
    _SOURVOLT_UND_PROT_STAT_OFF,
    SOUR_VOLT_UND_PROT_STAT,
    SOUR_VOLT_UND_PROT,//70
    SOUR_VOLT_UND_PROT_DEL_DEF,
    SOUR_VOLT_UND_PROT_DEL_MIN,
    SOUR_VOLT_UND_PROT_DEL_MAX,
    _SOUR_CURR_PROT_STAT_ON,
    _SOUR_CURR_PROT_STAT_OFF,
    SOUR_CURR_PROT_STAT,
    SOUR_CURR_PROT,
    SOUR_CURR_PROT_DEL_DEF,
    SOUR_CURR_PROT_DEL_MIN,
    SOUR_CURR_PROT_DEL_MAX,//80
    _SOUR_CURR_UND_PROT_STAT_ON,
    _SOUR_CURR_UND_PROT_STAT_OFF,
    SOUR_CURR_UND_PROT_STAT,
    SOUR_CURRUND_PROT,
    SOUR_CURR_UND_PROT_DEL_DEF,
    SOUR_CURR_UND_PROT_DEL_MIN,
    SOUR_CURR_UND_PROT_DEL_MAX,
    _SOUR_POW_PROT_STAT_ON,
    _SOUR_POW_PROT_STAT_OFF,
    SOUR_POW_PROT_STAT,//90
    SOUR_POW_PROT,
    SOUR_POW_PROT_DEL_DEF,
    SOUR_POW_PROT_DEL_MIN,
    SOUR_POW_PROT_DEL_MAX,
    SOUR_VOLT_UND_PROT_WARM_DEF,
    SOUR_VOLT_UND_PROT_WARM_MIN,
    SOUR_VOLT_UND_PROT_WARM_MAX,
    SOUR_CURR_UND_PROT_WARM_DEF,
    SOUR_CURR_UND_PROT_WARM_MIN,
    SOUR_CURR_UND_PROT_WARM_MAX,//100
    SOUR_FUNC_MODE,
    _SOUR_EXT_PROG_1,
    _SOUR_EXT_PROG_0,
    _SOUR_EXT_PROG,
    SOUR_FILT_LEV, // 105
    _OUTP_ON,// Output子系统 19
    _OUTP_OFF,
    OUTP_DEL_FALL_DEF,
    OUTP_DEL_FALL_MIN,
    OUTP_DEL_FALL_MAX,
    OUTP_DEL_RISE_DEF,
    OUTP_DEL_RISE_MIN,
    OUTP_DEL_RISE_MAX,
    OUTP_INH_MODE,
    _OUTP_PON_STAT_LAST_ON,// 10
    _OUTP_PON_STAT_LAST_OFF,
    OUTP_PON_STAT,
    _OUTP_PROT_CLE,
    _OUTP_PROT_WDOG_ON,
    _OUTP_PROT_WDOG_OFF,
    OUTP_PROT_WDOG,
    OUTP_PROT_WDOG_DEL_DEF,
    OUTP_PROT_WDOG_DEL_MIN,
    OUTP_PROT_WDOG_DEL_MAX,//19
    STAT_OPER,// STATus子系统 9
    STAT_OPER_ENAB,
    STAT_OPER_COND,
    STAT_OPER_PTR,
    STAT_OPER_NTR,
    STAT_QUES_ENAB,
    STAT_QUES_COND,
    STAT_QUES_PTR,
    STAT_QUES_NTR,
    MEAS_CURR,// MEAS子系统 21
    MEAS_CURR_MAX,
    MEAS_CURR_MIN,
    MEAS_CURR_HIGH,
    MEAS_CURR_LOW,
    MEAS_ARR_CURR,
    MEAS_VOLT,
    MEAS_VOLT_MAX,
    MEAS_VOLT_MIN,
    MEAS_VOLT_HIGH,// 10
    MEAS_VOLT_LOW,
    MEAS_ARR_VOLT,
    MEAS_POW,
    MEAS_POW_MAX,
    MEAS_POW_MIN,
    MEAS_ARR_POW,
    MEAS,
    MEAS_LOCAI_VOLT,
    MEAS_REMO_VOLT,
    MEAS_AHO,
    MEAS_WHO,//21
    None
};

// 除了在这里声明注册以外还要在main函数使用qRegisterMetaType<>("")注册类型
Q_DECLARE_METATYPE(LANUAGE)
Q_DECLARE_METATYPE(WORKMODE)
Q_DECLARE_METATYPE(TIMEOUTCODE)
Q_DECLARE_METATYPE(USBERROR)
Q_DECLARE_METATYPE(TCPERROR)
Q_DECLARE_METATYPE(MONITORROLE)
Q_DECLARE_METATYPE(TERMINALROLE)
Q_DECLARE_METATYPE(FILEFORMAT)
#endif // ENUM_H
