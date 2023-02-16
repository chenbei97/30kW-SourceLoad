#include "lineargradientparamwidget.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LinearGradientParamWidget ::LinearGradientParamWidget(QWidget*parent):
    GradientParamWidget (QGradient::LinearGradient,parent)
{
        // 线性渐变参数使用默认的布局设置即可，无需额外的布局
    addGradientDescription(tr("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 过渡线 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n"
                              "9.线性渐变的特点是在起点和终点之间插入颜色,过渡效果是线性的.\n"
                              "10.线性渐变没有额外参数设置,与径向和锥形渐变不同."));
}
