#!/usr/bin/env python

'''
usage: python GCG_data.py /path/to/xml/files /path/to/auxiliary/file

This code is for generating the definitions of data types specified by
the user.  The GRI class GRIDataDefines.h will include the auxiliary file.

The generated code goes into the file located at /path/to/auxiliary/file,
which should be included by GRIDataDefines.h

This code generation currently supports the definition of C-style structs.
The user must provide a name for the struct.  The user can also define an
arbitrary number of variables (by providing the type and name) to belong
to the struct

An example of the XML style is:

<datat>
  <dataname dname = "peak" />
     <var>
         <vtype type = "double" vname = "value" />
         <vtype type = "double" vname = "sigma" />
         <vtype type = "double" vname = "rate" />
         <vtype type = "int" vname = "chan" />
     </var>
</datat>

This will be translated to:

typedef struct{
     double value;
     double sigma;
     double rate;
     int chan;
} peak;

All of the <datat> objects in the XML file should be encapsulated by
the <DataDefines> tag.

Written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

import XMLParser
import sys

if len(sys.argv) < 3:
  print 'usage: python GCG_data.py /path/to/xml/files /path/to/auxiliary/file'
  exit()

parser = XMLParser.DataParser(sys.argv[1], sys.argv[2])
parser.Parse()
