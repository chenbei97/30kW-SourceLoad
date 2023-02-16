#include "radialgradientparamwidget.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

RadialGradientParamWidget::RadialGradientParamWidget(QWidget*parent)
    :GradientParamWidget (QGradient::RadialGradient,parent)
{
        // 径向渐变参数需要额外的布局去设置radius、center、centerradius、focal、focalradius
        // 测试发现radius就是centerradius; focal-radius设置和返回的不是一个值,可能不起作用,可能返回inf和nan,也可能直接变成2(似乎是最大值)
        // 所以不把这2个参数提供给用户设置了,因为我也没弄明白,文档也没有明确的含义
        addGradientDescription(tr("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 过渡线 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
                                  "9.简单径向渐变的特点是在焦点和围绕它的圆上的端点之间插入颜色,扩展径向渐变在焦点和中心圆之间插入颜色,过渡效果是辐射的."
                                  "径向渐变提供额外3个参数:渐变中心半径center-radius,渐变中心center,渐变焦点focal.\n"
                                  "10.center-radius表示辐射的半径,一般不超过绘图区域本身的宽度,超过的话不会有错误但是没有意义.默认值1.0.\n"
                                  "11.center默认在绘图区域左上角(0,0),取值(0,center-radius)或者(center-radius,0)会起到与center-radius的设置抵消的作用."
                                  "12.focal在简单径向渐变中位于中心圆内,在扩展径向渐变中可以具有任何位置,"
                                  "如果希望得到对称的渐变效果,center和focal坐标要相等.\n"
                                  "13.这些参数设置的上限理论上没有限制,但是仍然需要规定一个上限值,开发者设定为20000.\n"
                                  "14.注意:参数动态可视化不代表已设置该参数,设置完成按钮具有最终确定权."
                                 ));
        QCheckBox* checkbox = new QCheckBox;
        checkbox->setCheckState(Qt::Checked);
        checkbox->setToolTip(tr("注意:参数动态可视化不代表已设置该参数,设置完成按钮具有最终确定权"));
        mIsRealTime = true;
        connect(checkbox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                this,[=](int state){ mIsRealTime = state == Qt::Checked;});
        insertGradientWidget(0,checkbox);

        GradientParamSpinBox * center_radius = new GradientParamSpinBox;
        GradientParamSpinBox * center_x = new GradientParamSpinBox;
        GradientParamSpinBox * center_y = new GradientParamSpinBox;
        GradientParamSpinBox * focal_x = new GradientParamSpinBox;
        GradientParamSpinBox * focal_y = new GradientParamSpinBox;
        center_radius->setValue(1.0);//默认1.0

        QGroupBox * pointbox = new QGroupBox(tr("参数设置"));
        QFormLayout * pointlay = new QFormLayout;
        pointlay->addRow(tr("&是否参数动态可视化?"),checkbox);
        pointlay->addRow("&center-radius",center_radius);
        pointlay->addRow("&center-x",center_x);
        pointlay->addRow("&center-y",center_y);
        pointlay->addRow("&focal-x",focal_x);
        pointlay->addRow("&focal-y",focal_y);
        pointbox->setLayout(pointlay);

        QHBoxLayout * lay = new QHBoxLayout;
        lay->addWidget(pointbox);

        insertGradientLayout(1,lay);

        mRadialParam["center-radius"] = 0.;
        mRadialParam["center-x"] = 0.;
        mRadialParam["center-y"] = 0.;
        mRadialParam["focal-x"] = 0.;
        mRadialParam["focal-y"] = 0.;

        connect(center_radius,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double radius){mRadialParam["center-radius"] = radius;
            emit radialGradientParams(mRadialParam,mIsRealTime,false);});
        connect(center_x,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double x){mRadialParam["center-x"] = x;
            emit radialGradientParams(mRadialParam,mIsRealTime,false);});
        connect(center_y,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double y){mRadialParam["center-y"] = y;
            emit radialGradientParams(mRadialParam,mIsRealTime,false);});
        connect(focal_x,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double x){mRadialParam["focal-x"] = x;
            emit radialGradientParams(mRadialParam,mIsRealTime,false);});
        connect(focal_y,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double y){mRadialParam["focal-y"] = y;
            emit radialGradientParams(mRadialParam,mIsRealTime,false);});

        connect(mOkBtn,&QPushButton::clicked,this,[=]{
            emit radialGradientParams(mRadialParam,false,true);
        });
}
