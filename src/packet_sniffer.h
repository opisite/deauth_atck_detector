#ifndef PACKET_SNIFFER_H
#define PACKET_SNIFFER_H

typedef struct {
    uint8_t it_version;
    uint8_t it_pad;
    uint16_t it_len;
    uint32_t it_present;
} radiotap_header_t;

typedef struct {
    uint8_t frame_ctrl[2];
    uint8_t duration_id[2];
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint8_t seq_ctrl[2];
} mgmt_frame_t;

void packet_capture_wrapper(const char *device);
void packet_capture(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);

#endif
