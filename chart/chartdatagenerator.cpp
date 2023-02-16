#include "chartdatagenerator.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartDataGenerator::ChartDataGenerator(QObject *parent) : QObject(parent)
{
        mPen = QPen(QBrush(Qt::red,Qt::SolidPattern),2);
        //mLineChart->setUseOpenGL(true);
}

QLineSeries * ChartDataGenerator:: lineseries(const QString &name, const QColor &color)
{
    QLineSeries * series = new QLineSeries;
    series->setName(name);
    series->setColor(color);
    QPen pen = series->pen();
    pen.setWidth(2);
    series->setPen(pen);
    series->setPointLabelsFormat("((@xPoint, @yPoint))");
    series->setPointLabelsFont(QFont("Times New Roman",8));
    return series;
}

QLineSeries * ChartDataGenerator::linechart(int amplitude,int count)
{
    QLineSeries * mLineChart = new QLineSeries;
    mLineChart->setName("line chart");//曲线要设置名字,图例显示的是这个名字
    mLineChart->setPen(mPen);
    QPointF point;
    qsrand(QTime::currentTime().second());
    for(int x = 0; x <count; ++x){
        point.setX((qreal)x);point.setY(qrand() % amplitude + 1);//至少1.0,防止非0对数错误
        *mLineChart << point;// Logarithms of zero and negative values are undefined.
    }
    return mLineChart;
}

QLineSeries * ChartDataGenerator::sinechart(int amplitude, int count)
{
        QLineSeries * mSineChart = new QLineSeries;
        mSineChart->setPen(mPen);
        qsrand(QTime::currentTime().second());
        for (int i = 0; i < count; ++i) {
            QPointF p((qreal) i, qSin(Pi / 50 * i) * amplitude);
            p.ry() += qrand() % 20;
            *mSineChart << p;
        }
        return mSineChart;
}


QScatterSeries * ChartDataGenerator::scatterseries(const QString &n, const QColor &c, double size)
{
    QScatterSeries * series = new QScatterSeries;
    series->setName(n);
    series->setBorderColor(Qt::white);
    series->setColor(c);
    series->setMarkerShape(QScatterSeries::MarkerShape::MarkerShapeCircle);
    series->setMarkerSize(size);
    QPen pen = series->pen();
    pen.setWidth(2);
    series->setPen(pen);
    series->setPointLabelsFormat("((@xPoint, @yPoint))");
    series->setPointLabelsFont(QFont("Times New Roman",8));
    return  series;
}

QPieSeries * ChartDataGenerator::pie()
{
    //    QHPieModelMapper * mapper = new QHPieModelMapper(this);
    //    mapper->setModel(mTableModel);
    //    mapper->setSeries(series);
    //    mapper->setFirstColumn(0);
    //    mapper->setColumnCount(6);
    //    mapper->setLabelsRow(0);
    //    mapper->setValuesRow(1);

    //    QVPieModelMapper * mapper = new QVPieModelMapper(this);
    //    mapper->setModel(mTableModel);
    //    mapper->setSeries(series);
    //    mapper->setFirstRow(0);
    //    mapper->setRowCount(mTableModel->rowCount());
    //    mapper->setLabelsColumn(0);
    //    mapper->setValuesColumn(1);
    QPieSeries * series = new QPieSeries;
    series->append("A(10%)",0.1);
    series->append("B(15%)",0.15);
    series->append("C(20%)",0.2);
    series->append("D(50%)",0.5);
    series->append("E(5%)",0.05);
    series->setHoleSize(0.2);
    series->setPieSize(0.7);
    series->setPieStartAngle(0.);
    series->setPieEndAngle(360.);
    series->setLabelsVisible(true);
    series->setLabelsPosition(QPieSlice::LabelOutside);
    series->setHorizontalPosition(0.5);
    series->setVerticalPosition(0.5);
    series->setName("percent");
    foreach(auto s, series->slices())
    {
        s->setLabelColor(Qt::black);
        s->setLabelFont(QFont("Times New Roman",12));
        s->setBorderColor(Qt::black);
        s->setBorderWidth(2);
    }
    return series;
}

QValueAxis * ChartDataGenerator::axis(int tickcount,const QString& title,const QString& format)
{
    QValueAxis * ax = new QValueAxis;
    ax->setLabelsFont(QFont("Times New Roman",12));
    ax->setTitleText(title);
    ax->setLabelFormat(format); // 横轴是整数间隔
    ax->setTickCount(tickcount);
    ax->setTitleFont(QFont("Times New Roman",16));
    //ax->setReverse(true);

    //ax->setShadesVisible(true);
    //ax->setShadesColor(Qt::blue);
    //ax->setShadesBrush(QBrush(Qt::green)); // brush's color=shadecolor

    //ax->setShadesBorderColor(Qt::red);
    //ax->setShadesPen(QPen(QBrush(Qt::yellow),10)); // pen's color = bordercolor
    //ax->setLineVisible(true);
    //ax->setLinePenColor(Qt::red);
    //ax->setLinePen(QPen(QBrush(Qt::blue,Qt::BrushStyle::SolidPattern),5)); // pen's color = pencolor,样式还是起作用的但是没多大意义

    //ax->setGridLineVisible(true);
    //ax->setGridLineColor(Qt::red);
    //ax->setGridLinePen(QPen(QBrush(Qt::blue),5)); // pen's color = color ,minorgrid同理
    return ax;

}

QLogValueAxis* ChartDataGenerator::logaxis(qreal base,const QString& title)
{
        QLogValueAxis *ax = new QLogValueAxis;
        ax->setLabelsFont(QFont("Times New Roman",12));
        ax->setTitleText(title);
        ax->setLabelFormat("%g"); // 纵轴是对数间隔
        ax->setBase(base); // log底数
        ax->setMinorTickCount(-1); // 次要刻度线数自动计算
        ax->setTitleFont(QFont("Times New Roman",16));
        return ax;
}

QImage ChartDataGenerator::startPattern()
{
    QPainterPath starPath;
    starPath.moveTo(28, 15);
    for (int i = 1; i < 5; ++i) {
        starPath.lineTo(14 + 14 * qCos(0.8 * i * Pi),
                        15 + 14 * qSin(0.8 * i * Pi));
    }
    starPath.closeSubpath();

    QImage star(30, 30, QImage::Format_ARGB32);
    star.fill(Qt::transparent);

    QPainter painter(&star);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QRgb(0xf6a625));
    painter.setBrush(painter.pen().color());
    painter.drawPath(starPath);
    return star;
}
