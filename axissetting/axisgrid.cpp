#include "axisgrid.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisGrid::AxisGrid(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{
    mAxisGridVisibility = new QCheckBox;
    mAxisMinorGridVisibility = new QCheckBox;
    updateVisibilityState();
    mAxisGridColor = new QPushButton(tr("设置主网格颜色"));
    mAxisMinorGridColor = new QPushButton(tr("设置次网格颜色"));
    updateColorState();
    mAxisGridWidth = new QSpinBox;
    mAxisMinorGridWidth = new QSpinBox;
    updatePenWidthState();

    QFormLayout * lay = new QFormLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addRow(tr("&主网格可见"),mAxisGridVisibility);
    lay->addRow(tr("&主网格颜色"),mAxisGridColor);
    lay->addRow(tr("&主网格宽度"),mAxisGridWidth);
    lay->addRow(tr("&次网格可见"),mAxisMinorGridVisibility);
    lay->addRow(tr("&次网格颜色"),mAxisMinorGridColor);
    lay->addRow(tr("&次网格宽度"),mAxisMinorGridWidth);
    setTitle(tr("轴网格"));
    setLayout(lay);
}

void AxisGrid::disconnectAllConnections()
{
    disconnect(mAxisMinorGridVisibility,SIGNAL(stateChanged(int)),this,SLOT(changeMinorVisibility(int)));
    disconnect(mAxisGridColor,&QPushButton::clicked,this,&AxisGrid::changeMasterColor);
    disconnect(mAxisMinorGridColor,&QPushButton::clicked,this,&AxisGrid::changeMinorColor);
    disconnect(mAxisGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMasterPenWidth(int)));
    disconnect(mAxisMinorGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMinorPenWidth(int)));
}

void AxisGrid::updateState()
{
    updateVisibilityState();
    updateColorState();
    updatePenWidthState();
}

void AxisGrid::updateVisibilityState()
{
    Qt::CheckState state1 = mCurrentAxis->isGridLineVisible()?Qt::Checked:Qt::Unchecked;
    mAxisGridVisibility->setCheckState(state1);
    disconnect(mAxisGridVisibility,SIGNAL(stateChanged(int)),this,SLOT(changeMasterVisibility(int)));
    connect(mAxisGridVisibility,SIGNAL(stateChanged(int)),this,SLOT(changeMasterVisibility(int)));
    Qt::CheckState state2 = mCurrentAxis->isMinorGridLineVisible()?Qt::Checked:Qt::Unchecked;
    mAxisMinorGridVisibility->setCheckState(state2);

    disconnect(mAxisMinorGridVisibility,SIGNAL(stateChanged(int)),this,SLOT(changeMinorVisibility(int)));
    connect(mAxisMinorGridVisibility,SIGNAL(stateChanged(int)),this,SLOT(changeMinorVisibility(int)));
}

void AxisGrid::changeMasterVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setGridLineVisible(s);
}

void AxisGrid::changeMinorVisibility(int state)
{// qt文档说明了minorGridLineColor,minorGridLinePen,minorGridVisible 仅适用于支持次要轴网线的轴
    bool s = state == Qt::Checked; // 目前测试的情况是QValueAxis不支持次网格线,QLogValueAxis支持
    mCurrentAxis->setMinorGridLineVisible(s); // 相应的次网格线的颜色和画笔宽度等也不支持
}

void AxisGrid::updateColorState()
{
    disconnect(mAxisGridColor,&QPushButton::clicked,this,&AxisGrid::changeMasterColor);
    connect(mAxisGridColor,&QPushButton::clicked,this,&AxisGrid::changeMasterColor);

    disconnect(mAxisMinorGridColor,&QPushButton::clicked,this,&AxisGrid::changeMinorColor);
    connect(mAxisMinorGridColor,&QPushButton::clicked,this,&AxisGrid::changeMinorColor);
}

void AxisGrid::changeMasterColor()
{
    QColor oldcolor = mCurrentAxis->gridLineColor(); // 和gridLinePen().brush().color()等价,画笔颜色就是网格线颜色
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentAxis->setGridLineColor(color);
    });
    dlg->exec(); delete dlg;
}

void AxisGrid::changeMinorColor()
{
    QColor oldcolor = mCurrentAxis->minorGridLineColor(); // 同理
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentAxis->setMinorGridLineColor(color);
    });
    dlg->exec(); delete dlg;
}

void AxisGrid::updatePenWidthState()
{
    mAxisGridWidth->setValue(mCurrentAxis->gridLinePen().width());
    disconnect(mAxisGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMasterPenWidth(int)));
    connect(mAxisGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMasterPenWidth(int)));

    mAxisGridWidth->setValue(mCurrentAxis->minorGridLinePen().width());
    disconnect(mAxisMinorGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMinorPenWidth(int)));
    connect(mAxisMinorGridWidth,SIGNAL(valueChanged(int)),this,SLOT(changeMinorPenWidth(int)));
}

void AxisGrid::changeMasterPenWidth(int width)
{
    QPen pen = mCurrentAxis->gridLinePen();
    pen.setWidth(width);
    mCurrentAxis->setGridLinePen(pen);
}

void AxisGrid::changeMinorPenWidth(int width)
{
    QPen pen = mCurrentAxis->minorGridLinePen();
    pen.setWidth(width);
    mCurrentAxis->setMinorGridLinePen(pen);
}
