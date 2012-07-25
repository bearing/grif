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

from __future__ import print_function
import sys, os, xml
from xml.etree.ElementTree import ElementTree


def format(path):
    return path.replace('\\','/')


def print_error(error_message):
    """Print error message to stderr."""
    print("*** Error: " + error_message, file=sys.stderr)


def print_warning(warning_message):
    """Print warning message."""
    print("*** Warning: " + warning_message)


class ClassXml(object):
    """
    An object to parse and store the class information from one /XML/classes/*.XML file.
    """
    def __init__(self, class_xml_file):
        self.filename = class_xml_file
        self.name = ""
        self.header_name = ""
        self.class_name = ""
        self.isdaq = False
        self.data_blocks = []
        self.parse()
    
    def parse(self):
        # <class>
        #   <Name name = "A04" />
        #   <Header hname = "AnalysisThreadSeries.h" />
        #   <Info cname = "AnalysisThreadSeries" isdaq = "false" />
        #   <DataBlocks>
        #     <Data block = "ADCOutput" />
        #     <Data block = "CHAN" />
        #     <Data block = "TS" />
        #   </DataBlocks>
        # </class>
        tree = ElementTree()
        try:
            tree.parse(self.filename)
        except:
            print_error("Could not parse file: " + self.filename)
            sys.exit(-1)
        # Read <class> tag
        try:
            Name = tree.findall('Name')[0]
        except:
            print_error("Problem finding <Name> tag in " + self.filename)
            sys.exit(-1)
        try:
            Header = tree.findall('Header')[0]
        except:
            print_error("Problem finding <Header> tag in " + self.filename)
            sys.exit(-1)
        try:
            Info = tree.findall('Info')[0]
        except:
            print_error("Problem finding <Info> tag in " + self.filename)
            sys.exit(-1)
        try:
            DataBlocks = tree.findall('DataBlocks')[0]
        except:
            print_error("Problem finding <DataBlocks> tag in " + self.filename)
            sys.exit(-1)
        try:
            self.name = Name.attrib['name']
            self.header_name = Header.attrib['hname']
            self.class_name = Info.attrib['cname']
            self.isdaq = (Info.attrib['isdaq'].lower() == 'true')
        except:
            print_error("Attributes \'name\', \'hname\', \'cname\', and \'isdaq\'\n"
                        + "    are not all specified in " + self.filename)
            sys.exit(-1)
        try:
            Data = DataBlocks.findall('Data')
        except:
            print_error("Problem finding <Data> tag in " + self.filename)
            sys.exit(-1)
        try:
            self.data_blocks = [data.attrib['block'] for data in Data]
        except:
            print_error("Attribute \'block\' not specified in <Data> tag in " + self.filename)
            sys.exit(-1)
        # print class information
        print("Class: " + self.name)
        print("    " + self.header_name + " " + self.class_name + " " + str(self.isdaq))
        dbstr = "    data blocks: "
        for db in self.data_blocks:
            dbstr += db + " "
        print(dbstr)
    

class AppXmlThread(object):
    """Holds one <object> tag from APP.XML."""
    def __init__(self, name, class_name):
        self.name = name
        self.class_name = class_name
        self.class_xml = None
        self.links_as_writer = []
        self.links_as_reader = []
        # print("Object: " + self.name + " " + self.class_name)
    

class AppXmlLink(object):
    """Holds one <link> tag from APP.XML."""
    def __init__(self, writer, reader, data):
        self.writer = writer
        self.reader = reader
        self.data = data
    
    def __str__(self):
        return self.writer + " ---> " + self.reader + " (" + self.data + ")"
    

class AppXml(object):
    """An object to parse and store information from the /XML/app/APP.XML file."""
    def __init__(self, project_path):
        self.project_path = project_path
        self.filename = format(os.path.join(self.project_path, "XML/app/APP.XML"))
        self.classes_xml_dir = format(os.path.join(self.project_path, "XML/classes"))
        self.threads = []
        self.thread_names = []
        self.links = []
        self.link_names = []
        self.warnings = False
        self.parse()
        self.check()
        if self.warnings:
            print("AppXml: There were warnings")
    
    def parse(self):
        """Parse the APP.XML file.  Searches through <Objects> and <Links> tabs and adds AppXmlThread
        and AppXmlLink objects to self.threads and self.links."""
        # <app>
        #   <Objects>
        #     <object name = "SIMDAQ1" class = "SIMDAQ"></object>
        #     <object name = "A00" class = "AnalysisThreadSeries"></object>
        #     <object name = "A01" class = "AnalysisThreadSeries"></object>
        #   </Objects>
        #   <Links>
        #     <link writer = "SIMDAQ1" reader = "A00" data = "ADCOutput"></link>
        #     <link writer = "SIMDAQ1" reader = "A00" data = "CHAN"></link>
        #     <link writer = "SIMDAQ1" reader = "A00" data = "TS"></link>
        #     <link writer = "A00" reader = "A01" data = "ADCOutput"></link>
        #     <link writer = "A00" reader = "A01" data = "CHAN"></link>
        #     <link writer = "A00" reader = "A01" data = "TS"></link>
        #   </Links>
        # </app>
        tree = ElementTree()
        try:
            tree.parse(self.filename)
        except:
            print_error("Could not parse file: " + self.filename)
            sys.exit(-1)
        # Read <object> tags
        try:
            threads = tree.findall('Objects')[0]
        except:
            print_error("Problem finding <Objects> tag in " + self.filename)
            sys.exit(-1)
        thread_list = threads.findall('object')
        if len(thread_list) == 0:
            print_error("No objects in " + self.filename)
            sys.exit(-1)
        for thread in thread_list:
            # <object name = "A86" class = "AnalysisThreadSeries"></object>
            try:
                name = thread.attrib['name']
                class_name = thread.attrib['class']
            except:
                print_error("Attributes \'name\' and \'class\' are not both specified in <object> tag in " + self.filename)
                sys.exit(-1)
            thread_object = AppXmlThread(name, class_name)
            self.threads.append(thread_object)
        # Read <link> tags
        try:
            links = tree.findall('Links')[0]
        except:
            print_error("Problem finding <Links> tag in " + self.filename)
            sys.exit(-1)
        link_list = links.findall('link')
        if len(link_list) == 0:
            print_error("No links in " + self.filename)
            sys.exit(-1)
        for link in link_list:
            # <link writer = "A01" reader = "A02" data = "CHAN"></link>
            try:
                writer = link.attrib['writer']
                reader = link.attrib['reader']
                data = link.attrib['data']
            except:
                print_error("Attributes \'writer\', \'reader\', and \'data\' are not all specified in <link> tag in " + self.filename)
                sys.exit(-1)
            link_object = AppXmlLink(writer, reader, data)
            self.links.append(link_object)
    
    def check(self):
        """Perform all checks that the APP.XML objects and links are well-formed."""
        return (self._check_no_duplicate_threads()
            and self._check_class_xmls()
            and self._check_daq_exists()
            and self._check_analysis_exists()
            and self._check_no_duplicate_links()
            and self._check_link_read_write()
            and self._check_link_data_blocks()
            and self._check_link_threads_exist()
            and self._check_daq_write_only())
    
    def find_thread_names(self):
        """Return sorted list of thread names.  Also updates self.thread_names."""
        self.thread_names = [thread.name for thread in self.threads]
        self.thread_names.sort()
        return self.thread_names
    
    def find_link_names(self):
        """Return sorted list of link names.  Also updates self.link_names."""
        self.link_names = [str(link) for link in self.links]
        self.link_names.sort()
        return self.link_names
    
    def _check_no_duplicate_threads(self):
        """Check that there are no duplicate objects (threads)."""
        print("AppXml: Checking that there are no duplicate objects")
        self.find_thread_names()
        thread_names_uniq = list(set(self.thread_names))
        if len(self.thread_names) != len(thread_names_uniq):
            print(len(self.thread_names), len(thread_names_uniq))
            print(self.thread_names)
            print(thread_names_uniq)
            print_error("List of objects in APP.XML contains duplicates")
            sys.exit(-1)
        thread_names_uniq.sort()
        print(thread_names_uniq)
        print("AppXml: Check passed")
        return True
    
    def _check_class_xmls(self):
        """Read class XML file for each object and check that the definitions match."""
        print("AppXml: Checking class XML files")
        for thread in self.threads:
            class_xml_file = format(os.path.join(self.classes_xml_dir, thread.name + ".XML"))
            thread.class_xml = ClassXml(class_xml_file)
            if thread.name != thread.class_xml.name:
                print(thread.name, thread.class_xml.name)
                # print(thread.class_name, thread.class_xml.class_name)
                print_error("Mismatch between APP.XML definition and class XML definition for "
                        + thread.name + " in " + thread.name + ".XML")
                sys.exit(-1)
        print("AppXml: Check passed")
        return True
    
    def _check_no_duplicate_links(self):
        """Check that there are no duplicate links."""
        print("AppXml: Checking that there are no duplicate links")
        self.find_link_names()
        link_names_uniq = list(set(self.link_names))
        if len(self.link_names) != len(link_names_uniq):
            print(len(self.link_names), len(link_names_uniq))
            print(self.link_names)
            print(link_names_uniq)
            print_error("List of links in APP.XML contains duplicates")
            sys.exit(-1)
        link_names_uniq.sort()
        for link_name in link_names_uniq:
            print(link_name)
        print("AppXml: Check passed")
        return True
    
    def _check_link_read_write(self):
        """Check that there are no links that read and write from the same object."""
        print("AppXml: Checking that links do not have the same object reading and writing")
        for thread in self.threads:
            thread.links_as_writer = []
            thread.links_as_reader = []
            for link in self.links:
                if thread.name == link.writer:
                    thread.links_as_writer.append(link)
                if thread.name == link.reader:
                    thread.links_as_reader.append(link)
                if thread.name == link.writer and thread.name == link.reader:
                    print_error("Class " + thread.name + " cannot read from and write to itself.\n"
                                + "    Offending link: "+str(link))
                    sys.exit(-1)
            if len(thread.links_as_writer) == 0 and len(thread.links_as_reader) == 0:
                print_warning("Object " + thread.name + " does not read or write any data blocks")
                self.warnings = True
        print("AppXml: Check passed")
        return True
    
    def _check_link_data_blocks(self):
        """Check that the data blocks in each link are known to the classes."""
        print("AppXml: Checking that the data blocks in each link are known to the classes")
        for thread in self.threads:
            if thread.class_xml is None:
                class_xml_file = format(os.path.join(self.classes_xml_dir, thread.name + ".XML"))
                thread.class_xml = ClassXml(class_xml_file)
            for link in self.links:
                if thread.name == link.writer or thread.name == link.reader:
                    if not link.data in thread.class_xml.data_blocks:
                        print_error("Data block " + link.data + " not specified in XML file for " + thread.name)
                        sys.exit(-1)
        print("AppXml: Check passed")
        return True
    
    def _check_link_threads_exist(self):
        """Check that all links refer to threads that exist."""
        print("AppXml: Checking that links refer to objects that are specified")
        self.find_thread_names()
        for link in self.links:
            if not link.writer in self.thread_names:
                print_error("Link " + str(link) + " refers to a writer object that is not specified: " + link.writer)
                sys.exit(-1)
            if not link.reader in self.thread_names:
                print_error("Link " + str(link) + " refers to a reader object that is not specified: " + link.reader)
                sys.exit(-1)                
        print("AppXml: Check passed")
        return True
    
    def _check_daq_exists(self):
        """Check that there is at least one DAQ thread."""
        print("AppXml: Checking that there is at least one DAQ object")
        num_daq = 0
        for thread in self.threads:
            if thread.class_xml is None:
                class_xml_file = format(os.path.join(self.classes_xml_dir, thread.name + ".XML"))
                thread.class_xml = ClassXml(class_xml_file)
            if thread.class_xml.isdaq:
                num_daq += 1
        if num_daq == 0:
            print_error("There are no DAQ objects specified in this application")
            sys.exit(-1)
        print("AppXml: Check passed")
        return True
    
    def _check_analysis_exists(self):
        """Check that there is at least one analysis thread."""
        print("AppXml: Checking that there is at least one analysis object")
        num_analysis = 0
        for thread in self.threads:
            if thread.class_xml is None:
                class_xml_file = format(os.path.join(self.classes_xml_dir, thread.name + ".XML"))
                thread.class_xml = ClassXml(class_xml_file)
            if not thread.class_xml.isdaq:
                num_analysis += 1
        if num_analysis == 0:
            print_error("There are no analysis objects specified in this application")
            sys.exit(-1)
        print("AppXml: Check passed")
        return True
    
    def _check_daq_write_only(self):
        """Check that each DAQ object writes at least one block but does not read."""
        print("AppXml: Checking that each DAQ writes at least one block but does not read")
        for thread in self.threads:
            if thread.class_xml is None:
                class_xml_file = format(os.path.join(self.classes_xml_dir, thread.name + ".XML"))
                thread.class_xml = ClassXml(class_xml_file)
            if thread.class_xml.isdaq:
                num_writing = 0
                for link in self.links:
                    if thread.name == link.reader:
                        print_error("DAQ object " + thread.name + " is not allowed to read")
                        sys.exit(-1)
                    if thread.name == link.writer:
                        num_writing += 1
                if num_writing == 0:
                    print_warning("DAQ object " + thread.name + " does not write any data blocks")
                    self.warnings = True
        print("AppXml: Check passed")
        return True
    

if __name__ == "__main__":
    try:
        project_path = sys.argv[1]
    except:
        print("usage: python draw_app_graph.py /path/to/project/dir")
        print(len(sys.argv))
        sys.exit(-1)
    
    project_path = os.path.abspath(project_path)
    
    print("")
    print("-"*20)
    print("Checking app and class XML files in project: {0}".format(project_path))
    
    App = AppXml(project_path)
    
    print("AppXml: Checks complete")
