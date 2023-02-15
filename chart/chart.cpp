#include "chart.h"

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags flags)
    : QChart(QChart::ChartTypeCartesian, parent, flags) // 笛卡尔坐标系
{
        setAcceptHoverEvents(true);

        legend()->setBackgroundVisible(false);
        legend()->setAlignment(Qt::AlignTop);
        legend()->setFont(QFont("Times New Roman",12));
        legend()->setBorderColor(Qt::darkGray);
        //legend()->setLabelBrush(QBrush(Qt::yellow,Qt::SolidPattern)); // 对于文字来说brushstyle没有意义
        legend()->setLabelColor(Qt::blue);//图例文字颜色统一用color即可,这2个会相互覆盖
        legend()->setMarkerShape(QLegend::MarkerShapeRectangle);
        legend()->setGeometry(QRectF(345.,80.,300.,60.));//实际返回QRectF(345,80 26x26)没有作用,但是还是设定一个期望的值
        legend()->update();
        //qDebug()<<legend()->geometry();
        //legend()->setMaximumSize(QSize(300,100)); // 这样图例无法居中
        //legend()->moveBy(0,0); // setPos(pos() + QPointF(dx, dy)),qDebug()<<legend()->pos()<<legend()->scenePos();
        legend()->show();
        setFont(QFont("Times New Roman",12));
        setTitleFont(QFont("Times New Roman",20));
        setTitleBrush(QBrush(Qt::green,Qt::CrossPattern));
        setDropShadowEnabled(true);
        setBackgroundVisible(true);
        setBackgroundRoundness(5.0); // 圆角直径,默认也是5.0
        setBoundingRegionGranularity(0.0); // 边界区域的精细程度(粒度),最低0最高1,默认0
        setBackgroundBrush(QBrush(QColor("#0776a9"),Qt::DiagCrossPattern)); // 背景默认浅蓝色,对角交叉网格好看
        setBackgroundPen(QPen(QBrush(QColor("#0776a9"),Qt::SolidPattern),5,
                              Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); // 背景方框的设置
        setPlotAreaBackgroundVisible(true);
       setPlotAreaBackgroundBrush(QBrush(QColor("#00aa7f"),Qt::CrossPattern)); // 绘图区域草绿色,交叉风格好看
       setPlotAreaBackgroundPen(QPen(QBrush(QColor("#00aa7f"),Qt::SolidPattern),5,
                                     Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin)); // 边框画刷没有意义,实线好看些
        setAnimationOptions(QChart::SeriesAnimations); // 缩放只让曲线有动画效果
        setAnimationEasingCurve(QEasingCurve(QEasingCurve::Linear)); // 默认曲线是线性
        setAnimationDuration(100); // 默认100ms
        //setTheme(QChart::ChartThemeBlueNcs); //主题会覆盖先前所有的设置
}
