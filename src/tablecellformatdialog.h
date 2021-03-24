/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QDialog>

#include <QTextCharFormat>

namespace KPIMTextEdit
{
class TableCellFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableCellFormatDialog(QWidget *parent);
    ~TableCellFormatDialog() override;

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

