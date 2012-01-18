#include "velodynedaq.h"
#include "velodynedata.h"
#include <iostream>

int VelodyneDAQ::acquireData(int n) {
    struct pcap_pkthdr header;
    const u_char *packet;

    /* declare pointers to packet headers */
    const struct sniff_ethernet *ethernet;  /* The ethernet header [1] */
    const struct sniff_ip *ip;              /* The IP header */
    const struct sniff_udp *tcp;            /* The TCP header */
    const u_char *payload;                    /* Packet payload */

    int size_ip;
    int size_tcp;
    int size_payload;


    packet = pcap_next(handle, &header);

    /* define ethernet header */
    ethernet = (struct sniff_ethernet*)(packet);

    /* define/compute ip header offset */
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return 1;
    }

    /* determine protocol  if it's not a UDP packet it exits*/
    switch(ip->ip_p) {
        case IPPROTO_TCP:
            printf("   Protocol: TCP\n");
            return 1;
        case IPPROTO_UDP:
            break;
        case IPPROTO_ICMP:
            printf("   Protocol: ICMP\n");
            return 1;
        case IPPROTO_IP:
            printf("   Protocol: IP\n");
            return 1;
        default:
            printf("   Protocol: unknown\n");
            return 1;
    }


    /* define/compute tcp header offset */
    tcp = (struct sniff_udp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = 8;
    if (size_tcp < 8) {
        printf("   * Invalid UDP header length: %u bytes\n", size_tcp);
        return 1;
    }

    /* define/compute tcp payload (segment) offset */
    payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);

    /* compute tcp payload (segment) size */
    size_payload = ntohs(ip->ip_len) - (size_ip + size_tcp);

    //checks to make sure that the destination port is correct for the velodyne lidar data
    if ((size_payload > 0) && (ntohs(tcp->th_dport) == 2368)) {
        this->read_payload(payload, size_payload);
    }
  return 0;
}

int VelodyneDAQ::connectToDAQ() {

   char *dev, errbuf[PCAP_ERRBUF_SIZE];

    dev =  "eth0";
    printf("Device: %s\n", dev);
    //This application must have root priveleges otherwise this won't work
    handle = pcap_open_live(dev,BUFSIZ, 1, 100, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
        return(2);
    }  
  return 0;  
}

int VelodyneDAQ::initialize(){
  return 0;
}

int VelodyneDAQ::loadConfiguration(){
  return 0;
}

int VelodyneDAQ::startDataAcquisition(){
  
  this->InitializeAccumulators(QDateTime::currentDateTime(),0,1e8,2,250); //edit this

this->StartTime = QDateTime::currentDateTime();

  return 0;  //feel free to delete after implementation

}

int VelodyneDAQ::stopDataAcquisition(){

  
  pcap_close(handle);

  return 0; //feel free to delete after implementation
  
}

GRIDAQBaseAccumNode* VelodyneDAQ::RegisterDataOutput(QString outName){

  GRIDAQBaseAccumNode* p = NULL; //feel free to delete after implementation
  if(outName == "HDLRAW") p = new GRIDAQAccumulator<velodynedatablock_t>(outName,1e8,2,250);
  return p; //feel free to delete after implementation
}




void VelodyneDAQ::read_payload(const u_char *payload, int len) {
    QDateTime currentTime = QDateTime::currentDateTime();
    qint64 t1 = StartTime.time().msecsTo(currentTime.time())*1E5;  // in 10ns ticks
    velodynedatablock_t *datablock = new velodynedatablock_t[1];
    const u_char *ch = payload;
    int i,j;
    u_short temp1, temp2, temp3; //endianness is flipped for all shorts
    for (i = 0; i < 12; ++i){
        temp1 = *ch;
        ch++;
        //printf("identifier: %04x \n", (*ch <<8) + temp1);
        datablock[0].firingdata[i].identifier = (*ch <<8) + temp1 ;

        ch++;
        temp1 = *ch;
        ch++;
        datablock[0].firingdata[i].rotationalposition = (*ch <<8) + temp1 ;
        ch++;
        for(j = 0; j < 32; ++j){
            temp1 = *ch;
            ch++;
            datablock[0].firingdata[i].distancedata[j].distance = (*ch <<8) + temp1 ;

            ch++;
            datablock[0].firingdata[i].distancedata[j].intensity = *ch;
            ch++;
        }

    }
    ch++;
    temp1 = *ch;
    ch++;
    temp2 = *ch;
    ch++;
    temp3 = *ch;
    // byte order of velodyne timestamp is unclear and it will cause problems because it overflows every hour
    // so the the computers timestamp will be used instead
    //datablock[0].timestamp = (*ch<< 24) + (temp3 << 16) + (temp2 << 8) + temp1;
    datablock[0].timestamp = t1;
    qint64 * qTS = new qint64[1];
    qTS[0] = t1;//datablock[0].timestamp;

    PostData(1,QString("HDLRAW"),datablock,qTS);
    delete [] datablock;
    delete [] qTS;
  return;
}


