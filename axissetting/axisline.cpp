#include "axisline.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisLine::AxisLine(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{
    mAxisLineVisibility = new QCheckBox;
    updateVisibilityState();
    mAxisLineColor = new QPushButton(tr("设置轴线颜色"));
    updaterColorState();
    mAxisLineWidth = new QSpinBox;
    updatePenWidthState();
    QFormLayout* lay = new QFormLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addRow(tr("&轴线可见"),mAxisLineVisibility);
    lay->addRow(tr("&轴线颜色"),mAxisLineColor);
    lay->addRow(tr("&轴线宽度"),mAxisLineWidth);
    setTitle(tr("轴线"));
    setLayout(lay);
}

void AxisLine::disconnectAllConnections()
{
    disconnect(mAxisLineVisibility,&QCheckBox::stateChanged,this,&AxisLine::changeVisibility);
    disconnect(mAxisLineColor,&QPushButton::clicked,this,&AxisLine::changeColor);
    disconnect(mAxisLineWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
}

void AxisLine::updateState()
{
    updateVisibilityState();
    updaterColorState();
    updatePenWidthState();
}

void AxisLine::updateVisibilityState()
{
    Qt::CheckState state = mCurrentAxis->isLineVisible()?Qt::Checked:Qt::Unchecked;
    mAxisLineVisibility->setCheckState(state);

    disconnect(mAxisLineVisibility,&QCheckBox::stateChanged,this,&AxisLine::changeVisibility);
    connect(mAxisLineVisibility,&QCheckBox::stateChanged,this,&AxisLine::changeVisibility);
}

void AxisLine::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setLineVisible(s);
}

void AxisLine::updaterColorState()
{
    disconnect(mAxisLineColor,&QPushButton::clicked,this,&AxisLine::changeColor);
    connect(mAxisLineColor,&QPushButton::clicked,this,&AxisLine::changeColor);
}

void AxisLine::changeColor()
{
    QColor oldcolor = mCurrentAxis->linePenColor(); // 和llinePen().brush().color()等价,画笔颜色就是轴线颜色,画笔的画刷样式对轴线有一些作用但是意义不大
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentAxis->setLinePenColor(color);
    });
    dlg->exec(); delete dlg;
}

void AxisLine::updatePenWidthState()
{
    mAxisLineWidth->setValue(mCurrentAxis->linePen().width());

    //No such slot AxisBox::changePenWidth(int) => 没有添加Q_OBJECT宏导致只能搜索父类,父类也没有就出现报错
    disconnect(mAxisLineWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
    connect(mAxisLineWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
}

void AxisLine::changePenWidth(int width)
{
    QPen pen = mCurrentAxis->linePen();
    pen.setWidth(width);
    mCurrentAxis->setLinePen(pen);
}
