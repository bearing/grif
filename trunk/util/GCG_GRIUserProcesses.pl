# usage: perl GCG_GRIUserProcesses.pl /path/to/xml/files /path/to/auxiliary/file
# This code is for generating the necessary #include statements for 
# GRIUserProcceses.h.
# The generated code goes into the file located at /path/to/auxiliary/file,
# which should be GRIUserProcceses.h.
#
# Written by Austin Benson: arbenson @ berkeley
# Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
# Domestic Nuclear Threat Security Initiative

use XML::Simple;
use File::Find;
use Data::Dumper;

# Get xml root file path (fed through command line)
if ( $#ARGV < 1) {
  print "ERROR: improper format\n";
  print "usage: perl GCG_GRIUserProcesses.pl /path/to/xml/files /path/to/auxiliary/file\n";
  exit 0;
}

# Get all xml files
my $xml_file_directory = $ARGV[0];
my @xml_files = ();
find(\&wanted, $xml_file_directory);
sub wanted{
  #if the file contains .XML or .xml, push on xml_files
  push(@xml_files, $File::Find::name) if (/.+\.xml/ or /.+\.XML/);
}


my $includer = "#ifndef GRIUSERPROCESSES_H\n";
$includer .= "#define GRIUSERPROCESSES_H\n";
$includer .= "\n/*\n";
$includer .= "This file includes all of the necessary user header files\n";
$includer .= "The data in this file was generated with GCG_GRIUserProccesses.pl,\n";
$includer .= "which can be found in grif/framework/util\n";
$includer .= "*/\n\n\n";

$includer .= "//Code-generated includes (harvested from XML files => Header tag)\n";

# loop through each XML file in the directory
foreach $file (@xml_files) {

  my $x = XML::Simple->new (ForceArray => 1);
  my $doc = $x->XMLin($file);

  # get the header name
  my $header_name = $doc->{Header}[0]->{hname};

  $includer .= "#include \"$header_name\"\n";

}

$includer .="\n\n#endif // GRIUSERPROCESSES_H\n";

open AUXILIARY_FILE, ">", $ARGV[1] or die;
print AUXILIARY_FILE $includer
