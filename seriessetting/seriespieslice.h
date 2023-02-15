#ifndef SERIESPIESLICE_H
#define SERIESPIESLICE_H

#include <QGroupBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QtCharts>
#include <QLineEdit>
#include <QCheckBox>
using namespace QtCharts;

class SeriesPieSlice : public QGroupBox
{
    Q_OBJECT
public:
    explicit SeriesPieSlice(QChart*,QWidget*parent=Q_NULLPTR);
    enum AssociateMode {RowMode,ColMode,RowRegionMode,ColRegionMode};
    void setCurrentSeries(QPieSeries*);
    QPieSeries* currentSeries() const;
    void updatePieSlice();
    void updateAssociateMode(int,int);
private:
    void updateState();
    void disconnectAllConnections();
    QChart * mChart;
    QPieSeries * mCurrentSeries;
    QIcon mIcon;
    AssociateMode mAssociateMode;
    QHash<int,int> mAssociateFlags;
    QColorDialog* colorDialog(const QColor&);
    QFontDialog* fontDialog(const QFont&);

    void updateShowInfoState();
    void updateLabelVisibleState();
    void updateExplodedState();
    void updateBorderWidthState();
    void updateDistanceFactorState();
    void updateArmLengthFactorState();
    void updateLabelFontState();
    void updateLabelColorState();
    void updateFillColorState();
    void updateBorderColorState();
    void updateLabelNameState();

    QComboBox * mCurrentSlice;
    QLabel * mSliceValue;
    QLabel * mSlicePercent;
    QLabel * mStartAngle;
    QLabel * mAngleSpan;
    QCheckBox * mLabelVisible;
    QCheckBox * mExploded;
    QSpinBox * mBorderWidth;
    QDoubleSpinBox * mDistanceFactor;
    QDoubleSpinBox * mArmLengthFactor;
    QPushButton * mLabelFont;
    QPushButton * mLabelColor;
    QPushButton * mFillColor;
    QPushButton * mBorderColor;
    QPushButton * mLabelName;
    QLineEdit * mLabelNameEdit;
private slots:
    void changeSlice(int);
    void changeLabelVisible(int);
    void changeExploded(int);
    void changeBorderWidth(int);
    void changeDistanceFactor(double);
    void changeArmLengthFactor(double);
    void changeLabelFont();
    void changeLabelColor();
    void changeFillColor();
    void changeBorderColor();
    void changeLabelName();
signals:
    void seriesColorChanged(QColor,int);
};

#endif // SERIESPIESLICE_H
