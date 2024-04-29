#include <unordered_map>
#include <mutex>
#include <string>
#include <thread>

#define TIME_TO_LIVE 5
#define EXIST_THRESHOLD 15

using namespace std;

class DeauthRegistry {
private:
    struct Entry {
        chrono::steady_clock::time_point startTime;
        int ttl;
    };

    unordered_map<string, Entry> registry;
    mutex registryMutex;
    bool running;
    bool alert;
    thread worker;

    void registryManager();

public:
    DeauthRegistry();
    ~DeauthRegistry();

    void addOrUpdateEntry(const string &macAddress);
    void checkAndRemoveExpired();
    bool getAlert();
};
