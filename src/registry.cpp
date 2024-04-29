#include <iostream>

#include "registry.h"

using namespace std;

DeauthRegistry::DeauthRegistry() : running(true), worker([this] { registryManager(); }) {
    alert = false;
}

DeauthRegistry::~DeauthRegistry() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}

void DeauthRegistry::addOrUpdateEntry(const string& macAddress) {
    auto it = registry.find(macAddress);
    if (it != registry.end()) {
        Entry *value = &it->second;
        value->ttl = TIME_TO_LIVE;
    } else {
        lock_guard<mutex> lock(registryMutex);
        auto &entry = registry[macAddress];

        entry.ttl = TIME_TO_LIVE;
        entry.startTime = chrono::steady_clock::now();
    }
}

void DeauthRegistry::registryManager() {
    while (running) {
        this_thread::sleep_for(chrono::seconds(1));
        lock_guard<mutex> lock(registryMutex);
        chrono::steady_clock::time_point now = chrono::steady_clock::now();
        for (auto it = registry.begin(); it != registry.end(); ) {
            auto elapsed = chrono::duration_cast<chrono::seconds>(now - it->second.startTime).count();
            if (--it->second.ttl <= 0) {
                it = registry.erase(it);
            } else if(elapsed > EXIST_THRESHOLD) {
                cout << "ALERT ALERT ALERT ALERT ALERT ALERT ALERT" << endl;
                it++;
                alert = true;
            } else {
                it++;
            }
        }
    }
}

bool DeauthRegistry::getAlert() {
    return alert;
}
