/*
  SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

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

