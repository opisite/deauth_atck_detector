#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <thread>
#include <chrono>
#include <pcap.h>

#include "packet_sniffer.h"
#include "deauth_detector.h"

using namespace std;

int main(int argc, char* argv[]) {
    const char *device = "wlan1";

    thread captureThread(packet_capture_wrapper, device);
    thread detectThread(detect_worker);

    while(1) {
        cout << "online" << endl;
        this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}
