#include "axisinfo.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisInfo::AxisInfo(QChart * chart,QAbstractAxis*axis,const QIcon&icon, QWidget*parent):
    AxisBox(chart,axis,icon,parent)
{
    // visible,alignment,orientation,type
    mAxisVisible = new QCheckBox;
    updateVisibilityState();

    mAxisReverse = new QCheckBox;
    updateReversedState();

    mAxisAlignment = new QLabel;
    updateAlignmentState();

    mAxisOrientation = new QLabel;
    updateOrientationState();

    mAxisType = new QLabel;
    updateTypeState();

    QFormLayout * lay = new QFormLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addRow(tr("轴对齐: "),mAxisAlignment);
    lay->addRow(tr("轴方向: "),mAxisOrientation);
    lay->addRow(tr("轴类型: "),mAxisType);
    lay->addRow(tr("轴可见"),mAxisVisible);
    lay->addRow(tr("轴反转"),mAxisReverse);
    setLayout(lay);
    setTitle(tr("轴"));
}

void AxisInfo::disconnectAllConnections()
{
    disconnect(mAxisVisible,&QCheckBox::stateChanged,this,&AxisInfo::changeVisibility);
    disconnect(mAxisReverse,&QCheckBox::stateChanged,this,&AxisInfo::changeReversed);
}

void AxisInfo::updateState()
{
        updateVisibilityState();
        updateReversedState();
        updateAlignmentState();
        updateOrientationState();
        updateTypeState();
}

void AxisInfo::updateVisibilityState()
{
    Qt::CheckState state = mCurrentAxis->isVisible()?Qt::Checked:Qt::Unchecked;
    mAxisVisible->setCheckState(state);
    // 信号连接这里因为有2个轴,所以至少连接2次,所以更新也要更新连接信号,因为2个mCurrentAxis不同
    // 之后就可能出现重复连接的问题,所以要事先断开连接再连接
    disconnect(mAxisVisible,&QCheckBox::stateChanged,this,&AxisInfo::changeVisibility);
    connect(mAxisVisible,&QCheckBox::stateChanged,this,&AxisInfo::changeVisibility);
}

void AxisInfo::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setVisible(s);
}

void AxisInfo::updateReversedState()
{
    Qt::CheckState state = mCurrentAxis->isReverse()?Qt::Checked:Qt::Unchecked;
    mAxisReverse->setCheckState(state);
    disconnect(mAxisReverse,&QCheckBox::stateChanged,this,&AxisInfo::changeReversed);
    connect(mAxisReverse,&QCheckBox::stateChanged,this,&AxisInfo::changeReversed);
}

void AxisInfo::changeReversed(int state)
{
    bool s = state == Qt::Checked;
    mCurrentAxis->setReverse(s);
}

void AxisInfo::updateAlignmentState()
{
    QString align;
    switch (mCurrentAxis->alignment()) {
            case Qt::AlignLeft: align=tr("左对齐");break;
            case Qt::AlignRight: align=tr("右对齐");break;
            case Qt::AlignBottom: align=tr("顶部对齐");break;
            case Qt::AlignTop:align=tr("底部对齐");break;
            default: break;
    }
    mAxisAlignment->setText(align);
}

void AxisInfo::updateOrientationState()
{
    QString orient = mCurrentAxis->orientation()==Qt::Horizontal?tr("水平"):tr("垂直");
    mAxisOrientation->setText(orient);
}

void AxisInfo::updateTypeState()
{
    QString type;
    switch (mCurrentAxis->type()) {
            case QAbstractAxis::AxisTypeNoAxis: type=tr("无坐标轴");break;
            case QAbstractAxis::AxisTypeValue: type = tr("线性坐标轴");break;
            case QAbstractAxis::AxisTypeBarCategory: type = tr("条形坐标轴");break;
            case QAbstractAxis::AxisTypeCategory: type = tr("类别坐标轴");break;
            case QAbstractAxis::AxisTypeDateTime: type = tr("时间坐标轴");break;
            case QAbstractAxis::AxisTypeLogValue: type = tr("对数坐标轴");break;
    }
    mAxisType->setText(type);
}
