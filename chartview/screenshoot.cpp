#include "screenshoot.h"

ScreenShoot::ScreenShoot(QChartView*chartview,QWidget*parent):QDialog(parent),mChartView(chartview)
{
    QVBoxLayout * lay = new QVBoxLayout(this);

    QGroupBox * area = new QGroupBox(tr("截图区域"));
    QHBoxLayout * arealay = new QHBoxLayout;
    QRadioButton * fullarea = new QRadioButton(tr("全屏"));
    QRadioButton * plotarea = new QRadioButton(tr("图表"));
    fullarea->setChecked(true);
    arealay->addWidget(fullarea);
    arealay->addWidget(plotarea);
    area->setLayout(arealay);

    QGroupBox * img = new QGroupBox(tr("图像格式"));
    QFormLayout * imglay = new QFormLayout ;
    QComboBox * combo = new QComboBox;
    combo->addItems(QStringList()<<"bmp"<<"jpg"<<"jpeg"<<"png");
    QSpinBox * spinbox = new QSpinBox;
    spinbox->setSuffix("%");
    spinbox->setValue(50);
    spinbox->setRange(0,100);
    imglay->addRow(tr("&格式选择"),combo);
    imglay->addRow(tr("&输出质量"),spinbox);
    img->setLayout(imglay);

    QHBoxLayout * btnlay = new QHBoxLayout;
    QPushButton * btn = new QPushButton(tr("截图"));
    btnlay->addStretch();
    btnlay->addWidget(btn);

    lay->addWidget(area);
    lay->addWidget(img);
    lay->addLayout(btnlay);

    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/screenshot.png"));
    resize(QSize(600,300));

    connect(btn,&QPushButton::clicked,this,[=]{
        QString directioy = QFileDialog::getExistingDirectory(this,tr("图片保存路径"));
        if (directioy.isEmpty()) return;

         if (fullarea->isChecked())
         {
                 QScreen *screen = QGuiApplication::primaryScreen();
                 QString filename = directioy+"/full-"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz") +
                         "." + combo->currentText();
                 screen->grabWindow(0).save(filename,combo->currentText().toStdString().c_str(),spinbox->value());
                 //qDebug()<<filePathName;
         }
         else {
             QString filename = directioy+"/chart-"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz") +
                     "." + combo->currentText();
                mChartView->grab().save(filename,combo->currentText().toStdString().c_str(),spinbox->value());
         }
         accept();
    });
}
