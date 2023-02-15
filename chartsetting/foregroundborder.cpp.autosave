#include "foregroundborder.h"

ForegroundBorder::ForegroundBorder(QChart* chart,const QIcon&icon,QWidget *parent) :
    QWidget(parent),mChart(chart),mIcon(icon)
{
    // 绘制区域边框的部件
    setFont(QFont("Times New Roman",12));
    setWindowIcon(mIcon);
    setMinimumSize(QSize(600,200));

    QComboBox * linestyle = new QComboBox;
    linestyle->addItems(QStringList()<<"NotUse"<<"Solid"<<"Dash"<<"Dot"<<"DashDot"<<"DashDotDot");
    linestyle->setCurrentIndex(mChart->plotAreaBackgroundPen().style()); // chart.cpp 默认DashDot
    connect(linestyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
            QPen pen = mChart->plotAreaBackgroundPen();
            pen.setStyle(Qt::PenStyle(index)); // index =0,1,2,3,4,5对应,6-Qt::CustomDashLine不使用
            mChart->setPlotAreaBackgroundPen(pen);
    });
    QSpinBox * linewidth = new QSpinBox;
    linewidth->setValue(mChart->plotAreaBackgroundPen().width());
    connect(linewidth,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[=](int width){
            QPen pen = mChart->backgroundPen();
            pen.setWidth(width);
            mChart->setPlotAreaBackgroundPen(pen);
    });
    QComboBox * brushstyle = new QComboBox;
    initBrushStyleComboBox(brushstyle);

    QComboBox * joinstyle = new QComboBox;
    initJoinStyleComboBox(joinstyle);
    QComboBox * capstyle = new QComboBox;
    initCapStyleComboBox(capstyle);

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&线型"),linestyle);
    lay->addRow(tr("&线宽"),linewidth);
    lay->addRow(tr("&边框样式"),brushstyle);
    lay->addRow(tr("&连接样式"),joinstyle);
    lay->addRow(tr("&端点样式"),capstyle);
    lay->setSpacing(20);// 两列间距

    setLayout(lay);
}

void ForegroundBorder::initBrushStyleComboBox(QComboBox *brushstyle)
{
    QListWidget * listwidget = new QListWidget;
    brushstyle->setModel(listwidget->model());
    brushstyle->setView(listwidget);
    brushstyle->resize(100,30);
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
        QPixmap p = pix.scaled(brushstyle->size(),Qt::IgnoreAspectRatio); // p.size()全部是(100,30)
        if (style == "NoBrush") p.fill(Qt::white); // NoBrush改用白色填充
        brushstyle->addItem(QIcon(p),style); // 添加图标-文字
    }
    brushstyle->setIconSize(brushstyle->size()); // 要设置,图标默认方形,这里设置为矩形长度
    brushstyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);//设置这个最好,其它的略短,符合内容大小的调整策略
    brushstyle->setCurrentIndex(mChart->plotAreaBackgroundPen().brush().style());//chart.cpp默认SolidPattern
    connect(brushstyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
            QPen pen = mChart->plotAreaBackgroundPen();
            QBrush brush = pen.brush();
            brush.setStyle(Qt::BrushStyle(index));
            pen.setBrush(brush);
            mChart->setPlotAreaBackgroundPen(pen);
    });
}

void ForegroundBorder::initJoinStyleComboBox(QComboBox * joinstyle)
{
    joinstyle->addItems(QStringList()<<"Miter"<<"Bevel"<<"Round"<<"SvgMiter");
    joinstyle->resize(100,30);
    QPixmap pix1 = QPixmap(":/images/MiterJoin.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    QPixmap pix2 = QPixmap(":/images/BevelJoin.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    QPixmap pix3 = QPixmap(":/images/RoundJoin.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    joinstyle->setItemIcon(0,QIcon(pix1));
    joinstyle->setItemIcon(1,QIcon(pix2));
    joinstyle->setItemIcon(2,QIcon(pix3));
    joinstyle->setItemIcon(3,QIcon(pix1)); //还用这个图,区别不大
    joinstyle->setCurrentIndex(2); // chart.cpp默认roundjoin
    joinstyle->setIconSize(joinstyle->size());

    connect(joinstyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){ // 0-0x00,1-0x40,2-0x80,3-0x100 对应0,64,128和256
            int mapidx = 0;
            switch (index) {
                case 0: mapidx=0;break;
                case 1: mapidx=0x40;break;
                case 2: mapidx=0x80;break;
                case 3: mapidx=0x100;break;
            }
            QPen pen = mChart->plotAreaBackgroundPen();
            pen.setJoinStyle(Qt::PenJoinStyle(mapidx));
            mChart->setPlotAreaBackgroundPen(pen);
    });
}

void ForegroundBorder::initCapStyleComboBox(QComboBox *capstyle)
{
    capstyle->addItems(QStringList()<<"Flat"<<"Square"<<"Round");
    capstyle->setCurrentIndex(2); // 和chart.cpp保持一致
    capstyle->resize(100,30);
    QPixmap pix1 = QPixmap(":/images/FlatCap.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    QPixmap pix2 = QPixmap(":/images/SquareCap.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    QPixmap pix3 = QPixmap(":/images/RoundCap.jpg").scaled(100,30,Qt::IgnoreAspectRatio);
    capstyle->setItemIcon(0,QIcon(pix1));
    capstyle->setItemIcon(1,QIcon(pix2));
    capstyle->setItemIcon(2,QIcon(pix3));
    capstyle->setIconSize(capstyle->size());
    connect(capstyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
            int mapidx = 0;
            switch (index) {
                case 0: mapidx=0;break;
                case 1: mapidx=0x10;break;
                case 2: mapidx=0x20;break;
            }
            QPen pen = mChart->plotAreaBackgroundPen();
            pen.setCapStyle(Qt::PenCapStyle(mapidx));
            mChart->setPlotAreaBackgroundPen(pen);
    });
}
