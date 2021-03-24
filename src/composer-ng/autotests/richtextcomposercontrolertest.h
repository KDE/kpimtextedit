/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RichTextComposerControlerTest : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerControlerTest(QObject *parent = nullptr);
    ~RichTextComposerControlerTest();

private Q_SLOTS:
    void shouldAlignCenter();
    void shouldAlignLeft();
    void shouldAlignRight();
    void shouldAlignJustify();
    void shouldHaveDefaultValue();
    void shouldBoldText();
    void shouldItalicText();
    void shouldTextUnderline();
    void shouldTextStrikeOut();

    void shouldFontFamily();
    void shouldFontSize();
    void shouldFont();
    void shouldTextSuperScript();
    void shouldTextSubScript();

    void shouldRemoveQuote_data();
    void shouldRemoveQuote();

    void shouldRemoveQuoteWithSpecificQuote_data();
    void shouldRemoveQuoteWithSpecificQuote();

    void shouldAddQuote_data();
    void shouldAddQuote();
};

