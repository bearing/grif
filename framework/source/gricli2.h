#ifndef GRICLI2_H
#define GRICLI2_H

#include <QDialog>
#include <QHash>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QString>
#include "GRIDefines.h"
#include "GRIProcessThread.h"

enum CLI_state_enum { MAIN, PROCESS_TOP };
typedef QList<ProcessCommand *> CLI_MACRO;

namespace Ui {
class GRICLI2;
}

class GRICLI2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit GRICLI2(QWidget *parent = 0,QLinkedList<GRIProcessThread*> *processes= 0);
    ~GRICLI2();
    
private slots:
    void on_runcmd_clicked();

private:
    Ui::GRICLI2 *ui;

public:
 void Launch();
 void Quit();

public slots:
 void ReceiveProcessGet(ProcessCommand *pc);

private:

 // All of these methods will be called internally
 // You only need to call the launch() method to start
 // and the quit() method to exit
 void DisplayMain();
 void DisplayHelp();
 void DisplayProcesses();
 void DisplayActions();

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

 QLinkedList<GRIProcessThread*> *processes_;
 QHash<QString, GRIProcessThread*> process_hash_;
 QHash<QString, CLI_MACRO> macro_hash_;
 enum CLI_state_enum cli_state_;
 mutable QMutex get_lock_;
};

#endif // GRICLI2_H
