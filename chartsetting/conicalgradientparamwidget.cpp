#include "conicalgradientparamwidget.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ConicalGradientParamWidget::ConicalGradientParamWidget(QWidget*parent):
    GradientParamWidget (QGradient::ConicalGradient,parent)
{
        // 锥形渐变参数需要额外的布局去设置center、angle
        addGradientDescription(tr("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 过渡线 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
                                  "9.锥形渐变的特点是围绕中心点插入颜色,过渡效果是锥形的."
                                  "锥形渐变提供额外2个参数:渐变中心center,渐变起始角度angle.\n"
                                  "10.center默认在绘图区域左上角(0,0).\n"
                                  "11.angle默认是0°,范围在[0°,360°]之间.\n"
                                  "12.注意,延展方法的设置对锥形渐变没有影响,原因是锥形梯度是封闭的,"
                                  "即锥形渐变从0到360度填充整个圆,而径向或线性渐变的边界可以分别通过其半径或最终停止点指定.\n"
                                  "13.注意:参数动态可视化不代表已设置该参数,设置完成按钮具有最终确定权."
                                 ));

        QCheckBox* checkbox = new QCheckBox;
        checkbox->setCheckState(Qt::Checked);
        checkbox->setToolTip(tr("注意:参数动态可视化不代表已设置该参数,设置完成按钮具有最终确定权"));
        mIsRealTime = true;
        connect(checkbox,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                this,[=](int state){ mIsRealTime = state == Qt::Checked;});
        insertGradientWidget(0,checkbox);

        GradientParamSpinBox * center_x = new GradientParamSpinBox;
        GradientParamSpinBox * center_y = new GradientParamSpinBox;
        GradientParamSpinBox * angle = new GradientParamSpinBox;
        angle->setRange(0.,360.0);
        angle->setSuffix("°");

        QGroupBox * pointbox = new QGroupBox(tr("参数设置"));
        QFormLayout * pointlay = new QFormLayout;
        pointlay->addRow(tr("&是否参数动态可视化?"),checkbox);
        pointlay->addRow("&center-x",center_x);
        pointlay->addRow("&center-y",center_y);
        pointlay->addRow("&angle",angle);
        pointbox->setLayout(pointlay);

        QHBoxLayout * lay = new QHBoxLayout;
        lay->addWidget(pointbox);

        insertGradientLayout(1,lay);

        mConicalParam.first = QPointF(0.,0.); // 默认
        mConicalParam.second = 0.;

        connect(center_x,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double x){mConicalParam.first.setX(x);
            emit conicalGradientParams(mConicalParam,mIsRealTime,false);});
        connect(center_y,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double y){mConicalParam.first.setY(y);
            emit conicalGradientParams(mConicalParam,mIsRealTime,false);});
        connect(angle,static_cast<void (GradientParamSpinBox::*)(double)>(&GradientParamSpinBox::valueChanged),
                this,[=](double ang){mConicalParam.second = ang;
            emit conicalGradientParams(mConicalParam,mIsRealTime,false);});
        connect(mOkBtn,&QPushButton::clicked,this,[=]{
            emit conicalGradientParams(mConicalParam,false,true);
        });
}
