#ifndef UTILS_H
#define UTILS_H

#include <memory>

#define MAC_STR_SIZE 13

using namespace std;

unique_ptr<string> mac_address_to_string(uint8_t* mac);

#endif
