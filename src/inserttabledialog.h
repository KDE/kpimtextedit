/*
  Copyright (c) 2012-2020 Laurent Montel <montel@kde.org>

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

#ifndef KPIMTEXTEDIT_INSERTTABLEDIALOG_H
#define KPIMTEXTEDIT_INSERTTABLEDIALOG_H

#include <QDialog>

#include <QTextLength>

namespace KPIMTextEdit {
class InsertTableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertTableDialog(QWidget *parent);
    ~InsertTableDialog();
    Q_REQUIRED_RESULT int columns() const;
    Q_REQUIRED_RESULT int rows() const;
    Q_REQUIRED_RESULT int border() const;

    void setColumns(int col);
    void setRows(int rows);
    void setBorder(int border);

    Q_REQUIRED_RESULT QTextLength::Type typeOfLength() const;
    void setTypeOfLength(QTextLength::Type type);
    Q_REQUIRED_RESULT int length() const;
    void setLength(int);

private:
    class InsertTableDialogPrivate;
    InsertTableDialogPrivate *const d;
};
}

#endif // KPIMTEXTEDIT_INSERTTABLEDIALOG_H
