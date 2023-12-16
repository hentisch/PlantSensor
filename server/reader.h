#ifndef reader_H
#define reader_H

// #include "Arduino.h"
// #include "Wire.h"

class Reader{
public: 
    virtual ~Reader()=0;
    int* read_data(int register);
};
#endif