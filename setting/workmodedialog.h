#ifndef WORKMODEDIALOG_H
#define WORKMODEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "../core/enum.h"

class WorkModeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WorkModeDialog(QWidget * parent = Q_NULLPTR);
    bool isCVMode;
signals:
      void workModeChanged(WORKMODE); //=>mTableSizeDlg
};

#endif // WORKMODEDIALOG_H
