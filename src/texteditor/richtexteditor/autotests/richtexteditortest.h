/*
   SPDX-FileCopyrightText: 2014-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RichTextEditorTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextEditorTest(QObject *parent = nullptr);
    ~RichTextEditorTest() override;

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeSpellCheckValue();
};
