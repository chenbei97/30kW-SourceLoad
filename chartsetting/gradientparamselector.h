#ifndef GRADIENTPARAMSELECTOR_H
#define GRADIENTPARAMSELECTOR_H

#include <QLabel>
#include <QVBoxLayout>
#include "colorbutton.h"

class GradientParamSelector : public QWidget
{
    Q_OBJECT
public:
    explicit GradientParamSelector(QWidget *parent = Q_NULLPTR);
    explicit GradientParamSelector(const QString&,const QColor&,QWidget *parent = Q_NULLPTR);
    explicit GradientParamSelector(const QString&,QWidget *parent = Q_NULLPTR);
    explicit GradientParamSelector(const QColor&,QWidget *parent = Q_NULLPTR);
    void setColor(const QColor&);
    QColor color() const;
    void setText(const QString&);
    QString text() const;
    void setGrayStyle();
    void setDefaultStyle();
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
private:
    void init();
    ColorButton * mButton;
    QLabel * mLabel;
};

#endif // GRADIENTPARAMSELECTOR_H
