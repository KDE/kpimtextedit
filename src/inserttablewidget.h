/*
  SPDX-FileCopyrightText: 2014-2024 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#pragma once

#include <QWidget>

#include <QTextLength>

namespace KPIMTextEdit
{
class InsertTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InsertTableWidget(QWidget *parent = nullptr);
    ~InsertTableWidget() override;
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
    void slotTypeOfLengthChanged(int);
    class InsertTableWidgetPrivate;
    std::unique_ptr<InsertTableWidgetPrivate> const d;
};
}
