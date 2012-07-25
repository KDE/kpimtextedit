/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <KDialog>
#include <QSpinBox>
#include <QTextLength>
class KComboBox;

namespace KPIMTextEdit {

class InsertTableWidget : public QWidget
{
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
  int length() const;

private:
  QSpinBox *mColumns;
  QSpinBox *mRows;
  QSpinBox *mBorder;
  QSpinBox *mLength;
  KComboBox *mTypeOfLength;
};

class InsertTableDialog : public KDialog
{
public:
  explicit InsertTableDialog(QWidget *parent);
  ~InsertTableDialog();
  int columns() const;
  int rows() const;
  int border() const;

  QTextLength::Type typeOfLength() const;
  int length() const;


private:
  class InsertTableDialogPrivate;
  InsertTableDialogPrivate *d;
};
}

#endif // INSERTTABLEDIALOG_H
