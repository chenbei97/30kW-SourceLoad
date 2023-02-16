#include "login_retrieve_password.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

RetrieveSecond::RetrieveSecond(QWidget*parent):QDialog(parent)
 , TipLabel(new QLabel(tr("第二步"))), PassWordLabel(new QLabel)
 , ResetBtn(new QPushButton(tr("重置密码?"))),EndBtn(new QPushButton(tr("结束")))
{
      setFont(QFont("Times New Roman",12));
      setFixedSize(QSize(400,150));
      setWindowIcon(QIcon(":/images/login_forgetpwd.png"));
      initUI();
      initConnections();
}

void RetrieveSecond::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setOpacity(0.5);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pix(":/images/forgetpwd_second.jpg");
    pix.scaled(size());
    p.drawPixmap(QPoint(0,0),pix);
    p.end();
    Q_UNUSED(e);
}

void RetrieveSecond::getUser(const QString &username,const QString &password)
{
    mUserName = username;
    PassWordLabel->setText(tr("用户名为 %1 的密码是 %2").arg(QT_TR_NOOP(username)).arg(QT_TR_NOOP(password)));
    PassWordLabel->setAlignment(Qt::AlignCenter);
}

void RetrieveSecond::initUI()
{
    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addWidget(TipLabel);
    lay1->addSpacerItem(new QSpacerItem(100,30));

    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addWidget(ResetBtn);
    lay2->addWidget(EndBtn);
    lay2->setAlignment(Qt::AlignCenter);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lay1);
    mainLayout->addWidget(PassWordLabel);
    mainLayout->addLayout(lay2);

    setLayout(mainLayout);
}

void RetrieveSecond::initConnections()
{
      connect(EndBtn,SIGNAL(clicked()),this,SLOT(accept()));

      connect(ResetBtn,&QPushButton::clicked,this,[=]{
            accept();
            emit next(mUserName); // 通知外部启动第三级窗口,需要把用户名携带出去
      });
}
