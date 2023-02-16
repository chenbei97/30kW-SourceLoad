#include "monitor_spinbox.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

// spinbox组件想要设置固定宽度和高度
MonitorSpinBox::MonitorSpinBox(QWidget * parent):QDoubleSpinBox(parent)
{
      this->setDecimals(3);
      this->setAlignment(Qt::AlignCenter);
      this->setMouseTracking(true);
      this->setSingleStep(0.1);// 调整范围0.1
      if (parent != Q_NULLPTR)
      {
            this->setFixedSize(parent->width()/4,40); // 组件有父对象的话就是监控界面本身,设定为监控界面本身的1/4宽度
      }
      // 当用户编辑完成点击其它组件(失去焦点)或者敲下回车时就会触发editingFinished信号
      // 紧接着该信号触发valueChanged信号,这样外部相当于间接的响应回车,达到一种丝滑的效果
      // 至于valueChanged和setValue的连接本来也是连接的,即使这里不设定父类也已设定过
      connect(this,SIGNAL(editingFinished()),this,SLOT(on_editingFinished()));
      connect(this,SIGNAL(valueChanged(double)),this,SLOT(setValue(double)));
}

void MonitorSpinBox::on_editingFinished()
{
      emit valueChanged(value());
}
