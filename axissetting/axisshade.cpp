#include "axisshade.h"

AxisShade::AxisShade(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{
        mAxisShadeVisibility = new QCheckBox;
        updateVisibilityState();

        mAxisShadeBorderColor = new QPushButton(tr("设置边框颜色"));
        updateBorderColorState();

        mAxisShadeBrushColor = new QPushButton(tr("设置背景颜色"));
        updateBrushColorState();

        mAxisShadePenWidth = new QSpinBox;
        updatePenWidthState();

        mAxisShadeBrushStyle = brushStyleCombo();
        updateBrushStyleState();

        QFormLayout* lay = new QFormLayout;
        lay->setSizeConstraint(QLayout::SetNoConstraint);
        lay->addRow(tr("&阴影可见"),mAxisShadeVisibility);
        lay->addRow(tr("&边框颜色"),mAxisShadeBorderColor);
        lay->addRow(tr("&边框宽度"),mAxisShadePenWidth);
        lay->addRow(tr("&背景颜色"),mAxisShadeBrushColor);
        lay->addRow(tr("&背景样式"),mAxisShadeBrushStyle);
        setTitle(tr("轴阴影"));
        setLayout(lay);
}

void AxisShade::disconnectAllConnections()
{
    disconnect(mAxisShadeVisibility,&QCheckBox::stateChanged,this,&AxisShade::changeVisibility);
    disconnect(mAxisShadeBorderColor,&QPushButton::clicked,this,&AxisShade::changeBorderColor);
    disconnect(mAxisShadeBrushColor,&QPushButton::clicked,this,&AxisShade::changeBrushColor);
    disconnect(mAxisShadeBrushStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrushStyle(int)));
    disconnect(mAxisShadePenWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
}

void AxisShade::updateState()
{
    updateVisibilityState();
    updateBorderColorState();
    updateBrushColorState();
    updateBrushStyleState();
    updatePenWidthState();
}

void AxisShade::updateVisibilityState()
{
    Qt::CheckState state = mCurrentAxis->shadesVisible()?Qt::Checked:Qt::Unchecked;
    mAxisShadeVisibility->setCheckState(state);

    disconnect(mAxisShadeVisibility,&QCheckBox::stateChanged,this,&AxisShade::changeVisibility);
    connect(mAxisShadeVisibility,&QCheckBox::stateChanged,this,&AxisShade::changeVisibility);
}

void AxisShade::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setShadesVisible(s);
}

void AxisShade::updateBorderColorState()
{
    disconnect(mAxisShadeBorderColor,&QPushButton::clicked,this,&AxisShade::changeBorderColor);
    connect(mAxisShadeBorderColor,&QPushButton::clicked,this,&AxisShade::changeBorderColor);
}

void AxisShade::changeBorderColor()
{
    QColor oldcolor = mCurrentAxis->shadesBorderColor(); // 画笔颜色就是边框颜色,画笔的画刷样式对边框没有意义
    //QColor oldcolor = mCurrentAxis->shadesPen().brush().color();//和shadesPen().brush().color()等价
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
        mCurrentAxis->setShadesBorderColor(color); // 2种方式均可
//        QPen pen = mCurrentAxis->shadesPen();
//        QBrush brush = pen.brush();
//        brush.setColor(color);
//        pen.setBrush(brush);
//        mCurrentAxis->setShadesPen(pen);
    });
    dlg->exec(); delete dlg;
}

void AxisShade::updateBrushColorState()
{
    disconnect(mAxisShadeBrushColor,&QPushButton::clicked,this,&AxisShade::changeBrushColor);
    connect(mAxisShadeBrushColor,&QPushButton::clicked,this,&AxisShade::changeBrushColor);
}

void AxisShade::changeBrushColor()
{
    QColor oldcolor = mCurrentAxis->shadesColor(); // 和shadesBrush().color()等价,画刷颜色就是背景颜色
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentAxis->setShadesColor(color);
    });
    dlg->exec(); delete dlg;
}

void AxisShade::updateBrushStyleState()
{
    mAxisShadeBrushStyle->setCurrentIndex(mCurrentAxis->shadesBrush().style());

    disconnect(mAxisShadeBrushStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrushStyle(int)));
    connect(mAxisShadeBrushStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(changeBrushStyle(int)));
}

void AxisShade::changeBrushStyle(int index)
{
    QBrush brush = mCurrentAxis->shadesBrush();
    brush.setStyle(Qt::BrushStyle(index));
    mCurrentAxis->setShadesBrush(brush);
}

void AxisShade::updatePenWidthState()
{
    mAxisShadePenWidth->setValue(mCurrentAxis->shadesPen().width());

    disconnect(mAxisShadePenWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
    connect(mAxisShadePenWidth,SIGNAL(valueChanged(int)),this,SLOT(changePenWidth(int)));
}

void AxisShade::changePenWidth(int width)
{
    QPen pen = mCurrentAxis->shadesPen();
    pen.setWidth(width);
    mCurrentAxis->setShadesPen(pen);
}





