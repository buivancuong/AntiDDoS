//
// Created by acsim on 10/14/2018.
//

#ifndef ANTIDDOS_REQUEST_H
#define ANTIDDOS_REQUEST_H


#include <string>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

class Request {

private:
    std::string srcIP;
    double timeStamp;

public:
    explicit Request(std::string requestLine);

    ~Request();

    std::string getSrcIP();

    double getTimeStamp();

};


#endif //ANTIDDOS_REQUEST_H
