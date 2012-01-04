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

#ifndef GRIF_FRAMEWORK_INCLUDE_GRIPROCLIST_H_
#define GRIF_FRAMEWORK_INCLUDE_GRIPROCLIST_H_

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


#endif  // GRIF_FRAMEWORK_INCLUDE_GRIPROCLIST_H_
