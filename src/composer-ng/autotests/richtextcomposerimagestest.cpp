/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerimagestest.h"
#include <QTest>

RichTextComposerImagesTest::RichTextComposerImagesTest(QObject *parent)
    : QObject(parent)
{
}

RichTextComposerImagesTest::~RichTextComposerImagesTest() = default;

QTEST_MAIN(RichTextComposerImagesTest)

#include "moc_richtextcomposerimagestest.cpp"
