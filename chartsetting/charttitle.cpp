#include "charttitle.h"

ChartTitle::ChartTitle(QChart *chart,const QIcon& icon,QWidget*parent):ChartBox(chart,icon,parent)
{
    setTitle(tr("标题"));
    mSetTitleEdit = new QLineEdit(mChart->title());
    mSetTitleBtn = new QPushButton(tr("设置标题"));
    mSetTitleFontBtn = new QPushButton(tr("设置标题字体"));
    mSetTitleColorBtn = new QPushButton(tr("设置标题颜色"));

    QHBoxLayout * hlay = new QHBoxLayout;
    hlay->setObjectName("TitleBoxTopLayout");
    hlay->addWidget(mSetTitleEdit);
    hlay->addWidget(mSetTitleBtn);
    QVBoxLayout * vlay = new QVBoxLayout;
    vlay->setObjectName("TitleBoxMainLayout");
    vlay->addLayout(hlay);
    vlay->addWidget(mSetTitleFontBtn);
    vlay->addWidget(mSetTitleColorBtn);

    setLayout(vlay);

    connect(mSetTitleBtn,&QPushButton::clicked,this,[=]{
        if (mSetTitleEdit->text().isEmpty()) return;
        mChart->setTitle(mSetTitleEdit->text());
    });

    connect(mSetTitleFontBtn,&QPushButton::clicked,this,[=]{
            QFont oldfont = mChart->titleFont();
            QFontDialog * dlg = fontDialog(oldfont);
            connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
                   ,this,[=](const QFont& font){mChart->setTitleFont(font);} // 此信号只在点击ok之后发射,使用这个信号让cancel看起来有用
            );
            dlg->exec(); delete dlg;
    });

    connect(mSetTitleColorBtn,&QPushButton::clicked,this,[=]{
            QColor oldcolor = mChart->titleBrush().color();
            QColorDialog * dlg = colorDialog(oldcolor);
            connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
                   ,this,[=](const QColor& color){mChart->setTitleBrush(QBrush(color));} // 此信号只在点击ok之后发射,使用这个信号让cancel看起来有用
            );
            dlg->exec(); delete dlg;
    });
}

