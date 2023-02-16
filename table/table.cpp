#include "table.h"

Table::Table(QTableView* tableview,int row, int col,QObject * parent) : QObject(parent)
{
        if (tableview == Q_NULLPTR) {
            qCritical()<<"QTableView's pointer cant not be nullptr!";
            return;
        }
        setTips();
        mBackColor = Qt::white; // 要初始化一下不然刚开始点击是黑色
        mForeColor = Qt::black;
        mTableView = tableview;
        initModels(row,col); // 先初始化model,最后初始化connect,不要改变顺序
        initConnections();
}

Table::Table(QTableView* tableview,TableSize size,QObject * parent) : QObject(parent)
{
    //Table(tableview,size.row,size.col,parent); // 委托构造可能会异常
    if (tableview == Q_NULLPTR) {
        qCritical()<<"QTableView's pointer cant not be nullptr!";
        return;
    }

    setTips();
    mBackColor = Qt::white;
    mForeColor = Qt::black;
    mTableView = tableview;
    initModels(size.row,size.col);
    initConnections();
}

void Table::initModels(int row,int col)
{
    mStandardModel = new QStandardItemModel(row,col,this);
    mSelectionModel = new QItemSelectionModel(mStandardModel);

    QStringList headerList;
    for (int j = 1; j <= col; ++j) headerList<<QString::number(j); // 不要用QString(j),乱码
    mStandardModel->setHorizontalHeaderLabels(headerList); // 默认表头是datetime,1,2,3..col的名称

    QtConcurrent::run([=]{
        QStandardItem * item; // 创建空白项,这是防止用户导出个空表导致程序异常
        for (int r = 0 ; r < row; ++r)
        {
            for (int c = 0; c < col; ++c)
            {
                item = new QStandardItem;
                item->setTextAlignment(Qt::AlignCenter);
                mStandardModel->setItem(r,c,item);
            }
        }
    });// 并行加速

    mTableView->setModel(mStandardModel);
    mTableView->setSelectionModel(mSelectionModel);//必须先设置选择模型再设置模式和行为
    mTableView->setSelectionMode(QAbstractItemView::ExtendedSelection); // 可以使用ctrl和shift控制选择多个项
    mTableView->setSelectionBehavior(QAbstractItemView::SelectItems); // 点击时选择单个项而不是一列/行
//    mTableView->setSelectionMode(QAbstractItemView::NoSelection);// 不起作用
//    mTableView->clearSelection();
//    mSelectionModel->clearSelection();
    mTableSize = {row,col}; // 这里可能被reset调用,所以要更新mTableSize

}

QStandardItem* Table::takeItem(const QModelIndex & index) const
{
    return mStandardModel->itemFromIndex(index);
}

void Table::initConnections()
{
    // 项选择模型的5个信号: currentChanged,currentColumnChanged,currentRowChanged,modelChanged和selectionChanged
    connect(mSelectionModel,static_cast<void (QItemSelectionModel::*)(const QModelIndex&,const QModelIndex&)>(&QItemSelectionModel::currentChanged),
            this,[=](const QModelIndex& current,const QModelIndex& previous){
            Q_UNUSED(previous);
            if (current.isValid()) //单元格有效的话
            {
                emit cellCurrentIndex(current);
                QStandardItem * item = mStandardModel->itemFromIndex(current);
                QString content = tr("无");
                if (item->text()!="") content = item->text();
                emit cellCurrentContent(content);

                // 存在的问题是,首次进入时单元格总会选中(0,0),这样该单元格就会呈现颜色
                // 而且mTableView->clearSelection()和mSelectionModel->clearSelection()初始化模型时不起作用
                //item->setBackground(QBrush(mBackColor,Qt::SolidPattern));
                //item->setForeground(QBrush(mForeColor));
                // 所以格式的变化改为在mTableView的pressed信号下触发执行
            }
    });

    connect(mTableView,static_cast<void (QTableView::*)(const QModelIndex&)>(&QTableView::pressed),this,
            [=](const QModelIndex&idx){
            QStandardItem * item = mStandardModel->itemFromIndex(idx);
            item->setBackground(QBrush(mBackColor,Qt::SolidPattern));// 这时就不再出现上述问题
            item->setForeground(QBrush(mForeColor));
    });
//    connect(mSelectionModel,static_cast<void (QItemSelectionModel::*)(const QModelIndex&,const QModelIndex&)>(&QItemSelectionModel::currentColumnChanged),
//            this,[=](const QModelIndex& current,const QModelIndex& previous){});
//    connect(mSelectionModel,static_cast<void (QItemSelectionModel::*)(const QModelIndex&,const QModelIndex&)>(&QItemSelectionModel::currentRowChanged),
//            this,[=](const QModelIndex& current,const QModelIndex& previous){});
}

void Table::setTips()
{
    mShowTips = true;
    mTableTips = new Infomation;
    mTableTips->setText(tr("1.不要导入大量数据(>20万行),程序可能异常退出,小于15万行是安全的.\n"
                           "2.导入导出未完成时不要对表格进行操作.例如快速下拉表格滑动条,"
                           "因为您滑动的位置表格尚未插入项导致访问不存在的单元格从而异常退出.\n"
                           "3.您可以事先设置好表格的尺寸再进行导入,可以避免这个问题.\n"
                           "4.导入导出完成时会有提示对话框,然后再进行任何操作."));
    mTableTips->button(0)->setButtonName(tr("我已明白"));
    MessageButton * button = new MessageButton(tr("不再提示"),1); // button id=1
    connect(button,&MessageButton::clicked,this,[=]{
         mTableTips->accept();
         mShowTips = false; // 全局不再提示
    });
    mTableTips->addButton(button);
    mTableTips->setButtonStyle(button,"color:#1afa29;border:1px solid #1afa29;font-size:24px;"
                                      "font-family:Times New Roman;min-width: 120px;min-height: 40px;");//style需要保持一致
    mTableTips->setTextAlignment(Qt::AlignLeft);
    mTableTips->setTextWrapIndent(0,0);
    mTableTips->setDialogFixedSize(1200,350);
}

void Table::showTips()
{
    if (mShowTips == false) return; // 不再提示

    int exec = mTableTips->exec();
    QPoint pos = mTableTips->pos();
    while (exec != QDialog::Accepted)
    {
        mTableTips->shake();
        exec = mTableTips->exec();
        mTableTips->move(pos);
    }
}

void Table::showTips(const QString & tip,const QSize& size,const QPoint& wrapIndent)
{
    Infomation * info = new Infomation;
    info->setDialogFixedSize(size);
    info->setText(tip);
    info->setTextWrapIndent(wrapIndent.x(),wrapIndent.y());
    info->exec();
    delete info;
}

int Table::row() const
{
    return mTableSize.row;
}

int Table::col() const
{
    return mTableSize.col;
}

void Table::appendRow()
{
    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.col; ++i)
    {
        curr = new QStandardItem;
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }
    mStandardModel->appendRow(itemList); // 添加工作完成

    // 下边只是为了选中新添加行的效果
    QModelIndex topLeft = mStandardModel->index(mTableSize.row,0); // 左下角
    QModelIndex bottomRight = mStandardModel->index(mTableSize.row,mTableSize.col-1);// 右下角
    QItemSelection selection(topLeft,bottomRight);

    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);

    ++mTableSize.row; // 记得更新

    //qDebug()<<"mrow = "<<mStandardModel->rowCount()<<"  mcol = "<<mStandardModel->columnCount();
    //qDebug()<<"trow = "<<mTableSize.row<<"  tcol = "<<mTableSize.col;
}

void Table::appendRow(const QStringList & colData)
{
    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.col; ++i)
    {
        curr = new QStandardItem(colData.at(i));
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }
    mStandardModel->appendRow(itemList);
    QModelIndex topLeft = mStandardModel->index(mTableSize.row,0);
    QModelIndex bottomRight = mStandardModel->index(mTableSize.row,mTableSize.col-1);
    QItemSelection selection(topLeft,bottomRight);
    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);
    ++mTableSize.row;
}

void Table::appendCol()
{
    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.row; ++i)
    {
        curr = new QStandardItem;
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }
    mStandardModel->appendColumn(itemList); // 添加工作完成

    // 下边只是为了选中新添加列的效果
    QModelIndex topLeft = mStandardModel->index(0,mTableSize.col); // 右上角
    QModelIndex bottomRight = mStandardModel->index(mTableSize.row-1,mTableSize.col);// 右下角
    QItemSelection selection(topLeft,bottomRight);

    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);

    ++mTableSize.col; // 记得更新
}

void Table::insertRow()
{
    QModelIndex idx = mSelectionModel->currentIndex(); // 当前选中项的索引
    if (idx.row()<0 && idx.column() <0) return;// 没有选中任何项不能插入

    //if (!mSelectionModel->hasSelection()) return; // 不能用这条语句代替
    // 首次进入默认没选中任何项,选择1个项或者2个项或者1行点击插入行后mrow和trow是正常相等的
    // 此时再次点击插入行,表格虽然在上次插入选中了新插入的行,但是下方的idx却返回(-1,-1),好像是假选中
    // 这样mStandardModel->insertRow插入失败,但是++mTableSize.row就导致2个值不相等了

    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.col; ++i)
    {
        curr = new QStandardItem;
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }
    //QModelIndex idx = mSelectionModel->currentIndex();
    //qDebug()<<"idx = "<<idx;
    mStandardModel->insertRow(idx.row(),itemList); // 指定行位置插入工作完成

    // 下边只是为了选中新插入行的效果
    QModelIndex topLeft = mStandardModel->index(idx.row(),0); // 左边
    QModelIndex bottomRight = mStandardModel->index(idx.row(),mTableSize.col-1);// 右边
    QItemSelection selection(topLeft,bottomRight);

    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);

    ++mTableSize.row; // 记得更新
}

void Table::insertRow(const QStringList & colData)
{
    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.col; ++i)
    {
        curr = new QStandardItem(colData.at(i));
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }
    mStandardModel->insertRow(0,itemList); // 指定插入第1行,本版本函数是给logData插入使用的
    QModelIndex topLeft = mStandardModel->index(0,0);
    QModelIndex bottomRight = mStandardModel->index(0,mTableSize.col-1);
    QItemSelection selection(topLeft,bottomRight);
    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);
    ++mTableSize.row;
}

void Table::insertCol()
{
    QModelIndex idx = mSelectionModel->currentIndex(); // 当前选中项的索引
    if (idx.row()<0 && idx.column() <0) return; // 没有选中任何项不能插入

    QList<QStandardItem*> itemList;
    QStandardItem * curr;
    for(int i = 0; i < mTableSize.col; ++i)
    {
        curr = new QStandardItem;
        curr->setTextAlignment(Qt::AlignCenter);
        itemList << curr;
    }

    mStandardModel->insertColumn(idx.column(),itemList); // 指定列位置插入工作完成

    // 下边只是为了选中新插入列的效果
    QModelIndex topLeft = mStandardModel->index(0,idx.column()); // 上边
    QModelIndex bottomRight = mStandardModel->index(mTableSize.row-1,idx.column());// 下边
    QItemSelection selection(topLeft,bottomRight);

    mSelectionModel->clear();
    mSelectionModel->select(selection,QItemSelectionModel::Select);

    ++mTableSize.col; // 记得更新
    QStringList headers;
    for(int i = 1; i <= mTableSize.col; ++i) headers<<QString::number(i);
    setHeader(headers); // 删除/插入列以后表头顺序不对了要手动更新,删除行底层自动更新
}

void Table::removeRow()
{
    QModelIndex idx = mSelectionModel->currentIndex(); // 当前选中项的索引
    if (idx.row()<0 && idx.column() <0) return; // 没有选中任何项不能删除
    mStandardModel->removeRow(idx.row()); // 移除该行
    --mTableSize.row;

    // 下边只是为了删除行后选中替代行的效果
    if  (idx.row() != mTableSize.row - 1) // 如果删除的不是最后1行
    {// 如果删除的是最后1行不会有任何项被选中
        QModelIndex topLeft = mStandardModel->index(idx.row(),0); // 左边
        QModelIndex bottomRight = mStandardModel->index(idx.row(),mTableSize.col-1);// 右边
        QItemSelection selection(topLeft,bottomRight);// 仍然选中相同位置的行
        mSelectionModel->clear();
        mSelectionModel->select(selection,QItemSelectionModel::Select);
    }
}

void Table::removeCol()
{
    QModelIndex idx = mSelectionModel->currentIndex(); // 当前选中项的索引
    if (idx.row()<0 && idx.column() <0) return; // 没有选中任何项不能删除
    mStandardModel->removeColumn(idx.column()); // 移除该列
    --mTableSize.col;
    QStringList headers;
    for(int i = 1; i <= mTableSize.col; ++i) headers<<QString::number(i);
    setHeader(headers);

    // 下边只是为了删除行后选中替代列的效果
    if  (idx.column() != mTableSize.col - 1) // 如果删除的不是最后1列
    {// 如果删除的是最后1列不会有任何项被选中
        QModelIndex topLeft = mStandardModel->index(0,idx.column()); // 上边
        QModelIndex bottomRight = mStandardModel->index(mTableSize.row-1,idx.column());// 下边
        QItemSelection selection(topLeft,bottomRight);// 仍然选中相同位置的列
        mSelectionModel->clear();
        mSelectionModel->select(selection,QItemSelectionModel::Select);
    }
}

void Table::setBold(bool checked)
{
    if (!mSelectionModel->hasSelection()) return; // 没有选中任何项
    QModelIndexList idxs = mSelectionModel->selectedIndexes();
    QModelIndex idx;
    foreach (idx, idxs){
        QStandardItem * item = mStandardModel->itemFromIndex(idx);
        QFont font = item->font();
        font.setBold(checked);
        item->setFont(font);
    }
}

void Table::setItalic(bool checked)
{
    if (!mSelectionModel->hasSelection()) return; // 没有选中任何项
    QModelIndexList idxs = mSelectionModel->selectedIndexes();
    QModelIndex idx;
    foreach (idx, idxs){
        QStandardItem * item = mStandardModel->itemFromIndex(idx);
        QFont font = item->font();
        font.setItalic(checked);
        item->setFont(font);
    }
}

void Table::setAlign(AlignType type)
{
    Qt::Alignment m = Qt::AlignCenter;
    switch (type) {
        case AlignType::Left: m = Qt::AlignLeft | Qt::AlignVCenter; break;
        case AlignType::Right: m = Qt::AlignRight | Qt::AlignVCenter; break;
        default:break;
    }

    if (!mSelectionModel->hasSelection()) return; // 没有选中任何项
    QModelIndexList idxs = mSelectionModel->selectedIndexes();
    QModelIndex idx;
    foreach (idx, idxs){
        QStandardItem * item = mStandardModel->itemFromIndex(idx);
        item->setTextAlignment(m);
    }
}

void Table::clear()
{ // 清空表格文本,但是不清除表格结构
    // 方法1,这个会清除表格结构,所以要重新初始化模型并构建连接
//    mStandardModel->clear();
//    delete mStandardModel;
//    initModels(mTableSize.row,mTableSize.col);
//    initConnections();

    // 方法2,选中的区域才能清除文本
//    QModelIndexList idxs = mSelectionModel->selectedIndexes();
//    if (idxs.empty()) return;
//    QModelIndex idx;
//    QStandardItem * item;
//    foreach(idx ,idxs)
//    {
//        item = mStandardModel->itemFromIndex(idx);
//        if (item->text()!="")
//        {
//            QStandardItem * olditem = mStandardModel->takeItem(idx.row(),idx.column());
//            delete olditem;
//            QStandardItem * newitem = new QStandardItem;
//            mStandardModel->setItem(idx.row(),idx.column(),newitem);
//        }
//    }
    // 方法3,不需要选中区域
    //QTime t;
    //t.start();
    for (int row = 0 ; row < mTableSize.row ; ++row)
    {
            QModelIndex idx;
            QStandardItem * item;
            for (int col = 0; col < mTableSize.col; ++col)
            {
                 idx = mStandardModel->index(row,col); // 使用item/takeItem获得的item访问text会异常
                 item = mStandardModel->itemFromIndex(idx);
                 if (item->text()!="") // 不为空的单元格清除
                 {
                     QStandardItem * olditem = mStandardModel->takeItem(row,col);
                     delete olditem;
                     QStandardItem * newitem = new QStandardItem;
                     newitem->setTextAlignment(Qt::AlignCenter);
                     mStandardModel->setItem(row,col,newitem);
                 }
            }
    }

    // 表头也要清理,变为默认的1,2,3,4..编号
    QStringList newHeader;
    for(int col = 1; col <= mTableSize.col; ++ col) newHeader<<QString::number(col);
    mStandardModel->setHorizontalHeaderLabels(newHeader);//没有啥好方法只能覆盖处理
    //qDebug()<<"clear cost time = "<<t.elapsed()/1000.0 <<"s"; //114.496s
}

void Table::clearConcurrent(bool show)
{
    QTime t;
    t.start();
    //qDebug()<<"clear row = "<<mTableSize.row <<" col = "<<mTableSize.col;
    QFuture<void> future = QtConcurrent::run(this,&Table::clear);
    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
    if (future.isFinished() ) {

        double costtime = t.elapsed()/1000.0;
        qDebug()<<"clearConcurrent cost time = "<<costtime <<"s";
        if (show)
        {
            Infomation * info = new Infomation;
            info->setText(tr("清理表格成功!(耗时%1s)").arg(QString::number(costtime)));
            info->setDialogFixedSize(900,280);
            info->setTextWrapIndent(2,18);
            info->exec();
            delete info;
        }
    }
}

void Table::reset(int row, int col)
{
    if (row < 0 || col <0 ) { // 其实不判断也可以,输入已经限制了不可能<0
        Critical * c= new Critical;
        c->setText(tr("表格行列数不能小于0!"));
        c->setDialogFixedSize(900,280);
        c->setTextWrapIndent(2,16);
        c->exec();
        delete c;
        return;
    }
    bool isEmpty = true;
    for (int r = 0 ; r < mTableSize.row ; ++r)
    {
            for (int c = 0; c < mTableSize.col; ++c)
            {
                 QModelIndex idx = mStandardModel->index(r,c);
                 QStandardItem *item = mStandardModel->itemFromIndex(idx);
                 if (item->text()!="") // 存在不为空的单元格
                 {
                    isEmpty = false;
                    break;
                 }
            }
    }
    if (!isEmpty)
    {
        Messagebox * box = new Messagebox;
        box->setText(tr("表格不为空,重置丢失内容,是否要继续?"));
        box->button(0)->setButtonName(tr("继续"));
        box->button(1)->setButtonName(tr("取消"));
        box->setDialogFixedSize(1000,280);
        box->setTextWrapIndent(2,16);
        int ret = box->exec();
        if (ret == QDialog::Rejected) return; // 不为空,取消重置直接返回
    }

    // 其它情况: 为空或者不为空但是仍然重置
    mStandardModel->clear();
    delete mStandardModel;
    initModels(row,col);
    initConnections();

}

void Table::reset(const TableSize & size)
{
        reset(size.row,size.col);
}

void Table::setColor(QColor fore, QColor back)
{
        mForeColor = fore;
        mBackColor = back;
}

void Table::setHeader(const QStringList & headers)
{
    mStandardModel->setHorizontalHeaderLabels(headers);
}

QStringList Table::header() const
{
        QStringList headers;
        QStandardItem * item;
        for(int col = 0 ; col < mStandardModel->columnCount(); ++col)
        {
            item = mStandardModel->horizontalHeaderItem(col);
            headers << item->text();
        }
        return  headers;
}


void Table::setDelegate(DelegateType type)
{
    Q_UNUSED(type);
}
