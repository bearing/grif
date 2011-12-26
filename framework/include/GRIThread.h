#ifndef GRIF_FRAMEWORK_INCLUDE_GRITHREAD_H_
#define GRIF_FRAMEWORK_INCLUDE_GRITHREAD_H_

#include <QTime>
#include <QTextStream>
#include <QThread>
#include <QString>
#include "GRILogMessage.h"

class GRIThread : public QThread {
  Q_OBJECT

  public:
  GRIThread() {}
  ~GRIThread() {}
  virtual void run () { exec (); }

  QTextStream log;
  void CommitLog(int level);

  void set_exit_thread_flag(bool exit_thread_flag) {
   if (sleeping_) {
      sleeping_ = false;
    }
    exit_thread_flag_ = exit_thread_flag;
  }
  bool get_exit_thread_flag() { return exit_thread_flag_; }
  void set_force_quit(bool force_quit) { force_quit_ = force_quit; }
  bool get_force_quit() { return force_quit_; }
  void set_sleeping(bool sleeping) { sleeping_ = sleeping; }
  bool get_sleeping() { return sleeping_; }
  void set_name(const QString& name) { setObjectName(name); }
  QString get_name() { return objectName(); }


 private:
  GRILogMessage log_msg_;
  bool exit_thread_flag_;
  bool sleeping_;
  bool force_quit_;

 signals:
  void logSignal(GRILogMessage m);
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRITHREAD_H_
