# Copyright (C) 2012 Gamma-ray Imaging Framework Team
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 3.0 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 
# The license can be found in the LICENSE.txt file.
# 
# Contact:
# Dr. Daniel Chivers
# dhchivers@lbl.gov

'''
usage: python GCG_data.py /path/to/data/xml/file /path/to/auxiliary/file

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
  print 'usage: python GCG_data.py /path/to/data/xml/file /path/to/auxiliary/file'
  exit()

parser = XMLParser.DataParser(sys.argv[1], sys.argv[2])
parser.Parse()
