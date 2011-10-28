#include "GRILoader.h"

GRILoader::GRILoader(QString localGRIFPath, GRIRegulator *regulator,
                     std::list<QString> fileNames) {
  file_names_ = fileNames;
  local_grif_path_ = localGRIFPath;
  regulator_ = regulator;
}

GRIRegulatorDetails *GRILoader::initRegulatorDetails() {
  /*
   *    A parsed XML file gives us:
   *    1. list of classes and names that will be used
   *    2. list of the processes
   *    3. list of links for each process
   *
   */

  /*
   * Algorithm for initRegulatorDetails()
   * 1. for each XML file
   *     2. loop through each object
   *           3. call load method on each using class and name (GRIUserLoader uses these)
   *           4. push this process onto the list of processes
   *               5. loop through each link of the object
   *                   6. create the data block
   *                   7. push the data block onto the list of data blocks
   * 8. return a new GRIRegulatorDetails with the list of processes and the list of data blocks
   */

  std::list<QString>::iterator it;
  std::list<GRIProcessThread*>* processes = new std::list<GRIProcessThread*>;
  std::list<GRIDataBlock*>* dataBlocks = new std::list<GRIDataBlock*>;

  for(it = file_names_.begin(); it != file_names_.end(); it++) {
    // get name of xml file
    QString name = *it;
    GRIParser parser;
    bool success = parser.Parse(local_grif_path_ + name);
    if(!success) {
      cout << "WARNING: Could not successfully parse XML file: " << name.toStdString().c_str() << endl;
      cout << "Skipping that XML file..." << endl;
      continue;
    }

    std::list<struct objectParsingDetails> objectsAndLinks = parser.get_objs_and_links();

    std::list<struct objectParsingDetails>::iterator obj_details_it;

    for(obj_details_it = objectsAndLinks.begin(); obj_details_it != objectsAndLinks.end();
	obj_details_it++) {

      struct objectParsingDetails obj_details = *obj_details_it;
      QString className = obj_details.className;
      QString objectName = obj_details.objectName;

      GRIProcessThread *proc = load(className, objectName);

      if(proc == 0) {
	cout << "WARNING: could not load class " << className.toStdString().c_str() <<
	  " with object name " << objectName.toStdString().c_str() <<
	  ". Please check GRIUserLoader::load(). Continuing without the file..." << endl;
	continue; //skip this process
      }

      proc->SetDefaultDetail(regulator_, objectName);

      std::list<struct linkParsingDetails *>::iterator link_details_it;
      std::list<struct linkParsingDetails*> *obj_links = obj_details.links;

      for(link_details_it = obj_links->begin(); link_details_it != obj_links->end(); link_details_it++) {
	struct linkParsingDetails *curr_link = *(link_details_it);
	// add data block to the process
	proc->AddDataBlock(curr_link->dataBlock, curr_link->writer==proc->name());
	QString writer = curr_link->writer;
	QString reader = curr_link->reader;
	QString db_name = curr_link->dataBlock;
	/* construct a new data block */
	GRIDataBlock *data = new GRIDataBlock(regulator_, regulator_->GetMemoryManager(),
					      reader, db_name, db_name, writer);
	dataBlocks->push_back(data);

      }
      processes->push_back(proc);
    }
    cout << "Successfully parsed XML file: " << name.toStdString().c_str() << endl;
  }
  //return the processes and dataBlocks
  return new GRIRegulatorDetails(processes, dataBlocks);
}
