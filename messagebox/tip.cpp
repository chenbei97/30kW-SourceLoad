#include "tip.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Tip::Tip()
{
    mTip = new QLabel;
    init();
//    Tip box;
//    box.setTip("这是一个小提示(一行文字)对话框");
//    box.show();
}

Tip::Tip(const QString& tip)
{
    mTip = new QLabel(tip);
    init();
}

void Tip::init()
{
    mTip->setAlignment(Qt::AlignCenter);
    mBtn = new QPushButton(tr("知道啦"));
    mBtn->setStyleSheet("color:blue;border:1px solid blue;font-size:24px;"
                                       "font-family:Times New Roman;min-width:120px;min-height:40px;");
    connect(mBtn,&QPushButton::clicked,this,[=]{accept();});

    QHBoxLayout * laybox = new QHBoxLayout;
    laybox->addWidget(mTip);
    mBox = new QGroupBox;
    mBox->setStyleSheet("border: 1px solid blue;"
                          "color: blue;"
                          "border-radius: 10px;"
                          "text-align: center;"
                          "font-size:24px;");
    mBox->setLayout(laybox);

    QHBoxLayout * laybtn = new QHBoxLayout;
    laybtn->addStretch();
    laybtn->addWidget(mBtn);

    QVBoxLayout * lay = new QVBoxLayout;
    lay->addWidget(mBox);
    lay->addLayout(laybtn);

    setLayout(lay);
    setWindowIcon(QIcon(":/images/tipIcon2.png"));
    setWindowTitle(tr("小提示"));
    setFont(QFont("Times New Roman",12));
    resize(800,200);
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags^Qt::WindowContextHelpButtonHint);
    setStyleSheet("background-image:url(:/images/tipbackground.jpg);");//无需再重载paintEvent函数
}

void Tip::setTip(const QString &tip)
{
    mTip->setText(tip);
}

//void Tip::paintEvent(QPaintEvent *e)
//{
//    QPainter p;
//    p.begin(this);
//    p.setOpacity(0.5);
//    p.setRenderHint(QPainter::SmoothPixmapTransform);
//    QPixmap pix(":/images/tipbackground.jpg");
//    p.drawPixmap(QPoint(0,0),pix);
//    p.end();
//    Q_UNUSED(e);
//}
