#ifndef INFOMATION_H
#define INFOMATION_H

#include "messagebox.h"

class Infomation : public Messagebox
{
    Q_OBJECT
private:
    typedef void (Infomation::*ButtonSlotClassFunc)();
    void test();
    void setSlotClassFunction(ButtonSlotClassFunc, int);
public:
    explicit Infomation();
};

#endif // INFOMATION_H
