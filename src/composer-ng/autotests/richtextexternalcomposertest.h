/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef RICHTEXTEXTERNALCOMPOSERTEST_H
#define RICHTEXTEXTERNALCOMPOSERTEST_H
#include <QObject>

class RichTextExternalComposerTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextExternalComposerTest(QObject *parent = nullptr);
    ~RichTextExternalComposerTest();
private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeUseExternalComposer();
};

#endif // RICHTEXTEXTERNALCOMPOSERTEST_H
