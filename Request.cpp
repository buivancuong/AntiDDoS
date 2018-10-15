//
// Created by acsim on 10/14/2018.
//

#include <iostream>
#include "Request.h"

std::string Request::getSrcIP() {
    return this->srcIP;
}

double Request::getTimeStamp() {
    return this->timeStamp;
}
Request::Request(std::string requestLine) {
    std::vector<std::string> oneRequest;
    boost::split(oneRequest, requestLine, boost::is_any_of(" :"));
    std::stringstream stringHour(oneRequest[0]);
    std::stringstream stringMinute(oneRequest[1]);
    std::stringstream stringSecond(oneRequest[2]);

    int numHour, numMinute;
    double numSecond;
    stringHour >> numHour;
    stringMinute >> numMinute;
    stringSecond >> numSecond;

    this->timeStamp = numHour * 3600 + numMinute * 60 + numSecond;
    this->srcIP = oneRequest[3];

}

Request::~Request() {
//    std::cout << "Deleted!" << std::endl;
    delete this;
}
