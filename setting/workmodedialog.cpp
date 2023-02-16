#include "workmodedialog.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

WorkModeDialog::WorkModeDialog(QWidget * parent) :QDialog(parent)
{
    setFont(QFont("Times New Roman",12));
    resize(450,120);
    setWindowTitle(tr("工作模式设置"));

    QLabel * ccLabel = new QLabel(tr("CC模式"));
    QLabel * cvLabel = new QLabel(tr("CV模式"));
    QRadioButton * ccBtn = new QRadioButton;
    QRadioButton * cvBtn = new QRadioButton;
    ccLabel->setBuddy(ccBtn);
    cvLabel->setBuddy(cvBtn);
    cvBtn->setChecked(true);
    isCVMode = true;

    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addStretch();
    lay1->addWidget(cvLabel);
    lay1->addWidget(cvBtn);
    lay1->addWidget(ccLabel);
    lay1->addWidget(ccBtn);
    lay1->addStretch();
    lay1->insertSpacing(3,50);
    QGroupBox * box1 = new QGroupBox;
    box1->setLayout(lay1);


    QLabel * picture = new QLabel;
    picture->setAlignment(Qt::AlignCenter);
    QPixmap  pic = QPixmap(":/images/workmode.jpg"); // QSize(500, 461)
    QPixmap newpic = pic.scaled(500,250); // 宽度不变,高度缩小,过高就会超出窗口高度
    picture->setPixmap(newpic);

    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addWidget(picture);
    QGroupBox * box2 = new QGroupBox;
    box2->setLayout(lay2);

    QHBoxLayout * mainlay = new QHBoxLayout(this);
    mainlay->addWidget(box1);
    mainlay->addWidget(box2);

    connect(cvBtn,&QRadioButton::pressed,this,[=]{ //clicked不符合用户习惯
        isCVMode = true;
        emit workModeChanged(WORKMODE::CV);
    });
    connect(ccBtn,&QRadioButton::pressed,this,[=]{
        isCVMode = false;
        emit workModeChanged(WORKMODE::CC);
    });
}
