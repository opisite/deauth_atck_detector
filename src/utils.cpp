#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

unique_ptr<string> mac_address_to_string(uint8_t* mac) {
    ostringstream stream;
    stream << hex << setfill('0');
    for (int i = 0; i < 6; ++i) {
        stream << setw(2) << static_cast<int>(mac[i]);
    }
    return make_unique<string>(stream.str());
}
