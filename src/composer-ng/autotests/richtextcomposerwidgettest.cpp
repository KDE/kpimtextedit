/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "richtextcomposerwidgettest.h"
#include "../richtextcomposerwidget.h"
#include <QTest>
#include <kpimtextedit/richtextcomposer.h>

RichTextComposerWidgetTest::RichTextComposerWidgetTest(QObject *parent)
    : QObject(parent)
{

}

RichTextComposerWidgetTest::~RichTextComposerWidgetTest()
{

}

void RichTextComposerWidgetTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposerWidget w;
    KPIMTextEdit::RichTextComposer *composer = w.findChild<KPIMTextEdit::RichTextComposer *>(QStringLiteral("richtextcomposer"));
    QVERIFY(composer);
    QVERIFY(w.richTextComposer());
}

QTEST_MAIN(RichTextComposerWidgetTest)
