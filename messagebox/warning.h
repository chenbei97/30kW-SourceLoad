#ifndef WARINGING_H
#define WARINGING_H

#include "messagebox.h"

class Warning : public Messagebox
{
    Q_OBJECT
    typedef void (Warning::*ButtonSlotClassFunc)();
    void setSlotClassFunction(ButtonSlotClassFunc, int);
public:
    explicit Warning();
};

#endif // WARINGING_H
