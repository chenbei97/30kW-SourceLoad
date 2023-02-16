#include "axislabel.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisLabel::AxisLabel(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{ // visible,color,brush,font,angle

    mAxisLabelVisibility = new QCheckBox;
    updateVisibilityState();

    mAxisLabelAngle = new GradientParamSpinBox;
    mAxisLabelAngle->setRange(0.,360.);
    mAxisLabelAngle->setSuffix("°");
    updateAngleState();

    mAxisLabelColor = new QPushButton(tr("设置标签颜色"));
    updateColorState();

    //mAxisLabelBrush = brushStyleCombo();
    //updateBrushState();

    mAxisLabelFont = new QPushButton(tr("设置标签格式"));
    updateFontState();

    QFormLayout* lay = new QFormLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addRow(tr("&标签可见"),mAxisLabelVisibility);
    lay->addRow(tr("&标签角度"),mAxisLabelAngle);
    lay->addRow(tr("&标签颜色"),mAxisLabelColor);
    lay->addRow(tr("&标签格式"),mAxisLabelFont);
    //lay->addRow(tr("&标签画刷"),mAxisLabelBrush);// 画刷对文字没有意义,没必要加

    setTitle(tr("轴标签"));
    setLayout(lay);
}

void AxisLabel::disconnectAllConnections()
{
    disconnect(mAxisLabelVisibility,&QCheckBox::stateChanged,this,&AxisLabel::changeVisibility);
    disconnect(mAxisLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
    disconnect(mAxisLabelColor,&QPushButton::clicked,this,&AxisLabel::changeColor);
    disconnect(mAxisLabelFont,&QPushButton::clicked,this,&AxisLabel::changeFont);
}

void AxisLabel::updateState()
{
    updateVisibilityState();
    updateAngleState();
    updateColorState();
    //updateBrushState();
    updateFontState();
}

void AxisLabel::updateVisibilityState()
{
    Qt::CheckState state = mCurrentAxis->labelsVisible()?Qt::Checked:Qt::Unchecked;
    mAxisLabelVisibility->setCheckState(state);

    disconnect(mAxisLabelVisibility,&QCheckBox::stateChanged,this,&AxisLabel::changeVisibility);
    connect(mAxisLabelVisibility,&QCheckBox::stateChanged,this,&AxisLabel::changeVisibility);
}

void AxisLabel::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setLabelsVisible(s);
}

void AxisLabel::updateAngleState()
{
    mAxisLabelAngle->setValue((double)mCurrentAxis->labelsAngle());

    disconnect(mAxisLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
    connect(mAxisLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
}

void AxisLabel::changeAngle(double angle)
{
    mCurrentAxis->setLabelsAngle((int)angle);
}

void AxisLabel::updateColorState()
{
    disconnect(mAxisLabelColor,&QPushButton::clicked,this,&AxisLabel::changeColor);
    connect(mAxisLabelColor,&QPushButton::clicked,this,&AxisLabel::changeColor);
}

void AxisLabel::changeColor()
{
    QColor oldcolor = mCurrentAxis->labelsColor();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentAxis->setLabelsColor(color);
    });
    dlg->exec(); delete dlg;
}

void AxisLabel::updateFontState()
{
    disconnect(mAxisLabelFont,&QPushButton::clicked,this,&AxisLabel::changeFont);
    connect(mAxisLabelFont,&QPushButton::clicked,this,&AxisLabel::changeFont);
}

void AxisLabel::changeFont()
{
    QFont oldfont = mCurrentAxis->labelsFont();
    QFontDialog * dlg = fontDialog(oldfont);
    connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
           ,this,[=](const QFont& font){mCurrentAxis->setLabelsFont(font);});
    dlg->exec(); delete dlg;
}

void AxisLabel::updateBrushState()
{
    mAxisLabelBrush->setCurrentIndex(mCurrentAxis->titleBrush().style());

    disconnect(mAxisLabelBrush,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrush(int)));
    connect(mAxisLabelBrush,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrush(int)));
}

void AxisLabel::changeBrush(int index)
{
    QBrush brush = mCurrentAxis->labelsBrush();
    brush.setStyle(Qt::BrushStyle(index));
    mCurrentAxis->setLabelsBrush(brush);
}
