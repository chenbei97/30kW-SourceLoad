#include "chartview.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartView::ChartView(QChart *chart, QWidget *parent) :QChartView(chart,parent),mChart(chart)
{
    mMouseMove = false;
    //setRubberBand(QChartView::RectangleRubberBand);//可以通过画方框放大,鼠标事件有自己实现无需使用
    setRenderHint(QPainter::Antialiasing); // 抗锯齿
    scene()->setFont(QFont("Times New Roman",12));
    //qDebug()<<(scene()->sceneRect()); // QRectF(-2.5,-2.5 373.5x108.5)
    setMouseTracking(true);
    mCoordX = new QGraphicsSimpleTextItem(chart);
    mCoordX->setPos(750, 100); // 这个位置是根据绘图区域估计的,其初始大小在resizeEvent是QSize(1014, 822)
    mCoordX->setText("X: ");
    mCoordY = new QGraphicsSimpleTextItem(chart);
    mCoordY->setPos(850, 100); // 放在右上角
    mCoordY->setText("Y: ");
}

void ChartView::resizeEvent(QResizeEvent *event)
{
    scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
    mChart->resize(event->size());
    mCoordX->setPos(event->size().width() - 250, 100); // 绘图区域被splitter拉伸后位置仍能自动保持右上角
    mCoordY->setPos(event->size().width() - 150, 100);
    QChartView::resizeEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) //左键按下,说明想要拖动
    {
          mMouseMove = true;
          mMouseLastPos = event->globalPos() - pos(); // 鼠标在电脑屏幕的位置-窗口的位置等于相对窗口的位置
          //chart()->zoom(1.2); // 有干扰作用,改为双击才能放大
    }
    if (event->button() == Qt::RightButton) chart()->zoom(0.8);

    QChartView::mousePressEvent(event);
}

void ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    chart()->zoom(1.2);
    QChartView::mouseDoubleClickEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    bool isLeftButton = event->buttons() & Qt::LeftButton; // 是左键按下
    bool isDrag = (event->globalPos() - mMouseLastPos).manhattanLength()
            > QApplication::startDragDistance();//且确实在拖动

    if (mMouseMove && isLeftButton && isDrag)
    {
        chart()->setAnimationOptions(QChart::NoAnimation); // 拖动时候的动画效果会导致拖动手感不好先禁用
        QPoint newpos = event->globalPos()-mMouseLastPos;
        chart()->scroll(-newpos.x(),newpos.y()); // x是反方向,表向右=视图向左
        mMouseLastPos = event->globalPos()-pos();//新的相对位置
    }

    mCoordX->setText(QString("X: %1").arg(mChart->mapToValue(event->pos()).x()));//把鼠标位置转成曲线对应位置的坐标
    mCoordY->setText(QString("Y: %1").arg(mChart->mapToValue(event->pos()).y()));

    return QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    mMouseMove = false;
    chart()->setAnimationOptions(QChart::SeriesAnimations);
    QChartView::mouseReleaseEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) chart()->zoom(1.2);//y>0是向前滚对应放大
    else chart()->zoom(0.8);
    QChartView::wheelEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Plus: chart()->zoomIn();break;
        case Qt::Key_Minus: chart()->zoomOut();break;
        case Qt::Key_Left: chart()->scroll(-10, 0);break;
        case Qt::Key_Right:chart()->scroll(10, 0);break;
        case Qt::Key_Up:chart()->scroll(0, 10);break;
        case Qt::Key_Down:chart()->scroll(0, -10);break;
        default:QChartView::keyPressEvent(event);break;
    }
}
