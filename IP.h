//
// Created by acsim on 9/30/2018.
//

#ifndef ANTIDDOS_IP_H
#define ANTIDDOS_IP_H

#include <iostream>
#include <bitset>
#include <string>

static const int N_BIT_SET = 1024;

class IP {
private:

    double lastTimeStamp;
    std::string srcIP;
    int frequency;
    std::bitset<N_BIT_SET> trace;

public:
    IP();

    explicit IP(std::string srcIP);

    IP(std::string srcIP, double lastTimeStamp);

    ~IP();

    std::string getSourceIP();

    double getLastTimeStamp();

    int getFrequency();

    std::bitset<N_BIT_SET> getTrace();

    int getTraceLast();

    void setSourceIP(std::string sourceIP);

    void setLastTimeStamp(double lastTimeStamp);

    void setLastTrace1();

    void setLastTrace0();

    void shiftLeftTrace();

};


#endif //ANTIDDOS_IP_H
