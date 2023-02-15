#ifndef CHARTTITLE_H
#define CHARTTITLE_H

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "chartbox.h"

class ChartTitle : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartTitle(QChart *chart,const QIcon&,QWidget*parent = Q_NULLPTR);
private:
    QLineEdit * mSetTitleEdit;
    QPushButton * mSetTitleBtn;
    QPushButton * mSetTitleFontBtn;
    QPushButton * mSetTitleColorBtn;
};

#endif // CHARTTITLE_H
