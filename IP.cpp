#include <utility>

//
// Created by acsim on 9/30/2018.
//

#include "IP.h"

IP::IP() {
    this->frequency = 0;
}

IP::IP(std::string srcIP) {
    this->srcIP = std::move(srcIP);
    this->frequency = 0;
}

IP::IP(std::string srcIP, double lastTimeStamp) {
    this->srcIP = std::move(srcIP);
    this->lastTimeStamp = lastTimeStamp;
    this->frequency = 0;
}

std::string IP::getSourceIP() {
    return this->srcIP;
}

double IP::getLastTimeStamp() {
    return this->lastTimeStamp;
}

int IP::getFrequency() {
    return this->frequency;
}

std::bitset<N_BIT_SET> IP::getTrace() {
    return this->trace;
}

void IP::setSourceIP(std::string sourceIP) {
    this->srcIP = std::move(sourceIP);
}

void IP::setLastTimeStamp(double lastTimeStamp) {
    this->lastTimeStamp = lastTimeStamp;
}

void IP::setLastTrace1() {
    trace.set(0, true);
    this->frequency++;
}

void IP::setLastTrace0() {
    trace.set(0, false);
}

void IP::shiftLeftTrace() {
    if (this->trace[N_BIT_SET - 1] == true) {
        this->frequency--;
    }
    trace<<=1;
}

int IP::getTraceLast() {
    if (this->trace[0] == true) {
        return 1;
    } else {
        return 0;
    }
}

//std::deque<int> IP::getListFreq() {
//    return this->listFreq;
//}
//
//void IP::updateListFreq() {
//    this->listFreq.push_front(this->frequency);
//}
//
//std::string IP::printListFreq() {
//    std::string string;
//    for (auto &iterator : listFreq) {
//        string += std::to_string(iterator);
//        string += "\t";
//    }
//    return string;
//}

IP::~IP() = default;
