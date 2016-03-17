/*
    This file is part of KDE.

    Copyright (c) 2015 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "syntaxhighlightertest_demo.h"

#include <QApplication>
#include <QToolBar>

#include <texteditor/plaintexteditor/plaintexteditor.h>

SyntaxHighlighterTest::SyntaxHighlighterTest(QTextDocument *doc)
    : KPIMTextEdit::SyntaxHighlighterBase(doc)
{
    init();
}

SyntaxHighlighterTest::~SyntaxHighlighterTest()
{

}

void SyntaxHighlighterTest::init()
{
    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::darkYellow);
    QRegularExpression commentRegex(QStringLiteral("TEST"));
    m_rules.append(KPIMTextEdit::Rule(commentRegex, commentFormat));
}

int main(int argv, char **argc)
{
    QApplication app(argv, argc);
    KPIMTextEdit::PlainTextEditor textEdit;
    (void)new SyntaxHighlighterTest(textEdit.document());
    textEdit.show();
    return app.exec();
}

