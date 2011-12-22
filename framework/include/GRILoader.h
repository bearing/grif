#ifndef GRILOADER_H
#define GRILOADER_H

#include <QList>
#include <QFile>
#include <QHash>
#include <QString>
#include "GRIDefines.h"
#include "GRIParser.h"
#include "GRIProcessThread.h"
#include "GRIThread.h"
#include "GRIRegulator.h"

class GRIRegulator;
class GRIProcessThread;

typedef std::pair<int, int> Edge;

class GRILoader {
 public:
  explicit GRILoader(GRIRegulator *regulator);

  virtual ~GRILoader() {}

  // Initialization of process threads based on the list fileNames
  // returns pointer to list of GRIProcessThread pointers for the framework
  void initRegulatorDetails();

 protected:
  virtual GRIProcessThread* load(QString process_name, QString object_name) = 0;

 private:
  void DetectCycles();
  void UpdateGraph(const QString& reader, const QString& writer);

  QString local_grif_path_;
  GRIRegulator* regulator_;
  QList<Edge> edges_;
  QHash<QString, int> node_index_;
};

#endif // GRILOADER_H
