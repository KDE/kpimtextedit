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

#ifndef KPIMTEXTEDIT_TABLEFORMATDIALOG_H
#define KPIMTEXTEDIT_TABLEFORMATDIALOG_H

#include <QDialog>

#include <QTextLength>

namespace KPIMTextEdit {
class TableFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableFormatDialog(QWidget *parent);
    ~TableFormatDialog();

    Q_REQUIRED_RESULT int columns() const;
    Q_REQUIRED_RESULT int rows() const;
    Q_REQUIRED_RESULT int border() const;

    void setColumns(int);
    void setRows(int);
    void setBorder(int);

    Q_REQUIRED_RESULT int padding() const;
    void setPadding(int);

    Q_REQUIRED_RESULT int spacing() const;
    void setSpacing(int);

    void setAlignment(Qt::Alignment alignment);
    Q_REQUIRED_RESULT Qt::Alignment alignment() const;

    Q_REQUIRED_RESULT QTextLength::Type typeOfLength() const;
    void setTypeOfLength(QTextLength::Type type);

    Q_REQUIRED_RESULT int length() const;
    void setLength(int);

    Q_REQUIRED_RESULT QColor tableBackgroundColor() const;
    void setTableBackgroundColor(const QColor &col);

    Q_REQUIRED_RESULT bool useBackgroundColor() const;

private:
    class TableFormatDialogPrivate;
    TableFormatDialogPrivate *const d;
};
}
#endif // KPIMTEXTEDIT_TABLEFORMATDIALOG_H
