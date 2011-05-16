# usage: perl GCG_data.pl /path/to/xml/files /path/to/auxiliary/file
# This code is for generating the definitions of data types specified by
# the user.  The GRI class GRIDataDefines.h will include the auxiliary file.
#
# The generated code goes into the file located at /path/to/auxiliary/file,
# which should be included by GRIDataDefines.h
#
# This code generation currently supports the definition of C-style structs.
# The user must provide a name for the struct.  The user can also define an
# arbitrary number of variables (by providing the type and name) to belong
# to the struct
#
# An example of the XML style is:
#
# <datat>
#   <dataname dname = "peak" />
#      <var>
#          <vtype type = "double" vname = "value" />
#          <vtype type = "double" vname = "sigma" />
#          <vtype type = "double" vname = "rate" />
#          <vtype type = "int" vname = "chan" />
#      </var>
# </datat>
#
# This will be translated to:
#
# typedef struct{
#      double value;
#      double sigma;
#      double rate;
#      int chan;
# } peak;
#
#  All of the <datat> objects in the XML file should be encapsulated by
#  the <DataDefines> tag.
#
#
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
  print "usage: perl GCG_data.pl /path/to/xml/files /path/to/auxiliary/file\n";
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


# Bookkeeping for the header file
my $includer = "#ifndef GRIDATDEFINES_AUX_H\n";
$includer .= "#define GRIDATADEFINES_AUX_H\n";
$includer .= "\n/*\n";
$includer .= "This file includes all of the user-defined data types\n";
$includer .= "The data in this file was generated with GCG_data.pl,\n";
$includer .= "which can be found in grif/framework/util\n";
$includer .= "*/\n\n\n";

$includer .= "//Code-generated data types\n";

# loop through each XML file in the directory
foreach $file (@xml_files) {

  my $x = XML::Simple->new (ForceArray => 1);
  my $doc = $x->XMLin($file);

  foreach $datat (@{$doc->{datat}}){

    my $num_vars = $datat->{numvars}[0]->{numv};
    my $curr_struct = "typedef struct {\n";

    foreach $var (@{$datat->{var}[0]->{vtype}}){
      my $curr_var = "\t" . $var->{type} . " " . $var->{vname} . ";\n";
      $curr_struct = $curr_struct . $curr_var;
    }

    $curr_struct = $curr_struct . "} " . $datat->{dataname}[0]->{dname} . ";\n\n";
    $includer = $includer . $curr_struct;
  }


}

$includer .= "\n\n#endif // GRIDATADEFINES_AUX_H\n";

open AUXILIARY_FILE, ">", $ARGV[1] or die $!;
print AUXILIARY_FILE $includer
