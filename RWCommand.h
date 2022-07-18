#ifndef RWCOMMAND_H
#define RWCOMMAND_H


class RWCommand
{
public:
    unsigned char writeBuffer[0x21];
    unsigned char readBuffer[0x20];

    RWCommand();
    ~RWCommand();
};

#endif // RWCOMMAND_H
