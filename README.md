# Gamma-ray Imaging Framework (GRIF)

GRIF is a software framework for real-time gamma-ray imaging written in C++ using Qt.  The software is 
multi-threaded and multi-platform.  The model for application developers is built around the separation of
data acquisition (DAQ) and analysis units.  Users are expected to use the APIs for GRIDAQThread
and GRIAnalysisThread to build their applications.

Memory is managed by GRIF, so the user does not need to worry about allocating and de-allocating
memory for data.  Use the PostData() and ReadData() API calls from GRIDAQThread and GRIAnalysisThread
to move data around the system.

GRIF uses XML for determining data dependences between DAQ and analysis units in the system.  XML can
also be used to provide non-standard data types to the GRIF system.

For example applications, see the `grif/examples` directory.

## Dependencies

GRIF assumes the following libraries:

* [Qt](http://qt.nokia.com/)
* [ROOT](http://root.cern.ch/drupal/)

## Contact

* Dan Chivers (chivers@berkeley.edu)
* Mark Bandstra (bandstra@berkeley.edu)
* Austin Benson (arbenson@berkeley.edu)
