#ifndef LOGPERIOD_H
#define LOGPERIOD_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QCheckBox>
#include "../messagebox/infomation.h"
#include "../messagebox/critical.h"
#include "../messagebox/warning.h"
#include "../core/enum.h"

class LogPeriod : public QDialog
{
    Q_OBJECT
public:
    explicit LogPeriod(QWidget * parent = Q_NULLPTR);
    struct Period {
        quint32 ms = 0;
        quint32 s = 0;
        quint32 m = 0;
        quint32 h = 0;
    } mPeriod;
private:
    QGridLayout * mainLayout;
    QCheckBox * mLogEnabled;
    QButtonGroup * mRadioBtnGroup;
    QRadioButton * mCsvRadioBtn;
    QRadioButton * mTxtRadioBtn;
    QComboBox * mPeriodList;
    QComboBox * mUnitList;
    QLabel * mLabel1;
    QLabel * mLabel2;
    QLineEdit * mCustomPeriodEdit;
    QCheckBox * isInsertCustomPeriod;
    QPushButton * mOkBtn;
    QString getCustomPeriod();
    void emitPeriod(QString);
private:
    void setEnabled(bool);
signals:
    void selectedPeriod(quint32, quint32 = 0,quint32  = 0, quint32 = 0);
    void logEnabled(bool);
    void logFileFormat(FILEFORMAT);
};

#endif // LOGPERIOD_H
