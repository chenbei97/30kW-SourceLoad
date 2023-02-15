#ifndef ASSOCIATEXYMODE_H
#define ASSOCIATEXYMODE_H

#include <QGroupBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "../tool/tableviewmodel.h"

class AssociateXYMode : public QGroupBox
{
    Q_OBJECT
public:
    explicit AssociateXYMode(TableViewModel*,QWidget*parent =Q_NULLPTR);
    bool isSingle() const;
    void adjustRange();
    int singleCol();
    QPoint doubleCols();
private:
    TableViewModel * mTableModel;
    QGroupBox * mAssociateModeBox;
    QRadioButton * mSingleBtn;
    QRadioButton * mDoubleBtn;
    QGroupBox * mSingleAssociateBox;
    QSpinBox * mSingleSpin;
    QGroupBox * mDoubleAssociateBox;
    QSpinBox * mDoubleSpinX;
    QSpinBox * mDoubleSpinY;
signals:
      void changeMode(bool);
};

#endif // ASSOCIATEXYMODE_H
