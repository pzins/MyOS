#ifndef __DRIVER_H
#define __DRIVER_H

class Driver
{
public:
    Driver();
    ~Driver();

    virtual void Activate();
    virtual int Reset();
    virtual void Deactivate();
};

class DriverManager
{
private:
    Driver* drivers[255];
    int num_drivers;

public:
    DriverManager();
    void AddDriver(Driver*);
    void ActivateAll();
};


#endif
