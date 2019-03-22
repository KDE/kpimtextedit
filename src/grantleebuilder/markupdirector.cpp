/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

#include "markupdirector.h"
#include <QDebug>
using namespace KPIMTextEdit;

MarkupDirector::MarkupDirector(Grantlee::AbstractMarkupBuilder *builder)
    : Grantlee::MarkupDirector(builder)
{

}

MarkupDirector::~MarkupDirector()
{

}

QTextFrame::iterator
MarkupDirector::processBlockContents(QTextFrame::iterator frameIt,
                                     const QTextBlock &block)
{
    //Same code as grantlee  but interprete margin

    auto blockFormat = block.blockFormat();
    auto blockAlignment = blockFormat.alignment();

    // TODO: decide when to use <h1> etc.

    if (blockFormat.hasProperty(QTextFormat::BlockTrailingHorizontalRulerWidth)) {
        m_builder->insertHorizontalRule();
        if (!frameIt.atEnd())
            return ++frameIt;
        return frameIt;
    }

    auto it = block.begin();

    // The beginning is the end. This is an empty block. Insert a newline and
    // move
    // on.
    if (it.atEnd()) {
        m_builder->addNewline();
        if (!frameIt.atEnd())
            return ++frameIt;
        return frameIt;
    }

    // Don't have p tags inside li tags.
    if (!block.textList()) {
        //Laurent : we need this margin as it's necessary to show blockquote

        // Don't instruct builders to use margins. The rich text widget doesn't
        // have
        // an action for them yet,
        // So users can't edit them. See bug
        // http://bugs.kde.org/show_bug.cgi?id=160600
        m_builder->beginParagraph(
                    blockAlignment,
                    blockFormat.topMargin(),
                    blockFormat.bottomMargin(),
                    blockFormat.leftMargin(),
                    blockFormat.rightMargin()
                    );
    }

    while (!it.atEnd()) {
        it = processFragment(it, it.fragment(), block.document());
    }
    // Don't have p tags inside li tags.
    if (!block.textList()) {
        m_builder->endParagraph();
    }

    if (!frameIt.atEnd())
        return ++frameIt;
    return frameIt;
}


QTextBlock::iterator
MarkupDirector::processFragment(QTextBlock::iterator it,
                                const QTextFragment &fragment,
                                QTextDocument const *doc)
{
    //Same code as Grantlee + a fix !

    //   Q_D( MarkupDirector );
    auto charFormat = fragment.charFormat();

    if (charFormat.objectType() >= QTextFormat::UserObject) {
        processCustomFragment(fragment, doc);
        if (!it.atEnd())
            return ++it;
        return it;
    }

    auto textObject = doc->objectForFormat(charFormat);
    if (textObject)
        return processCharTextObject(it, fragment, textObject);

    if (fragment.text().at(0).category() == QChar::Separator_Line) {
        m_builder->addNewline();

        if (!it.atEnd())
            return ++it;
        return it;
    }

    // The order of closing and opening tags can determine whether generated
    // html
    // is valid or not.
    // When processing a document with formatting which appears as
    // '<b><i>Some</i>
    // formatted<b> text',
    // the correct generated output will contain '<strong><em>Some</em>
    // formatted<strong> text'.
    // However, processing text which appears as '<i><b>Some</b> formatted<i>
    // text' might be incorrectly rendered
    // as '<strong><em>Some</strong> formatted</em> text' if tags which start at
    // the same fragment are
    // opened out of order. Here, tags are not nested properly, and the html
    // would
    // not be valid or render correctly by unforgiving parsers (like QTextEdit).
    // One solution is to make the order of opening tags dynamic. In the above
    // case, the em tag would
    // be opened before the strong tag '<em><strong>Some</strong> formatted</em>
    // text'. That would
    // require knowledge of which tag is going to close first. That might be
    // possible by examining
    // the 'next' QTextFragment while processing one.
    //
    // The other option is to do pessimistic closing of tags.
    // In the above case, this means that if a fragment has two or more formats
    // applied (bold and italic here),
    // and one of them is closed, then all tags should be closed first. They
    // will
    // of course be reopened
    // if necessary while processing the next fragment.
    // The above case would be rendered as '<strong><em>Some</em></strong><em>
    // formatted</em> text'.
    //
    // The first option is taken here, as the redundant opening and closing tags
    // in the second option
    // didn't appeal.
    // See testDoubleStartDifferentFinish,
    // testDoubleStartDifferentFinishReverseOrder

    processOpeningElements(it);

    // If a sequence such as '<br /><br />' is imported into a document with
    // setHtml, LineSeparator
    // characters are inserted. Here I make sure to put them back.
    auto sl = fragment.text().split(QChar(QChar::LineSeparator));
    QStringListIterator i(sl);
    auto paraClosed = false;
    while (i.hasNext()) {
        m_builder->appendLiteralText(i.next());
        if (i.hasNext()) {
            if (i.peekNext().isEmpty()) {
                if (!paraClosed) {
                    m_builder->endParagraph();
                    paraClosed = true;
                }
                m_builder->addNewline();
            } else if (paraClosed) {
                m_builder->beginParagraph(/* blockAlignment */);
                paraClosed = false;
            } else {
                //Bug fixing : missing end line here
                m_builder->addNewline();
            }
        }
    }
    if (!it.atEnd())
        ++it;

    processClosingElements(it);

    return it;
}
