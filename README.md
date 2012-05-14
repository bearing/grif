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

Also be sure to check out the [GRIF github wiki](https://github.com/bearing/grif/wiki), which has installation
instructions and supplementary documentation.

## Download

To download GRIF, just use:

    git clone git@github.com:bearing/grif.git

## Dependencies

GRIF assumes the following libraries:

* [Qt](http://qt.nokia.com/)
* [ROOT](http://root.cern.ch/drupal/)

## Contact

* Dan Chivers (chivers@berkeley.edu)
* Mark Bandstra (bandstra@berkeley.edu)
* Austin Benson (arbenson@berkeley.edu)

## Publications

If you use our software for your research, please cite the following reference:

* A\. Benson, M\. S\. Bandstra, D\. H\. Chivers, T\. Aucott, B\. Augarten, C\. Bates, A\. Midvidy, R\. Pavlovsky, J\. Siegrist, K\. Vetter. _The Gamma-Ray Imaging Framework._ Proceedings of the IEEE Symposium on Radiation Measurements and Applications (SORMA), Oakland, California, May 14--17, 2012. _[in preparation]_

## Acknowledgements

This project was funded through the [Domestic Nuclear Threat Security (DoNuTS) Initiative](http://donuts.berkeley.edu/) at the [University of California Berkeley](http://berkeley.edu/). The DoNuTS grant was awarded by the [United States Department of Homeland Security](http://www.dhs.gov/)'s Domestic Nuclear Detection Office (DNDO) Academic Research Initiative (ARI) program, Grant Award Number 2008-DN-077-ARI-001-02.
