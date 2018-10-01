#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
//#include <windows.h>
#include "IP.h"

void updateScreen(std::map<std::string, IP> ipList) {
    system("CLS");
    int i = 0;
    for (auto &it : ipList) {
        if (it.second.getFrequency() > 20) {
            i++;
            std::cout << i << ": " << it.first << " => " << it.second.getLastTimeStamp() << " " << "Freq: " << it.second.getFrequency() << std::endl;
        }
    }
}

int main() {

    std::map<std::string, IP> ipList;

//    std::thread print(updateScreen, ipList);

    std::fstream logFile;
    logFile.open("../http.log");
    std::string line;

    while (!logFile.eof()) {
        std::getline(logFile, line);
        std::vector<std::string> request;
        boost::split(request, line, boost::is_any_of(" "));
        // request[1] : time stamp
        // request [7] :: srcIP

        // if is new IP, add to map
        auto iter = ipList.find(request[7]);
        if (iter == ipList.end()) {
            IP *newIP = new IP(request[7], request[1]);
            ipList.insert(std::pair<std::string, IP>(request[7], *newIP));
        } else { // find on map
            (*iter).second.setLastTrace1();
            (*iter).second.shiftLeftTrace();
            for (auto it = ipList.begin(); it != ipList.end(); it++ ) {
                if (it != iter) {
                    (*it).second.setLastTrace0();
                    (*it).second.shiftLeftTrace();
                }
            }
        }
        updateScreen(ipList);
//        Sleep(10);
    }

    logFile.close();
    return 0;
}



