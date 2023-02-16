#ifndef GRADIENTPARAMWIDGET_H
#define GRADIENTPARAMWIDGET_H

#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include "../messagebox/critical.h"
#include "gradientparamselectorwidget.h"

class GradientParamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradientParamWidget(QGradient::Type,QWidget *parent = Q_NULLPTR);
    QVector<QPair<qreal,QColor>> defaultColors() const;
    void setGradientDescription(const QString&);
    void addGradientDescription(const QString&);
    void addGradientLayout(QLayout*);
    void addGradientWidget(QWidget*);
    void insertGradientLayout(int,QLayout*);
    void insertGradientWidget(int,QWidget*);
protected:
    QVBoxLayout * mGradientLayout;
    QPushButton * mOkBtn;
private:
    Critical * mCriticalDlg;
    bool mCriticalDlgIsCalled;
    QGradient::Type mGradientType;
    GradientParamSelectorWidget * mParamSelector;
    QTextEdit* mGradientDescription;
    QRadioButton * mSelectColorBtn;
    QRadioButton * mSelectTextBtn;
    QVector<QPair<qreal,QColor>> inputedColors(const QString&);
    QPair<qreal,QColor> takeStepColor(const QString&);
    bool checkStepColor(const QString&);
    bool checkStep(const QString&);
    bool checkColor(const QString&);
    QList<int> takeRgb(const QString&,int = 3);
    bool checkRgb(const QString&);
signals:
    void gradientParams(QVector<QPair<qreal,QColor>>,QGradient::Type);
};
#endif // GRADIENTPARAMWIDGET_H
