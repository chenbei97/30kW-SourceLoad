#ifndef ASSOCIATEBARMODE_H
#define ASSOCIATEBARMODE_H

#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "../tool/tableviewmodel.h"
using namespace QtCharts;

class AssociateBarMode : public QGroupBox
{
    Q_OBJECT
public:
    explicit AssociateBarMode(TableViewModel*,QWidget*parent =Q_NULLPTR);
    struct AssociateColParams
    {
        int firstColumn;
        int lastColumn;
        int startRow;
        int rowCount;
    };
    struct AssociateRowParams
    {
        int firstRow;
        int lastRow;
        int startColumn;
        int columnCount;
    };
    bool isColRegionMode() const;
    bool isRowRegionMode() const;
    bool isRowMode() const;
    bool isColMode() const;
    bool isRegionMode() const;
    bool isRowColMode() const;
    int associateRow() const;
    int associateCol() const;
    AssociateColParams associateColParams() const;
    AssociateRowParams associateRowParams() const;
    void adjustRange();
private:
    TableViewModel * mTableModel;

    QGroupBox * mAssociateModeBox;
    QRadioButton * mRowMode;
    QRadioButton * mColMode;
    QRadioButton * mRowRegionMode;
    QRadioButton * mColRegionMode;

    QGroupBox * mAssociateSpinBox;
    QGroupBox * mColRegionBox;
    QGroupBox * mRowRegionBox;

    QSpinBox * mFirstColumnSpin;
    QSpinBox * mLastColumnSpin;
    QSpinBox * mStartRowSpin;
    QSpinBox * mRowCountSpin;
    QSpinBox * mFirstRowSpin;
    QSpinBox * mLastRowSpin;
    QSpinBox * mStartColumnSpin;
    QSpinBox * mColumnCountSpin;
    QSpinBox * mRowSpin;
    QSpinBox * mColSpin;
signals:
     void modeChanged(bool);
};

#endif // ASSOCIATEBARMODE_H
