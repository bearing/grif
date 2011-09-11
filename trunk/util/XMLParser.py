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
    '''
    self.prefix = '#include \"GRIUserLoaderAux.h\"\n\n'
    self.prefix += '// GCG process prefix code for GRIUserLoader.cpp\n'
    self.prefix += 'GRIProcessThread *get_new_process(QString class_name, QString instance_name) {\n'
    self.prefix += '  GRIProcessThread *p = NULL;\n\n'
    self.suffix = '  return p;\n}\n'
    '''
    self.prefix = ''
    self.suffix = ''
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []
    self.classes = []

  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    name = tree.find('Info')
    if name is None:
      return
    classname = name.attrib['cname']
    if classname is None:
      return
    if classname in self.classes:
      return
    self.classes.append(classname)
    self.gen += '  if (class_name.contains(\"{0}\")) {1}\n'.format(classname, '{')
    self.gen += '    p = new {0}();\n'.format(classname)
    self.gen += '    return p;\n  }\n\n'


'''
DataParser is for generating the definitions of data types specified by
the user. The GRI class GRIDataDefines.h will include the auxiliary file.
'''
class DataParser(XMLParser):
  def __init__(self, dir, out):
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
    self.dir = dir
    self.files = []

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


  def ParseFile(self, path):
    param_list = '// GCG param list\n'
    get_param = '\n// GCG param getters\n' + 'template <class T> T getParam(QString name) {\n'
    get_param_vec = '\n// GCG param getters (vector)\n' + 'template <class T> vector<T> getStdVecParam(QString name) {\n'
    get_param_qvec = '\n// GCG param getters (QVector)\n' + 'template <class T> QVector<T> getQVecParam(QString name) {\n'
    
    set_param = '\n// GCG param setters\n' + 'template <class T> void setParam(QString name, T value) {\n'
    set_param_vec = '\n// GCG param setters (vector)\n' + 'template <class T> void setStdVecParam(QString name, vector<T> value) {\n'
    set_param_qvec = '\n// GCG param setters (QVector)\n' + 'template <class T> void setQVecParam(QString name, QVector<T> value) {\n'
    
    action_list = '\n// GCG action list\n' + '// PLEASE IMPLEMENT IN A SOURCE FILE. If code gen runs again, the implementation may be overwritten!\n'
    action_choice = '\n// GCG action chocie\nvoid runAction(QString name) {\n  if (false) { /*dummy holder for code generation*/ }\n'
    set_default_vals = '\n// GCG default value sets\n' + 'void setInitialGCGValues() {\n'

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
      param_list += 'private {0} {1};\n'.format(dtype, pname)

      get = '  if (name == \"{1}\") {0} return this->{1}; {2}\n'.format('{', pname, '}')
      set = '  if (name == \"{1}\") {0} this->{1} = value; {2}\n'.format('{', pname, '}')

      if dtype.find('QVector') != -1 and dtype.find('<') != -1:
        get_param_qvec += get
        set_param_qvec += set
      elif dtype.find('vector') != -1 and dtype.find('<') != -1:
        get_param_vec += get
        set_param_vec += set
      else:
        get_param += get;
        set_param += set;

      if 'default' in param.attrib:
        set_default_vals += '  {0} = {1};\n'.format(pname, param.attrib['default'])

    end = '}\n'
    get_param += end
    get_param_vec += end
    get_param_qvec += end
    set_param += end
    set_param_vec += end
    set_param_qvec += end
    set_default_vals += end

    rtas = tree.find('RunTimeActions')
    actions = []
    if not rtas is None:
      actions = rtas.findall('Action')
    for action in actions:
      aname = action.attrib['method']
      action_list += 'void {0}();\n'.format(aname)
      action_choice += '  else if (name == \"{1}\") {0} {1}(); {2}\n'.format('{', aname, '}')
      print_actions += '  cout << \"{0}()\" << endl;\n'.format(aname)

    action_choice += '  else { std::cout << \"could not parse action\" }\n}\n'
    print_actions += '}\n'

    auxiliary = tree.find('Auxiliary')
    if not auxiliary is None:
      fname = auxiliary.attrib['apath']
      auxfile = open(fname, 'w')
      auxfile.write(param_list)
      auxfile.write(get_param)
      auxfile.write(get_param_qvec)
      auxfile.write(get_param_vec)
      auxfile.write(set_param)
      auxfile.write(set_param_qvec)
      auxfile.write(set_param_vec)
      auxfile.write(set_default_vals)
      auxfile.write(action_list)
      auxfile.write(action_choice)
      auxfile.write(print_actions)

    if not self.skeldir:
      print 'No skeleton directory set.  Not creating a generic class'
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
    auxname = tree.find('Header').attrib['hname']
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
