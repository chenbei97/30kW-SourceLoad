#include "backgroundarea.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

BackgroundArea::BackgroundArea(QChart* chart,const QIcon&icon,QWidget *parent) :
    QWidget(parent),mChart(chart),mIcon(icon)
{
    // 背景区域设置的工具栏小部件 = 圆角尺寸 + 边界粒度 + 颜色填充 + 渐变填充(BackgroundAreaGradient)
    setFont(QFont("Times New Roman",12));
    setWindowIcon(mIcon);
    //setMinimumSize(QSize(600,200));
    initGradientBox();
    initMainLayout();
}

void BackgroundArea::closeChildrenWindows()
{
    mBackgroundAreaGradient->closeChildrenWindows(); //背景区域设置能出现弹出窗口的只有渐变填充这里会出现
}

void BackgroundArea::closeEvent(QCloseEvent *event)
{
    mBackgroundAreaGradient->closeChildrenWindows(); // closeChildrenWindows是最外部的mainwindow关闭事件才会触发
    event->accept(); // 这里希望只关闭这个父窗口也能关闭其子窗口,所以引入关闭事件
}
void BackgroundArea::initGradientDefaultParams()
{// 默认设置要保持一致,防止用户设置渐变参数后背景发生变化以为是自己设置参数导致的原因
    // 如果mGradientType是Linear,其默认参数是Linear的,但是正在设置Radial的参数,设置实时参数可视化,调节spinbox,背景会跟随
    // 此时点击取消实时参数可视化,那么背景就会变成linear的默认设置,会让用户误以为是radial参数设置的原因
    mGradientInterpolation = mBackgroundAreaGradient->selectedGradientInterpolation(); // 保持一致
    mGradientCoordinate = mBackgroundAreaGradient->selectedGradientCoordinate();
    mGradientSpread = mBackgroundAreaGradient->selectedGradientSpread();
    mGradientType = QGradient::NoGradient; // 不要一致,因为可以选颜色填充

    mLinearGradient.setStops(mBackgroundAreaGradient->defaultColors()); // 使用颜色选择部件的默认颜色来初始化3个渐变
    mRadialGradient.setStops(mBackgroundAreaGradient->defaultColors());
    mConicalGradient.setStops(mBackgroundAreaGradient->defaultColors());

    mLinearGradient.setInterpolationMode(mGradientInterpolation);
    mRadialGradient.setInterpolationMode(mGradientInterpolation);
    mConicalGradient.setInterpolationMode(mGradientInterpolation);

    mLinearGradient.setCoordinateMode(mGradientCoordinate);
    mRadialGradient.setCoordinateMode(mGradientCoordinate);
    mConicalGradient.setCoordinateMode(mGradientCoordinate);

    mLinearGradient.setSpread(mGradientSpread);
    mRadialGradient.setSpread(mGradientSpread);
    mConicalGradient.setSpread(mGradientSpread);

    mRadialGradient.setCenterRadius(1.0);
    mRadialGradient.setCenter(0.,0.);
    mRadialGradient.setFocalPoint(0.,0.);

    mConicalGradient.setAngle(0.);
    mConicalGradient.setCenter(0.,0.);
}

void BackgroundArea::initGradientBox()
{
    mBackgroundAreaGradient = new BackgroundAreaGradient(mChart,mIcon);
    initGradientDefaultParams();

    // backgroundareagradient.cpp 4个基本渐变基本参数的改变通知这里的4个变量改变
    connect(mBackgroundAreaGradient,static_cast<void (BackgroundAreaGradient::*)(QGradient::InterpolationMode)>
            (&BackgroundAreaGradient::interpolationChanged),this,[=](QGradient::InterpolationMode interpolate){
            mLinearGradient.setInterpolationMode(interpolate);
            mRadialGradient.setInterpolationMode(interpolate);
            mConicalGradient.setInterpolationMode(interpolate);
            mGradientInterpolation = interpolate;
            emit gradientTypeChanged(); // 这里调用1次是希望这里设置的效果立刻也能反馈
    });
    connect(mBackgroundAreaGradient,static_cast<void (BackgroundAreaGradient::*)(QGradient::CoordinateMode)>
            (&BackgroundAreaGradient::coordinateChanged),this,[=](QGradient::CoordinateMode coordinate){
            mLinearGradient.setCoordinateMode(coordinate);
            mRadialGradient.setCoordinateMode(coordinate);
            mConicalGradient.setCoordinateMode(coordinate);
            mGradientCoordinate = coordinate;
            emit gradientTypeChanged(); // 这里调用1次是希望这里设置的效果立刻也能反馈
    });
    connect(mBackgroundAreaGradient,static_cast<void (BackgroundAreaGradient::*)(QGradient::Spread)>
            (&BackgroundAreaGradient::spreadChanged),this,[=](QGradient::Spread spread){
            mLinearGradient.setSpread(spread);
            mRadialGradient.setSpread(spread);
            mConicalGradient.setSpread(spread);
            mGradientSpread = spread;
            emit gradientTypeChanged();
    });
    connect(mBackgroundAreaGradient,static_cast<void (BackgroundAreaGradient::*)(QGradient::Type)>
            (&BackgroundAreaGradient::gradientTypeChanged),this,[=](QGradient::Type type){
        mGradientType = type; emit gradientTypeChanged();}); // 特别的对于type的改变,通知私有信号

    // BackgroundAreaGradient 3类渐变高级参数的改变通知这里的3个变量改变
    // (1) gradientParams共同参数的更新
    connect(mBackgroundAreaGradient,static_cast<void(BackgroundAreaGradient::*)(QVector<QPair<qreal,QColor>>,QGradient::Type) >
            (&BackgroundAreaGradient::gradientParams),this,[=](QVector<QPair<qreal,QColor>> colorlist,QGradient::Type type){
        // setStops函数会自动清除上次的任何设置, 不会导致错位覆盖的情况
        switch (type) { // type是来自高级参数设置完成发出的,是为了更新这里的3个变量参数点
                case QGradient::LinearGradient: mLinearGradient.setStops(colorlist);break;
                case QGradient::RadialGradient: mRadialGradient.setStops(colorlist);break;
                case QGradient::ConicalGradient: mConicalGradient.setStops(colorlist);break;
                default: break; // 不会发生
            }
            emit gradientTypeChanged(); //当type和mGradientType一致时用户设置好高级参数才会立即体现效果
    });
    // (2) radialGradientParams额外参数的更新
    connect(mBackgroundAreaGradient,static_cast<void(BackgroundAreaGradient::*)(RadialGradientParamWidget::RadialParam,bool,bool) >
            (&BackgroundAreaGradient::radialGradientParams),this,[=](RadialGradientParamWidget::RadialParam param,bool isRealTime,bool isBtn){
            // spinbox和mOkBtn都可以发出信号更新这里的参数
             if (isBtn)
             {
                 // mRadialGradient.setFocalRadius(param.value("radius")); // 不提供给用户
                // mRadialGradient.setFocalRadius(param.value("focal-radius")); // 不提供给用户
                 mRadialGradient.setCenterRadius(param.value("center-radius"));
                 mRadialGradient.setCenter(param["center-x"],param["center-y"]);
                 mRadialGradient.setFocalPoint(param["focal-x"],param["focal-y"]);

                 emit gradientTypeChanged(); // 如果用户正在设置的参数type和mGradientType相等就会立即体现设置的参数效果
             }
             else { // 说明是spinbox的设置 如果选择参数实时可视化,只是为了体现效果,并不是设定了该参数,不能影响原有的mRadialGradient
                    if (isRealTime) {
                        QRadialGradient gradient = mRadialGradient; // 用临时变量来设置
                        gradient.setCenterRadius(param.value("center-radius"));
                        gradient.setCenter(param["center-x"],param["center-y"]);
                        gradient.setFocalPoint(param["focal-x"],param["focal-y"]);
                        //qDebug()<<gradient.radius()<<gradient.center()<<gradient.focalPoint();
                        mChart->setBackgroundBrush(gradient);
                    } // 如果不选择实时可视化,什么也不做,mOkBtn拥有最终决定权
             }
    });
    // (3) conicalGradientParams额外参数的更新
    connect(mBackgroundAreaGradient,static_cast<void(BackgroundAreaGradient::*)(ConicalGradientParamWidget::ConicalParam,bool,bool) >
            (&BackgroundAreaGradient::conicalGradientParams),this,[=](ConicalGradientParamWidget::ConicalParam param,bool isRealTime,bool isBtn){
                if (isBtn)
                {
                    mConicalGradient.setCenter(param.first);
                    mConicalGradient.setAngle(param.second);
                    emit gradientTypeChanged();
                }
                else {
                       if (isRealTime) {
                           QConicalGradient gradient = mConicalGradient;
                           gradient.setCenter(param.first);
                           gradient.setAngle(param.second);
                           mChart->setBackgroundBrush(gradient);
                       }
                }
    });
    connect(this,&BackgroundArea::gradientTypeChanged,this,[=]{ // 槽函数会借助mGradientType做判断
            switch (mGradientType) {
                    case QGradient::LinearGradient:mChart->setBackgroundBrush(mLinearGradient);break;
                    case QGradient::RadialGradient: mChart->setBackgroundBrush(mRadialGradient);break;
                    case QGradient::ConicalGradient: mChart->setBackgroundBrush(mConicalGradient);break;
                    case QGradient::NoGradient:break; //选择颜色填充时会变成这个,选择渐变填充时什么也不做
            }
    });
}

void BackgroundArea::initBrushStyleComboBox(QComboBox *brushstyle)
{
    QListWidget * listwidget = new QListWidget;
    brushstyle->setModel(listwidget->model());
    brushstyle->setView(listwidget);
    brushstyle->resize(100,30);
    QStringList iconnames;
    iconnames <<"NoBrush"<<"SolidPattern"<<"Dense1Pattern"<<"Dense2Pattern"<<"Dense3Pattern"
             <<"Dense4Pattern"<<"Dense5Pattern"<<"Dense6Pattern"<<"Dense7Pattern"<<"HorPattern"
            <<"VerPattern"<<"CrossPattern"<<"BDiagPattern"<<"FDiagPattern"<<"DiagCrossPattern";//下方4个对于边框没有意义
     // "LinearGradientPattern"<<"RadialGradientPattern"<<"ConicalGradientPattern"<<"TexturePattern"
    QString filename; // images.qrc,注意win+shift+s的截图并不是jpg格式读不出来
    foreach(const QString &style, iconnames)
    {
        filename = QStringLiteral(":/images/")+style+QStringLiteral(".jpg");
        QPixmap pix(filename);
        QPixmap p = pix.scaled(brushstyle->size(),Qt::IgnoreAspectRatio); // p.size()全部是(100,30)
        if (style == "NoBrush") p.fill(Qt::white); // NoBrush改用白色填充
        brushstyle->addItem(QIcon(p),style); // 添加图标-文字
    }
    brushstyle->setIconSize(brushstyle->size()); // 要设置,图标默认方形,这里设置为矩形长度
    brushstyle->setSizeAdjustPolicy(QComboBox::AdjustToContents);//设置这个最好,其它的略短,符合内容大小的调整策略
    brushstyle->setCurrentIndex(mChart->backgroundBrush().style());//chart.cpp默认SolidPattern

    connect(brushstyle,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){ // 0-14属于颜色填充,其它选项是渐变填充不在这里使用
            QBrush brush = mChart->backgroundBrush();
            brush.setStyle(Qt::BrushStyle(index));
            mChart->setBackgroundBrush(brush);
    });
}

void BackgroundArea::initMainLayout()
{
    QSpinBox * roundness = new QSpinBox;
    roundness->setValue(mChart->backgroundRoundness()); // chart.cpp 默认5.0
    roundness->setRange(0,1000);
    QDoubleSpinBox * granularity = new QDoubleSpinBox;
    granularity->setSingleStep(0.1);
    granularity->setDecimals(1);
    granularity->setRange(0.0,1.0);
    granularity->setValue(mChart->boundingRegionGranularity());
    connect(roundness,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,[=](int val){
            mChart->setBackgroundRoundness((qreal)val);
    });
    connect(granularity,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double val){
            mChart->setBoundingRegionGranularity(val);
    });

    QGroupBox* radiogroup = new QGroupBox;
    QRadioButton * colorBtn = new QRadioButton(tr("颜色填充"));
    QRadioButton * gradientBtn = new QRadioButton(tr("渐变填充"));
    colorBtn->setChecked(true);
    QHBoxLayout * radiolay = new QHBoxLayout;
    radiolay->addWidget(colorBtn);
    radiolay->addWidget(gradientBtn);
    radiogroup->setLayout(radiolay);

    QGroupBox* colorgroup = new QGroupBox;
    QPushButton * setColorBtn = new QPushButton(tr("设置颜色"));
    QComboBox * brushstyle = new QComboBox;
    initBrushStyleComboBox(brushstyle);
    QHBoxLayout * colorlay = new QHBoxLayout;
     colorlay->addWidget(setColorBtn);
     colorlay->addWidget(brushstyle);
     colorgroup->setLayout(colorlay);
     mBackgroundAreaGradient->setEnabled(false);

    connect(colorBtn,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            colorgroup->setEnabled(checked);
            mBackgroundAreaGradient->setEnabled(!checked);
            if (checked) {
                mGradientType = QGradient::NoGradient;//切成颜色填充
                mBackgroundAreaGradient->closeChildrenWindows(); // 要关闭渐变填充的所有子窗口
                mChart->setBackgroundBrush(QBrush(QColor("#0776a9"),Qt::DiagCrossPattern)); // 和chart.cpp保持一致的颜色填充默认背景
            }
    });

    connect(gradientBtn,static_cast<void (QRadioButton::*)(bool)>(&QRadioButton::clicked),
            this,[=](bool checked){
            colorgroup->setEnabled(!checked);
            mBackgroundAreaGradient->setEnabled(checked);
            if (checked) {
                mGradientInterpolation = mBackgroundAreaGradient->selectedGradientInterpolation();// 使能的改变不要改变,保持上次选择的
                mGradientCoordinate = mBackgroundAreaGradient->selectedGradientCoordinate();
                mGradientSpread = mBackgroundAreaGradient->selectedGradientSpread();
                mGradientType = mBackgroundAreaGradient->selectedGradientType();
                emit gradientTypeChanged();//更新为当前mGradientType的渐变填充默认背景
            }
    });
    connect(setColorBtn,&QPushButton::clicked,this,[=]{
        QColor initColor = mChart->backgroundBrush().color();
        QColorDialog * dlg = new QColorDialog(initColor);
        dlg->setFixedSize(800,400);
        dlg->setWindowIcon(mIcon);
        dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
        dlg->setWindowTitle(tr("设置背景颜色"));
        connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
               ,this,[=](const QColor& color){
            mChart->setBackgroundBrush(QBrush(color,Qt::BrushStyle(brushstyle->currentIndex())));} // 切换颜色时画刷样式和当前的渐变类型一致
        );
        dlg->exec(); delete dlg;
    });

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&圆角直径"),roundness);
    lay->addRow(tr("&区域粒度"),granularity);
    lay->addRow(tr("&填充方式"),radiogroup);
    lay->addRow(tr("&颜色填充"),colorgroup);
    lay->addRow(tr("&渐变填充"),mBackgroundAreaGradient);
    setLayout(lay);
}
