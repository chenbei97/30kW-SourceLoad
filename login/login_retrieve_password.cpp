#include "login_retrieve_password.h"

RetrievePassword::RetrievePassword(QObject *parent) : QObject(parent)
, FirstWindow(new RetrieveFirst), SecondWindow(new RetrieveSecond), ThirdWindow(new RetrieveThird)
{
     connect(FirstWindow,&RetrieveFirst::createAccount,this,&RetrievePassword::createAccount);
     connect(FirstWindow,static_cast<void (RetrieveFirst::*)(const QString&,const QString&)>(&RetrieveFirst::next),
              this,[=](const QString& username,const QString& password){
              SecondWindow->getUser(username,password);
              SecondWindow->exec();
     });
     connect(SecondWindow,static_cast<void (RetrieveSecond::*)(const QString&)>(&RetrieveSecond::next),
              this,[=](const QString& username){
              ThirdWindow->getUseName(username);
              ThirdWindow->exec();
     });
}

void RetrievePassword::process()
{
     FirstWindow->exec();
}
