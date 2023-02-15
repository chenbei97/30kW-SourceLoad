#include "associatebarseries.h"

AssociateBarSeries::AssociateBarSeries(QWidget*parent):QGroupBox(parent)
{
    mBarColor = Qt::white;
    mBarWidth = 10.;
    setFont(QFont("Times New Roman",12));
    setTitle(tr("曲线"));

    mBarWidthSpin = new QDoubleSpinBox;
    mBarWidthSpin->setMinimum(0.5);
    mBarWidthSpin->setSingleStep(0.1);
    mBarWidthSpin->setDecimals(1);
    mBarWidthSpin->setValue(mBarWidth);

    mBarColorBtn = new QPushButton(tr("设置曲线颜色"));
    mBarColorEdit = new QLineEdit(mBarColor.name());
    mBarColorEdit->setReadOnly(true);
    QFormLayout * linelay = new QFormLayout;
    linelay->addRow(tr("&柱宽"),mBarWidthSpin);
    linelay->addRow(tr("&颜色"),mBarColorBtn);
    linelay->addRow(tr("&当前颜色"),mBarColorEdit);
    setLayout(linelay);

    connect(mBarColorBtn,&QPushButton::clicked,this,[=]{
        QColorDialog * dlg = new QColorDialog(mBarColor);
        dlg->setFixedSize(800,400);
        dlg->setWindowIcon(QIcon(":/images/associatetable.png"));
        dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
        dlg->setWindowTitle(tr("设置颜色"));
        connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
               ,this,[=](const QColor& color){
            mBarColorEdit->setText(color.name()); //把选择的颜色显示在文本框/手动输入也可
            mBarColor = color;
        });
        dlg->exec(); delete dlg;
    });
    connect(mBarWidthSpin,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double width){mBarWidth = width;});
}

QColor AssociateBarSeries::barcolor() const
{
    return mBarColor;
}

double  AssociateBarSeries::barwidth() const
{
    return  mBarWidth;
}


void AssociateBarSeries::setBarColorEnabled(bool checked)
{
    mBarColorBtn->setEnabled(!checked);
    mBarColorEdit->setEnabled(!checked);
}
