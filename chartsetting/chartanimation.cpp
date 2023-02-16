#include "chartanimation.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartAnimation::ChartAnimation(QChart *chart,const QIcon& icon,QWidget*parent):ChartBox(chart,icon,parent)
{
    QLabel * option = new QLabel(tr("动画效果"));
    QLabel * curve = new QLabel(tr("动画曲线"));
    QLabel * theme = new QLabel(tr("动画主题"));
    QLabel * duration = new QLabel(tr("动画时长"));

     mAnimationOptions = new QComboBox;
     option->setBuddy(mAnimationOptions);
     mAnimationOptions->addItems(QStringList()<<"NoAnimation"<<"GridAxisAnimations"
                                 <<"SeriesAnimations"<<"AllAnimations");
     mAnimationOptions->setCurrentIndex(2); //chart.cpp 默认SeriesAnimations

     mAnimationCurves = new QComboBox;
     curve->setBuddy(mAnimationCurves);
     mAnimationCurves->addItems(QStringList()<<"Linear"<<"InQuad"<<"OutQuad"<<"InOutQuad"<<"OutInQuad" //4
                                <<"InCubic"<<"OutCubic"<<"InOutCubic"<<"OutInCubic"<<"InQuart"<<"OutQuart"<<"InOutQuart"//11
                                <<"OutInQuart"<<"InQuint"<<"OutQuint"<<"InOutQuint"<<"OutInQuint"<<"InSine"<<"OutSine"//18
                                <<"InOutSine"<<"OutInSine"<<"InExpo"<<"OutExpo"<<"InOutExpo"<<"OutInExpo"<<"InCirc"//25
                                <<"OutCirc"<<"InOutCirc"<<"OutInCirc"<<"InElastic"<<"OutElastic"<<"InOutElastic"<<"OutInElastic"//32
                                <<"InBack"<<"OutBack"<<"InOutBack"<<"OutInBack"<<"InBounce"<<"OutBounce"<<"InOutBounce"<<"OutInBounce"); // 40
     mAnimationCurves->setCurrentIndex(0); //chart.cpp 默认 linear

     mAnimationThemes = new QComboBox;
     theme->setBuddy(mAnimationThemes);
     mAnimationThemes->addItems(QStringList()<<"Light"<<"BlueCerulean"<<"Dark"<<"BrownSand"<<"BlueNcs"
                                <<"HighContrast"<<"BlueIcy"<<"Qt");
     mAnimationThemes->setCurrentIndex(0); // 默认Light

     mAnimationDuration = new QDoubleSpinBox;
     duration->setBuddy(mAnimationDuration);
     mAnimationDuration->setSingleStep(10.0); // 单步10ms
     mAnimationDuration->setSuffix("ms"); // 单位是ms
     mAnimationDuration->setRange(0,20000);
     mAnimationDuration->setDecimals(0);
     mAnimationDuration->setValue((double)mChart->animationDuration());

     connect(mAnimationOptions,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
             this,[=](int index){
            mChart->setAnimationOptions(QChart::AnimationOptions(index)); //索引对应
     });
     connect(mAnimationCurves,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
             this,[=](int index){
            mChart->setAnimationEasingCurve(QEasingCurve(QEasingCurve::Type(index))); //索引对应
     });
     connect(mAnimationThemes,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
             this,[=](int index){
            QBrush titlebrush = mChart->titleBrush(); // 之前的标题颜色
            QLegend::MarkerShape shape = mChart->legend()->markerShape(); // 之前的图例形状
            //QBrush backbrush = mChart->backgroundBrush(); // 之前的背景颜色,考虑到这样设置动画效果就没有意义了,背景还是要改变的
            mChart->setTheme(QChart::ChartTheme(index));//索引对应
            mChart->setTitleFont(QFont("Times New Roman",20)); // 切换主题以后希望坐标轴/标题的格式不变
            mChart->setTitleBrush(titlebrush);
            mChart->setBoundingRegionGranularity(0.0);// 边界区域粒度不变
            mChart->setBackgroundRoundness(5.0); // 圆角直径不变
            //mChart->setBackgroundBrush(backbrush);
            mChart->axisX()->setTitleFont(QFont("Times New Roman",16));
            mChart->axisY()->setTitleFont(QFont("Times New Roman",16));
            mChart->legend()->setFont(QFont("Times New Roman",12));
            mChart->legend()->setLabelColor(Qt::blue);
            //mChart->legend()->setLabelBrush(QBrush(Qt::blue)); //用2个之一均可
            mChart->legend()->setBorderColor(Qt::darkGray);
            mChart->legend()->setMarkerShape(shape);
     });
     connect(mAnimationDuration,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
             this,[=](double val){
            mChart->setAnimationDuration((int)val);
     });

     QGridLayout * lay = new QGridLayout;
     lay->addWidget(option,0,0);lay->addWidget(mAnimationOptions,0,1);
     lay->addWidget(curve,1,0);lay->addWidget(mAnimationCurves,1,1);
     lay->addWidget(theme,2,0);lay->addWidget(mAnimationThemes,2,1);
     lay->addWidget(duration,3,0);lay->addWidget(mAnimationDuration,3,1);
     setTitle(tr("动画"));
     setLayout(lay);
}
