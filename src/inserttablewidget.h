/*
  Copyright (c) 2014-2018 Montel Laurent <montel@kde.org>

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

#ifndef INSERTTABLEWIDGET_H
#define INSERTTABLEWIDGET_H

#include <QWidget>

#include <QTextLength>

namespace KPIMTextEdit {
class InsertTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InsertTableWidget(QWidget *parent = nullptr);
    ~InsertTableWidget();
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
    void slotTypeOfLengthChanged(int);
    class InsertTableWidgetPrivate;
    InsertTableWidgetPrivate *const d;
};
}

#endif // INSERTTABLEWIDGET_H
