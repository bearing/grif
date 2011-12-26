#ifndef GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_
#define GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_

#include <iostream>
#include <QString>

class GRILogMessage {
 public:
  GRILogMessage();
  ~GRILogMessage();

  bool SetMessageTime(QString s, int level = 0);
  void SetObjectName(QString s) {
    object_name_ = s;
  }
  bool IsLevelEnabled(int level);
  bool IsLevelEnabled();
  QString GetObjectName() {
    return object_name_;
  }
  int level;
  QString DateTime;
  QString MsgStr;

 private:
  QString object_name_;
  void SetTimeString();
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRILOGMESSAGE_H_
