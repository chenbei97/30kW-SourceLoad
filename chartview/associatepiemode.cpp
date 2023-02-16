#include "associatepiemode.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AssociatePieMode::AssociatePieMode(TableViewModel*model,QWidget*parent):
    QGroupBox(parent),mTableModel(model)
{
    setFont(QFont("Times New Roman",12));
    setTitle(tr("关联"));

    mAssociateModeBox = new QGroupBox(tr("关联模式"));
    mRowMode = new QRadioButton(tr("行表头关联"));
    mColMode = new QRadioButton(tr("列表头关联"));
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

    mRowRegionBox = new QGroupBox;
    mValuesRowSpin = new QSpinBox;
    mLabelsRowSpin = new QSpinBox;
    mFirstColumnSpin = new QSpinBox;
    mColumnCountSpin = new QSpinBox;
    mValuesRowSpin->setRange(1,mTableModel->rowCount());
    mLabelsRowSpin->setRange(1,mTableModel->rowCount());
    mFirstColumnSpin->setRange(1,mTableModel->columnCount());
    mColumnCountSpin->setRange(1,mTableModel->columnCount());
    QGridLayout * rectlay1 = new QGridLayout;
    rectlay1->addWidget(new QLabel(tr("标签行")),0,0);
    rectlay1->addWidget(mLabelsRowSpin,0,1);
    rectlay1->addWidget(new QLabel(tr("数值行")),0,2);
    rectlay1->addWidget(mValuesRowSpin,0,3);
    rectlay1->addWidget(new QLabel(tr("起始列")),1,0);
    rectlay1->addWidget(mFirstColumnSpin,1,1);
    rectlay1->addWidget(new QLabel(tr("列数")),1,2);
    rectlay1->addWidget(mColumnCountSpin,1,3);
    mRowRegionBox->setLayout(rectlay1);
    mRowRegionBox->setEnabled(false);

    mColRegionBox = new QGroupBox;
    mValuesColumnSpin = new QSpinBox;
    mLabelsColumnSpin = new QSpinBox;
    mFirstRowSpin = new QSpinBox;
    mRowCountSpin = new QSpinBox;
    mValuesColumnSpin->setRange(1,mTableModel->columnCount());
    mLabelsColumnSpin->setRange(1,mTableModel->columnCount());
    mFirstRowSpin->setRange(1,mTableModel->rowCount());
    mRowCountSpin->setRange(1,mTableModel->rowCount());
    QGridLayout * rectlay2 = new QGridLayout;
    rectlay2->addWidget(new QLabel(tr("标签列")),0,0);
    rectlay2->addWidget(mLabelsColumnSpin,0,1);
    rectlay2->addWidget(new QLabel(tr("数值列")),0,2);
    rectlay2->addWidget(mValuesColumnSpin,0,3);
    rectlay2->addWidget(new QLabel(tr("起始行")),1,0);
    rectlay2->addWidget(mFirstRowSpin,1,1);
    rectlay2->addWidget(new QLabel(tr("行数")),1,2);
    rectlay2->addWidget(mRowCountSpin,1,3);
    mColRegionBox->setLayout(rectlay2);
    mColRegionBox->setEnabled(false);

    QVBoxLayout * lay3 = new QVBoxLayout;
    lay3->addLayout(lay2);
    lay3->addWidget(mRowRegionBox);
    lay3->addWidget(mColRegionBox);
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
    });
    connect(mRowMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(checked);
            mColRegionBox->setEnabled(!checked);
            mRowRegionBox->setEnabled(!checked);
    });
    connect(mColRegionMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(!checked);
            mColRegionBox->setEnabled(checked);
            mRowRegionBox->setEnabled(!checked);
    });
    connect(mRowRegionMode,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            mColSpin->setEnabled(!checked);
            mRowSpin->setEnabled(!checked);
            mColRegionBox->setEnabled(!checked);
            mRowRegionBox->setEnabled(checked);
    });
}

bool AssociatePieMode::isRegionMode() const
{
    return isRowRegionMode() | isColRegionMode();
}

bool AssociatePieMode::isRowColMode() const
{
    return isRowMode() | isColMode();
}

bool AssociatePieMode::isRowRegionMode() const
{
    return mRowRegionMode->isChecked();
}

bool AssociatePieMode::isColRegionMode() const
{
    return mColRegionMode->isChecked();
}

bool AssociatePieMode::isRowMode() const
{
    return mRowMode->isChecked();
}

bool AssociatePieMode::isColMode() const
{
    return mColMode->isChecked();
}

int AssociatePieMode::associateRow() const
{
    return mRowSpin->value()-1;
}

int AssociatePieMode::associateCol() const
{
    return mColSpin->value() - 1;
}

AssociatePieMode::AssociateColParams  AssociatePieMode::associateColParams() const
{
    AssociateColParams  rect = {
        mLabelsColumnSpin->value()-1,
        mValuesColumnSpin->value()-1,
        mFirstRowSpin->value()-1,
        mRowCountSpin->value()
    };
    return rect;
}

AssociatePieMode::AssociateRowParams  AssociatePieMode::associateRowParams() const
{
    AssociateRowParams  rect = {
        mLabelsRowSpin->value()-1,
        mValuesRowSpin->value()-1,
        mFirstColumnSpin->value()-1,
        mColumnCountSpin->value()
    };
    return rect;
}

void AssociatePieMode::adjustRange()
{
    mRowSpin->setRange(1,mTableModel->rowCount());
    mColSpin->setRange(1,mTableModel->columnCount());

    mValuesRowSpin->setRange(1,mTableModel->rowCount());
    mLabelsRowSpin->setRange(1,mTableModel->rowCount());
    mFirstColumnSpin->setRange(1,mTableModel->columnCount());
    mColumnCountSpin->setRange(1,mTableModel->columnCount());

    mValuesColumnSpin->setRange(1,mTableModel->columnCount());
    mLabelsColumnSpin->setRange(1,mTableModel->columnCount());
    mFirstRowSpin->setRange(1,mTableModel->rowCount());
    mRowCountSpin->setRange(1,mTableModel->rowCount());
}
