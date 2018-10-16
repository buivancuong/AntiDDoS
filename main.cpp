#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <time.h>
#include <boost/algorithm/string.hpp>
//#include <thread>
#include <windows.h>
#include "IP.h"
#include "Request.h"

//void updateScreen(std::map<std::string, IP> ipList) { //std::map<std::string, IP> ipList, std::deque<std::string> requestDeque
//
//    system("CLS");
//    int i = 0;
//    for (auto &it : ipList) {
//        if (it.second.getFrequency() > 0) {
//            std::cout << i << ": " << it.first << " => " << it.second.getLastTimeStamp() << " " << "Freq: " << it.second.getFrequency() << std::endl;
//            i++;
//        }
//    }
////    while (!requestDeque.empty()) {
//////        system("CLS");
////        int i = 0;
////        for (auto &it : ipList) {
////            if (it.second.getFrequency() > 0) {
////                std::cout << i << ": " << it.first << " => " << it.second.getLastTimeStamp() << " " << "Freq: " << it.second.getFrequency() << std::endl;
////                i++;
////            }
////        }
//////        Sleep(3000);
//////        std::cout << "~~~~~~ New update screen ~~~~~~" << std::endl;
////    }
//}

int main() { //std::map<std::string, IP> ipList, std::deque<std::string> requestDeque

    std::map<std::string, IP> ipList;
    std::deque< std::pair<std::string, double> > requestDeque;

    std::fstream logFile;
    logFile.open("../mini30shttp.log", std::ios::in);
    std::string line;
//    int count = 0;

    // Load http.log into requestDeque
    std::cout << "Loading LogFile to memory, please wait ... " << std::endl;
    while (!logFile.eof()) {
        std::getline(logFile, line);
//        std::cout << line << std::endl;
        std::pair<std::string, double> pair;
        std::vector<std::string> oneRequest;
        boost::split(oneRequest, line, boost::is_any_of(" :"));
        std::stringstream stringHour(oneRequest[0]);
        std::stringstream stringMinute(oneRequest[1]);
        std::stringstream stringSecond(oneRequest[2]);

        int numHour, numMinute;
        double numSecond;
        stringHour >> numHour;
        stringMinute >> numMinute;
        stringSecond >> numSecond;

        pair.second = numHour * 3600 + numMinute * 60 + numSecond;
        pair.first = oneRequest[3];

//        std::cout << requestDeque.size() << std::endl;
//        Request *request = new Request(line);
        requestDeque.push_back(pair);
//        std::cout << "Queqe size: " << sizeof(requestDeque) << std::endl;
//        delete request;
    }
    logFile.close();
    int counter = 0;
    float deltaSlot = 0.001;
    double beginTimeStamp = requestDeque.front().second;
    double currentTimeStamp = 0;
    std::cout << "Complete loading LogFile to memory. Begin handle." << std::endl;

    clock_t start = clock();

    while (!requestDeque.empty()){
        counter++;
//        std::cout << "counter = " << counter << " queue size = " << requestDeque.size() << std::endl;
        std::pair<std::string, double> requestPair = requestDeque.front();
        currentTimeStamp = requestPair.second;
        auto iter = ipList.find(requestPair.first);
        if (iter == ipList.end()) {
            IP *newIP = new IP(requestPair.first, requestPair.second);
            newIP->setLastTrace1();
            ipList.insert(std::pair<std::string, IP>(requestPair.first, *newIP));
        } else {
            int distanceDeltaSlot = static_cast<int>((requestPair.second - iter->second.getLastTimeStamp()) / deltaSlot);
            if (distanceDeltaSlot >= 1) {
                for (int i = 0; i < distanceDeltaSlot; i++) {
                    iter->second.shiftLeftTrace();
                }
                iter->second.setLastTrace1();
                iter->second.setLastTimeStamp(requestPair.second);
            } else {
                deltaSlot /= 2;
            }
        }
        requestDeque.pop_front();

        if (counter % N_BIT_SET == 0) {
            if (static_cast<int>(currentTimeStamp - beginTimeStamp) % static_cast<int>(1 / deltaSlot) == 0) {
                for (auto &it : ipList) {
                    if ((currentTimeStamp - it.second.getLastTimeStamp()) / N_BIT_SET > 1) {
                        ipList.erase(it.first);
                    }
                }
            }


            for (auto &iterator : ipList) {
                if (iterator.second.getFrequency() >= 5) {
                    std::cout << "IP: " << iterator.first << " Last: " << iterator.second.getLastTimeStamp() << " Freq: " << iterator.second.getFrequency() << std::endl;
                }
            }
            std::cout << "*******************************************************" << std::endl;
        }
    }

    clock_t end = clock();

    std::cout << "Time: " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;

    system("pause");
    return 0;
}



