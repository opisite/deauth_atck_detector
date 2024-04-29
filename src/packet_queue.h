#ifndef PACKET_QUEUE_H
#define PACKET_QUEUE_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

extern queue<unique_ptr<string>> destQueue;
extern mutex queueMutex; 
extern condition_variable queueCV;

#endif
