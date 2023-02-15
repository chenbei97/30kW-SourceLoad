#include "associatexyaxis.h"

AssociateXYAxis::AssociateXYAxis(QWidget*parent):QGroupBox(parent)
{
        mAxisType.setX(AxisType::Value);
        mAxisType.setY(AxisType::Value);
        mAxisBase.setX(2.0);
        mAxisBase.setY(2.0);
        setFont(QFont("Times New Roman",12));
        setTitle(tr("坐标轴"));
        mLayout= new QVBoxLayout;
        initHorizonLayout();
        initVerticalLayout();
        setLayout(mLayout);
}

void AssociateXYAxis::initHorizonLayout()
{
    mHorizontalBox = new QGroupBox;
    QHBoxLayout * hboxlay = new QHBoxLayout;
    mHorizontalValueAxis = new QRadioButton(tr("线性"));
    mHorizontalLogAxis = new QRadioButton(tr("对数"));
    mHorizontalValueAxis->setChecked(true);

    mHorizontalLogBase = new QDoubleSpinBox;
    mHorizontalLogBase->setRange(0.01,100);
    mHorizontalLogBase->setSingleStep(0.01);
    mHorizontalLogBase->setValue(2.0);
    mHorizontalLogBase->hide();

    hboxlay->addWidget(new QLabel(tr("水平坐标轴")));
    hboxlay->addWidget(mHorizontalValueAxis);
    hboxlay->addWidget(mHorizontalLogAxis);
    hboxlay->addWidget(mHorizontalLogBase);
    mHorizontalBox->setLayout(hboxlay);
    mHorizontalBox->setEnabled(false);
    mLayout->addWidget(mHorizontalBox);

    QButtonGroup * group = new QButtonGroup;
    group->addButton(mHorizontalValueAxis,0);
    group->addButton(mHorizontalLogAxis,1);
    connect(group,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this,[=](int id){
            switch (id) {
                case 0:
                        mHorizontalLogBase->hide();
                        mAxisType.setX(AxisType::Value);
                        break;
                case 1:
                        mHorizontalLogBase->show();
                        mAxisType.setX(AxisType::Log);
                        break;
                default:break;
            }
    });
    connect(mHorizontalLogBase,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double base){mAxisBase.setX(base);});
}

void AssociateXYAxis::initVerticalLayout()
{
    mVerticalBox = new QGroupBox;
    QHBoxLayout * vboxlay = new QHBoxLayout;
    mVerticalValueAxis = new QRadioButton(tr("线性"));
    mVerticalLogAxis = new QRadioButton(tr("对数"));
    mVerticalValueAxis->setChecked(true);

    mVerticalLogBase = new QDoubleSpinBox;
    mVerticalLogBase->setRange(0.01,100);
    mVerticalLogBase->setSingleStep(0.01);
    mVerticalLogBase->setValue(2.0);
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

void AssociateXYAxis::setHorizontalEnabled(bool checked)
{
    mHorizontalBox->setEnabled(checked);
}

QPoint AssociateXYAxis::axisType() const
{
    return mAxisType;
}

QPointF AssociateXYAxis::axisBase() const
{
    return mAxisBase;
}
