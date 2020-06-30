/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RICHTEXTEDITORTEST_H
#define RICHTEXTEDITORTEST_H

#include <QObject>

class RichTextEditorTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextEditorTest(QObject *parent = nullptr);
    ~RichTextEditorTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeSpellCheckValue();
};

#endif // RICHTEXTEDITORTEST_H
