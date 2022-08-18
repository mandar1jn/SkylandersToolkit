#ifndef RWCOMMAND_H
#define RWCOMMAND_H


class RWCommand
{
public:
    unsigned char writeBuffer[0x21];
    unsigned char readBuffer[0x20];

    RWCommand();
    ~RWCommand();

    bool SendUnverified();
    bool SendVerified(char expectedChar, unsigned int attempts);
};

#endif // RWCOMMAND_H
