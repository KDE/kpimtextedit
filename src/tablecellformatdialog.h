/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>

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

#ifndef KPIMTEXTEDIT_TABLECELLFORMATDIALOG_H
#define KPIMTEXTEDIT_TABLECELLFORMATDIALOG_H

#include <QDialog>

#include <QTextCharFormat>

namespace KPIMTextEdit {
class TableCellFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableCellFormatDialog(QWidget *parent);
    ~TableCellFormatDialog();

    /**
     * @brief tableCellBackgroundColor
     * @return background color of cell.
     */
    Q_REQUIRED_RESULT QColor tableCellBackgroundColor() const;

    /**
     * @brief setTableCellBackgroundColor
     * @param color set the background color of cell
     */
    void setTableCellBackgroundColor(const QColor &color);

    /**
     * @brief verticalAlignment
     * @return vertical alignment for current cell.
     */
    Q_REQUIRED_RESULT QTextCharFormat::VerticalAlignment verticalAlignment() const;

    /**
     * @brief setVerticalAlignment
     * @param vertical vertical alignment for current cell.
     */
    void setVerticalAlignment(QTextCharFormat::VerticalAlignment vertical);

    /**
     * @brief useBackgroundColor
     * @return true if we define background color. Otherwise false and allows to clear it.
     */
    Q_REQUIRED_RESULT bool useBackgroundColor() const;

private:
    class TableCellFormatDialogPrivate;
    TableCellFormatDialogPrivate *const d;
};
}

#endif // KPIMTEXTEDIT_TABLECELLFORMATDIALOG_H
