#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <mutex>
#include <iostream>

#include "packet_sniffer.h"
#include "utils.h"
#include "deauth_detector.h"
#include "packet_queue.h"
#include "registry.h"

using namespace std;

mutex queueMutex;
condition_variable queueCV;
queue<unique_ptr<string>> destQueue;

void detect_worker() {
    DeauthRegistry registry;
    while (true) {
        unique_lock<mutex> lock(queueMutex);
        queueCV.wait(lock, []{ return !destQueue.empty(); });

        unique_ptr<string> addr = move(destQueue.front());
        destQueue.pop();
        lock.unlock();

        registry.addOrUpdateEntry(*addr);

        cout << "Deauth target: " << *addr << endl;

    }
}
