/*
   SPDX-FileCopyrightText: 2015-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RICHTEXTCOMPOSERWIDGETTEST_H
#define RICHTEXTCOMPOSERWIDGETTEST_H

#include <QObject>

class RichTextComposerWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerWidgetTest(QObject *parent = nullptr);
    ~RichTextComposerWidgetTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
};

#endif // RICHTEXTCOMPOSERWIDGETTEST_H
