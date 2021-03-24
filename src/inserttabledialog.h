/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

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

