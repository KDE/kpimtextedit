/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include "kpimtextedit_export.h"
#include <KDialog>
#include <QTextLength>

namespace KPIMTextEdit {

class KPIMTEXTEDIT_EXPORT InsertTableWidget : public QWidget
{
  Q_OBJECT
public:
  explicit InsertTableWidget(QWidget *parent = 0);
  ~InsertTableWidget();
  int columns() const;
  int rows() const;
  int border() const;

  void setColumns(int col);
  void setRows(int rows);
  void setBorder(int border);

  QTextLength::Type typeOfLength() const;
  void setTypeOfLength(QTextLength::Type type);

  int length() const;
  void setLength(int);

private Q_SLOTS:
  void slotTypeOfLengthChanged(int);

private:
  class InsertTableWidgetPrivate;
  InsertTableWidgetPrivate * const d;
};

class KPIMTEXTEDIT_EXPORT InsertTableDialog : public KDialog
{
public:
  explicit InsertTableDialog(QWidget *parent);
  ~InsertTableDialog();
  int columns() const;
  int rows() const;
  int border() const;

  void setColumns(int col);
  void setRows(int rows);
  void setBorder(int border);


  QTextLength::Type typeOfLength() const;
  void setTypeOfLength(QTextLength::Type type);
  int length() const;
  void setLength(int);


private:
  class InsertTableDialogPrivate;
  InsertTableDialogPrivate * const d;
};
}

#endif // INSERTTABLEDIALOG_H
