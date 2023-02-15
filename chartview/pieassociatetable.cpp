#include "pieassociatetable.h"

PieAssociateTable::PieAssociateTable(QTableView*tableview,QChartView*chartview,QWidget*parent):
    QDialog(parent),mTableView(tableview),mChartView(chartview)
{
    mCurrentSeries = nullptr;
    mAssociateMode = AssociateMode::RowMode;
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;
    mTip->setChart(mChartView->chart());
    mTableModel = static_cast<TableViewModel*>(mTableView->model());

    mMode = new AssociatePieMode(mTableModel);
    mSeries = new AssociatePieSeries;

    mOkBtn = new QPushButton(tr("关联"));
    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addStretch();
    lay1->addWidget(mOkBtn);

    QVBoxLayout * lay = new QVBoxLayout;
    lay->addWidget(mMode);
    lay->addWidget(mSeries);
    lay->addLayout(lay1);
    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/associatetable.png"));
    resize(QSize(800,400));
    setLayout(lay);
    initConnections();
}

void PieAssociateTable::onSeriesColorChanged(QColor color, int index)
{
    switch (mAssociateMode) {
        case 0: mTableModel->addCellMapping(QRect(index,mFlagValue,1,1),color);break; // row,col=index 行
        case 1: mTableModel->addCellMapping(QRect(mFlagValue,index,1,1),color);break;  // col,row=index 列
        case 2:mTableModel->addCellMapping(QRect(index,mRegionLabelValue.first,1,1),color); // 标签行和数值行颜色都改
            mTableModel->addCellMapping(QRect(index,mRegionLabelValue.second,1,1),color);break;  // firstCol+index = col, row = valuesRow(事先保存)
        case 3:mTableModel->addCellMapping(QRect(mRegionLabelValue.first,index,1,1),color); // 标签列和数值列颜色都改
            mTableModel->addCellMapping(QRect(mRegionLabelValue.second,index,1,1),color);break; // firstRow+index=row, col = valuesCol(事先保存)
    }
}

void PieAssociateTable::onOkBtn()
{
    //和柱状图一样,空表不能再绘制饼图
    if(!mTableModel->isAllDataValid()) {accept();return;}

    mChartView->chart()->removeAllSeries();
    mTableModel->clearMapping();
    if (mMode->isRegionMode()) regionMapping();
    else rcMapping();
    mLegend->mapping(mChartView->chart());
    emit associateCompeleted();
    accept();
}

void PieAssociateTable::rcMapping()
{
    auto slices = createSlices();
    if (slices.count() == 0) { accept(); return;}

    auto series = new QPieSeries;
    series->append(slices);
    setSeriesProperties(series);// series添加slices之后再设置才起作用
    series->setLabelsVisible(true);
    mChartView->chart()->addSeries(series);
    mChartView->chart()->createDefaultAxes();

    if (mMode->isRowMode()){ // 必须在chart()addSeries之后slices的color才生成可用于表格颜色映射
        for(int c = 0; c < mTableModel->columnCount(); ++c)
            mTableModel->addCellMapping(QRect(c,mMode->associateRow(),1,1),
                                        slices.at(c)->color());
    }
    else {
        for(int r = 0; r < mTableModel->rowCount(); ++r)
            mTableModel->addCellMapping(QRect(mMode->associateCol(),r,1,1)
                                        ,slices.at(r)->color());
    }

    mTip->mapping(series);
}

void PieAssociateTable::regionMapping()
{
    auto series = new QPieSeries;

    if (mMode->isRowRegionMode())
        createRowRegionMapping(series);
    else
        createColRegionMapping(series);

    mTip->mapping(series);
}

void PieAssociateTable::createColRegionMapping(QPieSeries*series)
{
    auto params = mMode->associateColParams();

//    if (!mTableModel->isColumnDataValid(params.labelsColumn,params.firstRow,params.rowCount)
//         || !mTableModel->isColumnDataValid(params.valuesColumn,params.firstRow,params.rowCount))
//        return;

//    QVPieModelMapper *mapper = new QVPieModelMapper;
//    mapper->setLabelsColumn(params.labelsColumn);
//    mapper->setValuesColumn(params.valuesColumn);
//    mapper->setFirstRow(params.firstRow);
//    mapper->setRowCount(params.rowCount);
//    mapper->setSeries(series);
//    mapper->setModel(mTableModel); // series自动生成好barset

    QList<QPieSlice*> slices;
    auto labelData = mTableModel->colData(params.labelsColumn,params.firstRow,params.rowCount);
    auto valueData = mTableModel->colData(params.valuesColumn,params.firstRow,params.rowCount);
    auto valueSum = mTableModel->dataSummary(valueData);
    Q_ASSERT(labelData.count() == valueData.count());
    for(int c = 0; c < labelData.count(); ++c)
    {
        //if (labelData[c].isValid() && valueData[c].isValid())
        auto label = labelData[c].toString();
        auto val = valueData[c].toDouble();

        QPieSlice * slice = new QPieSlice(QString("%1(%2%)").arg(label).arg(val/valueSum*100.0,0,'g',3),val);
        slices.append(slice);
    }

    series->append(slices);
    mChartView->chart()->addSeries(series); // 图表要先添加曲线,否则下方colorlist得到的全是黑色
    mChartView->chart()->createDefaultAxes();

    setSeriesProperties(series);
    QColorList colorlist;
    for(auto slice: slices) colorlist.append(slice->color());

    mTableModel->addColMapping(params.labelsColumn,params.firstRow,params.rowCount,colorlist); // 对标签列的连续行单元格区域映射颜色
    mTableModel->addColMapping(params.valuesColumn,params.firstRow,params.rowCount,colorlist); // 数值列
    mAssociateMode = AssociateMode::ColRegionMode;
    mRegionLabelValue = {params.labelsColumn,params.valuesColumn}; // 事先保存好标签和数值所在的列
    emit modeChanged(AssociateMode::ColRegionMode,params.firstRow); // 告知第1个块所在的表格行位置,这样可以返回firstRow+index得到不同块在表格的位置
}

void PieAssociateTable::createRowRegionMapping(QPieSeries*series)
{
    auto params = mMode->associateRowParams();

//    if (!mTableModel->isRowDataValid(params.labelsRow,params.firstColumn,params.columnCount)
//         || !mTableModel->isRowDataValid(params.valuesRow,params.firstColumn,params.columnCount))
//        return; // pie图无需判断,经测试标签和数值都全0也没事

//    QHPieModelMapper *mapper = new QHPieModelMapper;
//    mapper->setLabelsRow(params.labelsRow);
//    mapper->setValuesRow(params.valuesRow);
//    mapper->setFirstColumn(params.firstColumn);
//    mapper->setColumnCount(params.columnCount);
//    mapper->setSeries(series);
//    mapper->setModel(mTableModel); // series自动生成好barset
    // 不再使用这个映射,它的问题在于每次绘制完会把model的对应连续行或者连续列的数据清除
    // 也即是mapper是引用性质,而柱状图的mapper没有影响model

    QList<QPieSlice*> slices;//所以只能改用手动创建了
    auto labelData = mTableModel->rowData(params.labelsRow,params.firstColumn,params.columnCount);
    auto valueData = mTableModel->rowData(params.valuesRow,params.firstColumn,params.columnCount);
    auto valueSum = mTableModel->dataSummary(valueData);
    Q_ASSERT(labelData.count() == valueData.count());
    for(int c = 0; c < labelData.count(); ++c)
    {
        //if (labelData[c].isValid() && valueData[c].isValid())
        auto label = labelData[c].toString();
        auto val = valueData[c].toDouble();
        QPieSlice * slice = new QPieSlice(QString("%1(%2%)").arg(label).arg(val/valueSum*100.0,0,'g',3),val);
        slices.append(slice);
    }

    series->append(slices);
    mChartView->chart()->addSeries(series); // 图表要先添加曲线,否则下方colorlist得到的全是黑色
    mChartView->chart()->createDefaultAxes();

    setSeriesProperties(series);

    QColorList colorlist;
    for(auto slice: slices) colorlist.append(slice->color());

    mTableModel->addRowMapping(params.labelsRow,params.firstColumn,params.columnCount,colorlist); // 对标签行的连续列单元格区域映射颜色
    mTableModel->addRowMapping(params.valuesRow,params.firstColumn,params.columnCount,colorlist); // 数值行
    mAssociateMode = AssociateMode::RowRegionMode;
    mRegionLabelValue = {params.labelsRow,params.valuesRow}; // 事先保存好标签和数值所在的行
    emit modeChanged(AssociateMode::RowRegionMode,params.firstColumn);// 告知第1个块所在的表格列位置,这样可以返回firstCol+index得到不同块在表格的位置
}

void PieAssociateTable::initConnections()
{
    connect(mOkBtn,&QPushButton::clicked,this,&PieAssociateTable::onOkBtn);

    connect(this,&PieAssociateTable::tableChanged,this,[=]{
        disconnect(mOkBtn,&QPushButton::clicked,this,&PieAssociateTable::onOkBtn);
        mTableModel = static_cast<TableViewModel*>(mTableView->model());
        connect(mOkBtn,&QPushButton::clicked,this,&PieAssociateTable::onOkBtn);
        mMode->adjustRange();
    });

    connect(this,&PieAssociateTable::seriesColorChanged,this,&PieAssociateTable::onSeriesColorChanged);
}

QList<QPieSlice*> PieAssociateTable::createSlices()
{// 只用于行列模式
        int row,col;
        QVector<QVariant> data;
        QStringList header;

        if (mMode->isRowMode()) {
            row = mMode->associateRow();
            if (!mTableModel->isRowDataValid(row)) return QList<QPieSlice*>() ;
            data = mTableModel->rowData(row);
            header = mTableModel->horizontalHeaderLabels();
            mAssociateMode = AssociateMode::RowMode;
            mFlagValue = row;
            emit modeChanged(AssociateMode::RowMode);
        }
        else {
            col = mMode->associateCol();
            if (!mTableModel->isColumnDataValid(col)) return QList<QPieSlice*>() ;
            data = mTableModel->colData(col);
            header = mTableModel->verticalHeaderLabels();
            mAssociateMode = AssociateMode::ColMode;
            mFlagValue = col;
            emit modeChanged(AssociateMode::ColMode);
        }

        auto sum = mTableModel->dataSummary(data);
        QList<QPieSlice*> slices;
        for(int x = 0; x <data.count(); ++x)
        {
            QPieSlice * slice = new QPieSlice;
            if (data[x].isValid()) slice->setValue(data[x].toDouble());
            slice->setLabel(QString("%1(%2%)").arg(header[x]).
                            arg(data[x].toDouble() /  sum * 100.0,0,'g',3));
            slices.append(slice);
        }

        return slices;
}

 void PieAssociateTable::setSeriesProperties(QPieSeries*series)
{// 4种模式共用
        series->setLabelsVisible(true);
        series->setLabelsPosition(QPieSlice::LabelPosition(mSeries->labelPosition()));
        series->setHoleSize(mSeries->holeSize());
        series->setPieSize(0.7);
        series->setPieStartAngle(0.);
        series->setPieEndAngle(360.);
        series->setHorizontalPosition(0.5);
        series->setVerticalPosition(0.5);
        series->setName("pie curve");
        mCurrentSeries = series;
}
