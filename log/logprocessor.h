#ifndef LOGPROCESSOR_H
#define LOGPROCESSOR_H

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QtConcurrent>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include "../core/enum.h"

using namespace QtConcurrent;

class LogProcessor : public QObject
{
    Q_OBJECT
public:
    explicit LogProcessor(QObject *parent = Q_NULLPTR);
    virtual void generateLogFile(const QStringList&);
    QString logDir() const;
protected:
    QStringList mLogText;
    FILEFORMAT mFileFormat;
    QString mFileSeprator;
    QString mFileSuffix;
    QString mLogPath;
    void update(FILEFORMAT);
    bool isCsv() const;
    QString setFileHeader(const QStringList&,const QString&);
    void setFileContent(const QStringList &);
private:
    void updateFileFormat(FILEFORMAT);
    void updateFileSeptator();
    void updateFileSuffix();
    void setFilePath();
    QDir * mDirManager;
signals:
    void logFileFormat(FILEFORMAT);
};
#endif // LOGPROCESSOR_H
