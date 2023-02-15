#include "seriesbarset.h"

SeriesBarSet::SeriesBarSet(QChart * chart,QWidget* parent):
    QGroupBox(parent),mChart(chart),mCurrentSeries(nullptr)
{ // 这里设置labelcolor,color,但是这两个要保持相同,也就是每个类别的柱填充颜色和文字标签颜色一致
    // 然后1个label显示当前类别的sum
    mAssociateMode = AssociateMode::ColRegionMode; //barchart初始化用的列区域模式要保持一致
    mCurrentSeries = static_cast<QBarSeries*>(mChart->series().at(0));

    mCurrentCategory = new QComboBox;
    foreach(auto set,mCurrentSeries->barSets()) mCurrentCategory->addItem(set->label());
    connect(mCurrentCategory,SIGNAL(currentIndexChanged(int)),this,SLOT(changeCategory(int)));

    mSum = new QLabel;
    updateSumState();

    mColor = new QPushButton(tr("设置系列颜色"));
    updateColorState();

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&当前系列"),mCurrentCategory);
    lay->addRow(tr("&系列和数"),mSum);
    lay->addRow(tr("&系列颜色"),mColor);
    setLayout(lay);
    setTitle(tr("系列"));

}

void SeriesBarSet::updateBarSet()
{ // 提供给外部使用,对setCurrentSeries的封装,同时还要更新关于类别的combo
    mCurrentCategory->clear();
    if (mChart->series().count() == 0) {disconnectAllConnections();return;}// 先更新当前曲线
    mCurrentSeries = static_cast<QBarSeries*>(mChart->series().at(0));
    foreach(auto set,mCurrentSeries->barSets()) //拿到曲线的系列更新
        mCurrentCategory->addItem(set->label());
    updateState();//再去更新状态
}

void SeriesBarSet::disconnectAllConnections()
{
    disconnect(mColor,&QPushButton::clicked,this,&SeriesBarSet::changeColor);
}

void SeriesBarSet::updateAssociateMode(int mode,int flag)
{
    mAssociateMode = AssociateMode(mode); // 枚举值是对应的
    mAssociateFlags[mAssociateMode] = flag;
    // rowMode,flag=row,系列对应的表格行,系列只可能1个 , flag + 0
    // colMode,flag=col,系列对应的表格列,系列只可能1个, flag + 0
    // rowRegionMode,flag = firstRow, 区域的起始行,那么当前系列对应的表格行就是flag+index
    // colRegionMode,flag = firstCol, 区域的起始行,那么当前系列对应的表格列就是flag+index
}

void SeriesBarSet::setCurrentSeries(QBarSeries *series)
{
    mCurrentSeries = series;
    updateState();
}

QBarSeries* SeriesBarSet::currentSeries() const
{
    return  mCurrentSeries;
}

void SeriesBarSet::updateState()
{
    updateSumState();
    updateColorState();
}

void SeriesBarSet::updateSumState()
{
    mSum->setText(QString("%1").arg(
                      mCurrentSeries->barSets().
                      at(mCurrentCategory->currentIndex())->sum()));
}

void SeriesBarSet::changeCategory(int index)
{
    if (index < 0) return; // 更改系列后combo会调用clear,也会调用此函数,此时index返回-1要返回否则下边越界
    updateState();
}

void SeriesBarSet::updateColorState()
{
    disconnect(mColor,&QPushButton::clicked,this,&SeriesBarSet::changeColor);
    connect(mColor,&QPushButton::clicked,this,&SeriesBarSet::changeColor);
}

void SeriesBarSet::changeColor()
{
    auto barsets = mCurrentSeries->barSets();
    QColor oldcolor = barsets.at(mCurrentCategory->currentIndex())->color();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ // 只设置当前系列,且2个颜色要一致
            barsets.at(mCurrentCategory->currentIndex())->setColor(color);
            barsets.at(mCurrentCategory->currentIndex())->setLabelColor(color);
            switch (mAssociateMode) { // 不管哪种模式都告知了要映射的行列数和颜色,区别是区域模式还要根据起始行列来映射
                    //行/列模式告知系列对应的表格行/列,把接收来的原封不动返回,index一定为0
                    case 0: //qDebug()<<"（3）行模式，返回的实际行数 row= "<<mAssociateFlags[0];
                            emit seriesColorChanged(mCurrentSeries,color,mAssociateFlags[0]);
                            break; // row = row + 0
                    case 1: //qDebug()<<"（3）列模式，返回的实际列数 col= "<<mAssociateFlags[1];
                             emit seriesColorChanged(mCurrentSeries,color,mAssociateFlags[1]);
                            break; // col = col + 0
                    // 区域模式,当前系列的对应的表格实际行列数应当让起始行/列 + index(因为区域是连续的)
                    // 第3行开始,连续4行,表格行3,4,5,6对应index0,1,2,3,所以index+first就是表格实际行
                    case 2://qDebug()<<"（3）行区域模式，返回的实际行数 row= "<<mAssociateFlags[2]+mCurrentCategory->currentIndex();
                            emit seriesColorChanged(mCurrentSeries,color,mAssociateFlags[2]+mCurrentCategory->currentIndex());
                            break;  // row = firstRow+index
                    case 3://qDebug()<<"（3）列区域模式，返回的实际列数 col= "<<mAssociateFlags[3]+mCurrentCategory->currentIndex();
                            emit seriesColorChanged(mCurrentSeries,color,mAssociateFlags[3]+mCurrentCategory->currentIndex());
                            break; // col = firstCol + index
            }
    });
    dlg->exec(); delete dlg;
}

QColorDialog* SeriesBarSet::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}
