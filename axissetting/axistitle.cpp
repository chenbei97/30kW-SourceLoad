#include "axistitle.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisTitle::AxisTitle(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{ // visible,text,brush,font
        mAxisTitleVisible = new QCheckBox;
        updateTitleVisibilityState();

        mAxisTitleColor = new QPushButton(tr("设置标题颜色"));
        updateTitleColorState();

        mAxisTitleFont = new QPushButton(tr("设置标题格式"));
        updateTitleFontState();

        //mAxisTitleBrush = brushStyleCombo();
        //updateTitleBrushState();

        QFormLayout * formlay = new QFormLayout;
        formlay->setSizeConstraint(QLayout::SetNoConstraint);
        formlay->addRow(tr("&标题可见"),mAxisTitleVisible);
        formlay->addRow(tr("&标题颜色"),mAxisTitleColor);
        formlay->addRow(tr("&标题格式"),mAxisTitleFont);
        //formlay->addRow(tr("&标题画刷"),mAxisTitleBrush); // 画刷对文字没有意义,没必要加

        mAxisTitleText = new QLineEdit;
        mAxisTitleSetText = new QPushButton(tr("设置标题"));
        QHBoxLayout * tlay = new QHBoxLayout;
        tlay->setSizeConstraint(QLayout::SetNoConstraint);
        tlay->addWidget(mAxisTitleText);
        tlay->addWidget(mAxisTitleSetText);
        updateTitleTextState();

        QVBoxLayout * lay = new QVBoxLayout;
        lay->setSizeConstraint(QLayout::SetNoConstraint);
        lay->addLayout(formlay);
        lay->addLayout(tlay);
        setTitle(tr("轴标题"));
        setLayout(lay);
}

void AxisTitle::disconnectAllConnections()
{
    disconnect(mAxisTitleVisible,&QCheckBox::stateChanged,this,&AxisTitle::changeVisibility);
    disconnect(mAxisTitleSetText,&QPushButton::clicked,this,&AxisTitle::changeText);
    disconnect(mAxisTitleColor,&QPushButton::clicked,this,&AxisTitle::changeColor);
    disconnect(mAxisTitleFont,&QPushButton::clicked,this,&AxisTitle::changeFont);
}

void AxisTitle::updateState()
{
        updateTitleVisibilityState();
        updateTitleTextState();
        updateTitleColorState();
        //updateTitleBrushState();
        updateTitleFontState();
}

void AxisTitle::updateTitleVisibilityState()
{
    Qt::CheckState state = mCurrentAxis->isTitleVisible()?Qt::Checked:Qt::Unchecked;
    mAxisTitleVisible->setCheckState(state);

    disconnect(mAxisTitleVisible,&QCheckBox::stateChanged,this,&AxisTitle::changeVisibility);
    connect(mAxisTitleVisible,&QCheckBox::stateChanged,this,&AxisTitle::changeVisibility);
}

void AxisTitle::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setTitleVisible(s);
}

void AxisTitle::updateTitleTextState()
{
    mAxisTitleText->setText(mCurrentAxis->titleText());

    disconnect(mAxisTitleSetText,&QPushButton::clicked,this,&AxisTitle::changeText); // 防止重复连接
    connect(mAxisTitleSetText,&QPushButton::clicked,this,&AxisTitle::changeText);// 2个轴不一样,要在这里更新连接
}

void AxisTitle::changeText()
{
     mCurrentAxis->setTitleText(mAxisTitleText->text().simplified());
}

void AxisTitle::updateTitleColorState()
{
    disconnect(mAxisTitleColor,&QPushButton::clicked,this,&AxisTitle::changeColor);
    connect(mAxisTitleColor,&QPushButton::clicked,this,&AxisTitle::changeColor);
}

void AxisTitle::changeColor()
{
    QColor oldcolor = mCurrentAxis->titleBrush().color();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
            QBrush brush = mCurrentAxis->titleBrush();
            brush.setColor(color);
            mCurrentAxis->setTitleBrush(brush);
    });
    dlg->exec(); delete dlg;
}

void AxisTitle::updateTitleBrushState()
{
    mAxisTitleBrush->setCurrentIndex(mCurrentAxis->titleBrush().style());

    disconnect(mAxisTitleBrush,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrush(int)));
    connect(mAxisTitleBrush,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrush(int)));
}

void AxisTitle::changeBrush(int index)
{
    QBrush brush = mCurrentAxis->titleBrush();
    brush.setStyle(Qt::BrushStyle(index));
    mCurrentAxis->setTitleBrush(brush);
}

void AxisTitle::updateTitleFontState()
{
    disconnect(mAxisTitleFont,&QPushButton::clicked,this,&AxisTitle::changeFont);
    connect(mAxisTitleFont,&QPushButton::clicked,this,&AxisTitle::changeFont);
}

void AxisTitle::changeFont()
{
    QFont oldfont = mCurrentAxis->titleFont();
    QFontDialog * dlg = fontDialog(oldfont);
    connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
           ,this,[=](const QFont& font){mCurrentAxis->setTitleFont(font);});
    dlg->exec(); delete dlg;
}


