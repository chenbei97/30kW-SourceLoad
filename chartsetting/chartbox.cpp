#include"chartbox.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartBox::ChartBox(QChart *chart,const QIcon&icon,QWidget*parent):
    QGroupBox(parent),mChart(chart),mIcon(icon)
{
    setFont(QFont("Times New Roman",12));
    setMinimumWidth(450); // 和charttool.cpp保持一致稍小一些
}

void ChartBox::setChart(QChart *c)
{
    mChart = c;
}

QColorDialog* ChartBox::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    //connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::currentColorChanged)
           //,this,[=](const QColor& color){mChart->setTitleBrush(QBrush(color));} // 此信号切换颜色就会选择,使用这个信号其实2个按钮就没有意义
    //);
    // 之所以不使用QColorDialog::getColor是因为点击cancel会变成无效颜色(黑色)而不是上次的颜色
    //dlg->exec();//先连接信号再启动,否则不会绑定好信号槽
    //delete dlg; // 记得释放,否则析构时有些问题
    return dlg;
}

QFontDialog* ChartBox::fontDialog(const QFont &initFont)
{
    QFontDialog * dlg = new QFontDialog(initFont);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setWindowTitle(tr("设置格式"));
    dlg->setOptions(QFontDialog::DontUseNativeDialog);
    return dlg;
}
