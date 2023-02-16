#ifndef RETRIEVEPASSWORD_H
#define RETRIEVEPASSWORD_H

#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QRegExpValidator>
#include <QRegExp>
#include <QPaintEvent>
#include <QPainter>
#include <QDir>
#include "../messagebox/warning.h"
#include "../messagebox/critical.h"

class RetrieveFirst;
class RetrieveSecond;
class RetrieveThird;

class RetrievePassword : public QObject
{
      Q_OBJECT
public:
      explicit RetrievePassword(QObject * parent = Q_NULLPTR);
      void process();
private:
      RetrieveFirst * FirstWindow;
      RetrieveSecond * SecondWindow;
      RetrieveThird * ThirdWindow;
signals:
      void createAccount();
};

class RetrieveFirst: public QDialog
{
      Q_OBJECT
public:
      explicit RetrieveFirst(QWidget*parent = Q_NULLPTR);
protected:
      void paintEvent(QPaintEvent*e) override;
private:
      void initUI();
      void initConnections();
      bool usernameIsExist(const QString & username);
      QString findPassword();
      QLabel * TipLabel;
      QLabel * UserNameLabel;
      QLineEdit * UserNameEdit;
      QPushButton * NextBtn;
      QPushButton * CancelBtn;
signals:
      void next(const QString& username,const QString& password);
      void createAccount();
};

class RetrieveSecond: public QDialog
{
      Q_OBJECT
public:
      explicit RetrieveSecond(QWidget*parent = Q_NULLPTR);
      void getUser(const QString& username,const QString& password);
protected:
      void paintEvent(QPaintEvent*e) override;
private:
      void initUI();
      void initConnections();
      QString mUserName;
      QLabel * TipLabel;
      QLabel * PassWordLabel;
      QPushButton * ResetBtn;
      QPushButton * EndBtn;
signals:
      void next(const QString &username);
};

class RetrieveThird: public QDialog
{
      Q_OBJECT
public:
      explicit RetrieveThird(QWidget*parent = Q_NULLPTR);
      void getUseName(const QString& username);
protected:
      void paintEvent(QPaintEvent*e) override;
private:
      void initUI();
      void initConnections();
      QString mUserName;
      QLabel * TipLabel;
      QLabel * NewPwdLabel;
      QLineEdit * NewPwdEdit;
      QPushButton * OkBtn;
      QPushButton * CancelBtn;
signals:
      void back();
};
#endif // RETRIEVEPASSWORD_H
