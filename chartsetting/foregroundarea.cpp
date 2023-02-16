#include "foregroundarea.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ForegroundArea::ForegroundArea(QChart* chart,const QIcon&icon,QWidget *parent) :
    QGroupBox(parent),mChart(chart),mIcon(icon)
{
    // 绘制区域前景的布局
    setFont(QFont("Times New Roman",12));
    setWindowIcon(mIcon);
    //setMinimumSize(QSize(600,200));//不要设置这个否则无法自适应

    QHBoxLayout * lay = new QHBoxLayout;
    mColorButton = new QPushButton(tr("设置前景颜色"));
    mBrushStyle = new QComboBox;
    initBrushStyleComboBox();

    connect(mColorButton,&QPushButton::clicked,this,[=]{
        QColor initColor = mChart->plotAreaBackgroundBrush().color();
        QColorDialog * dlg = new QColorDialog(initColor);
        dlg->setFixedSize(800,400);
        dlg->setWindowIcon(mIcon);
        dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
        dlg->setWindowTitle(tr("设置颜色"));
        connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
               ,this,[=](const QColor& color){
            mChart->setPlotAreaBackgroundBrush(QBrush(color,Qt::BrushStyle(mBrushStyle->currentIndex())));
        });
        dlg->exec(); delete dlg;
    });
    lay->addWidget(mColorButton);
    lay->addWidget(mBrushStyle);

    setLayout(lay);
}

QPushButton * ForegroundArea::colorSetButton() const
{
    return mColorButton;
}

QComboBox* ForegroundArea::brushStyleComboBox() const
{
    return mBrushStyle;
}

void ForegroundArea::initBrushStyleComboBox()
{
    QListWidget * listwidget = new QListWidget;
    mBrushStyle->setModel(listwidget->model());
    mBrushStyle->setView(listwidget);
    mBrushStyle->resize(100,30);
    QStringList iconnames;
    iconnames <<"NoBrush"<<"SolidPattern"<<"Dense1Pattern"<<"Dense2Pattern"<<"Dense3Pattern"
             <<"Dense4Pattern"<<"Dense5Pattern"<<"Dense6Pattern"<<"Dense7Pattern"<<"HorPattern"
            <<"VerPattern"<<"CrossPattern"<<"BDiagPattern"<<"FDiagPattern"<<"DiagCrossPattern";//下方4个对于边框没有意义
     // "LinearGradientPattern"<<"RadialGradientPattern"<<"ConicalGradientPattern"<<"TexturePattern"
    QString filename; // images.qrc,注意win+shift+s的截图并不是jpg格式读不出来
    foreach(const QString &style, iconnames)
    {
        filename = QStringLiteral(":/images/")+style+QStringLiteral(".jpg");
        QPixmap pix(filename);
        QPixmap p = pix.scaled(mBrushStyle->size(),Qt::IgnoreAspectRatio); // p.size()全部是(100,30)
        if (style == "NoBrush") p.fill(Qt::white); // NoBrush改用白色填充
        mBrushStyle->addItem(QIcon(p),style); // 添加图标-文字
    }
    mBrushStyle->setIconSize(mBrushStyle->size()); // 要设置,图标默认方形,这里设置为矩形长度
    mBrushStyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);//设置这个最好,其它的略短,符合内容大小的调整策略

    mBrushStyle->setCurrentIndex(mChart->plotAreaBackgroundBrush().style());//chart.cpp默认SolidPattern
    connect(mBrushStyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
            QBrush brush = mChart->plotAreaBackgroundBrush();
            brush.setStyle(Qt::BrushStyle(index));
            mChart->setPlotAreaBackgroundBrush(brush);
    });
}

