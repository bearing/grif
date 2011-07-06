# usage: perl GCG_GRIUserLoader.pl /path/to/xml/files /path/to/auxiliary/file
# This is for generating code for the framework file GRIUserLoader.cpp.
# The generated code goes into the file located at /path/to/auxiliary/file
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
  print "usage: perl GCG_GRIUserLoader.pl /path/to/xml/files /path/to/auxiliary/file\n";
  exit 0;
}

# Get all xml files
my $xml_file_directory = $ARGV[0];
my @xml_files = ();
find(\&wanted, $xml_file_directory);
sub wanted{
  #if the file contains .XML or .xml, push on xml_files
  #push(@xml_files, $File::Find::name) if (/.+\.xml/ or /.+\.XML/);
  push(@xml_files, $File::Find::name) if (/\.xml$/ or /.\.XML$/);
}

my $selector = "#include \"GRIUserLoaderAux.h\"\n\n";

$selector .= "//GCG process selector code for GRIUserLoader.cpp\n";
$selector .= "GRIProcessThread *get_new_process(QString class_name, QString instance_name){\n\n";
$selector .= "\tGRIProcessThread *p = NULL;\n\n";


# loop through each XML file in the directory
foreach $file (@xml_files) {

  my $x = XML::Simple->new (ForceArray => 1);
  my $doc = $x->XMLin($file);

  # get the class name
  my $class_name = $doc->{Name}[0]->{cname};

  $selector .= "\tif(class_name.contains(\"$class_name\")){\n";
  $selector .= "\t\tp = new $class_name();\n";
  $selector .= "\t\treturn p;\n";
  $selector .= "\t}\n\n";

}

$selector .= "\treturn p;\n";
$selector .= "}\n";


open AUXILIARY_FILE, ">", $ARGV[1] or die;
print AUXILIARY_FILE $selector
