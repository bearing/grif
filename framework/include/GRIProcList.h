#ifndef FRAMEWORK_TRUNK_INCLUDE_GRIPROCLIST_H_
#define FRAMEWORK_TRUNK_INCLUDE_GRIPROCLIST_H_

#include <QString>
#include <QList>
#include <QHash>
#include "GRIProcessThread.h"

/*
 * Manages a DAQ process list and provides methods to
 * modify each Processes local state during runtime.
 */

struct process {
    QList<QString> sets_;
    QList<QString> gets_;
    QList<QString> run_actions_;
};



class GRIProcList {

  public:
    static const QString SETS;
    static const QString GETS;
    static const QString RUN_ACTIONS;

    /*
     * A new GRIProcList instantiated with the procList.
     * \param procList the process list this manage
     */
    GRIProcList();
    //GRIProcList(QList<GRIProcessThread>*);


    /*
     * Inserts a type into the the specified process thread's blah blah blah,
     * either sets, gets, or runActions.
     * \return 1 upon successful insertion.
     *      0 if element already exists. Note: it checks for duplicates and will not insert it.
     *      -1 if the list cannot be found, or an error was produced.
     */
    int insertType(QString, QString, QString);
    /*
     * Deletes the type from the process thread's list thing.
     * \return 1 upon successful deletion
     *      0 if it finds the list but doesn't find the element to delete.
     *      -1 if it can't find the list to delete from
     */
    int deleteType(QString, QString, QString);

    /*
     * Returns a QList of QStrings that represents the Sets of the process specified.
     * Returns the empty list if there is no sets list associated with this process.
     */
    QList<QString> retrieveSets(QString);
    /*
     * Returns a QList of QStrings that represents the Gets of the process specified.
     * Returns the empty list if there are no gets list associated with this process.
     */
    QList<QString> retrieveGets(QString);

    /*
     * Returns a QList of QStrings that represents the Sets of the process specified.
     * Returns the empty list if there are no runactions list associated with this process.
     */
    QList<QString> retrieveRunActions(QString);

  private:
    /* The map of process name to process struct. */
    QHash<QString, process*> process_map_;

    /* Retrieves the list specified by QString from the given process. */
    QList<QString>* getTypeList(process*, QString);

    /* Retrueves the List associated with this process name. */
    QList<QString>* retrieveList(QString, QString);
};


#endif  // FRAMEWORK_TRUNK_INCLUDE_GRIPROCLIST_H_

