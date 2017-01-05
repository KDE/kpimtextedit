/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

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

#include "richtextcomposercontrolertest.h"
#include "../richtextcomposercontroler.h"
#include "../richtextcomposer.h"
#include <KActionCollection>
#include <qtest.h>

RichTextComposerControlerTest::RichTextComposerControlerTest(QObject *parent)
    : QObject(parent)
{

}

RichTextComposerControlerTest::~RichTextComposerControlerTest()
{

}

void RichTextComposerControlerTest::shouldAlignLeft()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignLeft();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignLeft);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignRight()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignRight();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignRight);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignJustify()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignJustify();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignJustify);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAlignCenter()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.alignCenter();
    QVERIFY(controler.richTextComposer()->hasFocus());
    QCOMPARE(controler.richTextComposer()->alignment(), Qt::AlignHCenter);
    QVERIFY(controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    QVERIFY(!controler.painterActive());
    QVERIFY(!controler.richTextComposer()->acceptRichText());
}

void RichTextComposerControlerTest::shouldAddQuote()
{
#if 0
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);

    controler.alignLeft();
#endif
}

void RichTextComposerControlerTest::shouldBoldText()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextBold(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldItalicText()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextItalic(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.
}

void RichTextComposerControlerTest::shouldTextUnderline()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextUnderline(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.

}

void RichTextComposerControlerTest::shouldTextStrikeOut()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextStrikeOut(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO text format.

}

void RichTextComposerControlerTest::shouldFontFamily()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO
}

void RichTextComposerControlerTest::shouldFontSize()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO

}

void RichTextComposerControlerTest::shouldFont()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    //TODO

}

void RichTextComposerControlerTest::shouldTextSuperScript()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextSuperScript(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());
    //TODO

}

void RichTextComposerControlerTest::shouldTextSubScript()
{
    KPIMTextEdit::RichTextComposer composer;
    KActionCollection *actionCollection = new KActionCollection(&composer);
    composer.createActions(actionCollection);
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    composer.show();
    QTest::qWaitForWindowExposed(&composer);
    controler.setTextSubScript(true);
    QVERIFY(controler.richTextComposer()->hasFocus());
    QVERIFY(controler.richTextComposer()->acceptRichText());

    //TODO

}

QTEST_MAIN(RichTextComposerControlerTest)
