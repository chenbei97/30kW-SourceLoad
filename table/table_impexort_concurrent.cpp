#include "table.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void Table::importTableConcurrent(QStringList &content)
{
    QTime t;
    t.start();
    QFuture<void> future =QtConcurrent::run([=]{
        if (content.size() == 0) return;

        QString header = content.at(0);
        QStringList headerList;
        bool isCsv = header.contains(',');
        if (isCsv) headerList = header.split(',',QString::SkipEmptyParts);
        else headerList = header.split(QRegExp("\\s+"),QString::SkipEmptyParts);

        int row = content.count()-1;
        int col = headerList.count();

        clearConcurrent();
        //mStandardModel->clear(); // 会异常
        //mStandardModel->setRowCount(row);
        //mStandardModel->setColumnCount(col);
        mTableSize = {row,col};
        mStandardModel->setHorizontalHeaderLabels(headerList);

        QStandardItem * item;
         for( int r = 1; r <= row; ++r)
         {
             QString line = content.at(r);
             QStringList lineList;
             if (isCsv) lineList = line.split(',',QString::SkipEmptyParts);
             else lineList = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

             for (int c = 0; c < col; ++c)
             {
                 item = new QStandardItem(lineList.at(c));
                 item->setTextAlignment(Qt::AlignCenter);
                 mStandardModel->setItem(r-1,c,item);
             }
         }
    });
    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
    double costtime = t.elapsed()/1000.0;
    qDebug()<<"text import Concurrent cost time = "<< costtime<<"s";
    if (future.isFinished() ) {
        showTips(tr("成功导入文本!(耗时%1s)").arg(QString::number(costtime)),QSize(900,280));
    }
}

void Table::importTxtConcurrent()
{
    /*
        QFuture<void> future = QtConcurrent::run(&pool,[=]{importTxtOptim();});
        QtConcurrent::run(this,&Table::importTxtOptim);
        ASSERT failure in QWidget: "Widgets must be created in the GUI thread.", file kernel\qwidget.cpp, line 1142
        QObject::~QObject: Timers cannot be stopped from another thread

        QObject::connect: Cannot queue arguments of type 'Qt::Orientation'
        (Make sure 'Qt::Orientation' is registered using qRegisterMetaType().)

        QObject::connect: Cannot queue arguments of type 'QList<QPersistentModelIndex>'
        (Make sure 'QList<QPersistentModelIndex>' is registered using qRegisterMetaType().)

        QObject::connect: Cannot queue arguments of type 'QVector<int>'
        (Make sure 'QVector<int>' is registered using qRegisterMetaType().)

        qRegisterMetaType<QList<QPersistentModelIndex>>("QList<QPersistentModelIndex>'"); // 使用QtConcurrent::run会提示没有注册类型
        qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
        qRegisterMetaType<QVector<int>>("QVector<int>'");
        qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint'");
    */
    //QThreadPool pool; // 可以用也可以不用区别不大
    showTips();
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Txt文件"), debugDir.path(),tr("*.txt"));//这种弹窗的不能用在run内
    if (fileName.isEmpty()) {
        showTips(tr("取消导入Txt文件!"));
        return ;
    }
    // 上方的弹窗类型语句不可以放在run函数内执行
    clearConcurrent();
    QTime t;
    t.start();
    QFuture<void> future =QtConcurrent::run([=]{
//        QTime t;
//        t.start();
        //mStandardModel->clear();
        int row = 0, col = 0;
        QStandardItem * item;

        QFile outFile(fileName);
        outFile.open( QIODevice::ReadOnly );
        QTextStream ts(&outFile);
        ts.setAutoDetectUnicode(true);
        ts.setCodec(QTextCodec::codecForName("UTF-8"));

        QString header = ts.readLine(); // 事先读取1行
        if (header.isEmpty()) return; // 至少应该有1行数据
        QStringList headerList = header.split(QRegExp("\\s+"),QString::SkipEmptyParts);
        col = headerList.count(); // 解析表头就提前拿到了列数
        mStandardModel->setHorizontalHeaderLabels(headerList);//设置表头
        while (!ts.atEnd()) {
            QString line= ts.readLine();
            QStringList lineList =line.split(QRegExp("\\s+"),QString::SkipEmptyParts);
            for (int c = 0; c < col; ++c){
                item = new QStandardItem(lineList.at(c));
                item->setTextAlignment(Qt::AlignCenter);
                mStandardModel->setItem(row,c,item); // 第2行文本是表格的第1行,row此时应该从0开始,row尚未++,所以使用row
            }
            ++row;
        }
        outFile.close();
        mTableSize = {row,col};
         //qDebug()<<"importTxtConcurrent cost time = "<<t.elapsed()/1000.0 <<"s";
         qDebug()  << "now in thread" << QThread::currentThreadId();
    });

    // ① future.waitForFinished();//不要阻塞否则出错
    // ② run之后不能有执行语句(不论是计算时间还是消息对话框),或者导入时鼠标滑动表格的滑动条也会异常退出,似乎会因为主线程的缘故阻塞run的线程 ,
    // ③ 即使run后边不计算时间,同时呢引入了if (future.isFinished() )才出现消息对话框也不行,因为测试显示if (future.isFinished() )没有执行也就是没完成,也就是run的线程还在继续执行没有结束
    // ④解决方法是引入下边的事件刷新语句就可以正常计算时间也可以弹出对话框,处理调用线程的挂起事件的时间最长为毫秒，或者直到没有更多的事件要处理，以较短者为准
    // ⑤ 即使有以上方法也会出现异常,没导入完成就移动滑动条可能会出错,常见报下方2个错误
            // QVector<T>::at: "index out of range", file ..\..\include/QtCore/../../src/corelib/tools/qvector.h, line 423
            // QBitArray: "Size must be greater than or equal to 0.
            // 解决方法是不使用mStandardModel->clear();很可能导致了表格的尺寸出现问题,而是使用clearConcurrent(),也就是会保留上次导入的表格尺寸
            // 那么如果本次的表格尺寸大于上次的,会new item覆盖; 如果本次的表格尺寸小于上次的就不会出现问题
   // ⑥ 再就是main.cpp注释掉qRegisterMetaType<>关于QList<QPersistentModelIndex>,Qt::Orientation,QVector<int>,QAbstractItemModel::LayoutChangeHint
            // 经过测试如果注册,并行计算似乎失去了作用就会变得和不使用并行计算一样,所以要注释掉
            // 这4行代码本是为了解决编译器提示的警告例如"Make sure 'QList<QPersistentModelIndex>' is registered using qRegisterMetaType()"
            // 现在忽略掉这个警告把,至少并行计算是必须的不然会卡死
    // ⑥ 经过大量重复测试,依然可能偶尔出现异常退出的情况找不到原因,所以可以增加一个提示
        // 用户使用导入导出功能时,告知用户2个东西
        // 1. 不要导入大量数据(<20万行),导入<15万行是安全的,20万有较低概率异常退出,>21万概率很高
        // 2. 导入导出未完成时(没有出现完成的提示框)不要对表格进行任何操作,主要是移动表格的滑动条
        // 例如很快的下拉到底这种操作可能会导致失败,因为用户滑动的位置表格可能还尚未插入就会导致异常

    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5); // 此行语句
    if (future.isFinished() ) {
        double costtime = t.elapsed()/1000.0;
        qDebug()<<"importTxtConcurrent cost time = "<< costtime<<"s"; // 1.551s~1.62s
        showTips(tr("成功导入Txt文件!(耗时%1s)").arg(QString::number(costtime)),QSize(900,280));
    }
}

void Table::exportTxtConcurrent()
{
    showTips();
    QDir curdir = QDir::current();
    curdir.cdUp();

    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Txt文件"), curdir.path(),tr("*.txt"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导出Txt文件!"));
        return;
    }

    QTime t;
    t.start();
    QFuture<void> future = QtConcurrent::run([=]{
            QFile file(fileName);
            file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");//导入用的UTF-8,所以导出一定要设置为UTF-8
            stream.setAutoDetectUnicode(true);

            // 首先写入表头,如果没有表头,这里会异常
            QStandardItem * headerItem;
            for (int c = 0; c<mTableSize.col; ++c)
            {
                 headerItem =mStandardModel->horizontalHeaderItem(c);
                 if ( headerItem!= Q_NULLPTR) // 这层判断可以不要,nitModel()保证了一定有表头,默认1,2,3..排列,或者是importTxt导入的表头
                {
                     QString text = headerItem->text()==""?"NULL":headerItem->text();//这层判断也可以不要,表头不会为空文本
                     if (c != mTableSize.col-1)
                         stream<<text<<"\t";
                     else stream<<text<<"\n"; // 最后1列换行
                 }
            }

            // 然后写入表格文本
            QStandardItem * item;
            for (int r = 0; r < mTableSize.row; ++r)
            {
                for (int c = 0; c < mTableSize.col; ++c)
                {
                        item = mStandardModel->item(r,c);
                        QString text = item->text()==""?"NULL":item->text();

                        if (c != mTableSize.col-1)
                            stream<<text<<"\t";
                        else stream<<text<<"\n"; // 最后1列换行
                }
            }
            file.close();
    });
    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
    if (future.isFinished() ) {
        double costtime = t.elapsed()/1000.0;
        qDebug()<<"exportTxtConcurrent cost time = "<<costtime <<"s"; // 0.144 s
        showTips(tr("成功导出Txt文件!(耗时%1s)").arg(QString::number(costtime)),QSize(900,280));
    }
}

void Table::importCsvConcurrent()
{
    showTips();
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Csv文件"), debugDir.path(),tr("*.csv"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导入Csv文件!"));
        return ;
    }
    clearConcurrent();
    QTime t;
    t.start();
    QFuture<void> future = QtConcurrent::run([=]{
        //mStandardModel->clear();
        QStandardItem * item;
        int row = 0, col = 0;

        QFile outFile(fileName);
        outFile.open( QIODevice::ReadOnly );
        QTextStream ts(&outFile);
        ts.setAutoDetectUnicode(true); // 导入设置好gbk,这2行不设置也没事
        ts.setCodec(QTextCodec::codecForName("gbk")); //这里得用gbk,和txt不同

        QString header = ts.readLine();
        if (header.isEmpty()) return; // 至少有1行表头先读出来
        QStringList headerList = header.split(',',QString::SkipEmptyParts);
        col = headerList.count(); // 解析表头就提前拿到了列数
        mStandardModel->setHorizontalHeaderLabels(headerList);//设置表头

        while (!ts.atEnd()) { // 不再是直接ts.readAll()而是优化成使用readLine的同时就导入表格
            QString line= ts.readLine();
            QStringList lineList =line.split(',',QString::SkipEmptyParts);
            for (int c = 0; c < col; ++c)
            {
                item = new QStandardItem(lineList.at(c));
                item->setTextAlignment(Qt::AlignCenter);
                mStandardModel->setItem(row,c,item); // 第2行文本是表格的第1行,row此时应该从0开始,row尚未++,所以使用row
            }
            ++row;
        }
        outFile.close();
        mTableSize = {row,col}; // 要更新
    });
    //ASSERT failure in QBitArray::QBitArray: "Size must be greater than or equal to 0.", file tools\qbitarray.cpp, line 150
    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 0);
    if (future.isFinished() ) {
        double costtime = t.elapsed()/1000.0;
        qDebug()<<"importCsvConcurrent cost time = "<<costtime <<"s"; // 1.554s
        showTips(tr("成功导入Csv文件!(耗时%1s)").arg(QString::number(costtime)),QSize(900,280));
    }
}

void Table::exportCsvConcurrent()
{
        showTips();
        QDir debugDir= QDir::current();
        debugDir.cdUp();
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("Csv文件"), debugDir.path(),tr("*.csv"));
        if (fileName.isEmpty()) {
            showTips(tr("取消导出Csv文件!"));
            return;
        }
        QTime t;
        t.start();
        QFuture<void> future = QtConcurrent::run([=]{
            QFile outFile(fileName);
            outFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
            QTextStream ts(&outFile);
            ts.setCodec("gbk"); // 这里要设置好gbk即可

            // 首先写入表头,如果没有表头,这里会异常
            QStandardItem * headerItem;
            for (int c = 0; c<mTableSize.col; ++c)
            {
                headerItem =mStandardModel->horizontalHeaderItem(c);
                 if ( headerItem!= Q_NULLPTR) // 这层判断可以不要,nitModel()保证了一定有表头,默认1,2,3..排列,或者是importTxt导入的表头
                {
                     QString text = headerItem->text()==""?"NULL":headerItem->text();//这层判断也可以不要,表头不会为空文本
                     if (c != mTableSize.col-1)
                         ts << text << ","; // 文本用\t,CSV用,
                     else ts << text+ "\n";// 最后1列换行
                 } // 这里优化成一遍循环就写入了ts流
            }
            QStandardItem * item;
            for (int r = 0; r < mTableSize.row; ++r)
            {
                for (int c = 0; c < mTableSize.col; ++c)
                {
                        item = mStandardModel->item(r,c);
                        QString text = item->text()==""?"NULL":item->text();

                        if (c != mTableSize.col-1)
                            ts << text << ",";
                        else ts << text << "\n"; // 最后1列换行
                }
            }
            outFile.close();
        });
        while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
        if (future.isFinished() ) {
            double costtime = t.elapsed()/1000.0;
            qDebug()<<"exportCsvConcurrent cost time = "<<costtime <<"s"; //0.148 s
            showTips(tr("成功导出Csv文件!(耗时%1s)").arg(QString::number(costtime)),QSize(900,280));
        }
}
