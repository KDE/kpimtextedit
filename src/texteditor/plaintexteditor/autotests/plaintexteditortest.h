/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class PlainTextEditorTest : public QObject
{
    Q_OBJECT
public:
    explicit PlainTextEditorTest(QObject *parent = nullptr);
    ~PlainTextEditorTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeSpellCheckValue();
};

