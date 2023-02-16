#ifndef TABLE_H
#define TABLE_H

#include <QObject>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTextStream>
#include <QFileDialog>
#include <QRegExp>
#include <QDir>
#include <QDebug>
#include <QtConcurrent>
#include <QFuture>
#include <QTime>
#include <QPersistentModelIndex>
#include <QThreadPool>
#include <QApplication>
#include "../messagebox/critical.h"
#include "../messagebox/infomation.h"
#include "../messagebox/warning.h"
//#include "../csv/csvexport.h"

using namespace QtConcurrent;

class Table:  public QObject
{
    Q_OBJECT
public:
    enum class AlignType {Left,Center,Right};
    enum class DelegateType {LineEdit,IntSpin,DoubleSpin,Combo};
    struct TableSize { int row; int col;};

    explicit Table(QTableView*,int ,int ,QObject * parent = Q_NULLPTR);
    explicit Table(QTableView*,TableSize,QObject * parent = Q_NULLPTR);

    void importTxt(); // Get the text before importing the table
    void importCsv();   // Import table while text loop
    void importTable(QStringList& content); // should ≯ 10

    void exportTxt(); // Get the table text before exporting
    void exportCsv();  // Export text while the table loops

    //void importCsvExternel();
    //void exportCsvExternel();

    void importTxtOptim();
    void importCsvOptim();

    void importTableConcurrent(QStringList& content);

    void importTxtConcurrent();
    void exportTxtConcurrent();

    void importCsvConcurrent();
    void exportCsvConcurrent();

    int row() const;
    int col() const;

    void appendRow();
    void appendCol();

    void insertRow();
    void insertCol();

    void appendRow(const QStringList&);
    void insertRow(const QStringList&);

    void removeRow();
    void removeCol();

    void setBold(bool);
    void setItalic(bool);
    void setAlign(AlignType);
    void setColor(QColor,QColor);
    void setDelegate(DelegateType);

    void clear();
    void clearConcurrent(bool = false);

    void reset(int, int);
    void reset(const TableSize&);

    void setHeader(const QStringList&);
    QStringList header() const;

    QStandardItem * takeItem(const QModelIndex&) const;

private:
    void initModels(int,int);
    void initConnections();
    void setTips();
    void showTips();
    bool mShowTips;
    void showTips(const QString&,const QSize& =QSize(800,280),const QPoint& =QPoint(2,18));
    Infomation * mTableTips;
    QTableView * mTableView;
    QItemSelectionModel * mSelectionModel;
    QStandardItemModel * mStandardModel;
    TableSize mTableSize;
    //CSVExport  mCsvOpera;
    QColor mForeColor;
    QColor mBackColor;
signals:
    void cellCurrentIndex(const QModelIndex&);
    void cellCurrentContent(const QString&);
};

#endif // TABLE_H
