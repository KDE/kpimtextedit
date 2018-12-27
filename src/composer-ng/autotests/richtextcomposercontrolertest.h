/*
   Copyright (C) 2015-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef RICHTEXTCOMPOSERCONTROLERTEST_H
#define RICHTEXTCOMPOSERCONTROLERTEST_H

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

#endif // RICHTEXTCOMPOSERCONTROLERTEST_H
