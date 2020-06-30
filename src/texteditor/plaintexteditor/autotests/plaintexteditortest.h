/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLAINTEXTEDITORTEST_H
#define PLAINTEXTEDITORTEST_H

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

#endif // PLAINTEXTEDITORTEST_H
