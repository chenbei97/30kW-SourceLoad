#include "logprocessor.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LogProcessor::LogProcessor(QObject *parent) : QObject(parent)
{
    mDirManager = new QDir;
    setFilePath();
    QDir::setCurrent(mDirManager->path());

    mLogPath = "";
    mFileFormat = FILEFORMAT::CSV;// 先默认初始化是必要的
    mFileSeprator = ",";
    mFileSuffix = ".csv";

    connect(this,static_cast<void (LogProcessor::*)(FILEFORMAT)>(&LogProcessor::logFileFormat),
            this,[=](FILEFORMAT format){update(format);});
}

void LogProcessor::generateLogFile(const QStringList &text)
{
    if (mLogPath.isEmpty()) return; // 子类有义务更新此变量

    if (!mDirManager->exists(mLogPath)){ // 正常会存在,除非应用启动后创建的文件被人为删除
        qDebug()<<"file not exists!"; // 这时候倒是不影响,不存在的文件会自动被创建,区别是没有表头
        // ... 下方子类有义务先创建好表头,重载此函数实现
    }
    setFileContent(text);
}

QString LogProcessor::logDir() const
{
    return mDirManager->path();//告知外部保存的日志文件夹路径,communication调用
}

void LogProcessor::update(FILEFORMAT format)
{
    updateFileFormat(format);
    updateFileSeptator();
    updateFileSuffix();
}

bool LogProcessor::isCsv() const
{
    return mFileFormat==FILEFORMAT::CSV;
}

void LogProcessor::updateFileFormat(FILEFORMAT format)
{
    mFileFormat = format;
}

void LogProcessor::updateFileSeptator()
{
    mFileSeprator = isCsv()?",":"\t\t";
}

void LogProcessor::updateFileSuffix()
{
    mFileSuffix = isCsv()?".csv":".txt";
}

QString LogProcessor::setFileHeader(const QStringList & header,const QString& filename)
{
    // setFileHeader被子类调用能够保证使用的都是update后的信息(mFileSuffix,mFileSeprator)
    QString now = QDateTime::currentDateTime().toString("yyyy-MM-dd");

    QString filepath = now + "-"+filename + mFileSuffix;

    if (mDirManager->exists(filepath)){ // 上次应用运行已经创建过不要重复创建直接返回
        qDebug()<<"file already exists!"; return filepath;
    }

    QFile file;
    file.setFileName(filepath);
    QTextStream s(&file);
    s.setCodec("gbk");

    file.open(QIODevice::WriteOnly); // 不存在的话会自动创建
    if (!file.isOpen()) return filepath ;

    for(int i = 0 ; i < header.size(); ++i)
    {
        if (i != header.size()-1)
        {
            s << header.at(i) << mFileSeprator;
        }
        else s << header.at(i) << "\n";
    }
    file.close();

    return filepath;
}

void LogProcessor::setFileContent(const QStringList &text)
{
    QFuture<void> future = QtConcurrent::run([=]{
        QFile file;
        file.setFileName(mLogPath); // mLogPath每个子类都不同
        QTextStream stream(&file);
        stream.setCodec("gbk");
        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        if (!file.isOpen()) return ;

        int cols = text.size();
        //qDebug()<<"text = "<<text <<"  cols = "<<cols;
        for(int col = 0 ; col < cols; ++col)
        {
            if (col != cols-1){
                stream << text.at(col) << mFileSeprator;
            }
            else stream << text.at(col) <<"\n";
        }
        file.close();
    });
    while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
}

void LogProcessor::setFilePath()
{
    mDirManager->setPath(QDir::homePath()); // 至少是用户目录文件夹
    //qDebug()<<QDir::currentPath()<<"\n"<<QDir::homePath()<<"\n"<<QDir::rootPath();
    // build-30kW-SourceLoad-Debug; C:/Users/Lenovo; C:/ ;

    QString documentPath = mDirManager->path() + "/Documents";//一般都存在的
    if (!mDirManager->exists(documentPath))
    {
        bool ret = mDirManager->mkdir("Documents");
        if (!ret) return; // 如果创建失败就在用户文件夹下
    }

    mDirManager->cd("Documents");//创建成功或者本来就存在

    if (mDirManager->exists("30kW-LogFiles")){ // 可能已经创建过
        mDirManager->cd("30kW-LogFiles");
        return ;
    }

    bool ret = mDirManager->mkdir("30kW-LogFiles");
    if (!ret) return ; // 创建失败就在Documents目录

    mDirManager->cd("30kW-LogFiles");
}
