#include "axisbox.h"

AxisBox::AxisBox(QChart *chart,QAbstractAxis*axis, const QIcon&icon,QWidget*parent):
    QGroupBox(parent),mChart(chart),mCurrentAxis(axis),mIcon(icon)
{
        setFont(QFont("Times New Roman",12));
        setMinimumWidth(450); // 和charttool.cpp保持一致稍小一些
}


void AxisBox::setChart(QChart *c)
{
    mChart = c;
}

QChart* AxisBox::chart() const
{
    return  mChart;
}

void AxisBox::setCurrentAxis(QAbstractAxis *axis)
{
    mCurrentAxis = axis;
    updateState(); // 切换轴后更新状态
}

QAbstractAxis* AxisBox::currentAxis() const
{
    return  mCurrentAxis;
}

QColorDialog* AxisBox::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}

QFontDialog* AxisBox::fontDialog(const QFont &initFont)
{
    QFontDialog * dlg = new QFontDialog(initFont);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setWindowTitle(tr("设置格式"));
    dlg->setOptions(QFontDialog::DontUseNativeDialog);
    return dlg;
}

QComboBox* AxisBox::brushStyleCombo()
{
    QComboBox * brushstyle = new QComboBox;

    QListWidget * listwidget = new QListWidget;
    brushstyle->setModel(listwidget->model());
    brushstyle->setView(listwidget);
    brushstyle->resize(100,30);
    QStringList iconnames;
    iconnames <<"NoBrush"<<"SolidPattern"<<"Dense1Pattern"<<"Dense2Pattern"<<"Dense3Pattern"
             <<"Dense4Pattern"<<"Dense5Pattern"<<"Dense6Pattern"<<"Dense7Pattern"<<"HorPattern"
            <<"VerPattern"<<"CrossPattern"<<"BDiagPattern"<<"FDiagPattern"<<"DiagCrossPattern";
    QString filename;
    foreach(const QString &style, iconnames)
    {
        filename = QStringLiteral(":/images/")+style+QStringLiteral(".jpg");
        QPixmap pix(filename);
        QPixmap p = pix.scaled(brushstyle->size(),Qt::IgnoreAspectRatio); // p.size()全部是(100,30)
        if (style == "NoBrush") p.fill(Qt::white); // NoBrush改用白色填充
        brushstyle->addItem(QIcon(p),style); // 添加图标-文字
    }
    brushstyle->setIconSize(brushstyle->size()); // 要设置,图标默认方形,这里设置为矩形长度
    brushstyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);//设置这个最好,其它的略短,符合内容大小的调整策略

    return  brushstyle; // currentIndex以及currentIndexChanged外部要自行设置和连接,这是只是提供1个创建好的指针
}
