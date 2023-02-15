#ifndef SCREENSHOOT_H
#define SCREENSHOOT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QGuiApplication>
#include <QFileDialog>
#include <QScreen>
#include <QSpinBox>
#include <QDateTime>
#include <QFormLayout>
#include "../chartview/chartview.h"

class ScreenShoot : public QDialog
{
    Q_OBJECT
public:
    explicit ScreenShoot(QChartView*,QWidget*parent = Q_NULLPTR);
private:
    QChartView * mChartView;
};
#endif // SCREENSHOOT_H
