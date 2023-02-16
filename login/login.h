#ifndef LOGIN_H
#define LOGIN_H

#include <QLabel>
#include <QDialog>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QApplication>
#include <QPainter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include "login_create_account.h"
#include "login_retrieve_password.h"
#include "../messagebox/infomation.h"

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget * parent = Q_NULLPTR);
protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);
    bool mMouseMove = false;
    QPoint mMouseLastPos;
private:
    QLabel * UserNameLabel;
    QLineEdit * UserNameEdit;
    QLabel * PassWordLabel;
    QLineEdit * PassWordEdit;
    QPushButton * ForgetPassWordBtn;
    QPushButton * CreateCountBtn;
    QPushButton * OkBtn;
    QPushButton * CancelBtn;
private:
    void initUI();
    void initConnections();
    bool usernameIsExist();
    bool passwordIsCorrect();
    bool IsExist;
    QString mUserName;
    QString mPassWord;
    int tryCount;
    RetrievePassword * mPaswordModifier;
};

#endif // LOGIN_H
