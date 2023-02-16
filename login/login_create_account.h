#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFile>
#include <QDir>
#include  <QRegExp>
#include  <QRegExpValidator>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QPaintEvent>
#include <QPainter>
#include <QTextStream>
#include "../messagebox/warning.h"
#include "../messagebox/critical.h"

class CreateAccount : public QDialog
{
    Q_OBJECT
public:
    explicit CreateAccount(QWidget * parent = Q_NULLPTR);
    void createAccount();
    bool usernameIsExist(const QString& username);
protected:
    void paintEvent(QPaintEvent *e);
private:
    QGridLayout * mainLayout;
    QLabel * UserNameLabel;
    QLineEdit * UserNameEdit;
    QLabel * PassWordLabel;
    QLineEdit * PassWordEdit;
    QPushButton * OkBtn;
    QPushButton * CancelBtn;
};

#endif // CREATEACCOUNT_H
