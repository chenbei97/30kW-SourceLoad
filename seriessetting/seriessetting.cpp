#include "seriessetting.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesSetting::SeriesSetting(QChart*chart): mChart(chart),mCurrentSeries(nullptr)
{
    setObjectName("SeriesSettingsLayout");

    mCurrentSeries = mChart->series().at(0);

    mWhichSeries = new QGroupBox(tr("当前曲线"));
    initWhichSeries();
    mSeriesInfo = new SeriesInfo(mChart);

    addWidget(mWhichSeries);
    addWidget(mSeriesInfo);

    connect(mSeriesInfo,SIGNAL(nameChanged(const QString&,int)), //曲线改名要让选择曲线的combo同步更改
            this,SLOT(onNameChanged(const QString&,int)));
}

void SeriesSetting::updateGenericSetting()
{
    mCurrentSeriesCombo->clear(); // clear会发出currentIndexChanged信号

    if (!mChart->series().count())  {mSeriesInfo->disconnectAllConnections();return;} // 清除所有曲线时会导致at(0)越界,提前返回

    mCurrentSeries=mChart->series().at(0);
    foreach(QAbstractSeries* series, mChart->series())
        mCurrentSeriesCombo->addItem(series->name()); // addItem也会发出currentIndexChanged信号
}

void SeriesSetting::initWhichSeries()
{
    mCurrentSeriesCombo = new QComboBox;
    foreach(QAbstractSeries* series, mChart->series()) mCurrentSeriesCombo->addItem(series->name());
    mRemoveSeriesBtn = new QPushButton(tr("移除"));

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&切换曲线"),mCurrentSeriesCombo);
    lay->addRow(tr("&移除曲线"),mRemoveSeriesBtn);

    mWhichSeries->setLayout(lay);
    mWhichSeries->setMinimumWidth(450);

    connect(mCurrentSeriesCombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[=](int index){
            //qDebug()<<index<<"  "<<mChart->series().count(); // clear = -1    1
            if (index < 0) return; // clear会导致index=-1从而越界错误; addItem时再更新
            mCurrentSeries = mChart->series().at(index);
            mSeriesInfo->setCurrentSeries(mCurrentSeries,index);
            emit seriesToggled(index); // 通用设置切换当前曲线后告知LineSeriesSetting的特定设置进行更新
    });
    connect(mRemoveSeriesBtn,&QPushButton::clicked,this,[=]{
            int index = mCurrentSeriesCombo->currentIndex();
            if(index < 0 || index >= mChart->series().count()) return; // 防止越界

           QAbstractSeries *  series = mChart->series().at(index);
            mChart->removeSeries(series); // 把当前指向的曲线移除
            mCurrentSeriesCombo->removeItem(index); // 对应项也移除
            if (index > 0) --index;// 因为移除了1项,index传递给下边时要减一否则移除最后1个曲线index=2,count=2会越界
            // 但是移除第1个曲线时index=0,不能再--,自动还是第0个曲线,index不能<0
            emit seriesRemoved(series); // 移除该曲线后让表格对应的列也移除颜色变为白色

            if (mChart->series().count() == 0) return ; // 如果没有曲线seriesline不必再调整
            mCurrentSeries = mChart->series().at(index);
            emit seriesRemoved(index);// 让combo自动变为移除的上1个
    });
}

void SeriesSetting::onNameChanged(const QString& name,int index)
{
    mCurrentSeriesCombo->setItemText(index,name);
}

void SeriesSetting::closeChildrenWindows()
{

}
