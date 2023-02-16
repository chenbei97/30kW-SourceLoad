#ifndef CSVEXPORT_H
#define CSVEXPORT_H

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QString>
#include <QTextCodec>
#include <QMessageBox>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

class CSVExport : public QObject
{
    Q_OBJECT
public:
    explicit CSVExport();
    enum class ErrorCode {EmptyHeader,EmptyContent,Empty};
    enum class ErrorLevel {Information,Warning,Critical,Question};
    typedef QString (*TableContent)(void);

    void writeTable(const char*,const char *);
    void writeTable(const QString&,const QString&);
    void writeTable(const QString&,TableContent);

    void writeTablePy(const char *,const char *);
    void writeTablePy(const QString&,const QString&);
    void writeTablePy(const QString&,TableContent);

    QStringList tableHeader() const;
    QStringList tableContent() const;

    int row() const;
    int col() const;

    QString importCsv();
    QString importCsvPy();
private:
    int error(ErrorCode);
    int msg(const QString&,const QString&,ErrorLevel level = ErrorLevel::Warning);
    void writeTable(const QString&);
    void exportCsv();
    void clear();
    QString mWorkDir;
    QString mTableHeader;
    QString mTableContent;
    int mcols;
    int mrows;
};
#endif // CSVEXPORT_H
