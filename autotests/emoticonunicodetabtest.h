/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class EmoticonUnicodeTabTest : public QObject
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTabTest(QObject *parent = nullptr);
    ~EmoticonUnicodeTabTest() = default;
private Q_SLOTS:
    void shouldHaveDefaultValues();
};

