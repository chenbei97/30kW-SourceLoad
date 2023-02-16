#include "csvexport.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

CSVExport::CSVExport()
{
        QDir debugDir= QDir::current();
        debugDir.cdUp();
        mWorkDir = debugDir.path();
        //qDebug()<<"mWorkdir = "<<mWorkDir;

        mrows = mcols = 0;
}

QStringList CSVExport::tableHeader() const
{
    QStringList ret = mTableHeader.split(',');
    ret.removeLast(); // 去掉换行符
    return  ret;
}

QStringList CSVExport::tableContent() const
{
    return mTableContent.split('n');
}

int CSVExport::row() const
{
    return mrows;
}

int CSVExport::col() const
{
    return mcols;
}

void CSVExport::clear()
{
    mrows = 0;
    mcols = 0;
    mTableHeader.clear();
    mTableContent.clear();
}

int CSVExport::error(ErrorCode error)
{
    int ret = 0;
    switch (error) {
            case ErrorCode::Empty:
                    ret = msg(tr("警告"),tr("导出的CSV没有表头和内容,导出可能为空文件,是否继续执行?"));
                    break;
             case ErrorCode::EmptyHeader:
                    ret = msg(tr("警告"),tr("导出的CSV没有表头,是否继续执行?"));
                    break;
            case ErrorCode::EmptyContent:
                    ret = msg(tr("警告"),tr("导出的CSV没有内容,是否继续执行?"));
                    break;
    }
    return ret;
}

int CSVExport::msg(const QString & title, const QString & text,ErrorLevel level)
{
    int ret = 0; // 默认返回0
    QMessageBox::StandardButton result;
    switch (level) {
        case ErrorLevel::Information:
                result = QMessageBox::information(Q_NULLPTR,title,text,QMessageBox::Apply|QMessageBox::Ignore);
                break;
        case ErrorLevel::Warning:
                result = QMessageBox::warning(Q_NULLPTR,title,text,QMessageBox::Apply|QMessageBox::Ignore);
                break;
        case ErrorLevel::Critical:
                result = QMessageBox::critical(Q_NULLPTR,title,text,QMessageBox::Apply|QMessageBox::Ignore);
                break;
        case ErrorLevel::Question:
                result= QMessageBox::question(Q_NULLPTR,title,text,QMessageBox::Apply|QMessageBox::Ignore);
                break;
    }
    if (result == QMessageBox::Apply) ret = 1; // 表示用户仍然无视消息选择了要执行
    return ret;
}


void CSVExport::writeTable(const char* header,const char* content)
{
    clear(); // 先清理
    // 检查格式,内容必须有逗号分割,\n换行符结束
    QString conten(content);
    if (conten.right(1) !="\n")
    {
        QMessageBox::critical(Q_NULLPTR,tr("错误"),tr("内容输入文本必须包含换行符,且用逗号分开各项!"));
        return;
    }

    QString head(header);
    if (head.right(1) !="\n")
    {
          head = head + QStringLiteral("\n"); // 支持表头没有换行结束符
    }
    mTableHeader = head;
    mTableContent = conten;

    // 这里解析出内容的行列数(不包括表头)
    QStringList rowStr = conten.split("\n"); // 首先按照换行符分割
    mrows = rowStr.size()-1; // 得到行数,减去1是因为最后1个字符是\n,分割后最后1项是空字符串
    mcols = rowStr[0].split(",").size(); // 得到列数,至少会有1行可以[0]计算,逗号不是最后1项,所以不会产生空字符串无需-1

    exportCsv();
}

void CSVExport::writeTable(const QString & header,const QString & content)
{
        writeTable(header.toStdString().c_str(),content.toStdString().c_str());
}

void CSVExport::writeTable(const QString & header,TableContent func)
{
    if (func == Q_NULLPTR) {
        QMessageBox::critical(Q_NULLPTR,tr("错误"),tr("表格内容的函数指针不允许为空!"));
         return;
    }
    QString content = func();
    writeTable(header.toStdString().c_str(),content.toStdString().c_str());
}

void CSVExport::writeTable(const QString & text) // 私有函数,给文本流写入数据
{
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("CSV文件"), mWorkDir,tr("*.csv"));
    if (fileName.isEmpty()) {
        QMessageBox::information(Q_NULLPTR,tr("消息"),tr("取消导出CSV文件!"));
        return;
    }
    QFile outFile(fileName);
    outFile.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
    QTextStream ts(&outFile);
    ts.setCodec("gbk"); // 这里要设置好gbk即可
    ts << text;
    //QByteArray bytes = text.toLocal8Bit();
    //ts<< bytes;  //传字节,导出那里解析回字节似乎不行,所以就用上边方法就行
    outFile.close();
    QMessageBox::information(Q_NULLPTR,tr("消息"),tr("成功导出CSV文件!"));
}

void CSVExport::exportCsv()
{
     // error()函数返回0或1,返回0说明对话框选择了ignore,否则说明选择了apply,仍然执行
     if (mTableHeader.isEmpty() && mTableContent.isEmpty())
     {
          if (error(ErrorCode::Empty)) writeTable(""); // 什么也不写入
          return;
     }

     if (mTableHeader.isEmpty()) // 表头是空的,那么内容一定不为空
     {
         if(error(ErrorCode::EmptyHeader)) writeTable(mTableContent); // 把内容写入
         return;
     }

     if (mTableContent.isEmpty()) // 内容是空的,那么表头不为空
     {
         if(error(ErrorCode::EmptyContent)) writeTable(mTableHeader);// 把表头写入
         return;
     }

     // 到这里说明都不为空
     writeTable(mTableHeader+mTableContent);  // 把表头和内容写入
}

QString CSVExport::importCsv()
{
        // 提供函数导入外部的csv,获取文本用于其他用处
        QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("CSV文件"), mWorkDir,tr("*.csv"));
        if (fileName.isEmpty()) {
            QMessageBox::information(Q_NULLPTR,tr("消息"),tr("取消导入CSV文件!"));
            return "";
        }
        QFile outFile(fileName);
        outFile.open( QIODevice::ReadOnly );
        QTextStream ts(&outFile);
        ts.setAutoDetectUnicode(true); // 导入设置好gbk,这2行不设置也没事
        ts.setCodec(QTextCodec::codecForName("gbk")); //这里得用gbk,和txt不同
//        while (!ts.atEnd())
//        {
//            QString line = ts.readLine();
//        }
        QString content = ts.readAll();
        outFile.close();
        //qDebug()<<content; // 如果这里导出的是乱码可能是csv源文件存在问题
        // 考虑将破损的csv文件另存一份csv文件会自动整理好格式,再导入就不会乱码
        return content;
}
