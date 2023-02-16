#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QToolButton>
#include <QGridLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QContextMenuEvent>
#include <QCloseEvent>
#include <QPainter>
#include <QDialog>
#include <QRadioButton>
#include <QMessageBox>
#include <QActionGroup>
#include <QTranslator>
#include <QSettings>
#include "setting/setting.h"
#include "communication/communication.h"
#include "tool/tool.h"
#include "monitor/monitor.h"

class MainWindow : public QDialog
{
      Q_OBJECT
public:
      explicit MainWindow(QWidget *parent = Q_NULLPTR);
      void setLanuage(LANUAGE);
private:
      LANUAGE  mLanuage = LANUAGE::CN;
      Setting * mainSettings;
      Communication * mainCommunication;
      Tool * mainTools;
      void init();
      void resizeEvent(QResizeEvent*) override;
      void contextMenuEvent(QContextMenuEvent*) override;
      void paintEvent(QPaintEvent*) override;
      void closeEvent(QCloseEvent*) override;
      void initConnections();
      void changeLanguage(LANUAGE  lan);
private:
      QGridLayout * mainLayout;
      QToolButton * BasicSettingsTbn;
      QToolButton * CommunicationTbn;
      QToolButton * AdvanceTbn;
      QToolButton * AboutTbn;

      QMenuBar * MenuBar;
      QMenu * MenuSettings;
      QMenu * MenuAbout;
      QMenu * MenuLanguage;

      QAction * BasicSettingsAct;
      QAction * AboutHardWareAct;
      QAction * AboutSoftWareAct;
      QAction * AboutQtAct;
      QAction * AboutCNAct;
      QAction * AboutENAct;
};
#endif // MAINWINDOW_H
