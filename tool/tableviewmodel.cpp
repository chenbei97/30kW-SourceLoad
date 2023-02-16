#include "tableviewmodel.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

TableViewModel::TableViewModel(QObject *parent):QAbstractTableModel(parent)
{ // 参加编程手册 https://doc.qt.io/qt-5/model-view-programming.html
    // https://doc.qt.io/qt-5/qabstractitemmodel.html
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    mColumnCount = 6;
    mRowCount = 20;
    for(int i = 1; i<= mColumnCount;++i) mHorizontalHeader.append(QString::number(i));
    for(int i = 1; i<=mRowCount;++i) mVerticalHeader.append(QString::number(i));

    for (int i = 0; i < mRowCount; i++) {
        QVector<QVariant> * dataVec = new QVector<QVariant>(mColumnCount); // 6个值的行向量
        for (int k = 0; k < dataVec->size(); k++) {
            if (k % 2 == 0) dataVec->replace(k, i * 50 + qrand() % 20 + 1); // 偶数列
            else dataVec->replace(k, qrand() % 5000);  // 幅值5000
        }
        mData.append(dataVec);
    }
    setDefaultMapping();

    connect(this,SIGNAL(columnsInserted(const QModelIndex&, int, int)),
            this,SLOT(onColumnsInserted(const QModelIndex&, int, int)));
    connect(this,SIGNAL(columnsRemoved(const QModelIndex&, int, int)),
            this,SLOT(onColumnsRemoved(const QModelIndex&, int, int)));
    connect(this,SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this,SLOT(onRowsInserted(const QModelIndex&, int, int)));
    connect(this,SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
            this,SLOT(onRowsRemoved(const QModelIndex&, int, int)));
    connect(this,&TableViewModel::modelReset,this,&TableViewModel::onModelReset);
    connect(this,SIGNAL(headerDataChanged(Qt::Orientation, int, int)),
            this,SLOT(onHeaderDataChanged(Qt::Orientation, int, int)));
    connect(this,SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&,const QVector<int>&)),
            this,SLOT(onDataChanged(const QModelIndex&,const QModelIndex&,const QVector<int>&)));
}

int TableViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mData.count();
}

int TableViewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return mColumnCount;
}

QVariant TableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal && !mHorizontalHeader.isEmpty())
    {
        //qDebug()<<"col section = "<<section;
        if (section<mHorizontalHeader.count()) // 可能出现越界
            return mHorizontalHeader[section];
        else return QString("%1").arg(section + 1);
    }
    else if (orientation == Qt::Vertical && !mVerticalHeader.isEmpty())
    {
        //qDebug()<<"row section = "<<section<<"  vh count = "<<mVerticalHeader.count();
        if (section < mVerticalHeader.count())
            return mVerticalHeader[section];
        else return QString("%1").arg(section + 1);
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

QVariant TableViewModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        //qDebug()<<QString("(%1,%2)").arg(index.row()).arg(index.column());
        //qDebug()<<QString("(rows = %1,cols = %2)").arg(mData.count()).arg(mData.at(index.row())->count());
        return mData[index.row()]->at(index.column());

    }
    else if (role == Qt::BackgroundRole)
    {
        for (int id : mCellMapping.keys())
        {
              int rectId = index.column()+1 + (index.row() + 1) * 10;
              if (rectId == id) // 根据索引也就是单元格左上角得到这个单元格的标记
                  return mCellMapping.value(id); // 返回单元格对应的颜色
        }
        return QColor(Qt::white);
    }
    return QVariant();
}

bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        mData[index.row()]->replace(index.column(), value);
        emit dataChanged(index, index); // 数据变化就会自动调整
        return true;
    }
    return false;
}

void TableViewModel::clear()
{
    beginResetModel();
    mData.clear();
    mRowCount = 0;
    mColumnCount = 0;
    mVerticalHeader.clear();
    mHorizontalHeader.clear();
}

void TableViewModel::update()
{ // 外部appendRow结束后必须调用
    mRowCount = mData.count();
    if (mRowCount) mColumnCount = mData[0]->count(); // 1行没有,就还是0不变
    //qDebug()<<"update => row = "<<mRowCount<<"  col = "<<mColumnCount;

    if (mHorizontalHeader.isEmpty()) // clear后是empty,如果setHorizontalHeaderLabels调用过就不会为空,直接使用提供的列名即可
        for(int i = 1; i<= mColumnCount;++i) mHorizontalHeader.append(QString::number(i));//否则使用数字

    for(int i = 1; i<=mRowCount;++i) mVerticalHeader.append(QString::number(i));

    setDefaultMapping();
    endResetModel();
}

void TableViewModel::setHorizontalHeaderLabels(const QStringList &labels)
{
    mColumnCount = labels.count();
    mHorizontalHeader.clear();
    mHorizontalHeader = labels;
}

QStringList TableViewModel::horizontalHeaderLabels() const
{
    return mHorizontalHeader;
}

QString TableViewModel::horizontalHeaderLabel(int col) const
{
    return mHorizontalHeader[col];
}

void TableViewModel::setVerticalHeaderLabels(const QStringList &labels)
{
    mRowCount = labels.count();
    mVerticalHeader.clear();
    mVerticalHeader = labels;
}

QStringList TableViewModel::verticalHeaderLabels() const
{
    return mVerticalHeader;
}

QString TableViewModel::verticalHeaderLabel(int row) const
{
    return mVerticalHeader[row];
}

void TableViewModel::appendRow()
{// 添加空行
    insertRow(mData.count()-1);
}

void TableViewModel::appendRow(QVector<QVariant>*data)
{
     beginInsertRows(QModelIndex(),mData.count(),mData.count()+1);
     mData.append(data);
     endInsertRows();
     ++mRowCount;
}

void TableViewModel::appendRow(const QStringList &data)
{
    beginInsertRows(index(mData.count(),data.count()),mData.count(),mData.count()+1);
    QVector<QVariant> *v = new QVector<QVariant>;
    foreach (const QString & d, data) v->append(d);
    mData.append(v);
    endInsertRows();
    ++mRowCount;
}

/*
mTableModel->insertRow(152); // 插入151行应该在位置150插入,152不报错会自动append
mTableModel->appendRow(); // 快捷方法
mTableModel->insertColumn(6); // 插入第6列应在位置5插入,6不报错会自动append
mTableModel->appendColumn();
mTableModel->appendColumn();
mTableModel->removeRow(153);// 越界不报错,但是移除不了
mTableModel->removeRow(150); // 移除第151行
mTableModel->removeColumn(8); // 越界不报错,但是移除不了
mTableModel->removeColumn(5); // 移除第6列
*/

bool TableViewModel::insertRows(int position, int rows, const QModelIndex &parent)
{ // insertRow调用本函数 insertRows(arow, 1, aparent);
    beginInsertRows(parent, position, position+rows-1);
    auto oldrow = mData.count();
    for (int row = 0; row < rows; ++row) {
        mData.insert(position,new QVector<QVariant>(mColumnCount)); // 必须插入大小为列数的向量否则越界
    }
    auto newrow = mData.count();
    endInsertRows();
    for(int i = 0; i < newrow-oldrow;++i) ++mRowCount; // 也有可能插入的位置不对,实际没有插入
    return true;
}

bool TableViewModel::removeRows(int position, int rows, const QModelIndex &parent)
{ // removeRow调用本函数
    beginRemoveRows(parent, position, position+rows-1);
    auto oldrow = mData.count();
    for (int row = 0; row < rows; ++row) {
        mData.removeAt(position);
    }
    auto newrow = mData.count(); //可能移除的行索引不对实际没有移除
    for(int i = 0; i< oldrow-newrow; ++i) --mRowCount;
    endRemoveRows();
    return true;
}

bool TableViewModel::insertColumns(int column, int count, const QModelIndex &parent)
{// 同理 insertColumn, insertColumns(acolumn, 1, aparent);
    beginInsertColumns(parent,column,column+count-1);

    foreach(auto vector, mData){
        for (int col = 0; col < count; ++col) { //column=6,vector.size=5
            if (column>vector->count()) {
                vector->append(QVariant());
            }
            else vector->insert(column,QVariant());//insert(6)会导致越界
        }
    }
    endInsertColumns();
    for (int col = 0; col < count; ++col) ++mColumnCount;
    return true;
}

void TableViewModel::appendColumn()
{
    if (mColumnCount == 0) insertColumn(0);
    else insertColumn(mColumnCount);
}

bool TableViewModel::removeColumns(int column, int count, const QModelIndex &parent)
{// 同理
    beginRemoveColumns(parent,column,column+count-1);
    bool isRemoved =true;
    foreach(auto vector, mData)
    {
        for (int col = 0; col < count; ++col) {
            //qDebug()<<column<<vector->count();
            if (column < vector->count()) // column=6,vector.size=6,即vector最大[5],不包括等号
               vector->removeAt(column); //不越界时移除该列
            else { // 越界忽略
                isRemoved = false;
                break;
            }
        }
    }
    if (isRemoved) for (int col = 0; col < count; ++col) --mColumnCount;
    endRemoveColumns();
    return true;
}

QVector<QVariant> TableViewModel::rowData(int row) const
{ // 整行数据
    return *mData[row];
}

QVector<QVariant> TableViewModel::rowData(int row, int firstCol, int colCount) const
{ // 该行指定长度范围的数据
    // row外部传入可以保证不越界, firstCol+colCount可能越界
    // 例如只有6列,firstCol=5,colCount=6,就会导致c可以取到10,导致越界
    int lastCol = firstCol + colCount - 1;
    if (lastCol>mColumnCount - 1) lastCol = mColumnCount - 1; // lastCol最多取到列数-1

    QVector<QVariant> d;
    for(int c = firstCol; c <= lastCol; ++c)
        d.append(rowData(row)[c]);
    return d;
}

QVector<QVariant> TableViewModel::colData(int col) const
{
    QVector<QVariant> d;
    for(int r = 0 ; r < mData.count(); ++r)
        d.append(mData.at(r)->at(col));
    return d;
}

QVector<QVariant> TableViewModel::colData(int col, int firstRow, int rowCount) const
{
    int lastRow = firstRow + rowCount -1;
    if (lastRow>mRowCount-1) lastRow = mRowCount -1; // 同理最多取到行数-1
    QVector<QVariant> d;
    for(int r = firstRow ; r <=lastRow; ++r)
    {
        d.append(mData.at(r)->at(col)); // col外部可以保证不越界
    }
    return d;
}

qreal TableViewModel::dataSummary(const QVector<QVariant> &data) const
{
    // 验证数据是否有效,如果和为0,直方图绘制该系列会导致
        // "ASSERT: "width > 0.0" in file painting\qrasterizer.cpp, line 761" 异常
        qreal sum = 0.0;
        for(auto d: data) sum +=d.toDouble();
        return  sum;
}

bool TableViewModel::isRowDataValid(int row) const
{
    return  dataSummary(rowData(row)) > 0.0;
}

bool TableViewModel::isRowDataValid(int row, int firstCol, int colCount) const
{
    return dataSummary(rowData(row,firstCol,colCount));
}

bool TableViewModel::isColumnDataValid(int col) const
{
    return dataSummary(colData(col)) > 0.0;
}

bool TableViewModel::isColumnDataValid(int col, int firstRow, int rowCount) const
{
    return dataSummary(colData(col,firstRow,rowCount));
}

bool TableViewModel::isRowRegionDataValid(int firstRow, int lastRow, int firstCol, int colCount) const
{
    qreal sum = 0.0;
    for(int r = firstRow; r <= lastRow; ++r)
        sum += dataSummary(rowData(r,firstCol,colCount));
    return sum>0.0;
}

bool TableViewModel::isColRegionDataValid(int firstCol, int lastCol, int firstRow, int rowCount) const
{
    qreal sum = 0.0;
    for(int c = firstCol; c <= lastCol; ++c)
        sum += dataSummary(colData(c,firstRow,rowCount));
    return sum>0.0;
}

bool TableViewModel::isAllDataValid() const
{
    double sum = 0.0;
    for(int r = 0; r < mData.count(); ++r)
        sum += dataSummary(rowData(r));
    return sum > 0.0;
}

Qt::ItemFlags TableViewModel::flags(const QModelIndex &index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

void TableViewModel::addCellMapping(QRect rect,QColor color)
{
    Q_ASSERT(rect.width() == rect.height() == 1); // 限制区域必须是单个单元格
    int rows = rect.y()+1, cols = rect.x()+1;// x()是列数-1,y()是行数-1
    int rectId = rows * 10 + cols;// 第1行第1列其标记11,第3行第4列其标记 34
    // 水平区域标记是11,12,...,1n 以及垂直21,31,...n1,这样每个单元格有唯一键值

    mCellMapping.insert(rectId,color);// 单元格和颜色一一对应,覆盖插入
}

QColor TableViewModel::cellColor(int id) const
{
    Q_ASSERT(id>=11);
    return mCellMapping[id];
}

QColor TableViewModel::cellColor(int row, int col) const
{ // row,col是以0为标准的
    auto id = (row+1) * 10 + col+1;
    Q_ASSERT(id>=11);
    return mCellMapping[id];
}

void TableViewModel::addColMapping(int col ,QColor color)
{ // 对整列单元格进行映射,col也是从0计算的
    for(int row = 0;row<mRowCount; ++ row)
    { // col是水平方向,表示x坐标, y坐标遍历行即可
        addCellMapping(QRect(col,row,1,1),color);
    }
}

void TableViewModel::addColMapping(int col, int firstRow, int rowCount, QColor color)
{ // 对列的指定范围进行映射,例如起始2,共3行的区域,即2,3,4, 所以不取等号2+3=5(用于柱状图)
    auto lastRow = firstRow+rowCount - 1; // 0基准的实际值
    if (lastRow > mRowCount-1) lastRow = mRowCount - 1; // 最后1行不能超过最大行数-1
    for(int row = firstRow; row <= lastRow; ++row) // 0基准取等号
    {
        addCellMapping(QRect(col,row,1,1),color);
    }
}

void TableViewModel::addColMapping(int col, int firstRow, int rowCount,QList<QColor> colorlist)
{ // 对列的指定范围进行映射,且每个单元格使用不同的颜色(用于饼图)
    auto lastRow = firstRow+rowCount - 1; // 0基准的实际值
    if (lastRow > mRowCount-1) lastRow = mRowCount - 1; // 最后1行不能超过最大行数-1
    for(int row = firstRow; row <= lastRow; ++row) // 0基准取等号
    {
        addCellMapping(QRect(col,row,1,1),colorlist[row-firstRow]); // 注意颜色不能越界,从0开始的
    }
}

void TableViewModel::addRowMapping(int row, QColor color)
{// 对整行单元格进行映射,row从0计算
    for(int col = 0; col < mColumnCount; ++col)
    {// row是垂直方向,表示y坐标, x坐标遍历列即可
        addCellMapping(QRect(col,row,1,1),color);
    }
}

void TableViewModel::addRowMapping(int row, int firstCol, int colCount, QColor color)
{// 对行的指定范围映射相同颜色
    auto lastCol = firstCol+colCount - 1; // 0基准的实际值
    if (lastCol > mColumnCount-1) lastCol = mColumnCount - 1; // 最后1列不能超过最大列数-1
    for(int col = firstCol; col <= lastCol; ++col)// 0基准取等号
    {
        addCellMapping(QRect(col,row,1,1),color);
    }
}

void TableViewModel::addRowMapping(int row, int firstCol, int colCount, QList<QColor> colorlist)
{// 对行的指定范围映射不同颜色
    auto lastCol = firstCol+colCount - 1; // 0基准的实际值
    if (lastCol > mColumnCount-1) lastCol = mColumnCount - 1; // 最后1列不能超过最大列数-1
    for(int col = firstCol; col <= lastCol; ++col)// 0基准取等号
    {
        addCellMapping(QRect(col,row,1,1),colorlist[col-firstCol]);
    }
}

void TableViewModel::addDoubleColMapping(QXYSeries* series,int xCol, int yCol)
{// 对指定的2列进行映射
   QVXYModelMapper *mapper = new QVXYModelMapper;
   mapper->setXColumn(xCol);
   mapper->setYColumn(yCol);
   mapper->setSeries(series);
   mapper->setModel(this);
   addColMapping(xCol,series->color());
   addColMapping(yCol,series->color());
}

void TableViewModel::addColRegionMapping(int firstColumn, int lastColumn, int firstRow, int rowCount,QList<QColor> colorlist)
{// 对指定的连续列区域进行映射
    for(int col = firstColumn; col <= lastColumn; ++col) // 每列都映射,firstColumn和lastColumn基准是0,要带等号
    {
        addColMapping(col,firstRow,rowCount,colorlist[col-firstColumn]); // 对该列的指定段映射
    }
}

void TableViewModel::addRowRegionMapping(int firstRow, int lastRow, int firstColumn, int columnCount,QList<QColor> colorlist)
{// 对指定的连续行区域进行映射
    for(int row = firstRow; row <= lastRow; ++row) // 每行都映射,firstRow,lastRow基准是0带等号
    { // 首行可能改变,例如第4行开始连续3行,对应4,5,6; 此时colorlist获取的颜色个数是3,使用colorlist[row]会导致越界
        // 0,1,2不越界,所以对应关系是row-firstRow
        addRowMapping(row,firstColumn,columnCount,colorlist[row-firstRow]); // 对该行的指定段映射
    }
}

void TableViewModel::setDefaultMapping()
{
    clearMapping();
    for(int row=1; row <=mRowCount; ++row)
    {
        for(int col=1;col<=mColumnCount;++col)
        {
            mCellMapping.insert(row*10+col,Qt::white);//需要提供个默认映射值
        }
    }
    //qDebug()<<mCellMapping.count();
    Q_ASSERT(mCellMapping.count()== mRowCount * mColumnCount);
}

void TableViewModel::clearMapping()
{
    mCellMapping.clear();
}

void TableViewModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    //qDebug()<<"insert row in loc "<<first+1<<" to "<<last+1;
}

void TableViewModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    //qDebug()<<"remove row in loc "<<first+1<<" to "<<last+1;
}

void TableViewModel::onColumnsInserted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    //qDebug()<<"insert column in loc "<<first+1<<" to "<<last+1;
}

void TableViewModel::onColumnsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent);
    Q_UNUSED(first);
    Q_UNUSED(last);
    //qDebug()<<"remove column in loc "<<first+1<<" to "<<last+1;
}

void TableViewModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                   const QVector<int> &roles)
{
    Q_UNUSED(roles);
    Q_UNUSED(topLeft);
    Q_UNUSED(bottomRight);
    //auto tl = QString("(%1,%2)").arg(topLeft.row()+1).arg(topLeft.column()+1);
    //auto br = QString("(%1,%2)").arg(bottomRight.row()+1).arg(bottomRight.column()+1);
    //qDebug()<<"from "<<tl<<"  to "<<br<<"  data is changed";
}

void TableViewModel::onModelReset()
{
    qDebug()<<"model is ready to reset";
}

void TableViewModel::onHeaderDataChanged(Qt::Orientation orientation, int first, int last)
{
    //qDebug()<<"orientation = "<<orientation << "  from header"<<first+1<<" to header"<<last+1;
    Q_UNUSED(orientation);
    Q_UNUSED(first);
    Q_UNUSED(last);
}




















