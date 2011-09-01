#!/usr/bin/env python

import sys
import os
import xml
from xml.etree.ElementTree import ElementTree

'''
GRIF Code Generation
'''

'''
XMLParser is the base class for the GRIF XML parsing.
'''
class XMLParser:
  def __init__(self, prefix, suffix, dir, out):
    self.prefix = prefix
    self.suffix = suffix
    self.gen = ''
    self.out = out
    self.dir = dir
    self.files = []


  def ParseDirectory(self, path):
    # Check each file in the directory. If the file ends in .xml or .XML, add
    # it to the list of files
    dirents = os.listdir(self.dir)
    for dirent in dirents:
      if not os.path.isdir(dirent):
        if str(dirent).find('.xml') != -1 or str(dirent).find('.XML') != -1:
          self.files.append(self.dir + '/' + str(dirent))


  def ParseFile(self, path):
    pass


  def Parse(self):
    self.ParseDirectory(self.dir)
    for file in self.files:
      self.ParseFile(file)
    self.PrintToFile(self.out)


  def PrintToFile(self, path):
    f = open(self.out, 'w')
    f.write(self.prefix + self.gen + self.suffix)


'''
GRIUserProcessesParser is for generating the necessary #include statements for
GRIUserProcceses.h.
'''
class GRIUserProcessesParser(XMLParser):
  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    # get the header name
    header = tree.find('Header')
    name = header.attrib['hname']
    self.gen += '#include \"' + name + '\"\n'


'''
GRIUserLoader is for generating code for the framework file GRIUserLoader.cpp.
'''
class GRIUserLoaderParser(XMLParser):
  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    name = tree.find('Name')
    classname = name.attrib['cname']
    self.gen += '  if (class_name.contains(\"' + classname + '\")) {\n'
    self.gen += '    p = new ' + classname + '();\n'
    self.gen += '    return p;\n  }\n\n'


'''
DataParser is for generating the definitions of data types specified by
the user. The GRI class GRIDataDefines.h will include the auxiliary file.
'''
class DataParser(XMLParser):
  def ParseFile(self, path):
    tree = ElementTree()
    tree.parse(path)
    datatypes = tree.findall('datat')
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

