#!/usr/bin/env python

import sys
import os
import xml
from xml.etree.ElementTree import ElementTree

'''
GRIF Code Generation

These are utility classes used by the GCG_*.py scripts.

written by Austin Benson: arbenson @ berkeley
Berkeley Applied Research on the Imaging of Neutrons and Gamma-rays
Domestic Nuclear Threat Security Initiative
'''

'''
XMLParser is the base class for the GRIF XML parsing.
'''
class XMLParser:
  def __init__(self, dir, out):
    self.prefix = ''
    self.suffix = ''
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []

  def DirentFormatCheck(self, dirent):
      if not (dirent.find('.xml') != -1 or dirent.find('.XML') != -1):
        return False
      if not (dirent.find('#') == -1 and dirent.find('~') == -1):
        return False
      return True

  def ParseDirectory(self, path):
    # Check each file in the directory. If the file ends in .xml or .XML, add
    # it to the list of files
    dirents = os.listdir(self.dir)
    for dirent in dirents:
      if not os.path.isdir(dirent):
        entry = str(dirent)
        if self.DirentFormatCheck(entry):
          print 'Found file: {0}'.format(entry)
          self.files.append('{0}/{1}'.format(self.dir, entry))

  def ParseFile(self, path):
    pass

  def Parse(self):
    self.ParseDirectory(self.dir)
    for file in self.files:
      self.ParseFile(file)
    self.PrintToFile(self.out)


  def PrintToFile(self, path):
    if self.out:
      f = open(self.out, 'w')
      f.write(self.prefix + self.gen + self.suffix)


'''
GRIUserProcessesParser is for generating the necessary #include statements for
GRIUserProcceses.h.
'''
class GRIUserProcessesParser(XMLParser):
  def __init__(self, dir, out):
    self.prefix = '#ifndef GRIUSERPROCESSES_AUX_H\n'
    self.prefix += '#define GRIUSERPROCESSES_AUX_H\n'
    self.prefix += '\n/*\n'
    self.prefix += 'This file includes all of the necessary user header files\n'
    self.prefix += 'The data in this file was generated with GCG_GRIUserProccesses.pl,\n'
    self.prefix += 'which can be found in grif/framework/util\n'
    self.prefix += '*/\n\n\n'
    self.prefix += '//Code-generated includes (harvested from XML files => Header tag)\n'
    self.suffix = '\n\n#endif // GRIUSERPROCESSES_AUX_H\n'
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []
    self.headers = []

  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    # get the header name
    header = tree.find('Header')
    if header is None:
      return
    name = header.attrib['hname']
    if not name in self.headers:
      self.headers.append(name)
      self.gen += '#include \"{0}\"\n'.format(name)


'''
GRIUserLoader is for generating code for the framework file GRIUserLoader.cpp.
'''
class GRIUserLoaderParser(XMLParser):
  def __init__(self, dir, out):
    self.prefix = ''
    self.suffix = ''
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []
    self.objs = []

  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    info = tree.find('Info')
    if info is None:
      return
    classname = info.attrib['cname']

    name = tree.find('Name')
    if name is None:
      return
    objname = name.attrib['name']

    if classname is None or objname is None:
      return

    if objname in self.objs:
      return

    self.objs.append(objname)
    self.gen += '  if (object_name == \"{0}\") {1}\n'.format(objname, '{')
    self.gen += '    p = new {0}();\n'.format(classname)
    self.gen += '    return p;\n  }\n\n'


'''
DataParser is for generating the definitions of data types specified by
the user. The GRI class GRIDataDefines.h will include the auxiliary file.
'''
class DataParser(XMLParser):
  def __init__(self, file, out):
    self.prefix = '#ifndef GRIDATDEFINES_AUX_H\n'
    self.prefix += '#define GRIDATADEFINES_AUX_H\n'
    self.prefix += '\n'
    self.prefix += '// This file includes all of the user-defined data types\n'
    self.prefix += '// The data in this file was generated with GCG_data.py,\n'
    self.prefix += '// which can be found in grif/framework/util\n'
    self.prefix += '\n'
    self.prefix += '//Code-generated data types\n'
    self.suffix = '#endif // GRIDATADEFINES_AUX_H\n'
    self.gen = ''
    self.out = out
    self.file = str(file)

  def Parse(self):
    self.ParseFile(self.file)
    self.PrintToFile(self.out)

  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    datatypes = tree.findall('datat')
    if datatypes is None:
      return
    for dtype in datatypes:
      struct = 'typedef struct {\n'
      vars = dtype.find('var').findall('vtype')
      for var in vars:
        type = var.attrib['type']
        name = var.attrib['vname']
        struct += '  ' + type + ' ' + name + ';\n'
      
      dataname = dtype.find('dataname')
      name = dataname.attrib['dname']
      struct += '} ' + name + ';\n\n'
      self.gen += struct


'''
ClassParser does the code generation for skeleton files
'''
class ClassParser(XMLParser):
  def SetSkelDir(self, path):
    self.skeldir = path

  def SetAuxOnly(self, aux_only):
    self.aux_only = aux_only

  def ParseFile(self, path):
    param_list = '\n// GCG param list\n private:\n'
    get_param = '\n// Dynamic Gets\n'
    get_bool = 'bool DynamicGetBool(const QString& name) {\n'
    get_char = 'char DynamicGetChar(const QString& name) {\n'
    get_int = 'int DynamicGetInt(const QString& name) {\n'
    get_float = 'float DynamicGetFloat(const QString& name) {\n'
    get_double = 'double DynamicGetDouble(const QString& name) {\n'
    
    set_param = '\n// Dyanimc Sets\n'
    set_bool = 'void DynamicSetBool(const QString& name, bool value) {\n'
    set_char = 'void DynamicSetChar(const QString& name, char value) {\n'
    set_int = 'void DynamicSetInt(const QString& name, int value) {\n'
    set_float = 'void DynamicSetFloat(const QString& name, float value) {\n'
    set_double = 'void DynamicSetDouble(const QString& name, double value) {\n'
    
    action_list = '\n// Dynamic Run Actions\n' 
    action_list += '// PLEASE IMPLEMENT IN A SOURCE FILE. '
    action_list += 'If code gen runs again, the implementation '
    action_list += 'may be overwritten!\n'
    action_choice = '\n// GCG action chocie\n'
    action_choice += 'void DynamicRunAction(const QString& name) '
    action_choice += '{\n  if (false) '
    action_choice += '{ /*dummy holder for code generation*/ }\n'
    set_default_vals = '\n// GCG default value sets\n'
    set_default_vals += 'void setInitialGCGValues() {\n'

    print_actions = '\n//GCG print actions (for GRICLI)\nvoid printActions() {\n'

    tree = ElementTree()
    tree.parse(path)

    rtps = tree.find('RunTimeParams')
    params = []
    if not rtps is None:
      params = rtps.findall('Param')
    for param in params:
      pname = param.attrib['pname']
      dtype = param.attrib['type']
      param_list += '  {0} {1};\n'.format(dtype, pname)

      get = '  if (name == \"{1}\") {0} return {1}; {2}\n'.format('{', pname, '}')
      set = '  if (name == \"{1}\") {0} {1} = value; {2}\n'.format('{', pname, '}')

      if dtype == 'bool':
        get_bool += get
        set_bool += set
      elif dtype == 'char':
        get_char += get
        set_char += set
      elif dtype == 'int':
        get_int += get
        set_int += set
      elif dtype == 'float':
        get_float += get
        set_float += set
      elif dtype == 'double':
        get_double += get
        set_double += set
      else:
        warning = 'WARNING: Could not parse data type {0}.'.format(dtype)
        warning += ' Skipping variable {0}.'.format(pname)
        print warning
        print 'Acceptable data types are bool, char, int, float, and double\n'

      if 'default' in param.attrib:
        set_default_vals += '  {0} = {1};\n'.format(pname, param.attrib['default'])

    ret = '  return 0;\n'
    end = '}\n'
    get_bool += ret + end
    get_char += ret + end
    get_int += ret + end
    get_float += ret + end
    get_double += ret + end
    set_bool += end
    set_char += end
    set_int += end
    set_float += end
    set_double += end
    set_default_vals += end

    rtas = tree.find('RunTimeActions')
    actions = []
    if not rtas is None:
      actions = rtas.findall('Action')
    for action in actions:
      aname = action.attrib['method']
      action_list += 'void {0}();\n'.format(aname)
      action_choice += '  else if (name == \"{1}\") {0} {1}(); {2}\n'.format('{', aname, '}')
      print_actions += '  std::cout << \"{0}()\" << std::endl;\n'.format(aname)

    action_choice += '  else { std::cout << \"could not parse action\" << std::endl; }\n}\n'
    print_actions += '}\n'

    auxiliary = tree.find('Auxiliary')
    if not auxiliary is None:
      fname = auxiliary.attrib['apath']
      auxfile = open(fname, 'w')
      auxfile.write(get_bool)
      auxfile.write(get_char)
      auxfile.write(get_int)
      auxfile.write(get_double)
      auxfile.write(get_float)
      auxfile.write(set_bool)
      auxfile.write(set_char)
      auxfile.write(set_int)
      auxfile.write(set_double)
      auxfile.write(set_float)
      auxfile.write(set_default_vals)
      auxfile.write(action_list)
      auxfile.write(action_choice)
      auxfile.write(print_actions)
      auxfile.write(param_list)

    if not self.skeldir:
      print 'No skeleton directory set.  Not creating a generic class'
      return

    if self.aux_only:
      print 'Not creating a skeleton file, aux file has been updated.'
      return

    skeleton = tree.find('Skeleton')
    if skeleton is None:
      return
    skeldir = skeleton.attrib['spath']

    header = tree.find('Header')
    if header is None:
      return
    skelname = header.attrib['hname']

    skeldst = '{0}/{1}'.format(skeldir, skelname)
    classname = tree.find('Info').attrib['cname']
    isdaq = tree.find('Info').attrib['isdaq']

    replace = 'my_grif_daq'
    skelfile = '{0}/{1}'.format(self.skeldir, 'GRIDAQThreadGeneric.h')

    if isdaq.find('false') != -1 or isdaq.find('no') != -1 or isdaq.find('0') != -1:
      replace = 'my_grif_analysis'
      skelfile = '{0}/{1}'.format(self.skeldir, 'GRIAnalysisThreadGeneric.h')

    print 'Using skeleton file {0} to create {1}\n'.format(skelfile, skeldst)

    grifdef = 'GRIF CODE GENERATION\n#define GRIF_CG'
    auxname = tree.find('Auxiliary').attrib['hname']
    grifincl = 'GRIF CODE GENERATION\n #include \"{0}\"'.format(auxname)

    # String replacement
    input = open(skelfile, 'r')
    output = open(skeldst, 'w')
    for line in input:
      line = line.replace(replace, classname)
      line = line.replace('GCG_DEF', grifdef)
      line = line.replace('GCG_INCLUDE', grifincl)
      output.write(line)

    input.close()
    output.close()
