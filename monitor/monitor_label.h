#ifndef MONITORLABEL_H
#define MONITORLABEL_H

#include <QLabel>
class MonitorLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MonitorLabel(QWidget * parent = Q_NULLPTR);
    explicit MonitorLabel(const char* ="",QWidget * parent = Q_NULLPTR);
    explicit MonitorLabel(const QString & ,QWidget * parent = Q_NULLPTR);
private:
    void init();
};
#endif // MONITORLABEL_H
