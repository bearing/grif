#ifndef VELODYNEDATA_H
#define VELODYNEDATA_H

#include <sys/types.h>


/**
  Author: Cameron Bates cameron.r.bates@gmail.com
  This header has all the data structures that need to be shared
  by multiple classes to acquire and analyze daq data from the Velodyne HDL-32E


  */
struct distancedata_t {
        u_short distance;
        u_char intensity;
};
struct firingdata_t {
        u_short identifier; //endianness is flipped for all pairs aka all ushorts
        u_short rotationalposition;
        struct distancedata_t distancedata[32];
};
struct velodynedatablock_t {
        struct firingdata_t firingdata[12];
        unsigned long long timestamp;
};


struct velodynepointcloud_t {
        double x;
        double y;
        double z;
        unsigned long long timestamp;
};


#endif // VELODYNEDATA_H
