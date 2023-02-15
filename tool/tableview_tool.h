#ifndef TABLEVIEWWIDGET_H
#define TABLEVIEWWIDGET_H

#include <QMainWindow>
#include <QTableView>
#include <QDockWidget>
#include <QToolBar>
#include <QLabel>
#include <QActionGroup>
#include <QAction>
#include <QStatusBar>
#include <QSignalMapper>
#include "../table/table.h"
#include "../table/tablesizedialog.h"
#include "../table/tablecolordialog.h"
#include "../core/enum.h"

class TableViewWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit TableViewWidget(QWidget * parent = Q_NULLPTR);
private:
    void initUI();
    void initActConnections();
    void initConnections();
    void updateCellTextFont(const QModelIndex&);

    struct CellTextFont { //默认左对齐/不加粗/不斜体,白底黑字
        bool bold = false;
        bool italic = false;
        Table::AlignType align = Table::AlignType::Left;
        QColor mForeColor = Qt::black;
        QColor mBackColor = Qt::white;
    } mCellFont;

private:
    QTableView * mTableView;
    Table * mTable;
    TableSizeDialog * mTableSizeDlg;
    QLabel * mCellLoc;
    QLabel * mCellContent;
    QLabel * mToolTip;
    QAction * ActAppendRow;
    QAction * ActAppendCol;
    QAction * ActInsertRow;
    QAction * ActInsertCol;
    QAction * ActRemoveRow;
    QAction * ActRemoveCol;
    QAction * ActSetBold;
    QAction * ActSetItalic;
    QAction * ActClearTable;
    QAction * ActSetColor;
    QAction * ActSetAlignLeft;
    QAction * ActSetAlignCenter;
    QAction * ActSetAlignRight;
    QAction * ActImportTxt;
    QAction * ActExportTxt;
    QAction * ActImportCsv;
    QAction * ActExportCsv;
    QAction * ActConfigTable;
};

#endif // TABLEVIEWWIDGET_H
