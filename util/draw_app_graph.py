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
import sys, os
from check_grif_xml import *
try:
    import pydot
except:
    print("Pydot is required to draw GRIF application graphs")
    sys.exit(-1)

# The following is code for drawing a graph using DOT
# For more information on DOT, please see the Graphviz DOT language reference:
#  http://www.graphviz.org/doc/info/index.html

class grif_node_style(object):
    def __init__(self, isdaq=False):
        self.shape = "box"
        self.style = "rounded, filled"
        alpha = "88"
        if isdaq:
            self.fillcolor = "#ffaaaa" + alpha
        else:
            self.fillcolor = "#aaaaff" + alpha
        self.fontname = "Helvetica"
        self.fontsize = "12.0"
    

class grif_edge_style(object):
    def __init__(self):
        self.fontname = "Courier"
        self.fontsize = "10.0"
    

def thread_to_pydot_node(thread):
    return pydot.Node(thread.name, label="<<table border=\"0\">"
                                + "<tr><td align=\"center\"><b>" + thread.name + "</b></td></tr>"
                                + "<tr><td align=\"center\">" + thread.class_xml.class_name + "</td></tr>"
                                + "<tr><td align=\"center\"><i>" + thread.class_xml.header_name + "</i></td></tr>"
                                + "</table>>",
        fontname=grif_node_style(isdaq=thread.class_xml.isdaq).fontname,
        fontsize=grif_node_style(isdaq=thread.class_xml.isdaq).fontsize,
        labelfontsize=grif_node_style(isdaq=thread.class_xml.isdaq).fontsize,
        shape=grif_node_style(isdaq=thread.class_xml.isdaq).shape,
        style=grif_node_style(isdaq=thread.class_xml.isdaq).style,
        fillcolor=grif_node_style(isdaq=thread.class_xml.isdaq).fillcolor)

def pydot_node_dict(thread_list):
    node_dict = {}
    for thread in thread_list:
        node_dict[thread.name] = thread_to_pydot_node(thread)
    return node_dict

def link_to_pydot_edge(link, node_dict):
    link_writer = node_dict[link.writer]
    link_reader = node_dict[link.reader]
    return pydot.Edge(link_writer, link_reader, label=" " + link.data + " ",
        fontname=grif_edge_style().fontname,
        fontsize=grif_edge_style().fontsize)

def pydot_edge_list(link_list, node_dict):
    edge_list = [link_to_pydot_edge(link, node_dict) for link in link_list]
    return edge_list

def app_to_pydot(thread_list, link_list):
    node_dict = pydot_node_dict(thread_list)
    edge_list = pydot_edge_list(link_list, node_dict)
    return node_dict.values(), edge_list


if __name__ == "__main__":

    def print_usage_message():
        print("Usage: python draw_app_graph.py /path/to/project/dir opts")
        print("    Options include:")
        print("       LR : draw in left-right orientation (default is up-down)")
        print("       output file extension(s) : these give the file types that will be plotted;")
        print("                                    an asterisk * will plot all file types")
    
    print("")
    if len(sys.argv) < 3:
        print_usage_message()
        print("")
        sys.exit(-1)
    project_dir = sys.argv[1]
    opts = sys.argv[2:]
    LR_orientation = ('LR' in opts)
    opts = list(set(opts) - set(['LR']))
    print("opts: ", opts)
    
    # start DOT graph using pydot
    if LR_orientation:
        graph = pydot.Dot(graph_type='digraph', rankdir='LR')
    else:
        graph = pydot.Dot(graph_type='digraph')

    write_format = {
        'png':graph.write_png,
        'gif':graph.write_gif,
        'jpg':graph.write_jpg,
        'pdf':graph.write_pdf,
        'svg':graph.write_svg
        }
    
    # check if output types are valid
    output_types = []
    if '*' in opts:
        output_types = write_format.keys()
        opts = list(set(opts) - set(['*']))
    for opt in opts:
        if opt in write_format.keys():
            output_types = list(set(output_types) | set([opt]))
    opts = list(set(opts) - set(output_types))
    print("output_types: ", output_types)
    print("unknown opts: ", opts)
    if len(opts) > 0:
        print_usage_message()
        outstr = ""
        for fmt in write_format.keys():
            outstr += fmt + " "
        print("           Allowed output types: " + outstr)
        print("")
        sys.exit(-1)
    
    project_dir = os.path.abspath(project_dir)
    app = AppXml(project_dir)
    
    graph_file = format(os.path.join(project_dir, "app"))
    node_list, edge_list = app_to_pydot(app.threads, app.links)
    
    for node in node_list:
        graph.add_node(node)
    
    for edge in edge_list:
        graph.add_edge(edge)
    
    for format in output_types:    
        print("Drawing graph in file: {0}".format(graph_file + '.' + format))
        write_format[format](graph_file + '.' + format)

