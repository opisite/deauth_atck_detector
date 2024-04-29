#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <unistd.h>
#include <iomanip>
#include <pcap.h>
#include <string>
#include <mutex>

#include "packet_sniffer.h"
#include "utils.h"
#include "deauth_detector.h"
#include "packet_queue.h"

using namespace std;

void packet_capture_wrapper(const char *device) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char filter_exp[] = "type mgt";
    struct bpf_program fp;
    pcap_t* handle;

    handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        cerr << "pcap_open_live() failed: " << errbuf << endl;
        return;
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        cerr << "Couldn't parse filter " << filter_exp << ": " << pcap_geterr(handle) << endl;
        pcap_close(handle);
        return;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Couldn't install filter " << filter_exp << ": " << pcap_geterr(handle) << std::endl;
        pcap_freecode(&fp);
        pcap_close(handle);
        return;
    }

    pcap_freecode(&fp);

    pcap_loop(handle, 0, packet_capture, nullptr);
    pcap_close(handle);
}

void packet_capture(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    radiotap_header_t *radiotap_header = (radiotap_header_t *)packet;
    mgmt_frame_t *mgmt_frame = (mgmt_frame_t *)((uint8_t *)radiotap_header + radiotap_header->it_len);
    
    if(mgmt_frame->frame_ctrl[0] != 0xC0) // Check for deauth frame;
        return;

    unique_ptr<string> MAC = mac_address_to_string(mgmt_frame->addr1);

    lock_guard<mutex> lock(queueMutex);
    destQueue.push(move(MAC));

    queueCV.notify_one();
}
