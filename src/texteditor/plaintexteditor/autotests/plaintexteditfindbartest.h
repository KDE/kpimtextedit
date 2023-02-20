/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PlainTextEditFindBarTest : public QObject
{
    Q_OBJECT
public:
    explicit PlainTextEditFindBarTest(QObject *parent = nullptr);
    ~PlainTextEditFindBarTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldSearchText_data();
    void shouldSearchText();
    void shouldReplaceAllText_data();
    void shouldReplaceAllText();
};
