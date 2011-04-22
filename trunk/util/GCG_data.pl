# usage: perl GCG_GRIUserProcesses.pl /path/to/xml/files /path/to/auxiliary/file
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
# struct peak{
#      double value;
#      double sigma;
#      double rate;
#      int chan;
# };
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
  print "ERROR: improper format";
  print "usage: perl GCG_GRIUserProcesses.pl /path/to/xml/files /path/to/auxiliary/file";
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

  #loop over each data type
  foreach $datatype ($doc->{DataDefines}->{datat}){
    my $curr_struct = "struct" . $datatype->{dataname}->{dname} . "{\n";

    #loop over each variable for the C struct
    foreach $var ($datatype->{var}){
      my $curr_type = $var->{type};
      my $curr_type_name = $var->{vname}
      $curr_struct = join($curr_struct, "\t", $curr_type, $curr_type_name, ";\n");
    }

    #format and append
    $curr_struct .= "};\n";
    $includer .= $curr_struct;

  }

}
#my $header_name = $doc->{Header}[0]->{hname};

$includer .="\n\n#endif // GRIDATADEFINES_AUX_H\n";

open AUXILIARY_FILE, ">", $ARGV[1] or die;
print AUXILIARY_FILE $includer
