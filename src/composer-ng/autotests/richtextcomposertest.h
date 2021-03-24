/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RichTextComposerTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerTest(QObject *parent = nullptr);
    ~RichTextComposerTest();

private Q_SLOTS:
    void shouldHaveDefaultValue();
    void shouldChangeMode();
    void testFormattingUsed();
    void testQuoting();
    void testCleanText();
    void testEnter();
    void testEnter_data();
    void testImages();
    void testImageHtmlCode();
    void testDeleteLine();
    void testDeleteLine_data();
    void testLoadImage();

    void testWrappedPlainText_data();
    void testWrappedPlainText();

    void testEnableDisableActions();
};

