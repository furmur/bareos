/*
   BAREOS® - Backup Archiving REcovery Open Sourced

   Copyright (C) 2007-2009 Free Software Foundation Europe e.V.

   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/
#ifndef _CONTENT_H_
#define _CONTENT_H_

#include <QtGui>
#include "ui_content.h"
#include "pages.h"

class Content : public Pages, public Ui::ContentForm
{
   Q_OBJECT

public:
   Content(QString storage, QTreeWidgetItem *parentWidget);
//   virtual void PgSeltreeWidgetClicked();
   virtual void currentStackItem();

public slots:
   void treeItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);

   void consoleRelease();
   void consoleUpdateSlots();
   void consoleLabelStorage();
   void consoleMountStorage();
   void statusStorageWindow();
   void consoleUnMountStorage();
   void consoleImportStorage();
   void consoleExportStorage();
   void consoleMoveUpStorage();
   void consoleMoveDownStorage();
   void showMediaInfo(QTableWidgetItem * item);

private slots:
   void populateContent();

private:
   bool m_currentAutoChanger;
   bool m_populated;
   bool m_needs_repopulate;
   bool m_firstpopulation;
   bool m_checkcurwidget;
   QString m_currentStorage;
};

#endif /* _STORAGE_H_ */
