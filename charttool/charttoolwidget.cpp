#include "charttoolwidget.h"

ChartToolWidget::ChartToolWidget(QWidget *parent) : QWidget(parent)
{
    // this => vlayout = > scrollarea => qwidget => vlayout => groupbox => gridlayout
    mainLayout = new QVBoxLayout; // 主布局
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mainLayout->setObjectName("ToolBoxMainLayout");
    mScrollArea = new QScrollArea; // 滚动区域
    mWidget = new QWidget; // 小部件
    mWidgetLayout = new QVBoxLayout; // 小部件的布局
    mWidgetLayout->setSizeConstraint(QLayout::SetNoConstraint);
    mWidgetLayout->setObjectName("ToolBoxPageLayout");

    // 小部件这层的布局是自由发挥的,每种曲线都有自己可设置的属性,外部使用addGroupBox来完成
    mWidget->setLayout(mWidgetLayout); // 小部件设置小部件布局
    //mScrollArea->setWidget(mWidget); // 小部件设置为滚动区域(给外部调用)
    mainLayout->addWidget(mScrollArea); // 主布局添加滚动区域(要依次按顺序添加否则不会显示groupbox)
    setLayout(mainLayout); // 主布局
    setFont(QFont("Times New Roman",12));
    setMinimumWidth(450); // 只能限制主布局不能限制ToolBox的宽度
    setMinimumHeight(800);
}

void ChartToolWidget::addGroupBox(QGroupBox * groupbox)
{
    mWidgetLayout->addWidget(groupbox);
}

void ChartToolWidget::addLayout(QLayout *lay)
{
    mWidgetLayout->addLayout(lay); // 小布局的布局去添加带有GroupBox的布局
}

void ChartToolWidget::updateScrollArea()
{
    mScrollArea->setWidget(mWidget);// 滚动区域只能设置1次,外部addGroupBox结束后才能调用
    mScrollArea->ensureWidgetVisible(mWidget,20,20);
    // 一旦设置外部再改变布局也不会显示
}


