/*
  SPDX-FileCopyrightText: 2012-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QDialog>

#include <QTextLength>

namespace KPIMTextEdit
{
class InsertTableDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertTableDialog(QWidget *parent);
    ~InsertTableDialog() override;
    [[nodiscard]] int columns() const;
    [[nodiscard]] int rows() const;
    [[nodiscard]] int border() const;

    void setColumns(int col);
    void setRows(int rows);
    void setBorder(int border);

    [[nodiscard]] QTextLength::Type typeOfLength() const;
    void setTypeOfLength(QTextLength::Type type);
    [[nodiscard]] int length() const;
    void setLength(int);

private:
    class InsertTableDialogPrivate;
    std::unique_ptr<InsertTableDialogPrivate> const d;
};
}
