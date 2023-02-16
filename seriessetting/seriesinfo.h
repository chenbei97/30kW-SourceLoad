#ifndef SERIESINFO_H
#define SERIESINFO_H

#include "seriesbox.h"

class SeriesInfo : public SeriesBox
{
    Q_OBJECT
public:
    explicit SeriesInfo(QChart *,QWidget* = Q_NULLPTR);
    void updateInfo();
    void disconnectAllConnections();
private:
    void updateState() override;
    QCheckBox * mSeriseVisible;
    QDoubleSpinBox * mSeriesOpacity;
    QLineEdit * mSeriesNameEdit;
    QPushButton * mSeriesNameBtn;
    QLabel * mSeriesType;
    void updateTypeState();
    void updateVisibilityState();
    void updateOpacityState();
    void updateNameState();
private slots:
    void changeVisibility(int);
    void changeOpacity(double);
    void changeName();
signals:
    void nameChanged(const QString&,int);
};

#endif // SERIESINFO_H
