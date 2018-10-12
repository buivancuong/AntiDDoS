#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <deque>
#include <boost/algorithm/string.hpp>
//#include <thread>
#include <sstream>
#include <windows.h>
#include "IP.h"

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
    std::deque<std::string> requestDeque;

    std::fstream logFile;
    logFile.open("../http.log", std::ios::in);
    std::string line;

    // Load http.log into requestDeque
    std::cout << "Loading LogFile to memory, please wait ... " << std::endl;
    while (!logFile.eof()) {
        std::getline(logFile, line);
        requestDeque.push_back(line);
    }
    logFile.close();
    std::cout << "Complete loading LogFile to memory. Begin handle." << std::endl;

//    std::thread printScreen (updateScreen, ipList, requestDeque);
//    printScreen.join();
//    std::cout << "Continue main program without printScreen threading" << std::endl;

    // Handler on requestDeque || 00:15:38.654741000 118.70.54.240
    line = requestDeque.front();
    std::vector<std::string> oneRequest;
    boost::split(oneRequest, line, boost::is_any_of(" :"));
    std::stringstream stringHour(oneRequest[0]);
    std::stringstream stringMinute(oneRequest[1]);
    std::stringstream stringSecond(oneRequest[2]);
    long double numHour, numMinute, numSecond;
    stringHour >> numHour;
    stringMinute >> numMinute;
    stringSecond >> numSecond;
    long double beginTime = numHour * 3600 + numMinute * 60 + numSecond ; // begin Time
    float deltaSlot = 0.001; // (second): slot time = 250 microsecond
    long double endTime = beginTime + deltaSlot;

    while (!requestDeque.empty()) {
//        std::cout << "quere size: " << requestDeque.size() << std::endl;

        while (!requestDeque.empty()) {
//            std::cout << "beginTime: " << beginTime << " endTime: " << endTime << " slot: " << deltaSlot << std::endl;

            line = requestDeque.front();
            boost::split(oneRequest, line, boost::is_any_of(" :"));
            std::stringstream oneStringHour(oneRequest[0]);
            std::stringstream oneStringMinute(oneRequest[1]);
            std::stringstream oneStringSecond(oneRequest[2]);
            long double oneNumHour, oneNumMinute, oneNumSecond;
            oneStringHour >> oneNumHour;
            oneStringMinute >> oneNumMinute;
            oneStringSecond >> oneNumSecond;
            long double oneTimeStamp = oneNumHour * 3600 + oneNumMinute * 60 + oneNumSecond;

            // if the request is on deltaSlot
//            std::cout << "oneTimeStamp: " << oneTimeStamp << " endTime: " << endTime << std::endl;
            if (oneTimeStamp <= endTime) {
                // find it on ipList
                auto iter = ipList.find(oneRequest[3]);
                // if it is new IP
                if (iter == ipList.end()) {
                    // create new IP
                    IP *newIP = new IP(oneRequest[3], oneRequest[2]);
//                    std::cout << "new IP" << std::endl;
                    ipList.insert(std::pair<std::string, IP>(oneRequest[3], *newIP));
                    // and set its last trace = 1
                    newIP->setLastTrace1();
//                    std::cout << "set trace last: " << newIP->getTraceLast() << std::endl;
                } else { // if it is on ipList, set last trace for it
                    // if its last trace = 0 (not yet request)
//                    std::cout << "old IP" << std::endl;
//                    std::cout << "get trace last " << iter->second.getTraceLast() << std::endl;
                    if (iter->second.getTraceLast() == 0) {
                        // set its last trace = 1
//                        std::cout << "not conflit" << std::endl;
                        iter->second.setLastTrace1();
                    } else { // if its last trace = 1 (this is second request on a deltaSlot)
                        // devide deltaSlot/2
//                        std::cout << "conflit" << std::endl;
                        deltaSlot /= 2;
                        // and break out small loop, also update beginTime and endTime for next small loop.
                        beginTime = endTime;
                        endTime = beginTime + deltaSlot;
                        // oops, need shift left all trace!
                        for (auto &it : ipList) {
                            it.second.shiftLeftTrace();
                        }
                        break;
                    }
                }
                // discard current element on requestDeque (first element of deque)
                requestDeque.pop_front();
            } else { // the request beyond the deltaSlot
                // update beginTime and endTime for next small loop
                beginTime = endTime;
                endTime = beginTime + deltaSlot;
                // oops, need shift left all trace!
                for (auto &it : ipList) {
                    it.second.shiftLeftTrace();
                }
                // break out small loop, do not pop_front requestDeque!
                break;
            }
        }
    }

    std::cout << "Complete handle Log File. Begin write result to ResultFile." << std::endl;

    std::fstream writeResult;
    writeResult.open("result", std::ios::out);
    for (auto it : ipList) {
        std::stringstream lineStream;
        lineStream << it.first << " => " << it.second.getLastTimeStamp() << " " << "Freq: " << it.second.getFrequency() << std::endl;
        writeResult << lineStream.str();
    }
    writeResult.close();

//    while (!logFile.eof()) {
//        std::getline(logFile, line);
//        std::vector<std::string> request;
//        boost::split(request, line, boost::is_any_of(" "));
//        // request[1] : time stamp
//        // request [7] : srcIP
//
//        // if is new IP, add to map
//        auto iter = ipList.find(request[7]);
//        if (iter == ipList.end()) {
//            IP *newIP = new IP(request[7], request[1]);
//            ipList.insert(std::pair<std::string, IP>(request[7], *newIP));
//        } else { // find on map
//            (*iter).second.setLastTrace1();
//            (*iter).second.shiftLeftTrace();
//            for (auto it = ipList.begin(); it != ipList.end(); it++ ) {
//                if (it != iter) {
//                    (*it).second.setLastTrace0();
//                    (*it).second.shiftLeftTrace();
//                }
//            }
//        }
////        updateScreen(ipList);
////        Sleep(10);
//    }
//
//    logFile.close();
    std::cout << "Complete write result to Result File." << std::endl;
    return 0;
}

//int main() {
//
//    std::map<std::string, IP> ipList;
//    std::deque<std::string> requestDeque;
//
//    std::thread mainThread(mainThreading, ipList, requestDeque);
////    std::thread updateScreenThread(updateScreen);
//    mainThread.join();
////    updateScreenThread.join();
//    system("pause");
//    return 0;
//}



