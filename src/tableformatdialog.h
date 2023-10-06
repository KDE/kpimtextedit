/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QDialog>

#include <QTextLength>

namespace KPIMTextEdit
{
class TableFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableFormatDialog(QWidget *parent);
    ~TableFormatDialog() override;

    [[nodiscard]] int columns() const;
    [[nodiscard]] int rows() const;
    [[nodiscard]] int border() const;

    void setColumns(int);
    void setRows(int);
    void setBorder(int);

    [[nodiscard]] int padding() const;
    void setPadding(int);

    [[nodiscard]] int spacing() const;
    void setSpacing(int);

    void setAlignment(Qt::Alignment alignment);
    [[nodiscard]] Qt::Alignment alignment() const;

    [[nodiscard]] QTextLength::Type typeOfLength() const;
    void setTypeOfLength(QTextLength::Type type);

    [[nodiscard]] int length() const;
    void setLength(int);

    [[nodiscard]] QColor tableBackgroundColor() const;
    void setTableBackgroundColor(const QColor &col);

    [[nodiscard]] bool useBackgroundColor() const;

private:
    class TableFormatDialogPrivate;
    std::unique_ptr<TableFormatDialogPrivate> const d;
};
}
