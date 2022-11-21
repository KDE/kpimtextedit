/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QObject>

class TextFindWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit TextFindWidgetTest(QObject *parent = nullptr);
    ~TextFindWidgetTest() override = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};