#ifndef ASSOCIATEPIEMODE_H
#define ASSOCIATEPIEMODE_H

#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QVPieModelMapper>
#include "../tool/tableviewmodel.h"
using namespace QtCharts;

class AssociatePieMode : public QGroupBox
{
    Q_OBJECT
public:
    explicit AssociatePieMode(TableViewModel*,QWidget*parent =Q_NULLPTR);
    struct AssociateColParams
    {
        int labelsColumn;
        int valuesColumn;
        int firstRow;
        int rowCount;
    };
    struct AssociateRowParams
    {
        int labelsRow;
        int valuesRow;
        int firstColumn;
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

    QSpinBox * mValuesColumnSpin;
    QSpinBox * mLabelsColumnSpin;
    QSpinBox * mFirstRowSpin;
    QSpinBox * mRowCountSpin;

    QSpinBox * mValuesRowSpin;
    QSpinBox * mLabelsRowSpin;
    QSpinBox * mFirstColumnSpin;
    QSpinBox * mColumnCountSpin;
    QSpinBox * mRowSpin;
    QSpinBox * mColSpin;
};

#endif // ASSOCIATEPIEMODE_H
