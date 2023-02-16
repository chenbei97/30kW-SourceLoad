#include "associatebarmode.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AssociateBarMode::AssociateBarMode(TableViewModel*model,QWidget*parent):
    QGroupBox(parent),mTableModel(model)
{
    setFont(QFont("Times New Roman",12));
    setTitle(tr("关联"));

    mAssociateModeBox = new QGroupBox(tr("关联模式"));
    mRowMode = new QRadioButton(tr("行关联"));
    mColMode = new QRadioButton(tr("列关联"));
    mRowRegionMode = new QRadioButton(tr("行区域关联"));
    mColRegionMode = new QRadioButton(tr("列区域关联")); // 常用

    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addWidget(mRowMode);
    lay1->addWidget(mColMode);
    lay1->addWidget(mRowRegionMode);
    lay1->addWidget(mColRegionMode);
    mAssociateModeBox->setLayout(lay1);
    mRowMode->setChecked(true);

    mAssociateSpinBox = new QGroupBox(tr("关联设置"));
    mRowSpin = new QSpinBox;
    mColSpin = new QSpinBox;
    mColSpin->setEnabled(false);
    mRowSpin->setRange(1,mTableModel->rowCount());
    mColSpin->setRange(1,mTableModel->columnCount());

    QHBoxLayout * lay21 = new QHBoxLayout;
    lay21->addWidget(new QLabel(tr("关联列数")));
    lay21->addWidget(mColSpin);
    QHBoxLayout * lay22 = new QHBoxLayout;
    lay22->addWidget(new QLabel(tr("关联行数")));
    lay22->addWidget(mRowSpin);
    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addLayout(lay21);
    lay2->addLayout(lay22);

    mColRegionBox = new QGroupBox;
    mFirstColumnSpin = new QSpinBox;
    mLastColumnSpin = new QSpinBox;
    mStartRowSpin = new QSpinBox;
    mRowCountSpin = new QSpinBox;
    mFirstColumnSpin->setRange(1,mTableModel->columnCount());
    mLastColumnSpin->setRange(1,mTableModel->columnCount());
    mStartRowSpin->setRange(1,mTableModel->rowCount());
    mRowCountSpin->setRange(1,mTableModel->rowCount());
    QGridLayout * rectlay1 = new QGridLayout;
    rectlay1->addWidget(new QLabel(tr("起始列")),0,0);
    rectlay1->addWidget(mFirstColumnSpin,0,1);
    rectlay1->addWidget(new QLabel(tr("终止列")),0,2);
    rectlay1->addWidget(mLastColumnSpin,0,3);
    rectlay1->addWidget(new QLabel(tr("起始行")),1,0);
    rectlay1->addWidget(mStartRowSpin,1,1);
    rectlay1->addWidget(new QLabel(tr("行数")),1,2);
    rectlay1->addWidget(mRowCountSpin,1,3);
    mColRegionBox->setLayout(rectlay1);
    mColRegionBox->setEnabled(false);

    mRowRegionBox = new QGroupBox;
    mFirstRowSpin = new QSpinBox;
    mLastRowSpin = new QSpinBox;
    mStartColumnSpin = new QSpinBox;
    mColumnCountSpin = new QSpinBox;
    mFirstRowSpin->setRange(1,mTableModel->rowCount());
    mLastRowSpin->setRange(1,mTableModel->rowCount());
    mStartColumnSpin->setRange(1,mTableModel->columnCount());
    mColumnCountSpin->setRange(1,mTableModel->columnCount());
    QGridLayout * rectlay2 = new QGridLayout;
    rectlay2->addWidget(new QLabel(tr("起始行")),0,0);
    rectlay2->addWidget(mFirstRowSpin,0,1);
    rectlay2->addWidget(new QLabel(tr("终止行")),0,2);
    rectlay2->addWidget(mLastRowSpin,0,3);
    rectlay2->addWidget(new QLabel(tr("起始列")),1,0);
    rectlay2->addWidget(mStartColumnSpin,1,1);
    rectlay2->addWidget(new QLabel(tr("列数")),1,2);
    rectlay2->addWidget(mColumnCountSpin,1,3);
    mRowRegionBox->setLayout(rectlay2);
    mRowRegionBox->setEnabled(false);

    QVBoxLayout * lay3 = new QVBoxLayout;
    lay3->addLayout(lay2);
    lay3->addWidget(mColRegionBox);
    lay3->addWidget(mRowRegionBox);
    mAssociateSpinBox->setLayout(lay3);


    QVBoxLayout * lay = new QVBoxLayout;
    lay->addWidget(mAssociateModeBox);
    lay->addWidget(mAssociateSpinBox);
    setLayout(lay);

    connect(mColMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(checked);
            mRowSpin->setEnabled(!checked);
            mColRegionBox->setEnabled(!checked);
            mRowRegionBox->setEnabled(!checked);
            emit modeChanged(false);
    });
    connect(mRowMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(checked);
            mColRegionBox->setEnabled(!checked);
            mRowRegionBox->setEnabled(!checked);
            emit modeChanged(false);
    });
    connect(mColRegionMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(!checked);
            mColRegionBox->setEnabled(checked);
            mRowRegionBox->setEnabled(!checked);
            emit modeChanged(true);
    });
    connect(mRowRegionMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(!checked);
            mColRegionBox->setEnabled(!checked);
            mRowRegionBox->setEnabled(checked);
            emit modeChanged(true);
    });
}

bool AssociateBarMode::isRegionMode() const
{
    return isRowRegionMode() | isColRegionMode();
}

bool AssociateBarMode::isRowColMode() const
{
    return isRowMode() | isColMode();
}

bool AssociateBarMode::isRowRegionMode() const
{
    return mRowRegionMode->isChecked();
}

bool AssociateBarMode::isColRegionMode() const
{
    return mColRegionMode->isChecked();
}

bool AssociateBarMode::isRowMode() const
{
    return mRowMode->isChecked();
}

bool AssociateBarMode::isColMode() const
{
    return mColMode->isChecked();
}

int AssociateBarMode::associateRow() const
{
    return mRowSpin->value()-1;
}

int AssociateBarMode::associateCol() const
{
    return mColSpin->value() - 1;
}

AssociateBarMode::AssociateColParams  AssociateBarMode::associateColParams() const
{
    AssociateColParams  rect = {
        mFirstColumnSpin->value()-1,
        mLastColumnSpin->value()-1,
        mStartRowSpin->value()-1,
        mRowCountSpin->value()
    };
    return rect;
}

AssociateBarMode::AssociateRowParams  AssociateBarMode::associateRowParams() const
{
    AssociateRowParams  rect = {
        mFirstRowSpin->value()-1,
        mLastRowSpin->value()-1,
        mStartColumnSpin->value()-1,
        mColumnCountSpin->value()
    };
    return rect;
}

void AssociateBarMode::adjustRange()
{
    mRowSpin->setRange(1,mTableModel->rowCount());
    mColSpin->setRange(1,mTableModel->columnCount());

    mFirstColumnSpin->setRange(1,mTableModel->columnCount());
    mLastColumnSpin->setRange(1,mTableModel->columnCount());
    mStartRowSpin->setRange(1,mTableModel->rowCount());
    mRowCountSpin->setRange(1,mTableModel->rowCount());

    mFirstRowSpin->setRange(1,mTableModel->rowCount());
    mLastRowSpin->setRange(1,mTableModel->rowCount());
    mStartColumnSpin->setRange(1,mTableModel->columnCount());
    mColumnCountSpin->setRange(1,mTableModel->columnCount());
}
