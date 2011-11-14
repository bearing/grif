# Gamma-ray Imaging Framework (GRIF)

GRIF is a software framework for real-time gamma-ray imaging written in C++ using Qt.  The software is 
multi-threaded and multi-platform.  The model for application developers is built around the separation of
data acquisition (DAQ) and analysis units.  Users are expected to use the APIs for GRIDAQThread
and GRIAnalysisThread to build their applications.

Memory is managed by GRIF, so the user does not need to worry about allocating and de-allocating
memory for data.  Use the PostData() and ReadData() API calls from GRIDAQThread and GRIAnalysisThread
to move data around the system.

GRIF uses XML for determining data dependences between DAQ and analysis units in the system.  XML can
also be used to provide non-standard data types to the GRIF system.  For example, if you want to pass
structs looking like:

struct {
  double energy;
  double timestamp;
}

then you can provide the appropriate XML file for GRIF to handle these types of objects.  GRIF uses
a suite of python scripts for code generation.

For example applications, see the grif/trunk/examples directory.

## Dependencies

GRIF assumes the following libraries:

* [Qt](http://qt.nokia.com/)
* [Root](http://root.cern.ch/drupal/)

## Contact

* Dan Chivers (chivers@berkeley.edu)
* Mark Bandstra (bandstra@berkeley.edu)
* Austin Benson (arbenson@berkeley.edu)

## TODO

* Get simulator running correctly (arbenson)
* Formalized unit tests (amidvidy)
* Put QtUtilTest in with test directory (amidvidy)
* GRILogger back up to date (baugarten)
* Install script (amidvidy)
* Put licensing stuff into each file (anyone)
* Client/server (arbenson)