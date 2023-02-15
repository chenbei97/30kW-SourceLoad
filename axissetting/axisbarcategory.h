#ifndef AXISBARCATEGORY_H
#define AXISBARCATEGORY_H

#include <QGroupBox>
#include <QFormLayout>
#include <QBarCategoryAxis>
#include <QChart>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
using namespace QtCharts;

class AxisBarCategory : public QGroupBox
{
    Q_OBJECT
public:
    explicit AxisBarCategory(QChart*,QWidget*parent=Q_NULLPTR);
    void setCurrentAxis(QBarCategoryAxis*);
    QBarCategoryAxis* currentAxis() const;
    void updateState();
    void disconnectAllConnections();
private:
    QChart * mChart;
    QBarCategoryAxis * mCurrentAxis;
    QPushButton * mClearBtn;
    QPushButton * mSetBtn;
    QLineEdit * mSetEdit;
    int mCategoriesCount;
    QStringList mOldCategories;
    void updateClearState();
    void updateSetState();
private slots:
    void clearCategories();
    void setCategories();
};

#endif // AXISBARCATEGORY_H
