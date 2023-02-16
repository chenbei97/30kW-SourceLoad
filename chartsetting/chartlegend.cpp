#include "chartlegend.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartLegend::ChartLegend(QChart *chart,const QIcon& icon,QWidget*parent)
    :ChartBox(chart,icon,parent)
{
    mLegendVisibleCheckBox = new QCheckBox(tr("图例可见"));
    mLegendBackVisibleCheckBox = new QCheckBox(tr("边框可见"));
    mLegendVisibleCheckBox->setCheckState(Qt::Checked); // chart.cpp 默认可见
    mLegendBackVisibleCheckBox->setCheckState(Qt::Unchecked);// chart.cpp 默认不可见
    connect(mLegendVisibleCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
            this,[=](int state){
            bool ret = state==Qt::Unchecked; // Qt::PartiallyChecked不会出现,Qt::Checked为true
            mChart->legend()->setVisible(!ret); // ret =true说明要隐藏
    });
    connect(mLegendBackVisibleCheckBox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
            this,[=](int state){
            bool ret = state==Qt::Unchecked; // Qt::PartiallyChecked不会出现,Qt::Checked为true
            mChart->legend()->setBackgroundVisible(!ret); // ret =true说明要隐藏
    });

    QGroupBox * pos = new QGroupBox;//不加文字,不好看
    QRadioButton * tp = new QRadioButton(tr("上方"));
    QRadioButton * bp = new QRadioButton(tr("下方"));
    QRadioButton * lp = new QRadioButton(tr("左侧"));
    QRadioButton * rp = new QRadioButton(tr("右侧"));
    QButtonGroup * btnbox = new QButtonGroup; // 方便借助QButtonGroup的buttonClicked(int id)信号
    btnbox->addButton(tp,1);btnbox->addButton(bp,2);btnbox->addButton(lp,3);btnbox->addButton(rp,4);
    tp->setChecked(true); // 默认上方
    QGridLayout * play = new QGridLayout;
    play->addWidget(tp,0,0);play->addWidget(bp,0,1);
    play->addWidget(lp,1,0);play->addWidget(rp,1,1);
    pos->setLayout(play);
    connect(btnbox,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,[=](int id){ // id预设好,方便设置图例位置
            switch (id) {
                case 1: mChart->legend()->setAlignment(Qt::AlignTop);break;
                case 2: mChart->legend()->setAlignment(Qt::AlignBottom);break;
                case 3: mChart->legend()->setAlignment(Qt::AlignLeft);break;
                case 4: mChart->legend()->setAlignment(Qt::AlignRight);break;
                default:break;
            }
    });

    mLegendX = new QDoubleSpinBox;
    mLegendY = new QDoubleSpinBox;
    mLegendW = new QDoubleSpinBox;
    mLegendH = new QDoubleSpinBox;
    mLegendX->setDecimals(0);
    mLegendY->setDecimals(0);
    mLegendW->setDecimals(0);
    mLegendH->setDecimals(0);
    mLegendX->setRange(0.,20000.);
    mLegendY->setRange(0.,20000.);
    mLegendW->setRange(0.,20000.);
    mLegendH->setRange(0.,20000.);
    connect(mLegendX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double x){ mLegendGeometry.setX(x);updateLegendLayout();});
    connect(mLegendY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double y){ mLegendGeometry.setY(y);updateLegendLayout();});
    connect(mLegendW,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double w){ mLegendGeometry.setWidth(w);updateLegendLayout();});
    connect(mLegendH,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double h){ mLegendGeometry.setHeight(h);updateLegendLayout();});
    //mLegendGeometry = mChart->legend()->geometry();
   //qDebug()<<mLegendGeometry;// chart.cpp期望345.,80.,300.,60.,实际返回345,80 26x26
    mLegendGeometry = QRectF(345.,80.,300.,60.);  // 既然如此直接在这设置期望的值,用户调节时直接从最接近期望的调整
    mLegendX->setValue(345.); // 虽然不能得到期望的,但是可以设定spinbox的初值为期望
    mLegendY->setValue(80.);
    mLegendW->setValue(300.);
    mLegendH->setValue(60.);
    QGroupBox * geobox = new QGroupBox;
    QFormLayout * geolay = new QFormLayout;
    geolay->addRow(tr("x"),mLegendX);
    geolay->addRow(tr("y"),mLegendY);
    geolay->addRow(tr("w"),mLegendW);
    geolay->addRow(tr("h"),mLegendH);
    geobox->setLayout(geolay);

    mLegendTextColorBtn = new QPushButton(tr("文字颜色"));
    connect(mLegendTextColorBtn,&QPushButton::clicked,this,[=]{
            QColor oldcolor = mChart->legend()->labelColor();
            QColorDialog * dlg = colorDialog(oldcolor);
            connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
                   ,this,[=](const QColor& color){mChart->legend()->setLabelColor(color);} // 此信号只在点击ok之后发射,使用这个信号让cancel看起来有用
            );
            dlg->exec(); delete dlg;
    });
    mLegendTextFontBtn = new QPushButton(tr("文字字体"));
    connect(mLegendTextFontBtn,&QPushButton::clicked,this,[=]{
            QFont oldfont = mChart->legend()->font();
            QFontDialog * dlg = fontDialog(oldfont);
            connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
                   ,this,[=](const QFont& font){mChart->legend()->setFont(font);} // 此信号只在点击ok之后发射,使用这个信号让cancel看起来有用
            );
            dlg->exec(); delete dlg;
    });

    mLegendBorderColorBtn = new QPushButton(tr("边框颜色"));
    connect(mLegendBorderColorBtn,&QPushButton::clicked,this,[=]{
            QColor oldcolor = mChart->legend()->borderColor();
            QColorDialog * dlg = colorDialog(oldcolor);
            connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
                   ,this,[=](const QColor& color){mChart->legend()->setBorderColor(color);} // 此信号只在点击ok之后发射,使用这个信号让cancel看起来有用
            );
            dlg->exec(); delete dlg;
    });

    QLabel * marker = new QLabel(tr("图例标记"));
    mLegendMarkers = new QComboBox;
    marker->setBuddy(mLegendMarkers);
    mLegendMarkers->addItems(QStringList()<<"Default"<<"Rectangle"<<"Circle"<<"Auto");
    connect(mLegendMarkers,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
           mChart->legend()->setMarkerShape(QLegend::MarkerShape(index));//索引对应
    });

    QGridLayout * lay = new QGridLayout;
    lay->addWidget(mLegendVisibleCheckBox,0,0);lay->addWidget(mLegendBackVisibleCheckBox,0,1);
    lay->addWidget(pos,1,0,1,2); // 横跨2列
    lay->addWidget(geobox,2,0,1,2);
    lay->addWidget(mLegendTextColorBtn,3,0);lay->addWidget(mLegendTextFontBtn,3,1);
    lay->addWidget(mLegendBorderColorBtn,4,0,1,2);
    lay->addWidget(marker,5,0);lay->addWidget(mLegendMarkers,5,1);
    setTitle(tr("图例"));
    setLayout(lay);
}

void ChartLegend::updateLegendLayout()
{
    mChart->legend()->setGeometry(mLegendGeometry);
    mChart->legend()->update();//测试发现2个update似乎可以注释掉可能已经内部自动调用了
    update();// 不过和Qt文档Legend Example（使用图例的案例）保持一致,还是显示调用比较好
}
