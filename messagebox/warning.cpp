#include "warning.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Warning::Warning()
{
    setDialogTitle(tr("警告对话框"));
    setDialogIcon(QPixmap(":/images/warningIcon.png"));
    setDialogFixedSize(800,280);
    setText(tr("这是一个警告对话框!"));
    setPixmap(QPixmap(":/images/warningIcon.png"));
    setPixmapBoxStyleSheet("border:2px solid #febe4d;border-radius:10px;");
    setTextBoxStyleSheet("border: 2px solid #febe4d;");
    setTextEditStyleSheet("color:#febe4d;font-size:24px;font-weight:bold;font-family:Times New Roman;text-align:center;");
    MessageButton* okbutton = button(0);
    okbutton->button()->setStyleSheet("color:#febe4d;border:1px solid #febe4d;font-size:24px;"
                                      "font-family:Times New Roman;min-width: 120px;min-height: 40px;");
    setTextWrapIndent(2,21);
    setTextColor(Qt::yellow);
    removeButton(1); // cancel's id =1
    Warning::setSlotClassFunction(&Warning::accept,0);
}

void Warning::setSlotClassFunction(ButtonSlotClassFunc p, int id)
{
    MessageButton * b = button(id);
    if (b == Q_NULLPTR)  return;
    if (p == 0) return;
    // 如果有自定义的按钮(id!=0,1),有义务断开这个按钮和之前槽函数的连接
    // 否则这个自定义按钮不仅连接之前的槽函数,还会连接新的p函数
    connect(b,&MessageButton::clicked,this,p);
}
