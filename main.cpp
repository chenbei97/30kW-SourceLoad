#include <QApplication>
#include <QTextCodec>
#include <QProcess>
#include "mainwindow.h"
//#include "test/test.h"
#include "login/login.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

QTranslator *translator=Q_NULLPTR;

int main(int argc, char *argv[])
{

        //AllTest::pytest();
        //AllTest::csv_test();
        //AllTest::concurrent_test();
        //AllTest::listFiles_test();
        //AllTest::animation_test();
        //return 0;

        QApplication a(argc, argv);
        QThreadPool::globalInstance()->setMaxThreadCount(16);
        QThreadPool::globalInstance()->waitForDone();

        qRegisterMetaType<LANUAGE>("LANUAGE");// 自定义注册类型
        qRegisterMetaType<WORKMODE>("WORKMODE");
        qRegisterMetaType<TIMEOUTCODE>("TIMEOUTCODE");
        qRegisterMetaType<USBERROR>("USBERROR");
        qRegisterMetaType<TCPERROR>("TCPERROR");
        qRegisterMetaType<MONITORROLE>("MONITORROLE");
        qRegisterMetaType<TERMINALROLE>("TERMINALROLE");
        qRegisterMetaType<FILEFORMAT>("FILEFORMAT");

        //qRegisterMetaType<QList<QPersistentModelIndex>>("QList<QPersistentModelIndex>'"); // 使用QtConcurrent::run会提示没有注册类型
        //qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
        //qRegisterMetaType<QVector<int>>("QVector<int>'");
        //qRegisterMetaType<QAbstractItemModel::LayoutChangeHint>("QAbstractItemModel::LayoutChangeHint'");

        QTextCodec * codec = QTextCodec::codecForName("GBK");
        QTextCodec::setCodecForLocale(codec);

        translator = new QTranslator;
        QSettings settings(QStringLiteral("WWB-Qt"),QCoreApplication::applicationName());//创建设置
        QString Language=settings.value(QStringLiteral("language"),QStringLiteral("cn")).toString();//读取language键的值,如果为空首次进入默认使用cn
        Language == QStringLiteral("cn")? translator->load(QStringLiteral(":/cn.qm")):translator->load(QStringLiteral(":/en.qm"));
        LANUAGE lan = Language==QStringLiteral("cn")?LANUAGE ::CN:LANUAGE ::EN;
        a.installTranslator(translator);
//        Login * login = new Login; // 带登录模块
//        if (login->exec() == QDialog::Accepted)
//        {
//              MainWindow w;
//              w.setLanuage(lan);
//              w.show();
//              int ret = a.exec();
//              if (ret == 773) { // exit(773)时重启,这个在切换语言中使用
//                  QProcess::startDetached(qApp->applicationFilePath(), QStringList());
//              }
//              return ret;
//        }
//        else return 0;

        // 不带登录模块
        MainWindow w;
        w.setLanuage(lan);
        w.show();
        int ret = a.exec();
        if (ret == 773) { // exit(773)时重启,这个在切换语言中使用
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        }
        return ret;
}

