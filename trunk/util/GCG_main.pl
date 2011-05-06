# usage: perl GCG.pl /path/to/xml/files /path/to/grif/skeletons
# This is a basic XML-reading code generation tool for GRIF
#
# This script looks at each .XML file in /path/to/xml/files and parses out
# serveral pieces of information:
#
# (1) Run-time parameters (including support for QVector and vector types)
# (2) Run-time actions
# (3) Initial values for run-time parameters
# (4) Class name
# (5) Header name
#
# A sample XML file would like the following:
#
# <Class>
#    <Name cname = "SIMDAQThread" isdaq = "true" />
#    <Auxiliary apath = "/Users/benson/Desktop/GCG_test/GCG_daq_aux.h" />
#    <Header hname = "SIMDAQThread.h" />
#    <Skeleton spath = "/Users/benson/Desktop/GCG_test" />
#    <RunTimeParams>
#        <Param pname = "e1" type = "double" default = "10.5" />
#        <Param pname = "ind" type = "int" default = "2" />
#        <Param pname = "e1_vec" type = "vector &lt; double &gt;" />
#        <Param pname = "ind_vec" type = "vector &lt; int &gt;" />
#        <Param pname = "e1_qvec" type = "QVector &lt; double &gt;" />
#        <Param pname = "ind_qvec" type = "QVector &lt; int &gt;"  />
#    </RunTimeParams>
#    <RunTimeActions>
#        <Action method = "DynamicAddPeak" />
#        <Action method = "DynamicRemovePeak" />
#        <Action method = "DynamicChangePeak" />
#    </RunTimeActions>
# </Class>
#
# The &lt; and &gt; are the XML characters for '<' and '>'.  The type attribute is simply
# stripped from the XML file, so the type can be any legally-define C/C++ structure.
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
  print "usage: perl GCG.pl /path/to/xml/files /path/to/grif/skeletons";
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

# Print what has been found, exit if no files found
if ($#xml_files < 0) {
  print "Could not find any xml files, exiting...";
  exit 0;
} else {
  print "Found the following files:\n";
  foreach $f (@xml_files) {
    print "$f\n";
  }
}

# loop through each XML file in the directory
foreach $file (@xml_files) {

  my $x = XML::Simple->new (ForceArray => 1);
  my $doc = $x->XMLin($file);

  my $param_list = "//GCG param list\n";

  my $get_param = "\n//GCG param getters\n" . "template <class T> T getParam(QString name){\n\n";
  my $get_param_vec = "\n//GCG param getters (vector)\n" . "template <class T> vector<T> getStdVecParam(QString name){\n\n";
  my $get_param_qvec = "\n//GCG param getters (QVector)\n" . "template <class T> QVector<T> getQVecParam(QString name){\n\n";

  my $set_param = "\n//GCG param setters\n" . "template <class T> void setParam(QString name, T value){\n\n";
  my $set_param_vec = "\n//GCG param setters (vector)\n" . "template <class T> void setStdVecParam(QString name, vector<T> value){\n\n";
  my $set_param_qvec = "\n//GCG param setters (QVector)\n" . "template <class T> void setQVecParam(QString name, QVector<T> value){\n\n";

  my $action_list = "\n//GCG action list\n" . "// PLEASE IMPLEMENT IN A SOURCE FILE. If code gen runs again, the implementation may be overwritten!\n";
  my $action_choice = "\n//GCG action chocie\nvoid runAction(QString name){\n\tif(false){/*dummy holder for code generation*/}\n";
  my $set_default_vals = "\n//GCG default value sets\n" . "void setInitialGCGValues(){\n";

  my $print_actions = "\n//GCG print actions (for GRICLI)\nvoid printActions(){\n";
  
  # code generation for each run-time parameter
  foreach $p ( @{$doc->{RunTimeParams}[0]->{Param}} ) {

    my $param_name = $p->{pname};
    my $data_type = $p->{type};

    $param_list .= "private $data_type $param_name;\n";

    my $temp_get_add = "\tif(name == \"$param_name\"){return this->$param_name;}\n\n";
    my $temp_set_add = "\tif(name == \"$param_name\"){this->$param_name = value;}\n\n";

    if($data_type =~ /QVector/ and $data_type =~ /</){
      $get_param_qvec .= $temp_get_add;
      $set_param_qvec .= $temp_set_add;
    }
    elsif($data_type =~ /vector/ and $data_type =~ /</){
      $get_param_vec .= $temp_get_add;
      $set_param_vec .= $temp_set_add;
    }
    else{
      $get_param .= $temp_get_add;
      $set_param .= $temp_set_add;
    }

    my $default_val = $p->{default};
    if(not $default_val == ""){
      $set_default_vals .= "\t$param_name = $default_val;\n";
    }
        
  }

  $get_param .= "}\n";
  $get_param_vec .= "}\n";
  $get_param_qvec .= "}\n";
  $set_param .= "}\n";
  $set_param_vec .= "}\n";
  $set_param_qvec .= "}\n";
  $set_default_vals .= "}\n";

  # code generation for each run-time action
  foreach $action ( @{$doc->{RunTimeActions}[0]->{Action}} ) {

    my $action_name = $action->{method};
    $action_list .= "void $action_name();\n";
    $action_choice .= "\telse if(name == \"$action_name\"){this->$action_name();}\n";
    
    $print_actions .= "\tcout << \"$action_name()\" << endl;\n";

  }
  
  $action_choice .= "\telse{cout << \"could not parse action\"}\n}";

  $print_actions .= "}\n";

  # get the auxiliary file for the code generation
  my $aux_file = $doc->{Auxiliary}[0]->{apath};
  open AUXILIARY_FILE, ">", $aux_file;

  # print to the auxiliary file
  print AUXILIARY_FILE $param_list;

  print AUXILIARY_FILE $get_param;
  print AUXILIARY_FILE $get_param_qvec;
  print AUXILIARY_FILE $get_param_vec;

  print AUXILIARY_FILE $set_param;
  print AUXILIARY_FILE $set_param_qvec;
  print AUXILIARY_FILE $set_param_vec;

  print AUXILIARY_FILE $set_default_vals;
  print AUXILIARY_FILE $action_list;
  print AUXILIARY_FILE $action_choice;
  print AUXILIARY_FILE $print_actions;

  close(AUXILIARY_FILE);

  # get the skeleton file path for creation
  my $skel_file_user = $doc->{Skeleton}[0]->{spath} . "/" . $doc->{Name}[0]->{cname} . ".h";
  my $is_daq = $doc->{Name}[0]->{isdaq};
  my $class_name = $doc->{Name}[0]->{cname};
  my $skel_dir = $ARGV[1];

  my $skel_file = "$skel_dir/";

  if ($is_daq =~ /true/ or $is_daq =~ /TRUE/ or $is_daq =~ /yes/ or $is_daq =~ /YES/ or $is_daq =~ 1) {
    $skel_file .= "GRIDAQThreadGeneric.h";
    my $class_name_replace = "my_grif_daq";
  } else {
    $skel_file .= "GRIAnalysisThreadGeneric.h";
    my $class_name_replace = "my_grif_analysis";
  }

  print "skeleton file name: $skel_file\n";
  print "skeleton user file name: $skel_file_user\n";
  system("cp $skel_file $skel_file_user");

  system("cp $skel_file_user $skel_file_user.bak");
  open MAIN_FILE, "<", "$skel_file_user";
  open REPLACEMENT_FILE, ">", "$skel_file_user.bak";

  $def_repl = "GRIF CODE GENERATION\n" . "#define GRIF_CG";

  $incl_repl = "GRIF CODE GENERATION\n" . " #include \"$aux_file\" ";

  # String replacement
  $class_name_replace = "my_grif_daq";
  while ($line = <MAIN_FILE>) {
    $line =~ s/$class_name_replace/$class_name/;
    $line =~ s/GCG_DEF/$def_repl/;
    $line =~ s/GCG_INCLUDE/$incl_repl/;
    print REPLACEMENT_FILE "$line";
  }

  close(REPLACEMENT_FILE);
  close(MAIN_FILE);

  system("mv $skel_file_user.bak $skel_file_user");

}
