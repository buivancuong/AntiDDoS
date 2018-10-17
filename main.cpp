#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <set>
#include <deque>
#include <utility>
#include <time.h>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <windows.h>
#include "IP.h"

int main() {

    std::map<std::string, IP> ipList;
    std::deque< std::pair<std::string, double> > requestDeque;

    std::fstream logFile;
    logFile.open("../http.log", std::ios::in);
    std::string line;
    std::cout << "Loading LogFile to memory, please wait ... " << std::endl;

    while (!logFile.eof()) {
        std::getline(logFile, line);
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
        requestDeque.push_back(pair);
    }
    logFile.close();

    int counter = 0;
    float deltaSlot = 0.001;
    int prevTimeStamp = static_cast<int>(requestDeque.front().second);
    int nextTimeStamp;
    std::cout << "Complete loading LogFile to memory. Begin handle." << std::endl;
    clock_t start = clock();

    while (!requestDeque.empty()){
        counter++;
        std::pair<std::string, double> requestPair = requestDeque.front();
        nextTimeStamp = static_cast<int>(requestDeque.front().second);
//        std::cout << "queue size: " << requestDeque.size() << " deltaSlot: " << deltaSlot << std::endl;

        auto iter = ipList.find(requestPair.first);
        if (iter == ipList.end()) {
            IP *newIP = new IP(requestPair.first, requestPair.second);
            newIP->setLastTrace1();
            ipList.insert(std::pair<std::string, IP>(requestPair.first, *newIP));
            deltaSlot *= 2;
        } else {
            int distanceDeltaSlot = static_cast<int>((requestPair.second - iter->second.getLastTimeStamp()) / deltaSlot);
            if (distanceDeltaSlot >= 1) {
                for (int i = 0; i < distanceDeltaSlot; i++) {
                    iter->second.shiftLeftTrace();
                }
                iter->second.setLastTrace1();
                iter->second.setLastTimeStamp(requestPair.second);
                deltaSlot *= 2;
            } else {
//                iter->second.shiftLeftTrace();
//                iter->second.setLastTrace1();
//                iter->second.setLastTimeStamp(requestPair.second); // sẽ giảm 1 nửa time nếu cmt dòng setter này.
                deltaSlot /= 2;
            }
        }
        requestDeque.pop_front();

        if (nextTimeStamp - prevTimeStamp == 5) {
//            std::cout << "prev: " << prevTimeStamp << " next: " << nextTimeStamp << std::endl;
            for (auto &iterator : ipList) {
                int distanceDeltaSlot = static_cast<int>((requestPair.second - iterator.second.getLastTimeStamp()) / deltaSlot);
                if (distanceDeltaSlot / N_BIT_SET > 1) {
                    for (int j = 0; j < distanceDeltaSlot; ++j) {
                        iterator.second.shiftLeftTrace();
                    }
                }
                if (iterator.second.getFrequency() >= 10) {
//                    iterator.second.updateListFreq();
                    std::cout << "IP: " << iterator.first << " Freq: " << iterator.second.getFrequency() << std::endl;
                }
            }
            std::cout << prevTimeStamp << " *******************************************************" << std::endl;
            prevTimeStamp = nextTimeStamp;
        }
    }

    clock_t endHandle = clock();

    std::cout << "Handle on: " << (endHandle - start) / CLOCKS_PER_SEC << std::endl;

//    std::fstream resultFile;
//    resultFile.open("../result", std::ios::out);
//
//    for (auto &item : ipList) {
//        std::string ip;
//        ip += item.first;
//        if (item.first.size() < 10) {
//            ip += "  ";
//        }
//        if (item.first.size() < 12) {
//            ip += "  ";
//        }
//        resultFile << ip << "\t\t" << item.second.printListFreq() << std::endl;
//    }
//
//    resultFile.close();
//
    clock_t end = clock();
//
//    std::cout << "Write result on: " << (double) (end - endHandle) / CLOCKS_PER_SEC << std::endl;
    std::cout << "Time: " << (double) (end - start) / CLOCKS_PER_SEC << std::endl;
    system("pause");
    return 0;
}