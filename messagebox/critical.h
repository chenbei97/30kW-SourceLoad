#ifndef CRITICAL_H
#define CRITICAL_H

#include "messagebox.h"

class Critical : public Messagebox
{
    Q_OBJECT
    typedef void (Critical::*ButtonSlotClassFunc)();
    void setSlotClassFunction(ButtonSlotClassFunc, int);
public:
    explicit Critical();
};

#endif // CRITICAL_H
