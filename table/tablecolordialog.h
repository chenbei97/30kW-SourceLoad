#ifndef TABLECOLORDIALOG_H
#define TABLECOLORDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QColorDialog>

class TableColorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableColorDialog(const QPalette& palette,QWidget * parent = Q_NULLPTR);
    QColor foregroundColor();
    QColor backgroundColor();
private:
    QGridLayout * mainLayout;
    QLabel * mForeLabel;
    QLabel * mBackLabel;
    QPushButton * mForeBtn;
    QPushButton * mBackBtn;
    QPushButton * mOkBtn;
    QPushButton * mCancelBtn;
    QColor mBackColor;
    QColor mForeColor;
};

#endif // TABLECOLORDIALOG_H
