/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <QObject>

class RichTextComposerActionsTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerActionsTest(QObject *parent = nullptr);
    ~RichTextComposerActionsTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldHaveActions();
    void shouldChangeEnableState();
};

