#ifndef VelodyneDAQ_H
#define VelodyneDAQ_H

#include "Qt"
#include "GRIDAQThread.h"

#include <stdio.h>
#include <pcap.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 This Class acquires data over ethernet from the Velodyne HDL-32E.
 Author: Cameron Bates cameron.r.bates@gmail.com
 Date: 10/18/2011
 This class uses the libpcap library to sniff packets on the local ethernet
 port eth0 and parse packets sent to the correct address. The ethernet header
 parsing section is based on the libpcap example at : http://www.tcpdump.org/pcap.html.
 It posts data as a struct of velodynedatablock_t the prototype for which can be found
 in velodynedata.h. The class velodyneanalysis converts the datablock format into x,y,z coordinates.
 */

class VelodyneDAQ : public GRIDAQThread{

public:

      VelodyneDAQ();

      int acquireData();
      int connectToDAQ();
      int initialize();
      int loadConfiguration();
      int startDataAcquisition();     //Called at the beginning each run.
      int stopDataAcquisition();      //Called at the end of each run.
      GRIDAQAccumNode* RegisterDataOutput(QString outName); //figure this out

      /******************************************************************/

private:
    void read_payload(const u_char *payload, int len);


    /* default snap length (maximum bytes per packet to capture) */
    #define SNAP_LEN 1518

    /* ethernet headers are always exactly 14 bytes [1] */
    #define SIZE_ETHERNET 14

    /* Ethernet addresses are 6 bytes */
    #define ETHER_ADDR_LEN	6

    /* Ethernet header */
    struct sniff_ethernet {
            u_char  ether_dhost[ETHER_ADDR_LEN];    /* destination host address */
            u_char  ether_shost[ETHER_ADDR_LEN];    /* source host address */
            u_short ether_type;                     /* IP? ARP? RARP? etc */
    };

    /* IP header */
    struct sniff_ip {
            u_char  ip_vhl;                 /* version << 4 | header length >> 2 */
            u_char  ip_tos;                 /* type of service */
            u_short ip_len;                 /* total length */
            u_short ip_id;                  /* identification */
            u_short ip_off;                 /* fragment offset field */
            #define IP_RF 0x8000            /* reserved fragment flag */
            #define IP_DF 0x4000            /* dont fragment flag */
            #define IP_MF 0x2000            /* more fragments flag */
            #define IP_OFFMASK 0x1fff       /* mask for fragmenting bits */
            u_char  ip_ttl;                 /* time to live */
            u_char  ip_p;                   /* protocol */
            u_short ip_sum;                 /* checksum */
            struct  in_addr ip_src,ip_dst;  /* source and dest address */
    };
    #define IP_HL(ip)               (((ip)->ip_vhl) & 0x0f)
    #define IP_V(ip)                (((ip)->ip_vhl) >> 4)

    /* TCP header */
    typedef u_int tcp_seq;

    struct sniff_udp {
            u_short th_sport;               /* source port */
            u_short th_dport;               /* destination port */
            tcp_seq th_seq;                 /* sequence number */
            tcp_seq th_ack;                 /* acknowledgement number */
    };

    QDateTime StartTime;

    pcap_t *handle;


};

#endif // VelodyneDAQ_H
