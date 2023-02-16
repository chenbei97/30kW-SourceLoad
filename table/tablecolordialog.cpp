#include "tablecolordialog.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

TableColorDialog::TableColorDialog(const QPalette& palette,QWidget*parent):QDialog(parent)
{
    mainLayout = new QGridLayout;
    mBackLabel= new QLabel(tr("选择背景色: "));
    mForeLabel = new QLabel(tr("选择前景色: "));
    mBackBtn = new QPushButton;
    mForeBtn = new QPushButton;
    mOkBtn = new QPushButton(tr("确定"));
    mCancelBtn = new QPushButton(tr("取消"));

    mForeColor = palette.color(QPalette::Text);
    mBackColor = palette.color(QPalette::Background);
    //qDebug()<<QString("color: %1;").arg(mForeColor.name()); // "color: #000000;"
    QString btnStyle("border: none;min-width:36px;min-height:36px;border-radius:18px;background-color:%1;");
    mForeBtn->setStyleSheet(btnStyle.arg(mForeColor.name()));//按钮用背景色显示颜色能看到
    mBackBtn->setStyleSheet(btnStyle.arg(mBackColor.name()));// 这里设置一遍是让本颜色对话框跟随外部TableView上次的设置

    connect(mForeBtn,&QPushButton::clicked,this,[=]{
         QColor color = QColorDialog::getColor(mForeColor,this,tr("选择前景色"));
         if (color.isValid()){
             mForeColor = color;//选择有效
             // QPalette pal = mBackBtn->palette(); // 这个不起作用
             //  pal.setColor(QPalette::Background,mBackColor);
             //  mForeBtn->setPalette(pal);
             mForeBtn->setStyleSheet(btnStyle.arg(mForeColor.name()));//这里设置一遍是为了让选中颜色后就能看见效果
         };
    });

    connect(mBackBtn,&QPushButton::clicked,this,[=]{
        QColor color = QColorDialog::getColor(mBackColor,this,tr("选择背景色"));
        if (color.isValid()){
            mBackColor = color;//选择有效
            mBackBtn->setStyleSheet(btnStyle.arg(mBackColor.name()));
        }
    });

    connect(mOkBtn,&QPushButton::clicked,this,[=]{accept();});
    connect(mCancelBtn,&QPushButton::clicked,this,&QDialog::reject);

    mainLayout->addWidget(mForeLabel,0,0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(mForeBtn,0,1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(mBackLabel,1,0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(mBackBtn,1,1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(mOkBtn,2,0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(mCancelBtn,2,1,1,1,Qt::AlignCenter);

    setLayout(mainLayout);
    setWindowIcon(QIcon(":/images/tablepen.png"));
    setFont(QFont("Times New Roman",12));
    setFixedSize(QSize(350,180));
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags^Qt::WindowContextHelpButtonHint);
}

QColor TableColorDialog::foregroundColor()
{
    return mForeColor;
}

QColor TableColorDialog::backgroundColor()
{
    return mBackColor;
}
