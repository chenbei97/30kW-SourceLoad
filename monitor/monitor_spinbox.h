#ifndef MONITORSPINBOX_H
#define MONITORSPINBOX_H

#include <QDoubleSpinBox>

class MonitorSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    explicit MonitorSpinBox(QWidget * parent = Q_NULLPTR);
private slots:
    void on_editingFinished();
};
#endif // MONITORSPINBOX_H
