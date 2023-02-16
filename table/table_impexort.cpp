#include "table.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void Table::importTxt()
{
        //QDir curdir = QDir::root(); //用户的C盘目录，current().cdUp()
        QDir curdir = QDir::current();
        curdir.cdUp();
        QString filepath = QFileDialog::getOpenFileName(Q_NULLPTR,tr("Txt文件"),
                                                        curdir.path(),"*.txt");
        if (filepath.isEmpty()) {
            showTips(tr("取消导入Csv文件!"));
            return;
        }
        QTime t;
        t.start();
        QStringList fileContent;
        QFile file(filepath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream.setAutoDetectUnicode(true);//可以不加此行代码
        stream.setCodec("UTF-8"); // UTF-8可以解析中文,GBK/GB2312/GB18030不行,因为txt按utf-8编码的

//        while (!stream.atEnd()) // 不使用这种,减少循环计算
//        {
//            QString line = stream.readLine();// 1行1行读取
//            fileContent.append(line); // 1行1行追加,之后按照行解析这个大字符串即可
//        }
        QString text = stream.readAll();
        fileContent = text.split("\n",QString::SkipEmptyParts);

        file.close();
        importTable(fileContent); // 完成实际导入表格的工作
        double costtime = t.elapsed()/1000.0;
        qDebug()<<"importTxt cost time = "<<costtime <<"s"; // 114.897s
        Infomation * info = new Infomation;
        info->setDialogFixedSize(900,280);
        info->setText(tr("成功导入Csv文件!(耗时%1s)").arg(QString::number(costtime)));
        info->setTextWrapIndent(2,18);
        info->exec();
        delete info;
}

void Table::importTxtOptim()
{ // 把importTxt中importTable内部的代码和上边的代码结合并去除不必要的部分得到优化的代码,速度略有提升但是还是卡
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("文本文件"), debugDir.path(),tr("*.txt"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导入Txt文件!"));
        return ;
    }
    QTime t;
    t.start();
    mStandardModel->clear();
    QStandardItem * item;
    int row = 0, col = 0;

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

    // int headerIdx = 0 ;
    while (!ts.atEnd()) { // 不再是直接ts.readAll()而是优化成使用readLine的同时就导入表格

        // 不推荐使用这种方式,会多很多的判断,事先把第1行读出来就没有特殊情况了
//        if (headerIdx == 0){ // 第1行是表头,取出可以计算文本列数
//            QString header = ts.readLine();
//            QStringList headerList = header.split(QRegExp("\\s+"),QString::SkipEmptyParts);
//            col = headerList.count(); // 解析表头就提前拿到了列数
//            mStandardModel->setHorizontalHeaderLabels(headerList);//设置表头
//            ++headerIdx; // 后边不会再执行了
//        }

        // 其余是文本
        QString line= ts.readLine();
        QStringList lineList =line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

        for (int c = 0; c < col; ++c)
        {
            item = new QStandardItem(lineList.at(c));
            item->setTextAlignment(Qt::AlignCenter);
            mStandardModel->setItem(row,c,item); // 第2行文本是表格的第1行,row此时应该从0开始,row尚未++,所以使用row
        }
        ++row;
    }
    outFile.close();
    //qDebug()<<"row = "<<row<<" col = "<<col;
    mTableSize = {row,col}; // 要更新
    qDebug()<<"importTxtOptim cost time = "<<t.elapsed()/1000.0 <<"s"; // 58.122 s
    showTips(tr("成功导入Txt文件!"));
}

void Table::importCsv()
{
     // 其实和importCsvExternal完全一样,只是把mCsvExport的程序移植在这里
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("CSV文件"), debugDir.path(),tr("*.csv"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导入CSV文件!"));
        return ;
    }
    QTime t;
    t.start();
    QFile outFile(fileName);
    outFile.open( QIODevice::ReadOnly );
    QTextStream ts(&outFile);
    ts.setAutoDetectUnicode(true); // 导入设置好gbk,这2行不设置也没事
    ts.setCodec(QTextCodec::codecForName("gbk")); //这里得用gbk,和txt不同
    QString text = ts.readAll();
    outFile.close();
    QStringList content = text.split("\r\n",QString::SkipEmptyParts);//csv读取到的文本每行可能以\r\n作为结束符
    importTable(content);
    qDebug()<<"importCsv cost time = "<<t.elapsed()/1000.0 <<"s"; // 114.653 s
    showTips(tr("成功导入CSV文件!"));
}

void Table::importCsvOptim()
{// 把importCsv中importTable内部的代码和上边的代码结合并去除不必要的部分得到优化的代码,速度略有提升但是还是卡
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("CSV文件"), debugDir.path(),tr("*.csv"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导入CSV文件!"));
        return ;
    }
    QTime t;
    t.start();
    mStandardModel->clear();
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
    qDebug()<<"importCsvOptim cost time = "<<t.elapsed()/1000.0 <<"s";// 57.752 s
    showTips(tr("成功导入CSV文件!"));
}

//void Table::importCsvExternel()
//{
//    QTime t;
//    t.start();
//    QString text = mCsvOpera.importCsv();// qDebug()<<text;  csv读取到的文本每行以\r\n作为结束符
//    QStringList content = text.split("\r\n",QString::SkipEmptyParts);//content 可能是空
//    importTable(content);
//    qDebug()<<"importCsvExternel cost time = "<<t.elapsed()/1000.0 <<"s"; // 没测试但是本质上应该和没优化的importCsv一个水平也就是114.653 s 实测118s
//}

void Table::importTable(QStringList &content)
{// 导入importTxt和importCsv共用的实际完成函数
     if (content.size() == 0) return; // 这里importCsv返回的可能是空字符串
     int row = content.count()-1; // 表格文本行数(去掉表头)
     QString header = content.at(0); // 解析表头,可以拿到列数
     QStringList headerList;

     bool isCsv = header.contains(',');
     if (isCsv) headerList = header.split(',',QString::SkipEmptyParts); // 导入csv用逗号分隔
     else headerList = header.split(QRegExp("\\s+"), // 导入txt是空格分开
                              QString::SkipEmptyParts);//处理空字符串时如果分割出空字符串舍弃(最后1项)
     int col = headerList.count();

     mStandardModel->clear(); //这里不要用reset函数,内部的代码判断冗余了
     mStandardModel->setRowCount(row);
     mStandardModel->setColumnCount(col);
     mTableSize = {row,col};
     mStandardModel->setHorizontalHeaderLabels(headerList);//设置表头
    /*
        \f   匹配一个换页符  \n  匹配一个换行符  \r  匹配一个回车符  \t  匹配一个制表符 \v 匹配一个垂直制表符
        以上都属于空白字符,\s表示匹配任何空白字符,\S则是匹配任何非空白字符
        \s+表示匹配上述任何一种空白字符,另外正则表达式前需要转义字符\,所以合并就是\\s+
    */
    QStandardItem * item;
     for( int r = 1; r <= row; ++r)
     {
         QString line = content.at(r); // 第r[1,20]行,因为content有21行
         QStringList lineList;
         if (isCsv) lineList = line.split(',',QString::SkipEmptyParts);
         else lineList = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

         for (int c = 0; c < col; ++c)
         {
             item = new QStandardItem(lineList.at(c)); // 指定内容创建单元格
             item->setTextAlignment(Qt::AlignCenter);
             mStandardModel->setItem(r-1,c,item); // 注意表格文本是第0行开始
         }
     }
}

void Table::exportTxt()
{
    QDir curdir = QDir::current();
    curdir.cdUp();

    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("文本文件"), curdir.path(),tr("*.txt"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导出Txt文件!"));
        return;
    }
    QTime t;
    t.start();
    QFile file(fileName);
    file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");//导入用的UTF-8,所以导出一定要设置为UTF-8
    //stream.setAutoDetectUnicode(true);

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

    qDebug()<<"exportTxt cost time = "<<t.elapsed()/1000.0 <<"s"; // 0.142s
    showTips(tr("成功导出Txt文件!"));
}

void Table::exportCsv()
{
    QDir debugDir= QDir::current();
    debugDir.cdUp();
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("CSV文件"), debugDir.path(),tr("*.csv"));
    if (fileName.isEmpty()) {
        showTips(tr("取消导出CSV文件!"));
        return;
    }
    QTime t;
    t.start();
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

    // 然后写入表格文本(这里能不能优化,不用双for来遍历？)
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
    qDebug()<<"exportCsv cost time = "<<t.elapsed()/1000.0 <<"s";//0.147s
    showTips(tr("成功导出CSV文件!"));
}

//void Table::exportCsvExternel()
//{
//    QTime t;
//    t.start();
//    QString header, content;

//    // 首先写入表头,如果没有表头,这里会异常
//    QStandardItem * headerItem;
//    for (int c = 0; c<mTableSize.col; ++c)
//    {
//        headerItem =mStandardModel->horizontalHeaderItem(c);
//         if ( headerItem!= Q_NULLPTR) // 这层判断可以不要,nitModel()保证了一定有表头,默认1,2,3..排列,或者是importTxt导入的表头
//        {
//             QString text = headerItem->text()==""?"NULL":headerItem->text();//这层判断也可以不要,表头不会为空文本
//             if (c != mTableSize.col-1)
//                 header = header+ text + ","; // 文本用\t,CSV用,
//             else header  = header+ text+ "\n";// 最后1列换行
//         } // 这里优化成一遍循环就写入了ts流
//    }

//    // 然后写入表格文本(这里能不能优化,不用双for来遍历？)
//    QStandardItem * item;
//    for (int r = 0; r < mTableSize.row; ++r)
//    {
//        for (int c = 0; c < mTableSize.col; ++c)
//        {
//                item = mStandardModel->item(r,c);
//                QString text = item->text()==""?"NULL":item->text();

//                if (c != mTableSize.col-1)
//                    content  = content +  text + ",";
//                else content  = content + text + "\n"; // 最后1列换行
//        }
//    }
//    // 这里循环一遍计算了1次,进入mCsvOpera又循环1次计算所以慢
//    mCsvOpera.writeTable(header,content);
//    qDebug()<<"exportCsvExternel cost time = "<<t.elapsed()/1000.0 <<"s"; //
//}

