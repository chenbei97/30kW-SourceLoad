#include "axissetting.h"

AxisSetting::AxisSetting(QChart*chart):mChart(chart)
{
    setObjectName("AxisSettingsLayout");
    mIcon.addFile(":/images/toolbox_axis.png");
    setSizeConstraint(QLayout::SetMaximumSize);
    mCurrentAxis = nullptr;

    if (mChart->axisX()) mCurrentAxis = mChart->axisX();// 默认X为操作轴,饼图没有轴
    if (!mCurrentAxis) return;

    mWhichAxis = new QGroupBox;
    mAxisInfo = new AxisInfo(mChart,mCurrentAxis,mIcon);
    mAxisTitle = new AxisTitle(mChart,mCurrentAxis,mIcon);
    mAxisLabel = new AxisLabel(mChart,mCurrentAxis,mIcon);
    mAxisShade = new AxisShade(mChart,mCurrentAxis,mIcon);
    mAxisLine = new AxisLine(mChart,mCurrentAxis,mIcon);
    mAxisGrid = new AxisGrid(mChart,mCurrentAxis,mIcon);

    mAxisValue = new AxisValue(mChart); //初始化布局,根据当前轴类型动态的显示和隐藏
    mAxisLog = new AxisLog(mChart);
    mAxisBar = new AxisBarCategory(mChart);
    //mAxisTime = new AxisTime(mChart);

    QGroupBox* box = new QGroupBox(tr("其它"));
    QVBoxLayout * lay = new QVBoxLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addWidget(mAxisValue);
    lay->addWidget(mAxisLog);
    lay->addWidget(mAxisBar);
    //lay->addWidget(mAxisTime);//不再支持时间轴
    box->setLayout(lay);


    initWhichAxis();

    addWidget(mWhichAxis);
    addWidget(mAxisInfo);
    addWidget(mAxisTitle);
    addWidget(mAxisLabel);
    addWidget(mAxisShade);
    addWidget(mAxisLine);
    addWidget(mAxisGrid);
    addWidget(box);
    //addSpacing(10);//为了防止挤压尾部留些空间

    //通过removeWidget和addWidget动态切换轴有点难
    // 而且由于QScrollArea布局必须事先确定的问题,不可能实现
    // 但是可以通过hide和show来显示这种效果,布局也已经事先确定了
    // 2023/2/9,由于show和hide会导致挤压,所以改用enable来设定,一个妥协策略
    switch (mCurrentAxis->type()) {
            case QAbstractAxis::AxisTypeValue:
                    mAxisValue->setCurrentAxis(static_cast<QValueAxis*>(mCurrentAxis)); // 确实是value类型才会初始化内部的信号连接
//                    mAxisLog->hide();
//                    mAxisBar->hide();
//                    mAxisTime->hide();
                    mAxisLog->setEnabled(false);
                    mAxisBar->setEnabled(false);
                    //mAxisTime->setEnabled(false);
                    break;
            case QAbstractAxis::AxisTypeLogValue:
                    mAxisLog->setCurrentAxis(static_cast<QLogValueAxis*>(mCurrentAxis));
//                    mAxisValue->hide();
//                    mAxisBar->hide();
//                    mAxisTime->hide();
                    mAxisValue->setEnabled(false);
                    mAxisBar->setEnabled(false);
                    //mAxisTime->setEnabled(false);
                    break;
            case QAbstractAxis::AxisTypeBarCategory:
                    mAxisBar->setCurrentAxis(static_cast<QBarCategoryAxis*>(mCurrentAxis));
//                    mAxisValue->hide();
//                    mAxisLog->hide();
//                    mAxisTime->hide();
                    mAxisValue->setEnabled(false);
                    mAxisLog->setEnabled(false);
                    //mAxisTime->setEnabled(false);
                    break;
//            case QAbstractAxis::AxisTypeDateTime:
////                    mAxisValue->hide();
////                    mAxisLog->hide();
////                    mAxisBar->hide();
//                    mAxisTime->setCurrentAxis(static_cast<QDateTimeAxis*>(mCurrentAxis));
//                    mAxisValue->setEnabled(false);
//                    mAxisLog->setEnabled(false);
//                    mAxisBar->setEnabled(false);
//                    break;
            default:break;
    }

    // 导入文件，清空图表和关联表格都会发出该信号，其中导入和清空需要调用disconnectAllConnections
    connect(this,&AxisSetting::associateCompeleted,this,[=]{mAxisX->click();});

}

void AxisSetting::disconnectAllConnections()
{
    mAxisInfo->disconnectAllConnections();
    mAxisTitle->disconnectAllConnections();
    mAxisLabel->disconnectAllConnections();
    mAxisShade->disconnectAllConnections();
    mAxisLine->disconnectAllConnections();
    mAxisGrid->disconnectAllConnections();

    mAxisValue->disconnectAllConnections();
    mAxisLog->disconnectAllConnections();
    mAxisBar->disconnectAllConnections();
    //mAxisTime->disconnectAllConnections();
}

void AxisSetting::initWhichAxis()
{ // operate which axis?
    mAxisX = new QRadioButton(tr("X轴"));
    mAxisY = new QRadioButton(tr("Y轴"));
    QButtonGroup * group = new QButtonGroup;
    group->addButton(mAxisX,1);
    group->addButton(mAxisY,2);
    mAxisX->setChecked(true); // 默认操作x轴

    QHBoxLayout * lay = new QHBoxLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addWidget(mAxisX);
    lay->addWidget(mAxisY);
    mWhichAxis ->setLayout(lay);
    mWhichAxis->setTitle(tr("当前轴"));
    mWhichAxis->setMinimumWidth(450);

    connect(group,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,[=](int id){
            if (id == 1)  mCurrentAxis = mChart->axisX();// 对X轴操作
            else mCurrentAxis = mChart->axisY(); // Y轴操作

            // 清除所有曲线时会导致返回的轴是空指针,提前返回,因为下边的groupbox不允许空指针
            if (mCurrentAxis == nullptr) {disconnectAllConnections();return;}

            mAxisInfo->setCurrentAxis(mCurrentAxis);
            mAxisTitle->setCurrentAxis(mCurrentAxis);
            mAxisLabel->setCurrentAxis(mCurrentAxis);
            mAxisShade->setCurrentAxis(mCurrentAxis);
            mAxisLine->setCurrentAxis(mCurrentAxis);
            mAxisGrid->setCurrentAxis(mCurrentAxis);

            // 和共有的工具不同,这2个要有条件的setCurrentAxis
            switch (mCurrentAxis->type()) { // 无论切换的X还是Y,类型都可能是Value或Log,都要去判断,只要是value类型就让value布局显示
                    case QAbstractAxis::AxisTypeValue:
                            mAxisValue->setCurrentAxis(static_cast<QValueAxis*>(mCurrentAxis));
//                            mAxisValue->show();
//                            mAxisLog->hide();
//                            mAxisBar->hide();
//                            mAxisTime->hide();
                            mAxisValue->setEnabled(true);
                            mAxisLog->setEnabled(false);
                            mAxisBar->setEnabled(false);
                            //mAxisTime->setEnabled(false);
                            break;
                    case QAbstractAxis::AxisTypeLogValue:
                            mAxisLog->setCurrentAxis(static_cast<QLogValueAxis*>(mCurrentAxis));
//                            mAxisValue->hide();
//                            mAxisLog->show();
//                            mAxisBar->hide();
//                            mAxisTime->hide();
                            mAxisValue->setEnabled(false);
                            mAxisLog->setEnabled(true);
                            mAxisBar->setEnabled(false);
                            //mAxisTime->setEnabled(false);
                            break;
                    case QAbstractAxis::AxisTypeBarCategory:
                            mAxisBar->setCurrentAxis(static_cast<QBarCategoryAxis*>(mCurrentAxis));
//                            mAxisValue->hide();
//                            mAxisLog->hide();
//                            mAxisBar->show();
//                            mAxisTime->hide();
                            mAxisValue->setEnabled(false);
                            mAxisLog->setEnabled(false);
                            mAxisBar->setEnabled(true);
                            //mAxisTime->setEnabled(false);
                            break;
//                    case QAbstractAxis::AxisTypeDateTime:
//                            mAxisTime->setCurrentAxis(static_cast<QDateTimeAxis*>(mCurrentAxis));
////                            mAxisValue->hide();
////                            mAxisLog->hide();
////                            mAxisBar->hide();
////                            mAxisTime->show();
//                            mAxisValue->setEnabled(false);
//                            mAxisLog->setEnabled(false);
//                            mAxisBar->setEnabled(false);
//                            mAxisTime->setEnabled(true);
//                            break;
                    default:break;
            }
    });
}
