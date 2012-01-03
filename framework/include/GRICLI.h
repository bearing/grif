// Copyright (C) 2012 Gamma-ray Imaging Framework Team
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// 
// The license can be found in the LICENSE.txt file.
//
// Contact:
// Dr. Daniel Chivers
// dhchivers@lbl.gov

#ifndef GRIF_FRAMEWORK_INCLUDE_GRICLI_H_
#define GRIF_FRAMEWORK_INCLUDE_GRICLI_H_

#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>
#include "GRIDefines.h"
#include "GRIProcessThread.h"

enum CLI_state_enum { MAIN, PROCESS_TOP };
typedef QList<ProcessCommand *> CLI_MACRO;

class GRICLI : public QObject{
 public:
  explicit GRICLI(QList<GRIProcessThread*> *processes);
  virtual ~GRICLI() {}

  // Call Launch() from main file to start the interface.  After launching, the
  // CLI cycles in an event loop waiting for commands.
  void Launch();

  // Stop the CLI.
  void Quit();

 public slots:
  void ReceiveProcessGet(ProcessCommand *pc);

 private:

  Q_OBJECT

  // All of these methods will be called internally
  // You only need to call the launch() method to start
  // and the quit() method to exit
  void DisplayMain();
  void DisplayHelp();
  void DisplayProcesses();

  void HandleMain(QString *instr_array, int n);
  void HandleProcessTop(QString *instr_array, int n);

  void Init();

  void HandleMacroDef(const QString& instr);
  void HandleMacroExecution(QString *instr_array, int n);

  // methods that parse input to do sets, gets, and actions
  void ProcessSet(const QString& name, const QString& value,
                  const QString& dataType);
  void ProcessGet(const QString& name, const QString& dataType);
  void ProcessAction(const QString& name);

  ProcessCommand *CreateGetCommand(const QString& name,const QString& dataType);
  ProcessCommand *CreateSetCommand(const QString& name, const QString& value,
                                   const QString& dataType);
  ProcessCommand *CreateRunActionCommand(const QString& name);

  // the current process for sets, gets, actions
  GRIProcessThread *curr_proc_;

  QList<GRIProcessThread*> *processes_;
  QHash<QString, GRIProcessThread*> process_hash_;
  QHash<QString, CLI_MACRO> macro_hash_;
  enum CLI_state_enum cli_state_;
  mutable QMutex get_lock_;
};

#endif  // GRIF_FRAMEWORK_INCLUDE_GRICLI_H_
