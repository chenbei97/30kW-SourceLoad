#include <chartview/associateaxis.h>

AssociateAxis::AssociateAxis(QTableView*tableview,QChartView*chartview,QWidget*parent):
    QGroupBox(parent),mTableView(tableview),mChartView(chartview)
{
        mTableModel = static_cast<TableViewModel*>(mTableView->model());
        mAxisType.setX(AxisType::Value);
        mAxisType.setY(AxisType::Value);
        mAxisBase.setX(1.0);
        mAxisBase.setY(1.0);
        setFont(QFont("Times New Roman",12));
        setTitle(tr("坐标轴"));
        mLayout= new QVBoxLayout;
        initHorizonLayout();
        initVerticalLayout();
        setLayout(mLayout);
}

void AssociateAxis::initHorizonLayout()
{
    mHorizontalBox = new QGroupBox;
    QHBoxLayout * hboxlay = new QHBoxLayout;
    mHorizontalValueAxis = new QRadioButton(tr("线性"));
    mHorizontalTimeAxis = new QRadioButton(tr("时间"));
    mHorizontalLogAxis = new QRadioButton(tr("对数"));
    mHorizontalValueAxis->setChecked(true);

    mHorizontalLogBase = new QDoubleSpinBox;
    mHorizontalLogBase->setRange(0.01,100);
    mHorizontalLogBase->setSingleStep(0.01);
    mHorizontalLogBase->hide();

    hboxlay->addWidget(new QLabel(tr("水平坐标轴")));
    hboxlay->addWidget(mHorizontalValueAxis);
    hboxlay->addWidget(mHorizontalTimeAxis);
    hboxlay->addWidget(mHorizontalLogAxis);
    hboxlay->addWidget(mHorizontalLogBase);
    mHorizontalBox->setLayout(hboxlay);
    mHorizontalBox->setEnabled(false);
    mLayout->addWidget(mHorizontalBox);

    QButtonGroup * group = new QButtonGroup;
    group->addButton(mHorizontalValueAxis,0);
    group->addButton(mHorizontalTimeAxis,1);
    group->addButton(mHorizontalLogAxis,2);
    connect(group,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,[=](int id){
            switch (id) {
                case 0:
                        mHorizontalLogBase->hide();
                        mAxisType.setX(AxisType::Value);
                        break;
                case 1:
                        mHorizontalLogBase->hide();
                        mAxisType.setX(AxisType::Time);
                        break;
                case 2:
                        mHorizontalLogBase->show();
                        mAxisType.setX(AxisType::Log);
                        break;
                default:break;
            }
    });
    connect(mHorizontalLogBase,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double base){mAxisBase.setX(base);});
}

void AssociateAxis::initVerticalLayout()
{
    mVerticalBox = new QGroupBox;
    QHBoxLayout * vboxlay = new QHBoxLayout;
    mVerticalValueAxis = new QRadioButton(tr("线性"));
    mVerticalLogAxis = new QRadioButton(tr("对数"));
    mVerticalValueAxis->setChecked(true);

    mVerticalLogBase = new QDoubleSpinBox;
    mVerticalLogBase->setRange(0.01,100);
    mVerticalLogBase->setSingleStep(0.01);
    mVerticalLogBase->hide();
    vboxlay->addWidget(new QLabel(tr("垂直坐标轴")));
    vboxlay->addWidget(mVerticalValueAxis);
    vboxlay->addWidget(mVerticalLogAxis);
    vboxlay->addWidget(mVerticalLogBase);
    mVerticalBox->setLayout(vboxlay);
    mLayout->addWidget(mVerticalBox);

    QButtonGroup * group = new QButtonGroup;
    group->addButton(mVerticalValueAxis,0);
    group->addButton(mVerticalLogAxis,1);
    connect(group,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,[=](int id){
            switch (id) {
                case 0:
                        mVerticalLogBase->hide();
                        mAxisType.setY(AxisType::Value);
                        break;
                case 1:
                        mVerticalLogBase->show();
                        mAxisType.setY(AxisType::Log);
                        break;
                default:break;
            }
    });
    connect(mVerticalLogBase,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double base){mAxisBase.setY(base);});
}

void AssociateAxis::setHorizontalEnabled(bool checked)
{
    mHorizontalBox->setEnabled(checked);
}

void AssociateAxis::setTimeAxisVisible(bool show)
{
    mHorizontalTimeAxis->setVisible(show);
}

QPoint AssociateAxis::axisType() const
{
    return mAxisType;
}

QPointF AssociateAxis::axisBase() const
{
    return mAxisBase;
}
