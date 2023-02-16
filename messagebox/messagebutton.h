#ifndef MESSAGEBUTTON_H
#define MESSAGEBUTTON_H

#include <QPushButton>

class MessageButton : public QObject
{
    Q_OBJECT
public:
    explicit MessageButton();
    explicit MessageButton(const QString&);
    explicit MessageButton(int);
    explicit MessageButton(const QString&,int);
    ~MessageButton();

    void setButton(QPushButton*);
    QPushButton* button() const;

    void setButtonName(const QString&);
    QString buttonName() const;

    void setId(int);
    int id() const;

private:
    QPushButton * _button;
    int _id = 0;

signals :
    void clicked(bool checked = false);
};

#endif // MESSAGEBUTTON_H
