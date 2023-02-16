#include "backgroundareagradient.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

BackgroundAreaGradient ::BackgroundAreaGradient (QChart*chart,const QIcon&icon,QWidget *parent) :
    QGroupBox(parent),mChart(chart),mIcon(icon)
{
    // 背景区域-工具栏下的渐变填充工具组，父类是背景区域工具栏
    mGradientInterpolation = QGradient::ColorInterpolation; // 默认
    mGradientCoordinate = QGradient::StretchToDeviceMode; // 默认
    mGradientSpread = QGradient::RepeatSpread; // 默认
    mGradientType = QGradient::LinearGradient; // 默认,这里的默认不要选择NoGradient,能进入本工具栏说明已经选择了渐变填充

    // （1）基本参数的设置布局
    QGroupBox * basicbox = new QGroupBox(tr("渐变基本参数设置"));
    QComboBox * interpolate = new QComboBox;
    QLinearGradient().setInterpolationMode(QGradient::InterpolationMode::ColorInterpolation);
    interpolate->addItems(QStringList()<<"Color"<<"Component");
    QComboBox * coordinate = new QComboBox;
    coordinate->addItems(QStringList()<<"Logical"<<"StretchToDevice"<<"ObjectBounding");
    coordinate->setCurrentIndex(1); // 默认StretchToDevice
    QComboBox * spread = new QComboBox;
    spread->addItems(QStringList()<<"PadSpread"<<"ReflectSpread"<<"RepeatSpread");
    spread->setCurrentIndex(2);// 默认RepeatSpread
    QComboBox * gradienttype = new QComboBox;
    gradienttype->addItems(QStringList()<<"Linear"<<"Radial"<<"Conical");

    // 4个渐变基本参数改变不在背景区域渐变窗口处理,而是在背景区域窗口处理
    connect(interpolate,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int idx){emit interpolationChanged(QGradient::InterpolationMode(idx));mGradientInterpolation=QGradient::InterpolationMode(idx);});
    connect(coordinate,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int idx){emit coordinateChanged(QGradient::CoordinateMode(idx));mGradientCoordinate=QGradient::CoordinateMode(idx);});
    connect(spread,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int idx){emit spreadChanged(QGradient::Spread(idx));mGradientSpread = QGradient::Spread(idx);});
    connect(gradienttype,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), //注意这里使用activated,单击了组合框就会更新
            this,[=](int idx){emit gradientTypeChanged(QGradient::Type(idx));mGradientType=QGradient::Type(idx);}); // NoGradient=3不使用
    // 是为了方便参数调整可视化后能点击当前项就能更新,而不是切换渐变类型再改回来才能更新

    QFormLayout * formlay = new QFormLayout;
    formlay->addRow(tr("&插值对象"),interpolate);
    formlay->addRow(tr("&坐标模式"),coordinate);
    formlay->addRow(tr("&延展方法"),spread);
    formlay->addRow(tr("&渐变类型"),gradienttype);
    basicbox->setLayout(formlay);

    // （2）高级参数的设置布局
    QGroupBox * superbox = new QGroupBox(tr("渐变高级参数设置"));
    QPushButton * linearbtn = new QPushButton(tr("设置参数"));
    QPushButton * radialbtn = new QPushButton(tr("设置参数"));
    QPushButton * conicalbtn = new QPushButton(tr("设置参数"));
    QFormLayout * superlay = new QFormLayout;
    superlay->addRow(tr("&线性渐变"),linearbtn);
    superlay->addRow(tr("&径向渐变"),radialbtn);
    superlay->addRow(tr("&锥形渐变"),conicalbtn);
    superbox->setLayout(superlay);

    mLinearGradientParamWidget = new LinearGradientParamWidget;
    connect(mLinearGradientParamWidget,&LinearGradientParamWidget::gradientParams,
            this,&BackgroundAreaGradient ::gradientParams);
    connect(linearbtn,&QPushButton::clicked,this,[=]{mLinearGradientParamWidget->show();});

    mRadialGradientParamWidget = new RadialGradientParamWidget;
    connect(mRadialGradientParamWidget,&LinearGradientParamWidget::gradientParams,
            this,&BackgroundAreaGradient ::gradientParams);
    connect(mRadialGradientParamWidget,&RadialGradientParamWidget::radialGradientParams,
            this,&BackgroundAreaGradient ::radialGradientParams);
    connect(radialbtn,&QPushButton::clicked,this,[=]{mRadialGradientParamWidget->show();});

    mConicalGradientParamWidget = new ConicalGradientParamWidget;
    connect(mConicalGradientParamWidget,&ConicalGradientParamWidget::gradientParams,
            this,&BackgroundAreaGradient ::gradientParams);
    connect(mConicalGradientParamWidget,&ConicalGradientParamWidget::conicalGradientParams,
            this,&BackgroundAreaGradient ::conicalGradientParams);
    connect(conicalbtn,&QPushButton::clicked,this,[=]{mConicalGradientParamWidget->show();});

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(basicbox);
    lay->addWidget(superbox);
    setLayout(lay);
}

QVector<QPair<qreal,QColor>> BackgroundAreaGradient::defaultColors() const
{
    return mLinearGradientParamWidget->defaultColors(); // 默认都是一样的无论哪种渐变类型
}

void BackgroundAreaGradient ::closeChildrenWindows()
{
    mLinearGradientParamWidget->close();
    mRadialGradientParamWidget->close();
    mConicalGradientParamWidget->close();
}

QGradient::InterpolationMode BackgroundAreaGradient::selectedGradientInterpolation() const
{
    return  mGradientInterpolation;
}

QGradient::CoordinateMode BackgroundAreaGradient ::selectedGradientCoordinate() const
{
    return mGradientCoordinate;
}

QGradient::Spread BackgroundAreaGradient ::selectedGradientSpread() const
{
    return mGradientSpread;
}

QGradient::Type BackgroundAreaGradient::selectedGradientType() const
{
    return  mGradientType;
}
