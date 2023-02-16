#include "chartmargin.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartMargin::ChartMargin(QChart *chart,const QIcon& icon,QWidget*parent):ChartBox(chart,icon,parent)
{
    QLabel * lm = new QLabel(tr("左边距"));
    QLabel * rm = new QLabel(tr("右边距"));
    QLabel * tm = new QLabel(tr("上边距"));
    QLabel * bm = new QLabel(tr("下边距"));
    QSpinBox * lms = new QSpinBox;
    QSpinBox * rms = new QSpinBox;
    QSpinBox * tms = new QSpinBox;
    QSpinBox * bms = new QSpinBox;
    lms->setValue(mChart->margins().left());
    rms->setValue(mChart->margins().right());
    tms->setValue(mChart->margins().top());
    bms->setValue(mChart->margins().bottom());
    mMarginResetBtn = new QPushButton(tr("恢复默认边距"));

    connect(lms,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int val){
            QMargins m = mChart->margins();
            m.setLeft(val);
            mChart->setMargins(m);
    });
    connect(rms,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int val){
            QMargins m = mChart->margins();
            m.setRight(val);
            mChart->setMargins(m);
    });
    connect(tms,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int val){
            QMargins m = mChart->margins();
            m.setTop(val);
            mChart->setMargins(m);
    });
    connect(bms,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int val){
            QMargins m = mChart->margins();
            m.setBottom(val);
            mChart->setMargins(m);
    });
    connect(mMarginResetBtn,&QPushButton::clicked,this,[=]{
        mChart->setMargins(QMargins(20,20,20,20));
        lms->setValue(20);rms->setValue(20);tms->setValue(20);bms->setValue(20);
    });

    QGridLayout * lay = new QGridLayout;
    lay->addWidget(lm,0,0);lay->addWidget(lms,0,1);
    lay->addWidget(rm,1,0);lay->addWidget(rms,1,1);
    lay->addWidget(tm,2,0);lay->addWidget(tms,2,1);
    lay->addWidget(bm,3,0);lay->addWidget(bms,3,1);
    lay->addWidget(mMarginResetBtn,4,0,-1,-1); //自动横跨

    setTitle(tr("边距"));
    setLayout(lay);
}
