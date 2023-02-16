#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QStackedWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "workmodedialog.h"
#include "logperiod.h"
#include "querypanel.h"

class Setting : public QDialog
{
    Q_OBJECT
public:
    explicit Setting(QWidget * parent = Q_NULLPTR);
    QWidget * statckWidget(int) const;
private:
    void initUI();
    void initConnections();
    QGridLayout * mainLayout;
    QHBoxLayout * topLayout;
    QHBoxLayout * bottomLayout;
    QListWidget * listwidget;
    QStackedWidget * stackwidget;
    QPushButton * okBtn;
    QPushButton * cancelBtn;
    WorkModeDialog * mWorkMode;
    LogPeriod * mLogPeriod;
    QueryPanel * mTerminal;
signals:
    void workModeChanged(WORKMODE);
    void selectedPeriod(quint32, quint32 = 0,quint32 = 0, quint32 = 0);
    void setTerminalCommand(const QByteArray&);
    void terminalQueryData(const QByteArray&);
    void logEnabled(bool);
    void logFileFormat(FILEFORMAT);
};

#endif // SETTING_H
