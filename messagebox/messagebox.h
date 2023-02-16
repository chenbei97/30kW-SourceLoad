#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QPropertyAnimation>
#include "messagebutton.h"
#include "tip.h"

class Messagebox : public QDialog
{
    Q_OBJECT
private:
    typedef void (Messagebox::*ButtonSlotClassFunc)();
    void setSlotClassFunction(ButtonSlotClassFunc,int);

public:
    typedef  void (*ButtonSlotFunc)();
    explicit Messagebox(QWidget*parent=Q_NULLPTR);
    virtual ~Messagebox();

    void setDialogIcon(const QIcon&);
    void setDialogIcon(const QPixmap&);
    void setDialogTitle(const QString&);
    void setDialogFixedSize(const QSize&);
    void setDialogFixedSize(int,int);
    void setDialogFont(const QFont&);

    void setPixmap(const QIcon&,const QSize&);
    void setPixmap(const QIcon&,int,int);
    void setPixmap(const QPixmap&);
    void setPixmap(const QPixmap&,const QSize&);
    void setPixmap(const QPixmap&,int,int);
    void setText(const QString&);
    void setTextAlignment(Qt::Alignment);
    void setTextWeight(int);
    void setTextWeight(QFont::Weight);
    void setTextItalic(bool);
    void setTextColor(const QColor&);
    void setTextFamily(const QString&);
    void setTextWrapIndent(int,int);

    void setTextBoxStyleSheet(const QString&);
    void setTextEditStyleSheet(const QString&);
    void setPixmapBoxStyleSheet(const QString&);

    void setSlotFunction(ButtonSlotFunc,int);

    MessageButton * button(int) const;
    QList<MessageButton*> buttons() const;

    void removeButton(int);
    void addButton(MessageButton*);
    void setButtonStyle(MessageButton*,const QString&);

    void shake(int = 300,int = 5,int = 20,
               QEasingCurve::Type = QEasingCurve::InQuad);
private:
    QVBoxLayout * mLayout;
    QGroupBox * mIconBox;
    QHBoxLayout * mIconLay;
    QGroupBox * mTextBox;
    QHBoxLayout * mTextLay;
    QLabel * mIconLabel;
    QTextEdit * mTextEdit;
    QHBoxLayout * mTopLay;
    QHBoxLayout * mBottomLay;
    MessageButton * mOkBtn;
    MessageButton * mCancelBtn;
    QList<MessageButton*> mBtns;
    QPropertyAnimation *mShakeAnimation;
};

#endif // MESSAGEBOX_H
