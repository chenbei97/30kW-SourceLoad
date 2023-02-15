#ifndef AXISBOX_H
#define AXISBOX_H

#include <QGroupBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QtCharts/QChart>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSpinBox>
#include <QDebug>
using namespace QtCharts;

// 和chartbox完全一样,只是为了对称性保持风格一致
class AxisBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit AxisBox(QChart * chart,QAbstractAxis*, const QIcon&icon, QWidget*parent = Q_NULLPTR);
    void setChart(QChart*);
    QChart* chart() const;
    void setCurrentAxis(QAbstractAxis*);
    QAbstractAxis * currentAxis() const;
    virtual void disconnectAllConnections() = 0;
    QColorDialog* colorDialog(const QColor&);
    QFontDialog* fontDialog(const QFont&);
    QComboBox * brushStyleCombo();
protected:
    QChart * mChart;
    QAbstractAxis * mCurrentAxis;
    QIcon mIcon;
    virtual void updateState()= 0;
};
#endif // AXISBOX_H
