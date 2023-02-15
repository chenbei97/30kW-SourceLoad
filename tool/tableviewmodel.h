#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QVXYModelMapper>
#include <QHash>
#include <QVector>
#include <QRect>
#include <QColor>
#include <QTime>
#include <QtCharts>
#include <QPair>
using namespace QtCharts;

class TableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    void clear();
    void update();

    void setHorizontalHeaderLabels(const QStringList &labels);
    QStringList horizontalHeaderLabels() const;
    QString horizontalHeaderLabel(int) const;

    void setVerticalHeaderLabels(const QStringList&labels);
    QStringList verticalHeaderLabels() const;
    QString verticalHeaderLabel(int) const;

    void appendRow();
    void appendRow(QVector<QVariant> *);
    void appendRow(const QStringList&);
    void appendColumn();

    qreal dataSummary(const QVector<QVariant>&) const;
    bool isRowDataValid(int) const;
    bool isRowDataValid(int,int,int) const;
    bool isColumnDataValid(int) const;
    bool isColumnDataValid(int,int,int) const;
    bool isRowRegionDataValid(int,int,int,int) const;
    bool isColRegionDataValid(int,int,int,int) const;
    bool isAllDataValid() const;

    QVector<QVariant> rowData(int) const;
    QVector<QVariant> rowData(int,int,int) const;
    QVector<QVariant> colData(int) const;
    QVector<QVariant> colData(int,int,int) const;

    void addCellMapping(QRect,QColor);
    void addColMapping(int,QColor);
    void addColMapping(int,int,int,QColor);
    void addColMapping(int,int,int,QList<QColor>);
    void addRowMapping(int,QColor);
    void addRowMapping(int,int,int,QColor);
    void addRowMapping(int,int,int,QList<QColor>);
    void addDoubleColMapping(QXYSeries*,int,int);
    void addColRegionMapping(int,int,int,int,QList<QColor>);
    void addRowRegionMapping(int,int,int,int,QList<QColor>);

    QColor cellColor(int) const;
    QColor cellColor(int,int) const;

    void setDefaultMapping();
    void clearMapping();
private:
    QList<QVector<QVariant> * > mData;
    QHash<int,QColor> mCellMapping;
    QStringList mHorizontalHeader;
    QStringList mVerticalHeader;
    int mColumnCount;
    int mRowCount;
private slots:
    void onColumnsInserted(const QModelIndex &parent, int first, int last);
    void onColumnsRemoved(const QModelIndex &parent, int first, int last);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onModelReset();
    void onHeaderDataChanged(Qt::Orientation orientation, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                       const QVector<int> &roles = QVector<int>());
};

#endif // TABLEVIEWMODEL_H
