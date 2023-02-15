#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QColorDialog>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget*parent=Q_NULLPTR);
    explicit ColorButton(const QColor&,QWidget*parent=Q_NULLPTR);
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setGrayStyle();
    void setDefaultStyle();
    void setColor(const QColor&);
    QColor color() const;
    QColorDialog* colorDialog(const QColor&initColor);
private:
    void init();
    QColor mColor;
    QSize mSize;
};

#endif // COLORBUTTON_H
