#ifndef MONITOR_H
#define MONITOR_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QCheckBox>
#include <QEvent>
#include <QShowEvent>
#include <QStatusBar>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include "monitor_label.h"
#include "monitor_spinbox.h"
#include "../core/enum.h"

class Monitor : public QMainWindow
{
    Q_OBJECT
public:
    explicit Monitor(QWidget *parent = Q_NULLPTR);

    void setWorkMode(WORKMODE);
    WORKMODE workMode() const;

    void setMessageState(TIMEOUTCODE);
    void setConnectState(USBERROR);
    void setConnectState(TCPERROR);
    void showMeasureState(const QList<QByteArray> &);
    void showSetState(MONITORROLE,bool);

    static QString translateErrorCode(USBERROR);
    static QString translateErrorCode(TCPERROR);
    static QString translateErrorCode(TIMEOUTCODE);
    static QString translateErrorCode(MONITORROLE);
public slots:
    bool eventFilter(QObject* , QEvent *) override;
private:
    void initCVUI();
    void initCCUI();
    void initCommonUI();
    void showEvent(QShowEvent *) override;
    void paintEvent(QPaintEvent*) override;
    WORKMODE mWorkMode;
private:
    QSplitter * mainLayout;
    QLabel* IsShowResistanceLabel;
    QCheckBox* IsShowResistance;

    MonitorLabel * ShowPower;
    QLabel * SetPowerSourceLabel;
    MonitorSpinBox * SetPowerSource;
    QLabel * SetPowerLoadLabel;
    MonitorSpinBox * SetPowerLoad;

    MonitorLabel * ShowResistance;
    QLabel * SetResistanceSourceLabel;
    MonitorSpinBox * SetResistanceSource;
    QLabel * SetResistanceLoadLabel;
    MonitorSpinBox * SetResistanceLoad;

    MonitorLabel * ShowVoltage;
    MonitorLabel * ShowCurrent;

    QGroupBox * VoltageGroupBox;
    QGroupBox * CurrentGroupBox;
    QGroupBox * PowerGroupBox;
    QGroupBox * ResistanceGroupBox;

    QLabel * ConnectState;
    QLabel * MessageRequestState;
    QLabel * MessageResponseState;
    QLabel * MessageSetState;

 signals:
     void setMonitorRole(MONITORROLE , const QByteArray& );
};
#endif // MONITOR_H
