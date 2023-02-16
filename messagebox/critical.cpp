#include "critical.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Critical::Critical()
{
    setDialogTitle(tr("错误对话框"));
    setDialogIcon(QPixmap(":/images/criticalIcon.png"));
    setDialogFixedSize(800,300);
    setText(tr("这是一个错误对话框!"));
    setPixmap(QPixmap(":/images/criticalIcon.png"));
    setPixmapBoxStyleSheet("border:2px solid #d81e06;border-radius:10px;");
    setTextBoxStyleSheet("border: 2px solid #d81e06;");
    setTextEditStyleSheet("color:#d81e06;font-size:24px;font-weight: bold;font-family:Times New Roman;text-align: center;");
    MessageButton* okbutton = button(0);
    okbutton->button()->setStyleSheet("color:#d81e06;border:1px solid #d81e06;font-size:24px;"
                                      "font-family:Times New Roman;min-width: 120px;min-height: 40px;");
    setTextWrapIndent(2,21);
    setTextColor(Qt::red);
    removeButton(1); // cancel's id =1
    Critical::setSlotClassFunction(&Critical::accept,0);
}

void Critical::setSlotClassFunction(ButtonSlotClassFunc p, int id)
{
    MessageButton * b = button(id);
    if (b == Q_NULLPTR)  return;
    if (p == 0) return;
    connect(b,&MessageButton::clicked,this,p);
}

