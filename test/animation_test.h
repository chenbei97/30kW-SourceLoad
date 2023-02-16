#ifndef ANIMATION_TEST_H
#define ANIMATION_TEST_H

#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QApplication>
#include <QStateMachine>
#include <QSignalTransition>
#include <QTimeLine>
#include <QEasingCurve>
#include <QFinalState>
#include <QState>
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

namespace  AnimationTest {
     void pushbutton_animation()
     {
         char s[1] = {'a'}; char *ps = s;
         int argc=0; char**argv = &ps;
         QApplication a(argc,argv);

         // 此代码将在10秒内将按钮从屏幕左上角移动到位置(250,250)
         QPushButton button1("Animated Button1");
         button1.setMinimumSize(800,200);
         button1.show();
         QPropertyAnimation animation1(&button1, "geometry");
         animation1.setDuration(10000);
         animation1.setStartValue(QRect(0, 0, 100, 30));
         animation1.setEndValue(QRect(250, 250, 100, 30));
         //animation1.start();

         // 也可以设置位于起始值和结束值之间的值，然后插值将经过这些点。
         // 含义是，起点从(0,0)开始，控件尺寸是(800,200)，中点要到达位置(1000,1000)，尺寸扩大一倍
         // 之后返回位置(100,100)，尺寸变回原来大小
         QPushButton button2("Animated Button2");
         button2.setMinimumSize(800,200);
         button2.show();
         QPropertyAnimation animation2(&button2, "geometry");
         animation2.setDuration(10000);
         animation2.setKeyValueAt(0, QRect(0, 0, 800, 200));
         animation2.setKeyValueAt(0.5, QRect(1000, 1000, 1600, 400));
         animation2.setKeyValueAt(1, QRect(100, 100, 800, 200));
         //animation2.start();

         // 还可以使用QEasingCurve定义的缓动曲线
         QPushButton button3("Animated Button3");
         button3.setMinimumSize(800,200);
         button3.show();
         QPropertyAnimation animation3(&button3, "geometry");
         animation3.setDuration(30000);
         animation3.setStartValue(QRect(100, 100, 800, 200));
         animation3.setEndValue(QRect(1000, 1000, 800, 200));
         animation3.setEasingCurve(QEasingCurve::OutBounce);
         //animation3.start();

         QSequentialAnimationGroup *group = new QSequentialAnimationGroup;
         group->addAnimation(&animation3); // 倒序交替执行
         group->addAnimation(&animation2);
         group->addAnimation(&animation1);

         group->start();

         // 使用状态机的例子
         QPushButton *button = new QPushButton("Animated Button");
         button->resize(800,200);
         button->show();
         QStateMachine *machine = new QStateMachine;

         QState *state1 = new QState(machine); // 状态1 设置随geometry变化,
         state1->assignProperty(button, "geometry", QRect(100, 100, 800, 200)); // 状态1位置和大小
         machine->setInitialState(state1);

         QState *state2 = new QState(machine);
         state2->assignProperty(button, "geometry", QRect(500, 500, 200, 800));// 状态2位置和大小

         QSignalTransition *transition1 = state1->addTransition(button,SIGNAL(clicked()), state2); //状态1点击进入状态2
         transition1->addAnimation(new QPropertyAnimation(button, "geometry"));
         QSignalTransition *transition2 = state2->addTransition(button,SIGNAL(clicked()), state1);//状态2点击进入状态1
         transition2->addAnimation(new QPropertyAnimation(button, "geometry"));
         machine->start();

         // 状态机开始s1,结束s2,点击按钮转换成s2并发送finished信号就会关闭应用
         QPushButton button4("Final State");
         button4.resize(800,200);
         button4.show();
         QStateMachine machine4;
         QState *s1 = new QState();
         QFinalState *s2 = new QFinalState();
         s1->addTransition(&button4, SIGNAL(clicked()), s2);
         machine4.addState(s1);
         machine4.addState(s2);
         QObject::connect(&machine4, SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
         machine4.setInitialState(s1);
         machine4.start();

         a.exec();
     }
}

#endif // ANIMATION_TEST_H
