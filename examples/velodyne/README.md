# Velodyne HDL32E acquisition code

This project acquires data from the Velodyne HDL-32E lidar. It then converts
that data into 3 dimensional coordinates centered at the lidar. These datapoints
are then saved using the hdf5 libraries.

In addition to the standard framework dependancies this requires:

* HDF5 with c++ bindings
* libpcap

It is currently only linux compatible. In addition the program must be run as 
root in order to have access to the ethernet card.

