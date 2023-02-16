#include "axisbarcategory.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AxisBarCategory::AxisBarCategory(QChart * chart,QWidget*parent):
    QGroupBox(parent),mChart(chart),mCurrentAxis(nullptr),mCategoriesCount(0)
{
    mClearBtn = new QPushButton(tr("清空类别"));
    mSetBtn = new QPushButton(tr("设置类别"));
    mSetEdit = new QLineEdit;

    QVBoxLayout * lay = new QVBoxLayout;
    QHBoxLayout * btnlay = new QHBoxLayout;
    btnlay->addWidget(mClearBtn);
    btnlay->addWidget(mSetBtn);
    lay->addLayout(btnlay);
    lay->addWidget(mSetEdit);

    setLayout(lay);
    setMinimumWidth(450); // 跟随axisbox.cpp
    setTitle(tr("类别轴"));
}

void AxisBarCategory::disconnectAllConnections()
{
    disconnect(mClearBtn,&QPushButton::clicked,this,&AxisBarCategory::clearCategories);
    disconnect(mSetBtn,&QPushButton::clicked,this,&AxisBarCategory::setCategories);
}

void AxisBarCategory::setCurrentAxis(QBarCategoryAxis * axis)
{
    mCurrentAxis = axis;
    mCategoriesCount = mCurrentAxis->categories().count();
    mOldCategories = mCurrentAxis->categories();
    //qDebug()<<mOldCategories;
    updateState();
}

QBarCategoryAxis * AxisBarCategory::currentAxis() const
{
    return mCurrentAxis;
}

void AxisBarCategory::updateState()
{
    updateClearState();
    updateSetState();
}

void AxisBarCategory::updateClearState()
{
    disconnect(mClearBtn,&QPushButton::clicked,this,&AxisBarCategory::clearCategories);
    connect(mClearBtn,&QPushButton::clicked,this,&AxisBarCategory::clearCategories);
}

void AxisBarCategory::clearCategories()
{
    QStringList categories;
    for(int i = 0; i < mCategoriesCount; ++i) categories.append("");
    mCurrentAxis->clear();  // ASSERT: "width > 0.0" in file painting\qrasterizer.cpp, line 761
    mCurrentAxis->setCategories(categories);//调用clear必须设置categories,否则异常,使用空字符串体现清除效果
}

void AxisBarCategory::updateSetState()
{
    auto categories = mCurrentAxis->categories();
    QString text;
    for(auto c:categories)
    {
        text.append(c);
        text.append(',');
    }
    text.chop(1);
    mSetEdit->setText(text);
    disconnect(mSetBtn,&QPushButton::clicked,this,&AxisBarCategory::setCategories);
    connect(mSetBtn,&QPushButton::clicked,this,&AxisBarCategory::setCategories);
}

void AxisBarCategory::setCategories()
{
    auto categories = mSetEdit->text().simplified().split(",",QString::SkipEmptyParts);
    if (categories.count() < mCategoriesCount)
    {
        for(int c = categories.count(); c < mCategoriesCount; ++c)
        { // 例如原来5,现在2,还需要原来的后3个,也就是索引2,3,4,这里c=2 && <5
            categories << mOldCategories[c];
        }
    }
    else if (categories.count() > mCategoriesCount)
    { // 原来5,现在8,需要覆盖处理
        for (int c = 0 ; c < categories.count()-mCategoriesCount; ++c)
            categories.removeLast(); // 移除后3个
    }

    mCurrentAxis->clear(); // 要先清除,否则categories似乎会被保存累计
    mCurrentAxis->setCategories(categories);
    mOldCategories = categories; // 用mCurrentAxis->categories()会导致异常
}
