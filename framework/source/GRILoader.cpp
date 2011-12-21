#include "GRILoader.h"
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/visitors.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                              boost::property<boost::vertex_color_t,
                              boost::default_color_type> > Graph;

GRILoader::GRILoader(QString localGRIFPath, GRIRegulator *regulator,
                     QList<QString> fileNames) {
  file_names_ = fileNames;
  local_grif_path_ = localGRIFPath;
  regulator_ = regulator;
}

void GRILoader::initRegulatorDetails() {
  //
  // A parsed XML file gives us:
  // 1. list of classes and names that will be used
  // 2. list of the processes
  // 3. list of links for each process
  //
  // Algorithm for initRegulatorDetails()
  // 1. for each XML file
  //     2. loop through each object
  //           3. call load method on each using class and name (GRIUserLoader uses these)
  //           4. push this process onto the list of processes
  //               5. loop through each link of the object
  //                   6. create the data block
  //                   7. push the data block onto the list of data blocks
  //

  QList<QString>::iterator it;

  for(it = file_names_.begin(); it != file_names_.end(); ++it) {
    // get name of xml file
    QString name = *it;
    GRIParser parser;
    bool success = parser.Parse(local_grif_path_ + name);
    if(!success) {
      std::cout << "WARNING: Could not successfully parse XML file: "
                << name.toStdString().c_str() << std::endl;
      std::cout << "Skipping that XML file..." << std::endl;
      continue;
    }

    QList<struct objectParsingDetails> objectsAndLinks = parser.get_objs_and_links();

    QList<struct objectParsingDetails>::iterator obj_details_it;

    for(obj_details_it = objectsAndLinks.begin(); obj_details_it != objectsAndLinks.end();
	obj_details_it++) {

      struct objectParsingDetails obj_details = *obj_details_it;
      QString className = obj_details.className;
      QString objectName = obj_details.objectName;

      GRIProcessThread *proc = load(className, objectName);

      if (proc == 0) {
        std::cout << "WARNING: could not load class "
                  << className.toStdString().c_str()
                  << " with object name " << objectName.toStdString().c_str()
                  << ". Please check GRIUserLoader::load(). Continuing without the file..."
                  << std::endl;
        // skip this process
        continue;
      }

      proc->SetDefaultDetail(regulator_, objectName);

      QList<struct linkParsingDetails *>::iterator link_details_it;
      QList<struct linkParsingDetails*> *obj_links = obj_details.links;

      for(link_details_it = obj_links->begin(); link_details_it != obj_links->end();
          ++link_details_it) {
        struct linkParsingDetails *curr_link = *(link_details_it);
        // add data block to the process
        proc->AddDataBlock(curr_link->dataBlock, curr_link->writer==proc->get_name());
	QString writer = curr_link->writer;
	QString reader = curr_link->reader;
	QString db_name = curr_link->dataBlock;
	// construct a new data block
        GRIDataBlock *data = new GRIDataBlock(regulator_, regulator_->get_mem_mngr(),
                                              reader, db_name, db_name, writer);
        regulator_->AddDataBlock(data);
        UpdateGraph(reader, writer);
      }
      regulator_->AddProcess(proc);
    }
    std::cout << "Successfully parsed XML file: " << name.toStdString().c_str()
              << std::endl;
  }
  DetectCycles();
}

namespace boost {
  struct cycle_detector : public dfs_visitor<> {
    cycle_detector(bool& has_cycle)
      : has_cycle_(has_cycle) { }

    template <class Edge, class Graph>
    void back_edge(Edge, Graph&) { has_cycle_ = true; }
  protected:
    bool& has_cycle_;
  };
}

void GRILoader::UpdateGraph(const QString& reader, const QString& writer) {
  int r_ind, w_ind;
  if (!node_index_.contains(reader)) {
    r_ind = node_index_.size();
    node_index_.insert(reader, r_ind);
  } else {
    r_ind = node_index_.value(reader);
  }
  if (!node_index_.contains(writer)) {
    w_ind = node_index_.size();
    node_index_.insert(writer, w_ind);
  } else {
    w_ind = node_index_.value(writer);
  }
  edges_.push_back(Edge(r_ind, w_ind));
}

void GRILoader::DetectCycles() {
  std::cout << "checking for dependency cycles..." << std::endl;
  int num_nodes = node_index_.size();
  int num_edges = edges_.size();
  Edge edges[num_edges];
  for (int i = 0; i < num_edges; ++i) {
    edges[i] = edges_.at(i);
  }
  Graph g(edges, edges + sizeof(edges) / sizeof(Edge), num_nodes);
  bool has_cycle = false;
  boost::cycle_detector vis(has_cycle);
  boost::depth_first_search(g, visitor(vis));
  if (has_cycle) {
    std::cout << "WARNING: Your dependency graph has a cycle.  "
              << "Continuing anyway." << std::endl;
  } else {
    std::cout << "No dependency cycle detected." << std::endl;
  }
}
