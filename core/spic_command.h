#ifndef SPICCOMMAND_H
#define SPICCOMMAND_H
#include <QHash>
#include <QByteArray>
#include <QString>
#include "enum.h"

namespace SPICCOMAND {

    enum class SYMbol{
          COMMA, // 逗号","
          SEMICOLON, // 分号";"
          COLON, // 冒号":"
          SPACE, // 空格" "
          EOI, // 换行符"\n"
    };

    static QHash<QString,SYMbol> SYMBOL = {
          {QStringLiteral(","),SYMbol::COMMA},
          {QStringLiteral(";"),SYMbol::SEMICOLON},
          {QStringLiteral(":"),SYMbol::COLON},
          {QStringLiteral(" "),SYMbol::SPACE},
          {QStringLiteral("\n"),SYMbol::EOI},

    }; // end QHash<SYMbol,QString> SYMBOL

    inline static QByteArray SymbolCommandBytes(SYMbol symbol)
    {
          return SYMBOL.key(symbol).toLocal8Bit();
    }

    enum  class SOURce{

          SOUR_FUNC_VOLT, // "SOUR:FUNC VOLT" 设定电源的工作模式CV(默认)
          SOUR_FUNC_CURR, // "SOUR:FUNC CURR" 设定电源的工作模式CC
          SOUR_FUNC_INQUIRE,// "SOUR:FUNC?"查询电源的工作模式

          /*（1）************************************CV模式的1个电压、2个电流设定 开始*************************************/
          // 设定和查询CV优先模式下的电流上限值I+（*****CV模式的2个电流设定命令在这里*****）
          SOUR_CURR_LIM, // "SOUR:CURR:LIM 10"  源模式
          SOUR_CURR_LIM_DEF, // "SOUR:CURR:LIM DEF"(默认仪器额定电流值的1%)
          SOUR_CURR_LIM_MIN, // "SOUR:CURR:LIM MIN"
          SOUR_CURR_LIM_MAX, // "SOUR:CURR:LIM MAX"
          SOUR_CURR_LIM_INQUIRE, // "SOUR:CURR:LIM?"
          SOUR_CURR_LIM_DEF_INQUIRE, // "SOUR:CURR:LIM? DEF"
          SOUR_CURR_LIM_MIN_INQUIRE, // "SOUR:CURR:LIM? MIN"
          SOUR_CURR_LIM_MAX_INQUIRE, // "SOUR:CURR:LIM? MAX"
          // 设定和查询CV优先模式下的电流下限值I-
          SOUR_CURR_LIM_NEG,// "SOUR:CURR:LIM:NEG 10"  载模式
          SOUR_CURR_LIM_NEG_DEF, // "SOUR:CURR:LIM:NEG DEF"(默认仪器额定电流值的1%)
          SOUR_CURR_LIM_NEG_MIN, // "SOUR:CURR:LIM:NEG MIN"
          SOUR_CURR_LIM_NEG_MAX, // "SOUR:CURR:LIM:NEG MAX"
          SOUR_CURR_LIM_NEG_INQUIRE, // "SOUR:CURR:LIM:NEG?"
          SOUR_CURR_LIM_NEG_DEF_INQUIRE, // "SOUR:CURR:LIM:NEG? DEF"
          SOUR_CURR_LIM_NEG_MIN_INQUIRE, // "SOUR:CURR:LIM:NEG? MIN"
          SOUR_CURR_LIM_NEG_MAX_INQUIRE, // "SOUR:CURR:LIM:NEG? MAX"
          //设定CV优先模式下的输出电压值Vs （*****CV模式的1个电压设定命令在这里*****）
          SOUR_VOLT, // "SOUR:VOLT 50"(自定义值)
          SOUR_VOLT_DEF, // "SOUR:VOLT DEF"
          SOUR_VOLT_MIN, // "SOUR:VOLT MIN"
          SOUR_VOLT_MAX, // "SOUR:VOLT MAX"
          SOUR_VOLT_INQUIRE,//"SOUR:VOLT?"
          SOUR_VOLT_DEF_INQUIRE, // "SOUR:VOLT? DEF"
          SOUR_VOLT_MIN_INQUIRE, // "SOUR:VOLT? MIN"
          SOUR_VOLT_MAX_INQUIRE, // "SOUR:VOLT? MAX"
          /*（1）************************************CV模式的1个电压、2个电流设定 结束*************************************/


          /*（2）************************************CC模式的1个电流和2个电压设定 开始*************************************/
          // 设定和查询CC优先模式下的电压上限值Vh（*****CC模式的2个电压设定命令在这里*****）
          SOUR_VOLT_LIM, // "SOUR:VOLT:LIM 10"  源模式
          SOUR_VOLT_LIM_DEF, // "SOUR:VOLT:LIM DEF"(默认仪器额定电压值的1%)
          SOUR_VOLT_LIM_MIN, // "SOUR:VOLT:LIM MIN"
          SOUR_VOLT_LIM_MAX, // "SOUR:VOLT:LIM MAX"
          SOUR_VOLT_LIM_INQUIRE,// "SOUR:VOLT:LIM?"
          SOUR_VOLT_LIM_DEF_INQUIRE, // "SOUR:VOLT:LIM? DEF"
          SOUR_VOLT_LIM_MIN_INQUIRE, // "SOUR:VOLT:LIM? MIN"
          SOUR_VOLT_LIM_MAX_INQUIRE, // "SOUR:VOLT:LIM? MAX"
          // 设定和查询CC优先模式下的电压下限值Vl
          SOUR_VOLT_LIM_NEG,// "SOUR:VOLT:LIM:NEG 10"  载模式
          SOUR_VOLT_LIM_NEG_DEF, // "SOUR:VOLT:LIM:NEG DEF"(默认仪器额定电压值的1%)
          SOUR_VOLT_LIM_NEG_MIN, // "SOUR:VOLT:LIM:NEG MIN"
          SOUR_VOLT_LIM_NEG_MAX, // "SOUR:VOLT:LIM:NEG MAX"
          SOUR_VOLT_LIM_NEG_INQUIRE,// "SOUR:VOLT:LIM:NEG?"
          SOUR_VOLT_LIM_NEG_DEF_INQUIRE, // "SOUR:VOLT:LIM:NEG? DEF"
          SOUR_VOLT_LIM_NEG_MIN_INQUIRE, // "SOUR:VOLT:LIM:NEG? MIN"
          SOUR_VOLT_LIM_NEG_MAX_INQUIRE, // "SOUR:VOLT:LIM:NEG? MAX"
          // 设定和查询CC优先模式下的输出电流值Is （*****CC模式的1个电流设定命令在这里*****）
          SOUR_CURR, // "SOUR:CURR 10"(自定义值)
          SOUR_CURR_DEF, // "SOUR:CURR DEF" 仪器额定电流值的1%(默认)
          SOUR_CURR_MIN, // "SOUR:CURR MIN"
          SOUR_CURR_MAX, // "SOUR:CURR MAX"
          SOUR_CURR_INQUIRE, // "SOUR:CURR?"
          SOUR_CURR_DEF_INQUIRE, // "SOUR:CURR? DEF"
          SOUR_CURR_MIN_INQUIRE, // "SOUR:CURR? MIN"
          SOUR_CURR_MAX_INQUIRE, // "SOUR:CURR? MAX"
          /*（2）************************************CC模式的1个电流和2个电压设定 结束*************************************/


          /*（3）************************************CC/CV模式功率和电阻的2个设定值 开始*************************************/
          // 设定和查询功率上限值P+
          SOUR_POW_LIM,//"SOUR:POW:LIM 10"(自定义值)
          SOUR_POW_LIM_DEF,//"SOUR:POW:LIM DEF"(默认仪器额定功率值)
          SOUR_POW_LIM_MIN,//"SOUR:POW:LIM MIN"
          SOUR_POW_LIM_MAX,//"SOUR:POW:LIM MAX"
          SOUR_POW_LIM_INQUIRE,//"SOUR:POW:LIM?"
          SOUR_POW_LIM_DEF_INQUIRE,//"SOUR:POW:LIM? DEF"
          SOUR_POW_LIM_MIN_INQUIRE,//"SOUR:POW:LIM? MIN"
          SOUR_POW_LIM_MAX_INQUIRE,//"SOUR:POW:LIM? MAX"
          // 设定和查询功率下限值P-
          SOUR_POW_LIM_NEG,//"SOUR:POW:LIM:NEG 10"(自定义值)
          SOUR_POW_LIM_NEG_DEF,//"SOUR:POW:LIM:NEG DEF"(默认仪器额定功率值)
          SOUR_POW_LIM_NEG_MIN,//"SOUR:POW:LIM:NEG MIN"
          SOUR_POW_LIM_NEG_MAX,//"SOUR:POW:LIM:NEG MAX"
          SOUR_POW_LIM_NEG_INQUIRE,//"SOUR:POW:LIM:NEG?"
          SOUR_POW_LIM_NEG_DEF_INQUIRE,//"SOUR:POW:LIM:NEG? DEF"
          SOUR_POW_LIM_NEG_MIN_INQUIRE,//"SOUR:POW:LIM:NEG? MIN"
          SOUR_POW_LIM_NEG_MAX_INQUIRE,//"SOUR:POW:LIM:NEG? MAX"

          // 设定和查询电源内阻上限值Rh
          SOUR_RES_LIM, // "SOUR:RES:LIM 1000"
          SOUR_RES_LIM_DEF,// "SOUR:RES:LIM DEF"(默认1000Ω)
          SOUR_RES_LIM_MIN,// "SOUR:RES:LIM MIN"
          SOUR_RES_LIM_MAX,// "SOUR:RES:LIM MAX"
          SOUR_RES_LIM_INQUIRE, // "SOUR:RES:LIM?"
          SOUR_RES_LIM_DEF_INQUIRE,// "SOUR:RES:LIM? DEF"
          SOUR_RES_LIM_MIN_INQUIRE,// "SOUR:RES:LIM? MIN"
          SOUR_RES_LIM_MAX_INQUIRE,// "SOUR:RES:LIM? MAX"

          // 设定和查询电源内阻下限值Rl
          SOUR_RES_LIM_NEG, // "SOUR:RES:LIM:NEG 1000"
          SOUR_RES_LIM_NEG_DEF,// "SOUR:RES:LIM:NEG DEF"(默认1000Ω)
          SOUR_RES_LIM_NEG_MIN,// "SOUR:RES:LIM:NEG MIN"
          SOUR_RES_LIM_NEG_MAX,// "SOUR:RES:LIM:NEG MAX"
          SOUR_RES_LIM_NEG_INQUIRE, // "SOUR:RES:LIM:NEG?"
          SOUR_RES_LIM_NEG_DEF_INQUIRE,// "SOUR:RES:LIM:NEG? DEF"
          SOUR_RES_LIM_NEG_MIN_INQUIRE,// "SOUR:RES:LIM:NEG? MIN"
          SOUR_RES_LIM_NEG_MAX_INQUIRE,// "SOUR:RES:LIM:NEG? MAX"
          /*（3）************************************CC/CV模式功率和电阻的2个设定值 结束*************************************/


          /*（4）************************************电压、电流的上升和下降时间的设定命令 开始*************************************/
          // 设定和查询电压上升、下降时间，即这两个时间同时设置、同时生效
          SOUR_VOLT_SLEW, // "SOUR:VOLT:SLEW 0.1"
          SOUR_VOLT_SLEW_DEF, // "SOUR:VOLT:SLEW DEF"(默认0.1s)
          SOUR_VOLT_SLEW_MIN, // "SOUR:VOLT:SLEW MIN"
          SOUR_VOLT_SLEW_MAX, // "SOUR:VOLT:SLEW MAX"
          SOUR_VOLT_SLEW_DEF_INQUIRE, // "SOUR:VOLT:SLEW? DEF"
          SOUR_VOLT_SLEW_MIN_INQUIRE, // "SOUR:VOLT:SLEW? MIN"
          SOUR_VOLT_SLEW_MAX_INQUIRE, // "SOUR:VOLT:SLEW? MAX"
          // 只设定和查询电压上升时间
          SOUR_VOLT_SLEW_POS, // "SOUR:VOLT:SLEW:POS 0.1"
          SOUR_VOLT_SLEW_POS_DEF, // "SOUR:VOLT:SLEW:POS DEF"(默认0.1s)
          SOUR_VOLT_SLEW_POS_MIN, // "SOUR:VOLT:SLEW:POS MIN"
          SOUR_VOLT_SLEW_POS_MAX, // "SOUR:VOLT:SLEW:POS MAX"
          SOUR_VOLT_SLEW_POS_DEF_INQUIRE, // "SOUR:VOLT:SLEW:POS? DEF"(默认0.1s)
          SOUR_VOLT_SLEW_POS_MIN_INQUIRE, // "SOUR:VOLT:SLEW:POS? MIN"
          SOUR_VOLT_SLEW_POS_MAX_INQUIRE, // "SOUR:VOLT:SLEW:POS? MAX"
          // 只设定和查询电压下降时间
          SOUR_VOLT_SLEW_NEG, // "SOUR:VOLT:SLEW:NEG 0.1"
          SOUR_VOLT_SLEW_NEG_DEF, // "SOUR:VOLT:SLEW:NEG DEF"(默认0.1s)
          SOUR_VOLT_SLEW_NEG_MIN, // "SOUR:VOLT:SLEW:NEG MIN"
          SOUR_VOLT_SLEW_NEG_MAX, // "SOUR:VOLT:SLEW:NEG MAX"
          SOUR_VOLT_SLEW_NEG_DEF_INQUIRE, // "SOUR:VOLT:SLEW:NEG? DEF"
          SOUR_VOLT_SLEW_NEG_MIN_INQUIRE, // "SOUR:VOLT:SLEW:NEG? MIN"
          SOUR_VOLT_SLEW_NEG_MAX_INQUIRE, // "SOUR:VOLT:SLEW:NEG? MAX"

          // 设定和查询电流上升、下降时间，即这两个时间同时设置、同时生效
          SOUR_CURR_SLEW, // "SOUR:CURR:SLEW 0.1" (自定义值)
          SOUR_CURR_SLEW_DEF, // "SOUR:CURR:SLEW DEF" 0.1S(默认)
          SOUR_CURR_SLEW_MIN, // "SOUR:CURR:SLEW MIN"
          SOUR_CURR_SLEW_MAX, // "SOUR:CURR:SLEW MAX"
          SOUR_CURR_SLEW_DEF_INQUIRE, // "SOUR:CURR:SLEW? DEF"
          SOUR_CURR_SLEW_MIN_INQUIRE, // "SOUR:CURR:SLEW? MIN"
          SOUR_CURR_SLEW_MAX_INQUIRE, // "SOUR:CURR:SLEW? MAX"
          // 只设定和查询电流上升时间
          SOUR_CURR_SLEW_POS,// "SOUR:CURR:SLEW:POS 0.1"(自定义值)
          SOUR_CURR_SLEW_POS_DEF,// "SOUR:CURR:SLEW:POS DEF" 0.1S(默认)
          SOUR_CURR_SLEW_POS_MIN,// "SOUR:CURR:SLEW:POS MIN"
          SOUR_CURR_SLEW_POS_MAX,// "SOUR:CURR:SLEW:POS MAX"
          SOUR_CURR_SLEW_POS_DEF_INQUIRE,// "SOUR:CURR:SLEW:POS? DEF"
          SOUR_CURR_SLEW_POS_MIN_INQUIRE,// "SOUR:CURR:SLEW:POS? MIN"
          SOUR_CURR_SLEW_POS_MAX_INQUIRE,// "SOUR:CURR:SLEW:POS? MAX"
          // 只设定和查询电流下降时间
          SOUR_CURR_SLEW_NEG,// "SOUR:CURR:SLEW:NEG 0.1" 0.1S(自定义值)
          SOUR_CURR_SLEW_NEG_DEF,// "SOUR:CURR:SLEW:NEG DEF" 0.1S(默认)
          SOUR_CURR_SLEW_NEG_MIN,// "SOUR:CURR:SLEW:NEG MIN"
          SOUR_CURR_SLEW_NEG_MAX,// "SOUR:CURR:SLEW:NEG MAX"
          SOUR_CURR_SLEW_NEG_DEF_INQUIRE,// "SOUR:CURR:SLEW:NEG? DEF"
          SOUR_CURR_SLEW_NEG_MIN_INQUIRE,// "SOUR:CURR:SLEW:NEG? MIN"
          SOUR_CURR_SLEW_NEG_MAX_INQUIRE,// "SOUR:CURR:SLEW:NEG? MAX"
          /*（4）************************************电压、电流的上升和下降时间的设定命令 结束*************************************/


         /*（5）************************************电压、电流、功率的过、欠保护启用、限定值和延迟时间的设定 开始*************************************/
          //启用或禁用过电压保护。如果已启用过电压保护功能且输出进入电压限制，则将禁用输出
          SOUR_VOLT_PROT_STAT_1, // "SOUR:VOLT:PROT:STAT 1"
          SOUR_VOLT_PROT_STAT_ON = SOUR_VOLT_PROT_STAT_1, // "SOUR:VOLT:PROT:STAT ON"
          SOUR_VOLT_PROT_STAT_0, // "SOUR:VOLT:PROT:STAT 0"
          SOUR_VOLT_PROT_STAT_OFF = SOUR_VOLT_PROT_STAT_0, // "SOUR:VOLT:PROT:STAT OFF"
          SOUR_VOLT_PROT_STAT_INQUIRE,// "SOUR:VOLT:PROT:STAT?"
          // 设定和查询过电压保护的限定值
          SOUR_VOLT_PROT, // "SOUR:VOLT:PROT 10", (自定义值)
          SOUR_VOLT_PROT_DEF, // "SOUR:VOLT:PROT DEF",(默认仪器的额定电压值)
          SOUR_VOLT_PROT_MIN, // "SOUR:VOLT:PROT MIN",
          SOUR_VOLT_PROT_MAX, // "SOUR:VOLT:PROT MAX",
          SOUR_VOLT_PROT_INQUIRE, // "SOUR:VOLT:PROT?"
          // 设定和查询过电压保护的延迟时间
          SOUR_VOLT_PROT_DEL,// "SOUR:VOLT:PROT:DEL 60"
          SOUR_VOLT_PROT_DEL_DEF, // "SOUR:VOLT:PROT:DEL DEF"(默认60s)
          SOUR_VOLT_PROT_DEL_MIN, // "SOUR:VOLT:PROT:DEL MIN"
          SOUR_VOLT_PROT_DEL_MAX, // "SOUR:VOLT:PROT:DEL MAX"
          SOUR_VOLT_PROT_DEL_DEF_INQUIRE, // "SOUR:VOLT:PROT:DEL? DEF"
          SOUR_VOLT_PROT_DEL_MIN_INQUIRE, // "SOUR:VOLT:PROT:DEL? MIN"
          SOUR_VOLT_PROT_DEL_MAX_INQUIRE, // "SOUR:VOLT:PROT:DEL? MAX"

          // 启用或禁用欠电压保护。如果已启用欠电压保护功能且输出进入电压限制，则将禁用输出
          SOUR_VOLT_UND_PROT_STAT_1,//"SOUR:VOLT:UND:PROT:STAT 1"
          SOUR_VOLT_UND_PROT_STAT_ON = SOUR_VOLT_UND_PROT_STAT_1,//"SOUR:VOLT:UND:PROT:STAT ON"
          SOUR_VOLT_UND_PROT_STAT_0,//"SOUR:VOLT:UND:PROT:STAT 0"
          SOUR_VOLT_UND_PROT_STAT_OFF = SOUR_VOLT_UND_PROT_STAT_0,//"SOUR:VOLT:UND:PROT:STAT OFF"
          SOUR_VOLT_UND_PROT_STAT_INQUIRE,//"SOUR:VOLT:UND:PROT:STAT?"
          // 设定和查询欠电压保护的限定值
          SOUR_VOLT_UND_PROT, // "SOUR:VOLT:UND:PROT 10",(自定义值)
          SOUR_VOLT_UND_PROT_DEF, // "SOUR:VOLT:UND:PROT DEF",(默认0)
          SOUR_VOLT_UND_PROT_MIN, // "SOUR:VOLT:UND:PROT MIN",
          SOUR_VOLT_UND_PROT_MAX, // "SOUR:VOLT:UND:PROT MAX",
          SOUR_VOLT_UND_PROT_INQUIRE, // "SOUR:VOLT:UND:PROT?"
          // 设定和查询过电流保护的延迟时间
          SOUR_VOLT_UND_PROT_DEL, // "SOUR:VOLT:UND:PROT:DEL 10"
          SOUR_VOLT_UND_PROT_DEL_DEF, // "SOUR:VOLT:UND:PROT:DEL DEF"(默认60s)
          SOUR_VOLT_UND_PROT_DEL_MIN, // "SOUR:VOLT:UND:PROT:DEL MIN"
          SOUR_VOLT_UND_PROT_DEL_MAX, // "SOUR:VOLT:UND:PROT:DEL MAX"
          SOUR_VOLT_UND_PROT_DEL_DEF_INQUIRE, // "SOUR:VOLT:UND:PROT:DEL? DEF"
          SOUR_VOLT_UND_PROT_DEL_MIN_INQUIRE, // "SOUR:VOLT:UND:PROT:DEL? MIN"
          SOUR_VOLT_UND_PROT_DEL_MAX_INQUIRE, // "SOUR:VOLT:UND:PROT:DEL? MAX"

          // 启用或禁用过电流保护。如果已启用过电流保护功能且输出进入电流限制，则将禁用输出
          SOUR_CURR_PROT_STAT_1, // "SOUR:CURR:PROT:STAT 1"
          SOUR_CURR_PROT_STAT_ON = SOUR_CURR_PROT_STAT_1, // "SOUR:CURR:PROT:STAT ON"
          SOUR_CURR_PROT_STAT_0, // "SOUR:CURR:PROT:STAT 0"
          SOUR_CURR_PROT_STAT_OFF = SOUR_CURR_PROT_STAT_0, // "SOUR:CURR:PROT:STAT OFF"
          SOUR_CURR_PROT_STAT_INQUIRE,// "SOUR:CURR:PROT:STAT?"
          // 设定和查询过电流保护的限定值
          SOUR_CURR_PROT, // "SOUR:CURR:PROT 10", (自定义值)
          SOUR_CURR_PROT_DEF, // "SOUR:CURR:PROT DEF",(默认仪器的额定电流值)
          SOUR_CURR_PROT_MIN, // "SOUR:CURR:PROT MIN",
          SOUR_CURR_PROT_MAX, // "SOUR:CURR:PROT MAX",
          SOUR_CURR_PROT_INQUIRE, // "SOUR:CURR:PROT?"
          // 设定和查询过电流保护的延迟时间
          SOUR_CURR_PROT_DEL,// "SOUR:CURR:PROT:DEL 60"
          SOUR_CURR_PROT_DEL_DEF, // "SOUR:CURR:PROT:DEL DEF"(默认60s)
          SOUR_CURR_PROT_DEL_MIN, // "SOUR:CURR:PROT:DEL MIN"
          SOUR_CURR_PROT_DEL_MAX, // "SOUR:CURR:PROT:DEL MAX"
          SOUR_CURR_PROT_DEL_DEF_INQUIRE, // "SOUR:CURR:PROT:DEL? DEF"
          SOUR_CURR_PROT_DEL_MIN_INQUIRE, // "SOUR:CURR:PROT:DEL? MIN"
          SOUR_CURR_PROT_DEL_MAX_INQUIRE, // "SOUR:CURR:PROT:DEL? MAX"

          // 启用或禁用欠电流保护。如果已启用欠电流保护功能且输出进入电流限制，则将禁用输出
          SOUR_CURR_UND_PROT_STAT_1,//"SOUR:CURR:UND:PROT:STAT 1"
          SOUR_CURR_UND_PROT_STAT_ON = SOUR_CURR_UND_PROT_STAT_1,//"SOUR:CURR:UND:PROT:STAT ON"
          SOUR_CURR_UND_PROT_STAT_0,//"SOUR:CURR:UND:PROT:STAT 0"
          SOUR_CURR_UND_PROT_STAT_OFF = SOUR_CURR_UND_PROT_STAT_0,//"SOUR:CURR:UND:PROT:STAT OFF"
          SOUR_CURR_UND_PROT_STAT_INQUIRE,//"SOUR:CURR:UND:PROT:STAT?"
          // 设定和查询欠电流保护的限定值
          SOUR_CURR_UND_PROT, // "SOUR:CURR:UND:PROT 10",(自定义值)
          SOUR_CURR_UND_PROT_DEF, // "SOUR:CURR:UND:PROT DEF",(默认0)
          SOUR_CURR_UND_PROT_MIN, // "SOUR:CURR:UND:PROT MIN",
          SOUR_CURR_UND_PROT_MAX, // "SOUR:CURR:UND:PROT MAX",
          SOUR_CURR_UND_PROT_INQUIRE, // "SOUR:CURR:UND:PROT?"
          // 设定和查询过电流保护的延迟时间
          SOUR_CURR_UND_PROT_DEL, // "SOUR:CURR:UND:PROT:DEL 10"
          SOUR_CURR_UND_PROT_DEL_DEF, // "SOUR:CURR:UND:PROT:DEL DEF"(默认60s)
          SOUR_CURR_UND_PROT_DEL_MIN, // "SOUR:CURR:UND:PROT:DEL MIN"
          SOUR_CURR_UND_PROT_DEL_MAX, // "SOUR:CURR:UND:PROT:DEL MAX"
          SOUR_CURR_UND_PROT_DEL_DEF_INQUIRE, // "SOUR:CURR:UND:PROT:DEL? DEF"
          SOUR_CURR_UND_PROT_DEL_MIN_INQUIRE, // "SOUR:CURR:UND:PROT:DEL? MIN"
          SOUR_CURR_UND_PROT_DEL_MAX_INQUIRE, // "SOUR:CURR:UND:PROT:DEL? MAX"

          // 启用或禁用过功率保护。如果已启用过功率保护功能且输出进入功率限制，则将禁用输出
          SOUR_POW_PROT_STAT_1, // "SOUR:POW:PROT:STAT 1"
          SOUR_POW_PROT_STAT_ON = SOUR_POW_PROT_STAT_1, // "SOUR:POW:PROT:STAT ON"
          SOUR_POW_PROT_STAT_0, // "SOUR:POW:PROT:STAT 0"
          SOUR_POW_PROT_STAT_OFF = SOUR_POW_PROT_STAT_0, // "SOUR:POW:PROT:STAT OFF"
          SOUR_POW_PROT_STAT_INQUIRE,// "SOUR:POW:PROT:STAT?"
          // 设定和查询过功率保护的限定值
          SOUR_POW_PROT, // "SOUR:POW:PROT 10", (自定义值)
          SOUR_POW_PROT_DEF, // "SOUR:POW:PROT DEF",(默认仪器的额定功率值)
          SOUR_POW_PROT_MIN, // "SOUR:POW:PROT MIN",
          SOUR_POW_PROT_MAX, // "SOUR:POW:PROT MAX",
          SOUR_POW_PROT_INQUIRE, // "SOUR:POW:PROT?"
          // 设定和查询过功率保护的延迟时间
          SOUR_POW_PROT_DEL,// "SOUR:POW:PROT:DEL 60"
          SOUR_POW_PROT_DEL_DEF, // "SOUR:POW:PROT:DEL DEF"(默认60s)
          SOUR_POW_PROT_DEL_MIN, // "SOUR:POW:PROT:DEL MIN"
          SOUR_POW_PROT_DEL_MAX, // "SOUR:POW:PROT:DEL MAX"
          SOUR_POW_PROT_DEL_DEF_INQUIRE, // "SOUR:POW:PROT:DEL? DEF"
          SOUR_POW_PROT_DEL_MIN_INQUIRE, // "SOUR:POW:PROT:DEL? MIN"
          SOUR_POW_PROT_DEL_MAX_INQUIRE, // "SOUR:POW:PROT:DEL? MAX"
          // 没有欠功率保护的相关命令
          /*（5）************************************电压、电流、功率的过、欠保护启用、限定值和延迟时间的设定 结束*************************************/


          /*（6）************************************欠电压、欠电流保护的温机时间 开始*************************************/
          // 该命令用来设定欠电压保护的温机时间，即仪器预热时间，设置该时间是为了防止电压在不断上升过程中达到保护点而触发了保护状态。
          SOUR_VOLT_UND_PROT_WARM,// "SOUR:VOLT:UND:PROT:WARM 60"
          SOUR_VOLT_UND_PROT_WARM_DEF,// "SOUR:VOLT:UND:PROT:WARM DEF"(默认60s)
          SOUR_VOLT_UND_PROT_WARM_MIN,// "SOUR:VOLT:UND:PROT:WARM MIN"
          SOUR_VOLT_UND_PROT_WARM_MAX,// "SOUR:VOLT:UND:PROT:WARM MAX"
          SOUR_VOLT_UND_PROT_WARM_DEF_INQUIRE,// "SOUR:VOLT:UND:PROT:WARM? DEF"
          SOUR_VOLT_UND_PROT_WARM_MIN_INQUIRE,// "SOUR:VOLT:UND:PROT:WARM? MIN"
          SOUR_VOLT_UND_PROT_WARM_MAX_INQUIRE,// "SOUR:VOLT:UND:PROT:WARM? MAX"

          // 设定和查询欠电流保护的温机时间，即仪器预热时间，设置该时间是为了防止电流在不断上升过程中达到保护点而触发了保护状态
          SOUR_CURR_UND_PROT_WARM,// "SOUR:CURR:UND:PROT:WARM 60"
          SOUR_CURR_UND_PROT_WARM_DEF,// "SOUR:CURR:UND:PROT:WARM DEF"(默认60s)
          SOUR_CURR_UND_PROT_WARM_MIN,// "SOUR:CURR:UND:PROT:WARM MIN"
          SOUR_CURR_UND_PROT_WARM_MAX,// "SOUR:CURR:UND:PROT:WARM MAX"
          SOUR_CURR_UND_PROT_WARM_DEF_INQUIRE,// "SOUR:CURR:UND:PROT:WARM? DEF"
          SOUR_CURR_UND_PROT_WARM_MIN_INQUIRE,// "SOUR:CURR:UND:PROT:WARM? MIN"
          SOUR_CURR_UND_PROT_WARM_MAX_INQUIRE,// "SOUR:CURR:UND:PROT:WARM? MAX"
          /*（6）************************************欠电压、欠电流保护的温机时间 结束*************************************/


          /*（7）************************************其它命令 开始*************************************/
          // 设定和查询电源的功能模式，这样可以在启动并触发瞬变系统时确定输出电流的发生情况
          SOUR_FUNC_MODE_FIX, // "SOUR:FUNC:MODE FIX",表示工作在固定模式，将输出电流保持为其即时值(默认)
          SOUR_FUNC_MODE_LIST, // "SOUR:FUNC:MODE LIST",表示工作在List模式，发生触发时，LIST将导致输出跟随列表值
          SOUR_FUNC_MODE_BATT, // "SOUR:FUNC:MODE BATT",表示工作在电池测试模式，发生触发时，将依据已编辑的电池测试文件进行输出
          SOUR_FUNC_MODE_SOL, // "SOUR:FUNC:MODE SOL",表示工作在光伏曲线模拟模式，发生触发时，将依据已选定的PV曲线进行输出
          SOUR_FUNC_MODE_CARP, // "SOUR:FUNC:MODE CARP",表示工作在汽车波形模拟模式，发生触发时，将依据已选定的汽车波形进行输出
          SOUR_FUNC_MODE_INQUIRE, // "SOUR:FUNC:MODE?"

          // 启用或禁用或查询外部模拟量功能
          SOUR_EXT_PROG_1, // "SOUR:EXT:PROG 1"
          SOUR_EXT_PROG_ON = SOUR_EXT_PROG_1, // "SOUR:EXT:PROG ON"
          SOUR_EXT_PROG_0, // "SOUR:EXT:PROG 0"
          SOUR_EXT_PROG_OFF = SOUR_EXT_PROG_0, // "SOUR:EXT:PROG OFF"
          SOUR_EXT_PROG_INQUIRE, // "SOUR:EXT:PROG?"

          // 设置和查询通道的线性标定系数。该指令需设置两个参数，通过逗号隔开，逗号前面的为通道号，逗号后面的为线性标定系数
          // 通道号支持1,2,3，线性标定系数在-5000~5000之间，默认值-5000
          SOUR_EXT_PROG_CHAN1_MX, // "EXT:PROG:CHAN:MX 1,10"
          SOUR_EXT_PROG_CHAN1_MX_DEF, // "EXT:PROG:CHAN:MX 1,DEF"
          SOUR_EXT_PROG_CHAN1_MX_MIN, // "EXT:PROG:CHAN:MX 1,MIN"
          SOUR_EXT_PROG_CHAN1_MX_MAX, // "EXT:PROG:CHAN:MX 1,MAX"
          SOUR_EXT_PROG_CHAN2_MX, // "EXT:PROG:CHAN:MX 2,10"
          SOUR_EXT_PROG_CHAN2_MX_DEF, // "EXT:PROG:CHAN:MX 2,DEF"
          SOUR_EXT_PROG_CHAN2_MX_MIN, // "EXT:PROG:CHAN:MX 2,MIN"
          SOUR_EXT_PROG_CHAN2_MX_MAX, // "EXT:PROG:CHAN:MX 2,MAX"
          SOUR_EXT_PROG_CHAN3_MX, // "EXT:PROG:CHAN:MX 3,10"
          SOUR_EXT_PROG_CHAN3_MX_DEF, // "EXT:PROG:CHAN:MX 3,DEF"
          SOUR_EXT_PROG_CHAN3_MX_MIN, // "EXT:PROG:CHAN:MX 3,MIN"
          SOUR_EXT_PROG_CHAN3_MX_MAX, // "EXT:PROG:CHAN:MX 3,MAX"
          SOUR_EXT_PROG_CHAN1_MX_INQUIRE, // "EXT:PROG:CHAN:MX? 1"
          SOUR_EXT_PROG_CHAN2_MX_INQUIRE, // "EXT:PROG:CHAN:MX? 2"
          SOUR_EXT_PROG_CHAN3_MX_INQUIRE, // "EXT:PROG:CHAN:MX? 3"

          //设置和查询通道的偏移标定系数。该指令需设置两个参数，通过逗号隔开，逗号前面的为通道号，逗号后面的为偏移标定系数。
          // 通道号支持1,2,3，偏移标定系数在-5000~5000之间，默认值-5000
          SOUR_EXT_PROG_CHAN1_MB, // "EXT:PROG:CHAN:MB 1,10"
          SOUR_EXT_PROG_CHAN1_MB_DEF, // "EXT:PROG:CHAN:MB 1,DEF"
          SOUR_EXT_PROG_CHAN1_MB_MIN, // "EXT:PROG:CHAN:MB 1,MIN"
          SOUR_EXT_PROG_CHAN1_MB_MAX, // "EXT:PROG:CHAN:MB 1,MAX"
          SOUR_EXT_PROG_CHAN2_MB, // "EXT:PROG:CHAN:MB 2,10"
          SOUR_EXT_PROG_CHAN2_MB_DEF, // "EXT:PROG:CHAN:MB 2,DEF"
          SOUR_EXT_PROG_CHAN2_MB_MIN, // "EXT:PROG:CHAN:MB 2,MIN"
          SOUR_EXT_PROG_CHAN2_MB_MAX, // "EXT:PROG:CHAN:MB 2,MAX"
          SOUR_EXT_PROG_CHAN3_MB, // "EXT:PROG:CHAN:MB 3,10"
          SOUR_EXT_PROG_CHAN3_MB_DEF, // "EXT:PROG:CHAN:MB 3,DEF"
          SOUR_EXT_PROG_CHAN3_MB_MIN, // "EXT:PROG:CHAN:MB 3,MIN"
          SOUR_EXT_PROG_CHAN3_MB_MAX, // "EXT:PROG:CHAN:MB 3,MAX"
          SOUR_EXT_PROG_CHAN1_MB_INQUIRE, // "EXT:PROG:CHAN:MB? 1"
          SOUR_EXT_PROG_CHAN2_MB_INQUIRE, // "EXT:PROG:CHAN:MB? 2"
          SOUR_EXT_PROG_CHAN3_MB_INQUIRE, // "EXT:PROG:CHAN:MB? 3"

          // 设定滤波等级
          SOUR_FILT_LEV_LOW,// "SOUR:FILT:LEV LOW"
          SOUR_FILT_LEV_MED,// "SOUR:FILT:LEV MED"
          SOUR_FILT_LEV_FAST,// "SOUR:FILT:LEV FAST"
          SOUR_FILT_LEV_INQUIRE, // "SOUR:FILT:LEV?"
           /*（7）************************************其它命令 结束*************************************/

    }; // end enum SOURce

    static QHash<QString,SOURce> SOURCE= {

          {QStringLiteral("SOUR:FUNC VOLT"),SOURce::SOUR_FUNC_VOLT}, // 设定电源的工作模式CV(默认
          {QStringLiteral("SOUR:FUNC CURR"),SOURce::SOUR_FUNC_CURR},//设定电源的工作模式CC
          {QStringLiteral("SOUR:FUNC?"),SOURce::SOUR_FUNC_INQUIRE},// 查询电源工作模式

           /*（1）************************************CV模式的1个电压、2个电流设定 开始*************************************/
          {QStringLiteral("SOUR:CURR:LIM"),SOURce::SOUR_CURR_LIM},// 需要空格 设定和查询CV优先模式电流上限值 源模式
          {QStringLiteral("SOUR:CURR:LIM DEF"),SOURce::SOUR_CURR_LIM_DEF},
          {QStringLiteral("SOUR:CURR:LIM MIN"),SOURce::SOUR_CURR_LIM_MIN},
          {QStringLiteral("SOUR:CURR:LIM MAX"),SOURce::SOUR_CURR_LIM_MAX},
          {QStringLiteral("SOUR:CURR:LIM?"),SOURce::SOUR_CURR_LIM_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM? DEF"),SOURce::SOUR_CURR_LIM_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM? MIN"),SOURce::SOUR_CURR_LIM_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM? MAX"),SOURce::SOUR_CURR_LIM_MAX_INQUIRE},

          {QStringLiteral("SOUR:CURR:LIM:NEG"),SOURce::SOUR_CURR_LIM_NEG},// 需要空格设定和查询CV优先模式电流下限值 载模式
          {QStringLiteral("SOUR:CURR:LIM:NEG DEF"),SOURce::SOUR_CURR_LIM_NEG_DEF},
          {QStringLiteral("SOUR:CURR:LIM:NEG MIN"),SOURce::SOUR_CURR_LIM_NEG_MIN},
          {QStringLiteral("SOUR:CURR:LIM:NEG MAX"),SOURce::SOUR_CURR_LIM_NEG_MAX},
          {QStringLiteral("SOUR:CURR:LIM:NEG?"),SOURce::SOUR_CURR_LIM_NEG_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM:NEG? DEF"),SOURce::SOUR_CURR_LIM_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM:NEG? MIN"),SOURce::SOUR_CURR_LIM_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:LIM:NEG? MAX"),SOURce::SOUR_CURR_LIM_NEG_MAX_INQUIRE},

          {QStringLiteral("SOUR:VOLT"),SOURce::SOUR_VOLT}, // 需要空格 设定CV优先模式下的输出电压值Vs
          {QStringLiteral("SOUR:VOLT DEF"),SOURce::SOUR_VOLT_DEF},
          {QStringLiteral("SOUR:VOLT MIN"),SOURce::SOUR_VOLT_MIN},
          {QStringLiteral("SOUR:VOLT MAX"),SOURce::SOUR_VOLT_MAX},
          {QStringLiteral("SOUR:VOLT?"),SOURce::SOUR_VOLT_INQUIRE},
          {QStringLiteral("SOUR:VOLT? DEF"),SOURce::SOUR_VOLT_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT? MIN"),SOURce::SOUR_VOLT_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT? MAX"),SOURce::SOUR_VOLT_MAX_INQUIRE},
           /*（1）************************************CV模式的1个电压、2个电流设定 结束*************************************/


           /*（2）************************************CC模式的1个电流和2个电压设定 开始*************************************/
          {QStringLiteral("SOUR:VOLT:LIM"),SOURce::SOUR_VOLT_LIM}, // 需要空格 CC优先模式电压上限值 源模式
          {QStringLiteral("SOUR:VOLT:LIM DEF"),SOURce::SOUR_VOLT_LIM_DEF},
          {QStringLiteral("SOUR:VOLT:LIM MIN"),SOURce::SOUR_VOLT_LIM_MIN},
          {QStringLiteral("SOUR:VOLT:LIM MAX"),SOURce::SOUR_VOLT_LIM_MAX},
          {QStringLiteral("SOUR:VOLT:LIM?"),SOURce::SOUR_VOLT_LIM_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM? DEF"),SOURce::SOUR_VOLT_LIM_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM? MIN"),SOURce::SOUR_VOLT_LIM_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM? MAX"),SOURce::SOUR_VOLT_LIM_MAX_INQUIRE},

          {QStringLiteral("SOUR:VOLT:LIM:NEG"),SOURce::SOUR_VOLT_LIM_NEG}, // 需要空格 CC优先模式下的电压下限值 载模式
          {QStringLiteral("SOUR:VOLT:LIM:NEG DEF"),SOURce::SOUR_VOLT_LIM_NEG_DEF},
          {QStringLiteral("SOUR:VOLT:LIM:NEG MIN"),SOURce::SOUR_VOLT_LIM_NEG_MIN},
          {QStringLiteral("SOUR:VOLT:LIM:NEG MAX"),SOURce::SOUR_VOLT_LIM_NEG_MAX},
          {QStringLiteral("SOUR:VOLT:LIM:NEG?"),SOURce::SOUR_VOLT_LIM_NEG_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM:NEG? DEF"),SOURce::SOUR_VOLT_LIM_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM:NEG? MIN"),SOURce::SOUR_VOLT_LIM_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:LIM:NEG? MAX"),SOURce::SOUR_VOLT_LIM_NEG_MAX_INQUIRE},

          {QStringLiteral("SOUR:CURR"),SOURce::SOUR_CURR}, // 需要空格 CC优先模式下的输出电流
          {QStringLiteral("SOUR:CURR DEF"),SOURce::SOUR_CURR_DEF},
          {QStringLiteral("SOUR:CURR MIN"),SOURce::SOUR_CURR_MIN},
          {QStringLiteral("SOUR:CURR MAX"),SOURce::SOUR_CURR_MAX},
          {QStringLiteral("SOUR:CURR?"),SOURce::SOUR_CURR_INQUIRE},
          {QStringLiteral("SOUR:CURR? DEF"),SOURce::SOUR_CURR_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR? MIN"),SOURce::SOUR_CURR_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR? MAX"),SOURce::SOUR_CURR_MAX_INQUIRE},
          /*（2）************************************CC模式的1个电流和2个电压设定 结束*************************************/


             /*（3）************************************CC/CV模式功率和电阻的2个设定值 开始*************************************/
          {QStringLiteral("SOUR:POW:LIM"),SOURce::SOUR_POW_LIM}, // 需要空格 设定和查询功率上限值P+
          {QStringLiteral("SOUR:POW:LIM DEF"),SOURce::SOUR_POW_LIM_DEF},
          {QStringLiteral("SOUR:POW:LIM MIN"),SOURce::SOUR_POW_LIM_MIN},
          {QStringLiteral("SOUR:POW:LIM MAX"),SOURce::SOUR_POW_LIM_MAX},
          {QStringLiteral("SOUR:POW:LIM?"),SOURce::SOUR_POW_LIM_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM? DEF"),SOURce::SOUR_POW_LIM_DEF_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM? MIN"),SOURce::SOUR_POW_LIM_MIN_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM? MAX"),SOURce::SOUR_POW_LIM_MAX_INQUIRE},

          {QStringLiteral("SOUR:POW:LIM:NEG"),SOURce::SOUR_POW_LIM_NEG},// 需要空格 设定和查询功率下限值P-
          {QStringLiteral("SOUR:POW:LIM:NEG DEF"),SOURce::SOUR_POW_LIM_NEG_DEF},
          {QStringLiteral("SOUR:POW:LIM:NEG MIN"),SOURce::SOUR_POW_LIM_NEG_MIN},
          {QStringLiteral("SOUR:POW:LIM:NEG MAX"),SOURce::SOUR_POW_LIM_NEG_MAX},
          {QStringLiteral("SOUR:POW:LIM:NEG?"),SOURce::SOUR_POW_LIM_NEG_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM:NEG? DEF"),SOURce::SOUR_POW_LIM_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM:NEG? MIN"),SOURce::SOUR_POW_LIM_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:POW:LIM:NEG? MAX"),SOURce::SOUR_POW_LIM_NEG_MAX_INQUIRE},

          {QStringLiteral("SOUR:RES:LIM"),SOURce::SOUR_RES_LIM},  // 需要空格 设定和查询电源内阻上限值Rh
          {QStringLiteral("SOUR:RES:LIM DEF"),SOURce::SOUR_RES_LIM_DEF},
          {QStringLiteral("SOUR:RES:LIM MIN"),SOURce::SOUR_RES_LIM_MIN},
          {QStringLiteral("SOUR:RES:LIM MAX"),SOURce::SOUR_RES_LIM_MAX},
          {QStringLiteral("SOUR:RES:LIM?"),SOURce::SOUR_RES_LIM_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM? DEF"),SOURce::SOUR_RES_LIM_DEF_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM? MIN"),SOURce::SOUR_RES_LIM_MIN_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM? MAX"),SOURce::SOUR_RES_LIM_MAX_INQUIRE},

          {QStringLiteral("SOUR:RES:LIM:NEG"),SOURce::SOUR_RES_LIM_NEG},  // 需要空格 设定和查询电源内阻下限值Rl
          {QStringLiteral("SOUR:RES:LIM:NEG DEF"),SOURce::SOUR_RES_LIM_NEG_DEF},
          {QStringLiteral("SOUR:RES:LIM:NEG MIN"),SOURce::SOUR_RES_LIM_NEG_MIN},
          {QStringLiteral("SOUR:RES:LIM:NEG MAX"),SOURce::SOUR_RES_LIM_NEG_MAX},
          {QStringLiteral("SOUR:RES:LIM:NEG?"),SOURce::SOUR_RES_LIM_NEG_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM:NEG? DEF"),SOURce::SOUR_RES_LIM_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM:NEG? MIN"),SOURce::SOUR_RES_LIM_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:RES:LIM:NEG? MAX"),SOURce::SOUR_RES_LIM_NEG_MAX_INQUIRE},

           /*（3）************************************CC/CV模式功率和电阻的2个设定值 结束*************************************/


           /*（4）************************************电压、电流的上升和下降时间的设定命令 开始*************************************/
          // 设定和查询电压上升、下降时间，即这两个时间同时设置、同时生效
          {QStringLiteral("SOUR:VOLT:SLEW"),SOURce::SOUR_VOLT_SLEW},  // 需要空格
          {QStringLiteral("SOUR:VOLT:SLEW DEF"),SOURce:: SOUR_VOLT_SLEW_DEF},
          {QStringLiteral("SOUR:VOLT:SLEW MIN"),SOURce:: SOUR_VOLT_SLEW_MIN},
          {QStringLiteral("SOUR:VOLT:SLEW MAX"),SOURce:: SOUR_VOLT_SLEW_MAX},
          {QStringLiteral("SOUR:VOLT:SLEW? DEF"),SOURce:: SOUR_VOLT_SLEW_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW? MIN"),SOURce:: SOUR_VOLT_SLEW_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW? MAX"),SOURce:: SOUR_VOLT_SLEW_MAX_INQUIRE},
           // 只设定和查询电压上升时间
          {QStringLiteral("SOUR:VOLT:SLEW:POS"),SOURce::SOUR_VOLT_SLEW_POS},  // 需要空格
          {QStringLiteral("SOUR:VOLT:SLEW:POS DEF"),SOURce:: SOUR_VOLT_SLEW_POS_DEF},
          {QStringLiteral("SOUR:VOLT:SLEW:POS MIN"),SOURce:: SOUR_VOLT_SLEW_POS_MIN},
          {QStringLiteral("SOUR:VOLT:SLEW:POS MAX"),SOURce:: SOUR_VOLT_SLEW_POS_MAX},
          {QStringLiteral("SOUR:VOLT:SLEW:POS? DEF"),SOURce:: SOUR_VOLT_SLEW_POS_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW:POS? MIN"),SOURce:: SOUR_VOLT_SLEW_POS_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW:POS? MAX"),SOURce:: SOUR_VOLT_SLEW_POS_MAX_INQUIRE},
          // 只设定和查询电压下降时间
          {QStringLiteral("SOUR:VOLT:SLEW:NEG"),SOURce::SOUR_VOLT_SLEW_NEG},  // 需要空格
          {QStringLiteral("SOUR:VOLT:SLEW:NEG DEF"),SOURce:: SOUR_VOLT_SLEW_NEG_DEF},
          {QStringLiteral("SOUR:VOLT:SLEW:NEG MIN"),SOURce:: SOUR_VOLT_SLEW_NEG_MIN},
          {QStringLiteral("SOUR:VOLT:SLEW:NEG MAX"),SOURce:: SOUR_VOLT_SLEW_NEG_MAX},
          {QStringLiteral("SOUR:VOLT:SLEW:NEG? DEF"),SOURce:: SOUR_VOLT_SLEW_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW:NEG? MIN"),SOURce:: SOUR_VOLT_SLEW_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:SLEW:NEG? MAX"),SOURce:: SOUR_VOLT_SLEW_NEG_MAX_INQUIRE},
          // 设定和查询电流上升、下降时间，即这两个时间同时设置、同时生效
          {QStringLiteral("SOUR:CURR:SLEW"),SOURce::SOUR_CURR_SLEW},  // 需要空格
          {QStringLiteral("SOUR:CURR:SLEW DEF"),SOURce:: SOUR_CURR_SLEW_DEF},
          {QStringLiteral("SOUR:CURR:SLEW MIN"),SOURce:: SOUR_CURR_SLEW_MIN},
          {QStringLiteral("SOUR:CURR:SLEW MAX"),SOURce:: SOUR_CURR_SLEW_MAX},
          {QStringLiteral("SOUR:CURR:SLEW? DEF"),SOURce:: SOUR_CURR_SLEW_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW? MIN"),SOURce:: SOUR_CURR_SLEW_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW? MAX"),SOURce:: SOUR_CURR_SLEW_MAX_INQUIRE},
           // 只设定和查询电流上升时间
          {QStringLiteral("SOUR:CURR:SLEW:POS"),SOURce::SOUR_CURR_SLEW_POS},  // 需要空格
          {QStringLiteral("SOUR:CURR:SLEW:POS DEF"),SOURce:: SOUR_CURR_SLEW_POS_DEF},
          {QStringLiteral("SOUR:CURR:SLEW:POS MIN"),SOURce:: SOUR_CURR_SLEW_POS_MIN},
          {QStringLiteral("SOUR:CURR:SLEW:POS MAX"),SOURce:: SOUR_CURR_SLEW_POS_MAX},
          {QStringLiteral("SOUR:CURR:SLEW:POS? DEF"),SOURce:: SOUR_CURR_SLEW_POS_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW:POS? MIN"),SOURce:: SOUR_CURR_SLEW_POS_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW:POS? MAX"),SOURce:: SOUR_CURR_SLEW_POS_MAX_INQUIRE},
          // 只设定和查询电流下降时间
          {QStringLiteral("SOUR:CURR:SLEW:NEG"),SOURce::SOUR_CURR_SLEW_NEG},  // 需要空格
          {QStringLiteral("SOUR:CURR:SLEW:NEG DEF"),SOURce:: SOUR_CURR_SLEW_NEG_DEF},
          {QStringLiteral("SOUR:CURR:SLEW:NEG MIN"),SOURce:: SOUR_CURR_SLEW_NEG_MIN},
          {QStringLiteral("SOUR:CURR:SLEW:NEG MAX"),SOURce:: SOUR_CURR_SLEW_NEG_MAX},
          {QStringLiteral("SOUR:CURR:SLEW:NEG? DEF"),SOURce:: SOUR_CURR_SLEW_NEG_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW:NEG? MIN"),SOURce:: SOUR_CURR_SLEW_NEG_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:SLEW:NEG? MAX"),SOURce:: SOUR_CURR_SLEW_NEG_MAX_INQUIRE},
          /*（4）************************************电压、电流的上升和下降时间的设定命令 结束*************************************/

            /*（5）************************************电压、电流、功率的过、欠保护启用、限定值和延迟时间的设定 开始*************************************/
          //启用或禁用过电压保护。如果已启用过电压保护功能且输出进入电压限制，则将禁用输出
          {QStringLiteral("SOUR:VOLT:PROT:STAT 1"),SOURce:: SOUR_VOLT_PROT_STAT_1}, // 需要空格
          {QStringLiteral("SOUR:VOLT:PROT:STAT ON"),SOURce::SOUR_VOLT_PROT_STAT_ON},
          {QStringLiteral("SOUR:VOLT:PROT:STAT 0"),SOURce::SOUR_VOLT_PROT_STAT_0},
          {QStringLiteral("SOUR:VOLT:PROT:STAT OFF"),SOURce::SOUR_VOLT_PROT_STAT_OFF},
          {QStringLiteral("SOUR:VOLT:PROT:STAT?"),SOURce::SOUR_VOLT_PROT_STAT_INQUIRE},
           // 设定和查询过电压保护的限定值
          {QStringLiteral("SOUR:VOLT:PROT"),SOURce::SOUR_VOLT_PROT}, // 需要空格
          {QStringLiteral("SOUR:VOLT:PROT DEF"),SOURce::SOUR_VOLT_PROT_DEF},
          {QStringLiteral("SOUR:VOLT:PROT MIN"),SOURce::SOUR_VOLT_PROT_MIN},
          {QStringLiteral("SOUR:VOLT:PROT MAX"),SOURce::SOUR_VOLT_PROT_MAX},
          {QStringLiteral("SOUR:VOLT:PROT?"),SOURce::SOUR_VOLT_PROT_INQUIRE},
          // 设定和查询过电压保护的延迟时间
          {QStringLiteral("SOUR:VOLT:PROT:DEL"),SOURce::SOUR_VOLT_PROT_DEL},// 需要空格
          {QStringLiteral("SOUR:VOLT:PROT:DEL DEF"),SOURce::SOUR_VOLT_PROT_DEL_DEF},
          {QStringLiteral("SOUR:VOLT:PROT:DEL MIN"),SOURce::SOUR_VOLT_PROT_DEL_MIN},
          {QStringLiteral("SOUR:VOLT:PROT:DEL MAX"),SOURce::SOUR_VOLT_PROT_DEL_MAX},
          {QStringLiteral("SOUR:VOLT:PROT:DEL? DEF"),SOURce::SOUR_VOLT_PROT_DEL_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:PROT:DEL? MIN"),SOURce::SOUR_VOLT_PROT_DEL_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:PROT:DEL? MAX"),SOURce::SOUR_VOLT_PROT_DEL_MAX_INQUIRE},

          // 启用或禁用欠电压保护。如果已启用欠电压保护功能且输出进入电压限制，则将禁用输出
          {QStringLiteral("SOUR:VOLT:UND:PROT:STAT 1"),SOURce::SOUR_VOLT_UND_PROT_STAT_1},
          {QStringLiteral("SOUR:VOLT:UND:PROT:STAT ON"),SOURce::SOUR_VOLT_UND_PROT_STAT_ON},
          {QStringLiteral("SOUR:VOLT:UND:PROT:STAT 0"),SOURce::SOUR_VOLT_UND_PROT_STAT_0},
          {QStringLiteral("SOUR:VOLT:UND:PROT:STAT OFF"),SOURce::SOUR_VOLT_UND_PROT_STAT_OFF},
          {QStringLiteral("SOUR:VOLT:UND:PROT:STAT?"),SOURce::SOUR_VOLT_UND_PROT_STAT_INQUIRE},
          // 设定和查询欠电压保护的限定值
          {QStringLiteral("SOUR:VOLT:UND:PROT"),SOURce::SOUR_VOLT_UND_PROT}, // 需要空格
          {QStringLiteral("SOUR:VOLT:UND:PROT DEF"),SOURce::SOUR_VOLT_UND_PROT_DEF},
          {QStringLiteral("SOUR:VOLT:UND:PROT MIN"),SOURce::SOUR_VOLT_UND_PROT_MIN},
          {QStringLiteral("SOUR:VOLT:UND:PROT MAX"),SOURce::SOUR_VOLT_UND_PROT_MAX},
          {QStringLiteral("SOUR:VOLT:UND:PROT?"),SOURce::SOUR_VOLT_UND_PROT_INQUIRE},
          // 设定和查询过电流保护的延迟时间
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL"),SOURce::SOUR_VOLT_UND_PROT_DEL},// 需要空格
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL DEF"),SOURce::SOUR_VOLT_UND_PROT_DEL_DEF},
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL MIN"),SOURce::SOUR_VOLT_UND_PROT_DEL_MIN},
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL MAX"),SOURce::SOUR_VOLT_UND_PROT_DEL_MAX},
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL? DEF"),SOURce::SOUR_VOLT_UND_PROT_DEL_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL? MIN"),SOURce::SOUR_VOLT_UND_PROT_DEL_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:UND:PROT:DEL? MAX"),SOURce::SOUR_VOLT_UND_PROT_DEL_MAX_INQUIRE},
          // 启用或禁用过电流保护。如果已启用过电流保护功能且输出进入电流限制，则将禁用输出
          {QStringLiteral("SOUR:CURR:PROT:STAT 1"),SOURce::SOUR_CURR_PROT_STAT_1},
          {QStringLiteral("SOUR:CURR:PROT:STAT ON"),SOURce::SOUR_CURR_PROT_STAT_ON},
          {QStringLiteral("SOUR:CURR:PROT:STAT 0"),SOURce::SOUR_CURR_PROT_STAT_0},
          {QStringLiteral("SOUR:CURR:PROT:STAT OFF"),SOURce::SOUR_CURR_PROT_STAT_OFF},
          {QStringLiteral("SOUR:CURR:PROT:STAT?"),SOURce::SOUR_CURR_PROT_STAT_INQUIRE},
          // 设定和查询过电流保护的限定值
          {QStringLiteral("SOUR:CURR:PROT"),SOURce::SOUR_CURR_PROT}, // 需要空格
          {QStringLiteral("SOUR:CURR:PROT DEF"),SOURce::SOUR_CURR_PROT_DEF},
          {QStringLiteral("SOUR:CURR:PROT MIN"),SOURce::SOUR_CURR_PROT_MIN},
          {QStringLiteral("SOUR:CURR:PROT MAX"),SOURce::SOUR_CURR_PROT_MAX },
          {QStringLiteral("SOUR:CURR:PROT?"),SOURce::SOUR_CURR_PROT_INQUIRE},
          // 设定和查询过电流保护的延迟时间
          {QStringLiteral("SOUR:CURR:PROT:DEL"),SOURce::SOUR_CURR_PROT_DEL},
          {QStringLiteral("SOUR:CURR:PROT:DEL DEF"),SOURce::SOUR_CURR_PROT_DEL_DEF},// 需要空格
          {QStringLiteral("SOUR:CURR:PROT:DEL MIN"),SOURce::SOUR_CURR_PROT_DEL_MIN},
          {QStringLiteral("SOUR:CURR:PROT:DEL MAX"),SOURce::SOUR_CURR_PROT_DEL_MAX},
          {QStringLiteral("SOUR:CURR:PROT:DEL? DEF"),SOURce::SOUR_CURR_PROT_DEL_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:PROT:DEL? MIN"),SOURce::SOUR_CURR_PROT_DEL_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:PROT:DEL? MAX"),SOURce::SOUR_CURR_PROT_DEL_MAX_INQUIRE},
          // 启用或禁用欠电流保护。如果已启用欠电流保护功能且输出进入电流限制，则将禁用输出
          {QStringLiteral("SOUR:CURR:UND:PROT:STAT 1"),SOURce::SOUR_CURR_UND_PROT_STAT_1},
          {QStringLiteral("SOUR:CURR:UND:PROT:STAT ON"),SOURce::SOUR_CURR_UND_PROT_STAT_ON},
          {QStringLiteral("SOUR:CURR:UND:PROT:STAT 0"),SOURce::SOUR_CURR_UND_PROT_STAT_0},
          {QStringLiteral("SOUR:CURR:UND:PROT:STAT OFF"),SOURce::SOUR_CURR_UND_PROT_STAT_OFF},
          {QStringLiteral("SOUR:CURR:UND:PROT:STAT?"),SOURce::SOUR_CURR_UND_PROT_STAT_INQUIRE},
          // 设定和查询欠电流保护的限定值
          {QStringLiteral("SOUR:CURR:UND:PROT"),SOURce::SOUR_CURR_UND_PROT},// 需要空格
          {QStringLiteral("SOUR:CURR:UND:PROT DEF"),SOURce::SOUR_CURR_UND_PROT_DEF},
          {QStringLiteral("SOUR:CURR:UND:PROT MIN"),SOURce::SOUR_CURR_UND_PROT_MIN},
          {QStringLiteral("SOUR:CURR:UND:PROT MAX"),SOURce::SOUR_CURR_UND_PROT_MAX},
          {QStringLiteral("SOUR:CURR:UND:PROT?"),SOURce::SOUR_CURR_UND_PROT_INQUIRE},
          // 设定和查询过电流保护的延迟时间
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL"),SOURce::SOUR_CURR_UND_PROT_DEL},// 需要空格
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL DEF"),SOURce::SOUR_CURR_UND_PROT_DEL_DEF},
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL MIN"),SOURce::SOUR_CURR_UND_PROT_DEL_MIN},
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL MAX"),SOURce::SOUR_CURR_UND_PROT_DEL_MAX},
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL? DEF"),SOURce::SOUR_CURR_UND_PROT_DEL_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL? MIN"),SOURce::SOUR_CURR_UND_PROT_DEL_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:UND:PROT:DEL? MAX"),SOURce::SOUR_CURR_UND_PROT_DEL_MAX_INQUIRE},
          // 启用或禁用过功率保护。如果已启用过功率保护功能且输出进入功率限制，则将禁用输出
          {QStringLiteral("SOUR:POW:PROT:STAT 1"),SOURce::SOUR_POW_PROT_STAT_1},
          {QStringLiteral("SOUR:POW:PROT:STAT ON"),SOURce::SOUR_POW_PROT_STAT_ON},
          {QStringLiteral("SOUR:POW:PROT:STAT 0"),SOURce::SOUR_POW_PROT_STAT_0},
          {QStringLiteral("SOUR:POW:PROT:STAT OFF"),SOURce::SOUR_POW_PROT_STAT_OFF},
          {QStringLiteral("SOUR:POW:PROT:STAT?"),SOURce::SOUR_POW_PROT_STAT_INQUIRE},
          // 设定和查询过功率保护的限定值
          {QStringLiteral("SOUR:POW:PROT"),SOURce::SOUR_POW_PROT}, // 需要空格
          {QStringLiteral("SOUR:POW:PROT DEF"),SOURce::SOUR_POW_PROT_DEF},
          {QStringLiteral("SOUR:POW:PROT MIN"),SOURce::SOUR_POW_PROT_MIN},
          {QStringLiteral("SOUR:POW:PROT MAX"),SOURce::SOUR_POW_PROT_MAX},
          {QStringLiteral("SOUR:POW:PROT?"),SOURce::SOUR_POW_PROT_INQUIRE},
          // 设定和查询过功率保护的延迟时间
          {QStringLiteral("SOUR:POW:PROT:DEL"),SOURce::SOUR_POW_PROT_DEL},// 需要空格
          {QStringLiteral("SOUR:POW:PROT:DEL DEF"),SOURce::SOUR_POW_PROT_DEL_DEF},
          {QStringLiteral("SOUR:POW:PROT:DEL MIN"),SOURce::SOUR_POW_PROT_DEL_MIN},
          {QStringLiteral("SOUR:POW:PROT:DEL MAX"),SOURce::SOUR_POW_PROT_DEL_MAX},
          {QStringLiteral("SOUR:POW:PROT:DEL? DEF"),SOURce::SOUR_POW_PROT_DEL_DEF_INQUIRE},
          {QStringLiteral("SOUR:POW:PROT:DEL? MIN"),SOURce::SOUR_POW_PROT_DEL_MIN_INQUIRE},
          {QStringLiteral("SOUR:POW:PROT:DEL? MAX"),SOURce::SOUR_POW_PROT_DEL_MAX_INQUIRE},
          // 没有欠功率保护的相关命令
          /*（5）************************************电压、电流、功率的过、欠保护启用、限定值和延迟时间的设定 结束*************************************/


           /*（6）************************************欠电压、欠电流保护的温机时间 开始*************************************/
          // 该命令用来设定欠电压保护的温机时间，即仪器预热时间，设置该时间是为了防止电压在不断上升过程中达到保护点而触发了保护状态
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM"),SOURce::SOUR_VOLT_UND_PROT_WARM},// 需要空格
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM DEF"),SOURce::SOUR_VOLT_UND_PROT_WARM_DEF},
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM MIN"),SOURce::SOUR_VOLT_UND_PROT_WARM_MIN},
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM MAX"),SOURce::SOUR_VOLT_UND_PROT_WARM_MAX},
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM? DEF"),SOURce::SOUR_VOLT_UND_PROT_WARM_DEF_INQUIRE},
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM? MIN"),SOURce::SOUR_VOLT_UND_PROT_WARM_MIN_INQUIRE},
          {QStringLiteral("SOUR:VOLT:UND:PROT:WARM? MAX"),SOURce::SOUR_VOLT_UND_PROT_WARM_MAX_INQUIRE},
          // 设定和查询欠电流保护的温机时间，即仪器预热时间，设置该时间是为了防止电流在不断上升过程中达到保护点而触发了保护状态
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM"),SOURce::SOUR_CURR_UND_PROT_WARM},// 需要空格
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM DEF"),SOURce::SOUR_CURR_UND_PROT_WARM_DEF},
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM MIN"),SOURce::SOUR_CURR_UND_PROT_WARM_MIN},
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM MAX"),SOURce::SOUR_CURR_UND_PROT_WARM_MAX},
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM? DEF"),SOURce::SOUR_CURR_UND_PROT_WARM_DEF_INQUIRE},
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM? MIN"),SOURce::SOUR_CURR_UND_PROT_WARM_MIN_INQUIRE},
          {QStringLiteral("SOUR:CURR:UND:PROT:WARM? MAX"),SOURce::SOUR_CURR_UND_PROT_WARM_MAX_INQUIRE},
          /*（6）************************************欠电压、欠电流保护的温机时间 结束*************************************/

          /*（7）************************************其它命令 开始*************************************/
          // 设定和查询电源的功能模式，这样可以在启动并触发瞬变系统时确定输出电流的发生情况
          {QStringLiteral("SOUR:FUNC:MODE FIX"),SOURce::SOUR_FUNC_MODE_FIX},//表示工作在固定模式，将输出电流保持为其即时值(默认)
          {QStringLiteral("SOUR:FUNC:MODE LIST"),SOURce::SOUR_FUNC_MODE_LIST},//表示工作在List模式，发生触发时，LIST将导致输出跟随列表值
          {QStringLiteral("SOUR:FUNC:MODE BATT"),SOURce::SOUR_FUNC_MODE_BATT},// 表示工作在电池测试模式，发生触发时，将依据已编辑的电池测试文件进行输出
          {QStringLiteral("SOUR:FUNC:MODE SOL"),SOURce::SOUR_FUNC_MODE_SOL}, // 表示工作在光伏曲线模拟模式，发生触发时，将依据已选定的PV曲线进行输出
          {QStringLiteral("SOUR:FUNC:MODE CARP"),SOURce::SOUR_FUNC_MODE_CARP},//表示工作在汽车波形模拟模式，发生触发时，将依据已选定的汽车波形进行输出
          {QStringLiteral("SOUR:FUNC:MODE?"),SOURce::SOUR_FUNC_MODE_INQUIRE},
          // 启用或禁用或查询外部模拟量功能
          {QStringLiteral("SOUR:EXT:PROG 1"),SOURce::SOUR_EXT_PROG_1},
          {QStringLiteral("SOUR:EXT:PROG ON"),SOURce::SOUR_EXT_PROG_ON},
          {QStringLiteral("SOUR:EXT:PROG 0"),SOURce::SOUR_EXT_PROG_0},
          {QStringLiteral("SOUR:EXT:PROG OFF"),SOURce::SOUR_EXT_PROG_OFF},
          {QStringLiteral("SOUR:EXT:PROG?"),SOURce::SOUR_EXT_PROG_INQUIRE},
          // 设置和查询通道的线性标定系数。该指令需设置两个参数，通过逗号隔开，逗号前面的为通道号，逗号后面的为线性标定系数
          // 通道号支持1,2,3，线性标定系数在-5000~5000之间，默认值-5000
          {QStringLiteral("EXT:PROG:CHAN:MX 1,"),SOURce::SOUR_EXT_PROG_CHAN1_MX},
          {QStringLiteral("EXT:PROG:CHAN:MX 1,DEF"),SOURce::SOUR_EXT_PROG_CHAN1_MX_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MX 1,MIN"),SOURce::SOUR_EXT_PROG_CHAN1_MX_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MX 1,MAX"),SOURce::SOUR_EXT_PROG_CHAN1_MX_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MX 2,"),SOURce::SOUR_EXT_PROG_CHAN2_MX},
          {QStringLiteral("EXT:PROG:CHAN:MX 2,DEF"),SOURce::SOUR_EXT_PROG_CHAN2_MX_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MX 2,MIN"),SOURce::SOUR_EXT_PROG_CHAN2_MX_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MX 2,MAX"),SOURce::SOUR_EXT_PROG_CHAN2_MX_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MX 3,"),SOURce::SOUR_EXT_PROG_CHAN3_MX},
          {QStringLiteral("EXT:PROG:CHAN:MX 3,DEF"),SOURce::SOUR_EXT_PROG_CHAN3_MX_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MX 3,MIN"),SOURce::SOUR_EXT_PROG_CHAN3_MX_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MX 3,MAX"),SOURce::SOUR_EXT_PROG_CHAN3_MX_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MX? 1"),SOURce::SOUR_EXT_PROG_CHAN1_MX_INQUIRE},
          {QStringLiteral("EXT:PROG:CHAN:MX? 2"),SOURce::SOUR_EXT_PROG_CHAN2_MX_INQUIRE},
          {QStringLiteral("EXT:PROG:CHAN:MX? 3"),SOURce::SOUR_EXT_PROG_CHAN3_MX_INQUIRE},

          //设置和查询通道的偏移标定系数。该指令需设置两个参数，通过逗号隔开，逗号前面的为通道号，逗号后面的为偏移标定系数。
          // 通道号支持1,2,3，偏移标定系数在-5000~5000之间，默认值-5000
          {QStringLiteral("EXT:PROG:CHAN:MB 1"),SOURce::SOUR_EXT_PROG_CHAN1_MB},
          {QStringLiteral("EXT:PROG:CHAN:MB 1,DEF"),SOURce::SOUR_EXT_PROG_CHAN1_MB_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MB 1,MIN"),SOURce::SOUR_EXT_PROG_CHAN1_MB_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MB 1,MAX"),SOURce::SOUR_EXT_PROG_CHAN1_MB_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MB 2"),SOURce::SOUR_EXT_PROG_CHAN2_MB},
          {QStringLiteral("EXT:PROG:CHAN:MB 2,DEF"),SOURce::SOUR_EXT_PROG_CHAN2_MB_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MB 2,MIN"),SOURce::SOUR_EXT_PROG_CHAN2_MB_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MB 2,MAX"),SOURce::SOUR_EXT_PROG_CHAN2_MB_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MB 3"),SOURce::SOUR_EXT_PROG_CHAN3_MB},
          {QStringLiteral("EXT:PROG:CHAN:MB 3,DEF"),SOURce::SOUR_EXT_PROG_CHAN3_MB_DEF},
          {QStringLiteral("EXT:PROG:CHAN:MB 3,MIN"),SOURce::SOUR_EXT_PROG_CHAN3_MB_MIN},
          {QStringLiteral("EXT:PROG:CHAN:MB 3,MAX"),SOURce::SOUR_EXT_PROG_CHAN3_MB_MAX},
          {QStringLiteral("EXT:PROG:CHAN:MB? 1"),SOURce::SOUR_EXT_PROG_CHAN1_MB_INQUIRE},
          {QStringLiteral("EXT:PROG:CHAN:MB? 2"),SOURce::SOUR_EXT_PROG_CHAN2_MB_INQUIRE},
          {QStringLiteral("EXT:PROG:CHAN:MB? 3"),SOURce::SOUR_EXT_PROG_CHAN3_MB_INQUIRE},
          {QStringLiteral("SOUR:FILT:LEV LOW"),SOURce::SOUR_FILT_LEV_LOW},// 设定滤波等级
          {QStringLiteral("SOUR:FILT:LEV MED"),SOURce::SOUR_FILT_LEV_MED},
          {QStringLiteral("SOUR:FILT:LEV FAST"),SOURce::SOUR_FILT_LEV_FAST},
          {QStringLiteral("SOUR:FILT:LEV?"),SOURce::SOUR_FILT_LEV_INQUIRE},
           /*（7）************************************其它命令 结束*************************************/

    }; // end QHash<QString,SOURce> SOURCE

    inline static QByteArray SourceCommandBytes(SOURce command)
    {
          return SOURCE.key(command).toLocal8Bit();
    }

    enum  class MEASure{

          // 启动、触发并返回电流测量的平均值
          MEAS_CURR_INQUIRE, // "MEAS:CURR?"
          MEAS_CURR_AVE_INQUIRE = MEAS_CURR_INQUIRE,
          // 启动、触发并返回电流测量的最大值
          MEAS_CURR_MAX_INQUIRE, // "MEAS:CURR:MAX?"
          // 启动、触发并返回电流测量的最小值
          MEAS_CURR_MIN_INQUIRE, // "MEAS:CURR:MIN?"
          // 启动、触发并返回电流脉冲波形的高电平
          MEAS_CURR_HIGH_INQUIRE, // "MEAS:CURR:HIGH?"
          // 启动、触发并返回电流脉冲波形的低电平
          MEAS_CURR_LOW_INQUIRE, // "MEAS:CURR:LOW?"
          // 启动并触发电流测量，返回数字化电流测量样本的列表
          MEAS_ARR_CURR, // "MEAS:ARR:CURR?"


          // 启动、触发并返回电压测量的平均值
          MEAS_VOLT_INQUIRE, // "MEAS:VOLT?"
          MEAS_VOLT_AVE_INQUIRE = MEAS_VOLT_INQUIRE,
          // 启动、触发并返回电压测量的最大值
          MEAS_VOLT_MAX_INQUIRE, // "MEAS:VOLT:MAX?"
          // 启动、触发并返回电压测量的最小值
          MEAS_VOLT_MIN_INQUIRE, // "MEAS:VOLT:MIN?"
          // 启动、触发并返回电压脉冲波形的高电平
          MEAS_VOLT_HIGH_INQUIRE, // "MEAS:VOLT:HIGH?"
          // 启动、触发并返回电压脉冲波形的低电平
          MEAS_VOLT_LOW_INQUIRE, // "MEAS:VOLT:LOW?"
          // 启动并触发电压测量，返回数字化电压测量样本的列表
          MEAS_ARR_VOLT, // "MEAS:ARR:VOLT?"

          // 启动、触发并返回功率测量的平均值
          MEAS_POW_INQUIRE, // "MEAS:POW?"
          MEAS_POW_AVE_INQUIRE = MEAS_POW_INQUIRE,
          // 启动、触发并返回功率测量的最大值
          MEAS_POW_MAX_INQUIRE, // "MEAS:POW:MAX?"
          // 启动、触发并返回功率测量的最小值
          MEAS_POW_MIN_INQUIRE, // "MEAS:POW:MIN?"
          // 启动并触发功率测量，返回数字化功率测量样本的列表
          MEAS_ARR_POW, // "MEAS:ARR:POW?"

          // 测量多种数据：电压，电流，功率，安时，瓦时
          MEAS_INQUIRE, // "MEAS?"

          // 测量本地电压
          MEAS_LOCA_VOLT_INQUIRE, // "MEAS:LOCAI:VOLT?"
          // 测量远端电压
          MEAS_REMO_VOLT_INQUIRE, // "MEAS:REMO:VOLT?"

          // 测量安培/瓦特-小时值。可再生能量的机型适用的指令。此类测量独立于其他测量
          // 通过累积全范围电流测量，约为200k 样本/秒，创建安培-小时测量
          // 累加器可以承载至少100,000小时的充足信息
          MEAS_AHO_INQUIRE,// "MEAS:AHO?"
          MEAS_WHO_INQUIRE// "MEAS:WHO?"

    };

    static QHash<QString,MEASure> MEASURE = {

          {QStringLiteral("MEAS:CURR?"),                  MEASure::MEAS_CURR_INQUIRE},// 启动、触发并返回电流测量的平均值
          {QStringLiteral("MEAS:CURR?"),                  MEASure::MEAS_CURR_AVE_INQUIRE},
          {QStringLiteral("MEAS:CURR:MAX?"),        MEASure::MEAS_CURR_MAX_INQUIRE}, // 启动、触发并返回电流测量的最大值
          {QStringLiteral("MEAS:CURR:MIN?"),          MEASure::MEAS_CURR_MIN_INQUIRE}, // 启动、触发并返回电流测量的最小值
          {QStringLiteral("MEAS:CURR:HIGH?"),        MEASure::MEAS_CURR_HIGH_INQUIRE}, // 启动、触发并返回电流脉冲波形的高电平
          {QStringLiteral("MEAS:CURR:LOW?"),         MEASure::MEAS_CURR_LOW_INQUIRE}, // 启动、触发并返回电流脉冲波形的低电平
          {QStringLiteral("MEAS:ARR:CURR?"),          MEASure::MEAS_ARR_CURR}, // 启动并触发电流测量，返回数字化电流测量样本的列表

          {QStringLiteral("MEAS:VOLT?"),                    MEASure::MEAS_VOLT_INQUIRE},// 启动、触发并返回电压测量的平均值
          {QStringLiteral("MEAS:VOLT?"),                    MEASure::MEAS_VOLT_AVE_INQUIRE},
          {QStringLiteral("MEAS:VOLT:MAX?"),          MEASure::MEAS_VOLT_MAX_INQUIRE}, // 启动、触发并返回电压测量的最大值
          {QStringLiteral("MEAS:VOLT:MIN?"),           MEASure::MEAS_VOLT_MIN_INQUIRE}, // 启动、触发并返回电压测量的最小值
          {QStringLiteral("MEAS:VOLT:HIGH?"),         MEASure::MEAS_VOLT_HIGH_INQUIRE}, // 启动、触发并返回电压脉冲波形的高电平
          {QStringLiteral("MEAS:VOLT:LOW?"),          MEASure::MEAS_VOLT_LOW_INQUIRE}, // 启动、触发并返回电压脉冲波形的低电平
          {QStringLiteral("MEAS:ARR:VOLT?"),           MEASure::MEAS_ARR_CURR }, // 启动并触发电压测量，返回数字化电压测量样本的列表

          {QStringLiteral("MEAS:POW?"),                     MEASure::MEAS_POW_INQUIRE},// 启动、触发并返回功率测量的平均值
          {QStringLiteral("MEAS:POW?"),                     MEASure::MEAS_POW_AVE_INQUIRE},
          {QStringLiteral("MEAS:POW:MAX?"),           MEASure::MEAS_POW_MAX_INQUIRE}, // 启动、触发并返回功率测量的最大值
          {QStringLiteral("MEAS:POW:MIN?"),            MEASure::MEAS_POW_MIN_INQUIRE}, // 启动、触发并返回功率测量的最小值
          {QStringLiteral("MEAS:ARR:POW?"),           MEASure::MEAS_ARR_POW}, // 启动并触发功率测量，返回数字化功率测量样本的列表

          {QStringLiteral("MEAS?"),                             MEASure:: MEAS_INQUIRE}, // 测量多种数据：电压，电流，功率，安时，瓦时
          {QStringLiteral("MEAS:LOCAI:VOLT?"),     MEASure::MEAS_LOCA_VOLT_INQUIRE},// 测量远端电压
          {QStringLiteral("MEAS:REMO:VOLT?"),     MEASure::MEAS_REMO_VOLT_INQUIRE},// 测量本地电压
          {QStringLiteral("MEAS:AHO?"),                   MEASure::MEAS_AHO_INQUIRE}, // 测量安培-小时值
          {QStringLiteral("MEAS:WHO?"),                  MEASure::MEAS_WHO_INQUIRE} //测量瓦特-小时值

    }; // end QHash<MEASure,QString> MEASURE

    inline static QByteArray MeasureCommandBytes(MEASure command)
    {
          return  MEASURE.key(command).toLocal8Bit();
    }

    // 使用带头路径的命令：MEAS:VOLT?;CURR?;POW?\n
    // 或者使用不带头路径的完整命令：MEAS:VOLT?;:MEAS:CURR?:MEAS:POW?\n 这里选择第2种类型

   // 固定查询命令的常量字符串：电压、电流、功率和电阻(暂时空出)的命令
    static const QByteArray MEAS_VCPR =  MeasureCommandBytes(MEASure::MEAS_VOLT_AVE_INQUIRE)+ //MEAS:VOLT?
                                                                        SymbolCommandBytes(SYMbol::SEMICOLON)+// 分号";"
                                                                        SymbolCommandBytes(SYMbol::COLON)+// 冒号 ":"
                                                                        MeasureCommandBytes(MEASure::MEAS_CURR_AVE_INQUIRE)+ // MEAS:CURR?
                                                                        SymbolCommandBytes(SYMbol::SEMICOLON)+// 分号";"
                                                                        SymbolCommandBytes(SYMbol::COLON)+// 冒号 ":"
                                                                        MeasureCommandBytes(MEASure::MEAS_POW_AVE_INQUIRE)+ // MEAS:POW?
                                                                        SymbolCommandBytes(SYMbol::EOI); // 结束符"\n"

    // 设定电压、电流、功率和电阻命令的快捷转换函数
    static QByteArray getSetCommand(MONITORROLE role,const QByteArray& value)
    {
          QByteArray ret;
          switch (role)
          {
                case MONITORROLE::PowerSource: ret = SourceCommandBytes(SOURce::SOUR_POW_LIM);break; // "SOUR:POW:LIM 10"
                case MONITORROLE::PowerLoad: ret = SourceCommandBytes(SOURce::SOUR_POW_LIM_NEG);break; // "SOUR:POW:LIM:NEG 10"
                case MONITORROLE::ResistanceSource: ret = SourceCommandBytes(SOURce::SOUR_RES_LIM);break;// "SOUR:RES:LIM 10"
                case MONITORROLE::ResistanceLoad: ret = SourceCommandBytes(SOURce::SOUR_RES_LIM_NEG);break;// "SOUR:RES:LIM:NEG 10"

                case MONITORROLE::Voltage: ret = SourceCommandBytes(SOURce::SOUR_VOLT);break; // "SOUR:VOLT 50"
                case MONITORROLE::CurrentSource: ret = SourceCommandBytes(SOURce::SOUR_CURR_LIM);break; // "SOUR:CURR:LIM 10"
                case MONITORROLE::CurrentLoad: ret = SourceCommandBytes(SOURce::SOUR_CURR_LIM_NEG);break; // "SOUR:CURR:LIM:NEG 10"

                case MONITORROLE::Current: ret = SourceCommandBytes(SOURce::SOUR_CURR);break; // "SOUR:CURR 50"
                case MONITORROLE::VoltageSource: ret = SourceCommandBytes(SOURce::SOUR_VOLT_LIM);break; // "SOUR:VOLT:LIM 10"
                case MONITORROLE::VoltageLoad: ret = SourceCommandBytes(SOURce::SOUR_VOLT_LIM_NEG);break; // "SOUR:VOLT:LIM:NEG 10"
                default: break; // 没有发送角色直接退出
          }
          // 命令+空格+值+结束符才是完整的命令(结束符在setVariable内添加)
          ret = ret +SymbolCommandBytes(SYMbol::SPACE) + value;
          return ret;
    }

    // 查询电压、电流、功率和电阻命令的快捷转换函数
    static QByteArray getQueryCommand(MONITORROLE role)
    {
          QByteArray  ret;
          switch (role)
          {
                case MONITORROLE::PowerSource: ret = SourceCommandBytes(SOURce::SOUR_POW_LIM_INQUIRE);break; // "SOUR:POW:LIM?"
                case MONITORROLE::PowerLoad: ret = SourceCommandBytes(SOURce::SOUR_POW_LIM_NEG_INQUIRE);break;// "SOUR:POW:LIM:NEG?"
                case MONITORROLE::ResistanceSource: ret = SourceCommandBytes(SOURce::SOUR_RES_LIM_INQUIRE);break;// "SOUR:RES:LIM?"
                case MONITORROLE::ResistanceLoad: ret = SourceCommandBytes(SOURce::SOUR_RES_LIM_NEG_INQUIRE);break;// "SOUR:RES:LIM:NEG?"

                case MONITORROLE::Voltage: ret = SourceCommandBytes(SOURce::SOUR_VOLT_INQUIRE);break;//"SOUR:VOLT?"
                case MONITORROLE::CurrentSource: ret = SourceCommandBytes(SOURce::SOUR_CURR_LIM_INQUIRE);break;// "SOUR:CURR:LIM?"
                case MONITORROLE::CurrentLoad: ret = SourceCommandBytes(SOURce::SOUR_CURR_LIM_NEG_INQUIRE);break;// "SOUR:CURR:LIM:NEG?"

                case MONITORROLE::Current: ret = SourceCommandBytes(SOURce::SOUR_CURR_INQUIRE);break;// "SOUR:CURR?"
                case MONITORROLE::VoltageSource: ret = SourceCommandBytes(SOURce::SOUR_VOLT_LIM_INQUIRE);break;// "SOUR:VOLT:LIM?"
                case MONITORROLE::VoltageLoad: ret = SourceCommandBytes(SOURce::SOUR_VOLT_LIM_NEG_INQUIRE);break;// "SOUR:VOLT:LIM:NEG?"

                default: break;
          }
          return ret;
    }

    // 设定和查询命令的合并命令字符串快捷转换函数
    static QByteArray getSetQueryCommand(MONITORROLE role,const QByteArray& value)
    {
           // "SOUR:VOLT 7.000",""SOUR:VOLT?" => "SOUR:VOLT 7.000;:SOUR:VOLT?\n" 设定后紧接着查询设定的结果
           auto sc = getSetCommand(role,value);
           auto qc = getQueryCommand(role);
           auto c = sc+SymbolCommandBytes(SYMbol::SEMICOLON)+
                               SymbolCommandBytes(SYMbol::COLON)+qc +
                               SymbolCommandBytes(SYMbol::EOI);
           return c;
    }

/*
    enum  class FORMat{
          FORM_ASC, // "FORM:ASC"
          FORM_BIN, // "FORM:BIN"
          FORM_INQUIRE // "FORM?"
    };
    static QHash<QString,FORMat> FORMAT = {

           {QStringLiteral("FORM:ASC"),FORMat::FORM_ASC},
           {QStringLiteral("FORM:BIN"),FORMat::FORM_BIN},
           {QStringLiteral("FORM?"),FORMat::FORM_INQUIRE},

    }; // end QHash<QStringFORMat> FORMAT

    inline static QByteArray FormatCommandBytes(FORMat format)
    {
          return FORMAT.key(format).toLocal8Bit();
    }
*/

/*
    enum  class IEee488{
         RST, //"*RST" 将仪器重置为典型或安全的预定义值
         CLS,//"*CLS" 清除状态命令，清除所有寄存器组中的事件寄存器。同时清除状态字节和错误队列
         TRG,// "*TRG" 当触发子系统选定总线作为其触发源时，执行该命令将生成触发
         WAL, // "*WAL" 在所有的未完成命令完成之前，将暂停其他命令的处理

         SAV,// "*SAV 0" 将仪器[On/Off]开关状态或一些参数设置保存到10个非易失性存储器中，位置可设置为0~9
         RCL,// "*RCL 0" 从指定的存储区域中调用仪器的参数设置，范围0-9

         IDN_INQUIRE,//"*IDN?" 查看仪器序列号、程序版本号等信息
         TST_INQUIRE, // "*TST?" 执行仪器自检。如果自检失败，一个或多个错误消息将生成
         ESR_INQUIRE,// "*ESR?" 读取并清除标准事件状态组的事件寄存器
         STB_INQUIRE, // "*STB?" 状态字节查询

         SRE,// "*SRE 24" 服务请求启用命令 启用使能寄存器中的第3位和第4位
         SRE_INQUIRE, // "*SRE? "查询服务请求使能寄存器的值

         PSC, // "*PSC 0/1/ON/OFF" 该指令用于控制是否在上电时将状态寄存器清零。该指令影响下一次上电时状态寄存器的值
         PSC_INQUIRE, // "*PSC?" 查询是否在上电时将状态寄存器清零

         ESE, // "*ESE 24" 为标准事件状态组设置使能寄存器的值 设置范围为0~255,启用使能寄存器中的3和4位
         ESE_INQUIRE, // "*ESE?" 查询标准事件状态组设置的使能寄存器的值

         OPC, // "*OPC"在标准事件寄存器中设置OPC（操作完成）位
         OPC_INQUIRE, // "*OPC?" 在所有的未完成操作完成后，将1返回到输出缓冲器。响应将延迟，直到所有挂起操作完成为止

    }; // end enum IEEE488
    static QHash <QString,IEee488> IEEE488 = {

          {QStringLiteral("*RST"),IEee488::RST},
          {QStringLiteral("*CLS"),IEee488::CLS},
          {QStringLiteral("*TRG"),IEee488::TRG},
          {QStringLiteral("*WAL"),IEee488::WAL},

          {QStringLiteral("*SAV"),IEee488::SAV}, // 需要空格 SAV和RCL需要跟上1个数,范围0-9
          {QStringLiteral("*RCL"),IEee488::RCL}, // 需要空格

          {QStringLiteral("*IDN"),IEee488::IDN_INQUIRE},
          {QStringLiteral("*TST"),IEee488::TST_INQUIRE},
          {QStringLiteral("*ESR"),IEee488::ESR_INQUIRE},
          {QStringLiteral("*STB"),IEee488::STB_INQUIRE},

          {QStringLiteral("*SRE"),IEee488::SRE}, // 需要空格 数值范围0-255
          {QStringLiteral("*SRE?"),IEee488::SRE_INQUIRE},

          {QStringLiteral("*PSC"),IEee488::PSC}, // 需要空格 ON/OFF
          {QStringLiteral("*PSC?"),IEee488::PSC_INQUIRE},

          {QStringLiteral("*ESE"),IEee488::ESE}, // 需要空格 数值范围0-255
          {QStringLiteral("*ESE?"),IEee488::ESE_INQUIRE},

          {QStringLiteral("*OPC"),IEee488::OPC}, // 不需要空格
          {QStringLiteral("*OPC?"),IEee488::OPC_INQUIRE},

    }; // end static QHash <QString,IEee488> IEEE488

    inline static QByteArray IEEE488CommandBytes(IEee488 command)
    {
          return IEEE488.key(command).toLocal8Bit();
    }
*/

/*
    enum  class SYSTem{

          SYST_POS_SAV0, // "SYST:POS SAV0" 等价于"*SAV0" 设置仪器上电时的一些参数设置或者工作状态
          SYST_POS_RST, //  "SYST:POS RST" 等价于"*RST" 设置仪器上电时的一些参数设置或者工作状态
          SYST_POS_INQUIRE, // "SYST:POS?" 查询仪器上电时的一些参数设置或者工作状态

          SYST_VERS_INQUIRE, // "SYST:VERS?" 查询当前使用的SCPI命令的版本号
          SYST_ERR_INQUIRE, // "SYST:ERR?" 查询仪器的错误信息情况
          SYST_CLE, //"SYST:CLE" 清除错误序列
          SYST_REM, // "SYST:REM"  设置仪器为远程控制模式(除Local键均不能使用)
          SYST_LOC,// "SYST:LOC" 设置仪器为本地模式
          SYST_RWL,// "SYST:RWL" 设置设备到远程状态，并锁上本地按键

          SYST_BEEP_IMM, // "SYST:BEEP:IMM" 设定蜂鸣器发出一次响声
          SYST_BEEP_ON,// "SYST:BEEP ON" 设定蜂鸣器使能(默认值)
          SYST_BEEP_OFF,// "SYST:BEEP OFF" 设定蜂鸣器失能
          SYST_BEEP_INQUIRE, // "SYST:BEEP?" 查询蜂鸣器的状态

          SYST_DATE,//"SYST:DATE 2017,06,30"  设置系统时钟的日期
          SYST_DATE_INQUIRE, //"SYST:DATE?" 查询系统时钟的日期
          SYST_TIME, // "SYST:TIME 20,30,0" 设置系统时钟的时间
          SYST_TIME_INQUIRE,//"SYST:TIME?" 查询系统时钟的时间

          // 设定通信端口,仪器标配USB、CAN、LAN、VCP四种接口，支持选配RS232、GPIB通讯接口
          SYST_COMM_SEL_USB,  // "SYST:COMM:SEL USB"
          SYST_COMM_SEL_CAN, // "SYST:COMM:SEL CAN"
          SYST_COMM_SEL_LAN, // "SYST:COMM:SEL LAN"
          SYST_COMM_SEL_VCP, // "SYST:COMM:SEL VCP"
          SYST_COMM_SEL_RS232, // "SYST:COMM:SEL RS232"
          SYST_COMM_SEL_GPIB, // "SYST:COMM:SEL GPIB"
          SYST_COMM_SEL_INQUIRE, // "SYST:COMM:SEL?"
          SYST_COMM_GPIB_ADDR,// "SYST:COMM:GPIB:ADDR 2" 设定GPIB的通讯地址,可设置的范围1~30
          SYST_COMM_GPIB_ADDR_INQUIRE,// "SYST:COMM:GPIB:ADDR?" 查询GPIB的通讯地址

          // 设定和查询RS232的波特率
          SYST_COMM_SER_BAUD_4800, // "SYST:COMM:SER:BAUD 4800"
          SYST_COMM_SER_BAUD_9600, // "SYST:COMM:SER:BAUD 9600" (默认值)
          SYST_COMM_SER_BAUD_19200, // "SYST:COMM:SER:BAUD 19200"
          SYST_COMM_SER_BAUD_38400, // "SYST:COMM:SER:BAUD 38400"
          SYST_COMM_SER_BAUD_57600, // "SYST:COMM:SER:BAUD 57600"
          SYST_COMM_SER_BAUD_115200, // "SYST:COMM:SER:BAUD 115200"
          SYST_COMM_SER_BAUD_INQUIRE,// "SYST:COMM:SER:BAUD?"

          // 设定和查询仪器的IP地址
          SYST_COMM_LAN_IP, // "SYST:COMM:LAN:IP "192.168.0.11""
          SYST_COMM_LAN_IP_INQUIRE,//"SYST:COMM:LAN:IP?"
          // 设定和查询LAN端口的IP模式,手动或自动
          SYST_COMM_LAN_IP_MODE_MAN,// "SYST:COMM:LAN:IP:MODE MAN"
          SYST_COMM_LAN_IP_MODE_AUTO,// "SYST:COMM:LAN:IP:MODE AUTO"
          SYST_COMM_LAN_IP_MODE_INQUIRE,// "SYST:COMM:LAN:IP:MODE?"
          // 设定和查询子网掩码
          SYST_COMM_LAN_SMAS, // "SYST:COMM:LAN:SMAS "255.255.255.1""
          SYST_COMM_LAN_SMAS_INQUIRE, // "SYST:COMM:LAN:SMAS?"
          // 设定LAN的网关地址
          SYST_COMM_LAN_DGAT,//"SYST:COMM:LAN:DGAT "192.168.0.1""
          SYST_COMM_LAN_DGAT_INQUIRE,//"SYST:COMM:LAN:DGAT?"

    }; // end enum SYSTem

    static QHash<QString,SYSTem> SYSTEM =
    {
            {QStringLiteral("SYST:POS SAV0"),SYSTem::SYST_POS_SAV0}, // 等价于"*SAV0" 设置仪器上电时的一些参数设置或者工作状态
            {QStringLiteral("SYST:POS RST"),SYSTem::SYST_POS_RST}, // 等价于"*RST" 设置仪器上电时的一些参数设置或者工作状态
            {QStringLiteral("SYST:POS?"),SYSTem::SYST_POS_INQUIRE}, // 查询仪器上电时的一些参数设置或者工作状态
            {QStringLiteral("SYST:VERS?"),SYSTem::SYST_VERS_INQUIRE},//查询当前使用的SCPI命令的版本号
            {QStringLiteral("SYST:ERR?"),SYSTem::SYST_ERR_INQUIRE},//查询仪器的错误信息情况
            {QStringLiteral("SYST:CLE"),SYSTem::SYST_CLE}, // 清除错误序列
            {QStringLiteral("SYST:REM"),SYSTem::SYST_REM},// 设置仪器为远程控制模式(除Local键均不能使用)
            {QStringLiteral("SYST:LOC"),SYSTem::SYST_LOC}, // 设置仪器为本地模式
            {QStringLiteral("SYST:RWL"),SYSTem::SYST_RWL}, // 设置设备到远程状态，并锁上本地按键
            {QStringLiteral("SYST:BEEP:IMM"),SYSTem::SYST_BEEP_IMM},//设定蜂鸣器发出一次响声
            {QStringLiteral("SYST:BEEP:ON"),SYSTem::SYST_BEEP_ON},//设定蜂鸣器使能(默认值)
            {QStringLiteral("SYST:BEEP:OFF"),SYSTem::SYST_BEEP_OFF},//设定蜂鸣器失能
            {QStringLiteral("SYST:BEEP?"),SYSTem::SYST_BEEP_INQUIRE},//查询蜂鸣器的状态
            {QStringLiteral("SYST:DATE"),SYSTem::SYST_DATE}, // 设置系统时钟的日期(要加空格设置2017,06,30)
            {QStringLiteral("SYST:DATE?"),SYSTem::SYST_DATE_INQUIRE},//查询系统时钟的日期
            {QStringLiteral("SYST:TIME"),SYSTem::SYST_TIME},// 设置系统时钟的时间(要加空格设置20,30,0)
            {QStringLiteral("SYST:TIME?"),SYSTem::SYST_TIME_INQUIRE},// 查询系统时钟的时间
            {QStringLiteral("SYST:COMM:SEL USB"),SYSTem::SYST_COMM_SEL_USB}, // 设置USB接口
            {QStringLiteral("SYST:COMM:SEL LAN"),SYSTem::SYST_COMM_SEL_LAN}, // 设置LAN接口
            {QStringLiteral("SYST:COMM:SEL GPIB"),SYSTem::SYST_COMM_SEL_GPIB},//设置GPIB接口
            {QStringLiteral("SYST:COMM:SEL CAN"),SYSTem::SYST_COMM_SEL_CAN}, // 设置CAN接口
            {QStringLiteral("SYST:COMM:SEL RS232"),SYSTem::SYST_COMM_SEL_RS232}, // 设置RS232接口
            {QStringLiteral("SYST:COMM:SEL?"),SYSTem::SYST_COMM_SEL_INQUIRE}, // 查询接口类型
            {QStringLiteral("SYST:COMM:GPIB:ADDR"),SYSTem::SYST_COMM_GPIB_ADDR},//设置GPIB通讯地址,(要加空格设置范围1-30)
            {QStringLiteral("SYST:COMM:GPIB:ADDR?"),SYSTem::SYST_COMM_GPIB_ADDR_INQUIRE},//查询GPIB通讯地址
            {QStringLiteral("SYST:COMM:SER:BAUD 4800"),SYSTem::SYST_COMM_SER_BAUD_4800}, // 波特率4800
            {QStringLiteral("SYST:COMM:SER:BAUD 9600"),SYSTem::SYST_COMM_SER_BAUD_9600},// 波特率9600
            {QStringLiteral("SYST:COMM:SER:BAUD 19200"),SYSTem::SYST_COMM_SER_BAUD_19200},// 波特率19200
            {QStringLiteral("SYST:COMM:SER:BAUD 38400"),SYSTem::SYST_COMM_SER_BAUD_38400},// 波特率38400
            {QStringLiteral("SYST:COMM:SER:BAUD 57600"),SYSTem::SYST_COMM_SER_BAUD_57600},// 波特率57600
            {QStringLiteral("SYST:COMM:SER:BAUD 115200"),SYSTem::SYST_COMM_SER_BAUD_115200},// 波特率115200
            {QStringLiteral("SYST:COMM:SER:BAUD?"),SYSTem::SYST_COMM_SER_BAUD_INQUIRE},// 查询波特率
            {QStringLiteral("SYST:COMM:LAN:IP"),SYSTem::SYST_COMM_LAN_IP}, // 设定IP地址(需要加空格"192.168.0.11",IP地址用双引号括起来)
            {QStringLiteral("SYST:COMM:LAN:IP?"),SYSTem::SYST_COMM_LAN_IP_INQUIRE}, // 查询IP地址
            {QStringLiteral("SYST:COMM:LAN:IP:MODE MAN"),SYSTem::SYST_COMM_LAN_IP_MODE_MAN}, // 设定LAN端口的IP模式为手动
            {QStringLiteral("SYST:COMM:LAN:IP:MODE AUTO"),SYSTem::SYST_COMM_LAN_IP_MODE_AUTO},// 设定LAN端口的IP模式为自动
            {QStringLiteral("SYST:COMM:LAN:IP:MODE?"),SYSTem::SYST_COMM_LAN_IP_MODE_INQUIRE},// 查询LAN端口IP模式
            {QStringLiteral("SYST:COMM:LAN:SMAS"),SYSTem::SYST_COMM_LAN_SMAS}, // 设定子网掩码(需要空格"255.255.255.1")
            {QStringLiteral("SYST:COMM:LAN:SMAS?"),SYSTem::SYST_COMM_LAN_SMAS_INQUIRE}, // 查询子网掩码
            {QStringLiteral("SYST:COMM:LAN:DGAT"),SYSTem::SYST_COMM_LAN_DGAT},//设定网关地址(需要空格"192.168.0.1")
            {QStringLiteral("SYST:COMM:LAN:DGAT?"),SYSTem::SYST_COMM_LAN_DGAT_INQUIRE}// 查询网关地址

    }; // end QHash<SYSTem,QString> SYST_QSTRING

    inline static QByteArray SystemCommandBytes(SYSTem command)
    {
          return SYSTEM.key(command).toLocal8Bit();
    }

*/

/*
    enum  class OUTPut{

          // 查询或启用、禁用电源输出
          OUTP_1, // "OUTP 1"
          OUTP_ON = OUTP_1, // "OUTP ON"
          OUTP_0, // "OUTP 0"
          OUTP_OFF = OUTP_0, // "OUTP OFF"

          // 设定和查询输出关闭的延迟时间
          OUTP_DEL_FALL, // "OUTP:DEL:FALL 6" (自定义值)
          OUTP_DEL_FALL_DEF, // "OUTP:DEL:FALL DEF"(默认0s)
          OUTP_DEL_FALL_MIN, // "OUTP:DEL:FALL MIN"
          OUTP_DEL_FALL_MAX, // "OUTP:DEL:FALL MAX"
          OUTP_DEL_FALL_DEF_INQUIRE, // "OUTP:DEL:FALL? DEF"
          OUTP_DEL_FALL_MIN_INQUIRE, // "OUTP:DEL:FALL? MIN"
          OUTP_DEL_FALL_MAX_INQUIRE, // "OUTP:DEL:FALL? MAX"

          // 设定和查询输出打开的延迟时间
          OUTP_DEL_RISE, // "OUTP:DEL:RISE 6" (自定义值)
          OUTP_DEL_RISE_DEF, // "OUTP:DEL:RISE DEF"
          OUTP_DEL_RISE_MIN, // "OUTP:DEL:RISE MIN"
          OUTP_DEL_RISE_MAX, // "OUTP:DEL:RISE MAX"
          OUTP_DEL_RISE_DEF_INQUIRE, // "OUTP:DEL:RISE? DEF"
          OUTP_DEL_RISE_MIN_INQUIRE, // "OUTP:DEL:RISE? MIN"
          OUTP_DEL_RISE_MAX_INQUIRE, // "OUTP:DEL:RISE? MAX"

          // 设置和查询数字IO的禁止输出模式
          OUTP_INH_MODE_LIVE, // "OUTP:INH:MODE LIVE"
          OUTP_INH_MODE_LATC, // "OUTP:INH:MODE LATC"
          OUTP_INH_MODE_INQUIRE,//"OUTPut:INHibit:MODE?"

          // 控制仪器上电时的一些参数设置以及输出状态：打开或关闭
          // RST：默认值，表示仪器开机上电时显示出厂时的初始化值
          // LAST_ON：表示仪器在下一次开机上电时显示的参数设置和输出状态为本次设置的参数和状态
          // LAST_OFF：表示仪器在下一次开机上电时显示的参数设置为本次设置的参数，且输出状态为Off
          // 其中，LAST_ON和LAST_OFF中涉及的参数设置为*SAV 0指令相关的参数
          OUTP_PON_STAT_RST, // "OUTP:PON:STAT RST"(默认值)
          OUTP_PON_STAT_LAST_ON, // "OUTP:PON:STAT LAST_ON"
          OUTP_PON_STAT_LAST_OFF, // "OUTP:PON:STAT LAST_OFF"
          OUTP_PON_STAT_INQUIRE, // "OUTP:PON:STAT?"

          // 清除保护状态
          OUTP_PROT_CLE, // "OUTP:PROT:CLE"

          // 控制启用或禁用软件看门狗，软件看门狗功能开启后，用户可指定一个延迟时间
          // 若指定的时间间隔内所有远程接口上都没有I/O活动（即无任何指令发送或接收），则会禁用输出
          // 输出会关闭锁定但是编程设定的输出状态不会发生改变
          OUTP_PROT_WDOG_1, // "OUTP:PROT:WDOG 1"
          OUTP_PROT_WDOG_ON = OUTP_PROT_WDOG_1, // "OUTP:PROT:WDOG ON"
          OUTP_PROT_WDOG_0, // "OUTP:PROT:WDOG 0"
          OUTP_PROT_WDOG_OFF = OUTP_PROT_WDOG_0, // "OUTP:PROT:WDOG OFF"
          OUTP_PROT_WDOG_INQUIRE, // "OUTP:PROT:WDOG?"

          // 设置软件看门狗超时时间,值的可编程设定范围为1到3600秒，每次编程设定的增量为1秒
          OUTP_PROT_WDOG_DEL, // "OUTP:PROT:WDOG:DEL 600" (自定义值)
          OUTP_PROT_WDOG_DEL_DEF, // "OUTP:PROT:WDOG:DEL DEF"(默认60s)
          OUTP_PROT_WDOG_DEL_MIN, // "OUTP:PROT:WDOG:DEL MIN"
          OUTP_PROT_WDOG_DEL_MAX, // "OUTP:PROT:WDOG:DEL MAX"
          OUTP_PROT_WDOG_DEL_DEF_INQUIRE, // "OUTP:PROT:WDOG:DEL? DEF"
          OUTP_PROT_WDOG_DEL_MIN_INQUIRE, // "OUTP:PROT:WDOG:DEL? MIN"
          OUTP_PROT_WDOG_DEL_MAX_INQUIRE, // "OUTP:PROT:WDOG:DEL? MAX"

    }; // end enum OUTPut

    static QHash<QString,OUTPut> OUTPUT = {
              {QStringLiteral("OUTP 1"),OUTPut::OUTP_1}, // 查询或启用、禁用电源输出
              {QStringLiteral("OUTP ON"),OUTPut::OUTP_ON},
              {QStringLiteral("OUTP 0"),OUTPut::OUTP_0},
              {QStringLiteral("OUTP OFF"),OUTPut::OUTP_OFF},
              // 设定和查询输出关闭的延迟时间
              {QStringLiteral("OUTP:DEL:FALL"),OUTPut::OUTP_DEL_FALL }, // 需要空格
              {QStringLiteral("OUTP:DEL:FALL DEF"),OUTPut::OUTP_DEL_FALL_DEF},
              {QStringLiteral("OUTP:DEL:FALL MIN"),OUTPut::OUTP_DEL_FALL_MIN},
              {QStringLiteral("OUTP:DEL:FALL MAX"),OUTPut::OUTP_DEL_FALL_MAX},
              {QStringLiteral("OUTP:DEL:FALL? DEF"),OUTPut::OUTP_DEL_FALL_DEF_INQUIRE},
              {QStringLiteral("OUTP:DEL:FALL? MIN"),OUTPut::OUTP_DEL_FALL_MIN_INQUIRE},
              {QStringLiteral("OUTP:DEL:FALL? MAX"),OUTPut::OUTP_DEL_FALL_MAX_INQUIRE},
              // 设定和查询输出打开的延迟时间
              {QStringLiteral("OUTP:DEL:RISE"),OUTPut::OUTP_DEL_RISE}, //  需要空格
              {QStringLiteral("OUTP:DEL:RISE DEF"),OUTPut::OUTP_DEL_RISE_DEF},
              {QStringLiteral("OUTP:DEL:RISE MIN"),OUTPut::OUTP_DEL_RISE_MIN},
              {QStringLiteral("OUTP:DEL:RISE MAX"),OUTPut::OUTP_DEL_RISE_MAX},
              {QStringLiteral("OUTP:DEL:RISE? DEF"),OUTPut::OUTP_DEL_RISE_DEF_INQUIRE},
              {QStringLiteral("OUTP:DEL:RISE? MIN"),OUTPut::OUTP_DEL_RISE_MIN_INQUIRE},
              {QStringLiteral("OUTP:DEL:RISE? MAX"),OUTPut::OUTP_DEL_RISE_MAX_INQUIRE},
              // 设置和查询数字IO的禁止输出模式
              {QStringLiteral("OUTP:INH:MODE LIVE"),OUTPut::OUTP_INH_MODE_LIVE},
              {QStringLiteral("OUTP:INH:MODE LATC"),OUTPut::OUTP_INH_MODE_LATC},
              {QStringLiteral("OUTP:INH:MODE?"),OUTPut::OUTP_INH_MODE_INQUIRE},
              // 清除保护状态
              {QStringLiteral("OUTP:PROT:CLE"),OUTPut::OUTP_PROT_CLE},
              // 控制启用或禁用软件看门狗
              {QStringLiteral("OUTP:PROT:WDOG 1"),OUTPut::OUTP_PROT_WDOG_1},
              {QStringLiteral("OUTP:PROT:WDOG ON"),OUTPut::OUTP_PROT_WDOG_ON},
              {QStringLiteral("OUTP:PROT:WDOG 0"),OUTPut::OUTP_PROT_WDOG_0},
              {QStringLiteral("OUTP:PROT:WDOG OFF"),OUTPut::OUTP_PROT_WDOG_OFF},
              {QStringLiteral("OUTP:PROT:WDOG?"),OUTPut::OUTP_PROT_WDOG_INQUIRE},
              // 设置软件看门狗超时时间,值的可编程设定范围为1到3600秒，每次编程设定的增量为1秒
              {QStringLiteral("OUTP:PROT:WDOG:DEL"),OUTPut::OUTP_PROT_WDOG_DEL},//需要空格
              {QStringLiteral("OUTP:PROT:WDOG:DEL DEF"),OUTPut::OUTP_PROT_WDOG_DEL_DEF},
              {QStringLiteral("OUTP:PROT:WDOG:DEL MIN"),OUTPut::OUTP_PROT_WDOG_DEL_MIN},
              {QStringLiteral("OUTP:PROT:WDOG:DEL MAX"),OUTPut::OUTP_PROT_WDOG_DEL_MAX},
              {QStringLiteral("OUTP:PROT:WDOG:DEL? DEF"),OUTPut::OUTP_PROT_WDOG_DEL_DEF_INQUIRE},
              {QStringLiteral("OUTP:PROT:WDOG:DEL? MIN"),OUTPut::OUTP_PROT_WDOG_DEL_MIN_INQUIRE},
              {QStringLiteral("OUTP:PROT:WDOG:DEL? MAX"),OUTPut::OUTP_PROT_WDOG_DEL_MAX_INQUIRE}

    }; // end  QHash<QString,OUTPut> OUTPUT

    inline static QByteArray OutputCommandBytes(OUTPut command)
    {

          return OUTPUT.key(command).toLocal8Bit();
    }
*/

/*
    enum class STATus{

          // 状态组分为：标准事件状态组、操作状态组和问题状态组
          //  （1）标准事件状态组 (没有这类命令)
          //  （2）操作状态组（事件寄存器一共13位，还有条件寄存器、NTR/PTR寄存器）

              事件寄存器的位含义：
              名称                      位          位值           含义
              ACQ-WTG           0              1               已触发(指示meter的触发状态)
              ARB-WTG           1              2               已触发(指示arb的触发状态)
              DLOGWTG       2              4               已触发(指示dlog的触发状态)
              ACQ-Active         3              8               ACQ已经触发，正在执行
              ARB-Active         4              16             ARB已经触发，正在执行
              DLOGActive     5              32             DLOG已经触发，正在执行
              OFF                     6              64             仪器on/off为off状态
              CC                       7              128           正向恒流状态
              CV                       8              256           恒压状态
              CW                      9              512           正向恒功率状态
              CC-                     11            2048          负向恒流状态
              CP-                     12            4096          负向恒功率状态
              是一种只读寄存器，可存储锁定Operation NTR和/或PTR滤波器通过的所有事件
              读取操作状态事件寄存器会清除它，*RST对此寄存器不起任何作用
              返回值为在寄存器中所有已启用位的二进制加权值的总和
              例如，如果设置了第3位（值8）和第5位（值32），则查询将返回+40
              000 0000 0010 1000 = 40

          // 查询操作状态组的事件寄存器
          STAT_OPER_INQUIRE, // "STAT:OPER?"
          // 操作状态组的使能寄存器设置值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_OPER_ENAB, // "STAT:OPER:ENAB 24" 启用使能寄存器中的第3 位和第4 位
          STAT_OPER_ENAB_INQUIRE, // "STAT:OPER:ENAB?"
          // 查询操作状态组的条件寄存器。它是一种只读寄存器，它可容纳仪器的活动（非锁定）运行状态
          STAT_OPER_COND_INQUIRE, // "STAT:OPER:COND?"
          // 设置和查询PTR正跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_OPER_PTR, // "STAT:OPER:PTR 24" 启用PTR寄存器中的第3位和第4位
          STAT_OPER_PTR_INQUIRE,// "STAT:OPER:PTR?"
          // 设置和查询NTR负跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_OPER_NTR, // "STAT:OPER:NTR 24" 启用NTR寄存器中的第3位和第4位
          STAT_OPER_NTR_INQUIRE,// "STAT:OPER:NTR?"
          //   （3）问题状态组（事件寄存器一共15位，还有条件寄存器、NTR/PTR寄存器）
              事件寄存器的位含义：
              名称          位         位值             含义
              OV             0           1                  过压保护
              OC +          1          2                  正向过流保护
              OC -           2          4                  负向过流保护
              OP +           3         8                 正向过功率保护
              OP -            4        16                负向过功率保护
              UV             5        32                欠电压保护
              OT             6        64                 过温度保护
              UC             7        128              欠流保护
              Errsense     8        256              Sense故障
              Share         9        512              均流故障
              Rvs            10      1024             输出反接
              INH           11       2048            外部禁止输出
              PS             12       4096            故障保护位(protectshutdown)
              OSC          13       8192            环路震荡故障
              UNR         14       16384          仪器内部未知故障
              是一种只读寄存器，可存储锁定Operation NTR和/或PTR滤波器通过的所有事件
              读取问题状态事件寄存器会清除它，同时*RST对此寄存器不起任何作用
              返回值为在寄存器中所有已启用位的二进制加权值的总和
              例如，如果设置了第2位（值4）和第4位（值16），则查询将返回+20
              000 0000 0001 0100 = 20

          // "STAT:QUES?" 查询问题状态组的事件寄存器
          STAT_QUES_INQUIRE,
          // 设置和查询问题状态组的使能寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_QUES_ENAB, // "STAT:QUES:ENAB 24" 启用问题使能寄存器中的第3位和第4位 = 8+16 = 24
          STAT_QUES_ENAB_INQUIRE, // "STAT:QUES:ENAB?"
          // 查询问题状态组的条件寄存器。它是一种只读寄存器，它可容纳仪器的活动（非锁定）运行状态
          STAT_QUES_COND_INQUIRE, // "STAT:QUES:COND?"
          // 设置和查询PTR正跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_QUES_PTR, // "STAT:QUES:PTR 24" 启用PTR寄存器中的第3位和第4位
          STAT_QUES_PTR_INQUIRE,// "STAT:QUES:PTR?"
          // 设置和查询NTR负跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          STAT_QUES_NTR, // "STAT:QUES:NTR 24" 启用NTR寄存器中的第3位和第4位
          STAT_QUES_NTR_INQUIRE,// "STAT:QUES:NTR?"
          // （4）预设所有的使能、PTR和NTR寄存器

                操作寄存器                   问题寄存器                         预设设置
                STAT:OPER:ENAB      STAT:QUES:ENAB      已禁用所有定义的位
                STAT:OPER:NTR         STAT:QUES:NTR         已禁用所有定义的位
                STAT:OPER:PTR         STAT:QUES:PTR           已启用所有定义的位

          STAT_PRES, // "STAT:PRES"

    }; // end enum STATus

    static QHash<QString,STATus> STATUS = {

           // 查询操作状态组的事件寄存器
          {QStringLiteral("STAT:OPER?"),STATus::STAT_OPER_INQUIRE},
          // 操作状态组的使能寄存器设置值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          {QStringLiteral("STAT:OPER:ENAB"),STATus::STAT_OPER_ENAB}, // 需要空格
          {QStringLiteral("STAT:OPER:ENAB?"),STATus::STAT_OPER_ENAB_INQUIRE},
           // 查询操作状态组的条件寄存器。它是一种只读寄存器，它可容纳仪器的活动（非锁定）运行状态
          {QStringLiteral("STAT:OPER:COND?"),STATus::STAT_OPER_COND_INQUIRE},
           // 设置和查询PTR正跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          {QStringLiteral("STAT:OPER:PTR"),STATus::STAT_OPER_PTR},
          {QStringLiteral("STAT:OPER:PTR?"),STATus::STAT_OPER_PTR_INQUIRE },
           // 设置和查询PTR负跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          {QStringLiteral("STAT:OPER:NTR"),STATus::STAT_OPER_NTR},
          {QStringLiteral("STAT:OPER:NTR?"),STATus::STAT_OPER_NTR_INQUIRE},
          // 查询问题状态组的事件寄存器
          {QStringLiteral("STAT:QUES?"),STATus::STAT_QUES_INQUIRE},
          // 设置和查询问题状态组的使能寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          {QStringLiteral("STAT:QUES:ENAB"),STATus::STAT_QUES_ENAB}, // 需要空格
          {QStringLiteral("STAT:QUES:ENAB?"),STATus::STAT_QUES_ENAB_INQUIRE},
          // 查询问题状态组的条件寄存器。它是一种只读寄存器，它可容纳仪器的活动（非锁定）运行状态
          {QStringLiteral("STAT:QUES:COND?"),STATus::STAT_QUES_COND_INQUIRE},
          // 设置和查询PTR正跃迁寄存器的值，十进制值等于寄存器中所有位的二进制加权值总和。设置范围：0~65535
          {QStringLiteral("STAT:QUES:PTR"),STATus::STAT_QUES_PTR},
          {QStringLiteral("STAT:QUES:PTR?"),STATus::STAT_QUES_PTR_INQUIRE},
          {QStringLiteral("STAT:QUES:NTR"),STATus::STAT_QUES_NTR},
          {QStringLiteral("STAT:QUES:NTR?"),STATus::STAT_QUES_NTR_INQUIRE},
          // 预设所有的使能、PTR和NTR寄存器
          {QStringLiteral("STAT:PRES"),STATus::STAT_PRES}

    }; // end  QHash<QString,STATus> STATUS

    inline static QByteArray StatusCommandBytes(STATus command)
    {
          return STATUS.key(command).toLocal8Bit();
    }
*/

} // end namespace spiccommand

#endif // SPICCOMMAND_H
