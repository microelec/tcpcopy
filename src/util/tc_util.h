#ifndef  TC_UTIL_INCLUDED
#define  TC_UTIL_INCLUDED

#include <xcopy.h>
#include <tcpcopy.h>


#define TCP_HDR_LEN(tcph) (tcph->doff << 2)
#define IP_HDR_LEN(iph) (iph->ihl << 2)                                                                 
#define EXTRACT_32BITS(p)   ((uint32_t)ntohl(*(uint32_t *)(p)))

#define TCP_PAYLOAD_LENGTH(iph, tcph) \
        (ntohs(iph->tot_len) - IP_HDR_LEN(iph) - TCP_HDR_LEN(tcph))

#if (TCPCOPY_UDP)
#define CHECKSUM_CARRY(x) \
        (x = (x >> 16) + (x & 0xffff), (~(x + (x >> 16)) & 0xffff))
#endif

inline uint64_t get_key(uint32_t s_ip, uint16_t s_port);
inline uint16_t get_appropriate_port(uint16_t orig_port, uint16_t add);
uint16_t get_port_by_rand_addition(uint16_t orig_port);
uint16_t get_port_from_shift(uint16_t orig_port, uint16_t rand_port,
        int shift_factor);
ip_port_pair_mapping_t *get_test_pair(ip_port_pair_mappings_t *target,
        uint32_t ip, uint16_t port);
int check_pack_src(ip_port_pair_mappings_t *target, uint32_t ip, 
        uint16_t port, int src_flag);
unsigned char *cp_fr_ip_pack(tc_ip_header_t *ip_header);
inline bool tcp_seq_before(uint32_t seq1, uint32_t seq2);
unsigned short csum (unsigned short *packet, int pack_len);
unsigned short tcpcsum(unsigned char *iphdr, unsigned short *packet,
        int pack_len);
#if (TCPCOPY_UDP)
void udpcsum(tc_ip_header_t *ip_header, tc_udp_header_t *udp_packet);
#endif
#if (TCPCOPY_PCAP)
int retrieve_devices(char *raw_device, devices_t *devices);
char *construct_filter(int flag, uint32_t ip, uint16_t port, char *filter);
#endif

int get_l2_len(const unsigned char *frame, const int pkt_len, 
        const int datalink);
unsigned char *
get_ip_data(pcap_t *pcap, unsigned char *frame, const int pkt_len, 
        int *p_l2_len);

#if (TCPCOPY_PCAP_SEND)
inline void fill_frame(struct ethernet_hdr *hdr, 
        unsigned char *smac, unsigned char *dmac);
#endif

#endif   /* ----- #ifndef TC_UTIL_INCLUDED  ----- */

