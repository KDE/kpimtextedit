/*
  SPDX-FileCopyrightText: 2019-2020 Montel Laurent <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "markupdirector.h"
#include "markupdirector_p.h"

#include "abstractmarkupbuilder.h"

#include <QBrush>
#include <QColor>
#include <QFlags>
#include <QMap>
#include <QStack>
#include <QString>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextFrame>
#include <QTextList>
#include <QTextTable>

#include <QDebug>
using namespace KPIMTextEdit;

MarkupDirector::MarkupDirector(KPIMTextEdit::AbstractMarkupBuilder *builder)
    : d_ptr(new MarkupDirectorPrivate(this))
    , m_builder(builder)
{
}

MarkupDirector::~MarkupDirector()
{
    delete d_ptr;
}

//#define ADD_HEADER_SUPPORT 1

QTextFrame::iterator MarkupDirector::processBlockContents(QTextFrame::iterator frameIt, const QTextBlock &block)
{
    // Same code as grantlee  but interprete margin

    const auto blockFormat = block.blockFormat();
    const auto blockAlignment = blockFormat.alignment();
    const bool leftToRightText = block.textDirection() == Qt::RightToLeft;

    // TODO: decide when to use <h1> etc.
#ifdef ADD_HEADER_SUPPORT
    if (blockFormat.headingLevel() > 0) {
        // Header
        qDebug() << " header " << blockFormat.headingLevel();
        m_builder->beginHeader(blockFormat.headingLevel());
    }
#endif
    if (blockFormat.hasProperty(QTextFormat::BlockTrailingHorizontalRulerWidth)) {
        m_builder->insertHorizontalRule();
        if (!frameIt.atEnd()) {
            return ++frameIt;
        }
        return frameIt;
    }

    auto it = block.begin();

    // The beginning is the end. This is an empty block. Insert a newline and
    // move
    // on.
    if (it.atEnd()) {
        m_builder->addSingleBreakLine();

        if (!frameIt.atEnd()) {
            return ++frameIt;
        }
        return frameIt;
    }

    // Don't have p tags inside li tags.
    if (!block.textList()) {
        // Laurent : we need this margin as it's necessary to show blockquote

        // Don't instruct builders to use margins. The rich text widget doesn't
        // have
        // an action for them yet,
        // So users can't edit them. See bug
        // https://bugs.kde.org/show_bug.cgi?id=160600
        m_builder->beginParagraph(blockAlignment,
                                  blockFormat.topMargin(),
                                  blockFormat.bottomMargin(),
                                  blockFormat.leftMargin(),
                                  blockFormat.rightMargin(),
                                  leftToRightText);
    }

    while (!it.atEnd()) {
        it = processFragment(it, it.fragment(), block.document());
    }
    // Don't have p tags inside li tags.
    if (!block.textList()) {
        m_builder->endParagraph();
    }

    if (!frameIt.atEnd()) {
        return ++frameIt;
    }
    return frameIt;
}

QTextBlock::iterator MarkupDirector::processFragment(QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc)
{
    // Same code as Grantlee + a fix !

    //   Q_D( MarkupDirector );
    const auto charFormat = fragment.charFormat();
    // Need to check if it's a image format.
    if (charFormat.isImageFormat()) {
        const auto imageFormat = charFormat.toImageFormat();
        return processImage(it, imageFormat, doc);
    }

    if (charFormat.objectType() >= QTextFormat::UserObject) {
        processCustomFragment(fragment, doc);
        if (!it.atEnd()) {
            return ++it;
        }
        return it;
    }

    auto textObject = doc->objectForFormat(charFormat);
    if (textObject) {
        return processCharTextObject(it, fragment, textObject);
    }

    const auto textStr = fragment.text();
    if (textStr.at(0).category() == QChar::Separator_Line) {
        m_builder->addSingleBreakLine();
        QString t;
        for (int i = 1; i < textStr.length(); ++i) {
            if (fragment.text().at(i).category() == QChar::Separator_Line) {
                m_builder->appendLiteralText(t);
                if (i < textStr.length() - 1) { // Don't add \n when we have the last char
                    m_builder->addSingleBreakLine();
                }
                t.clear();
            } else {
                t += fragment.text().at(i);
            }
        }
        if (!t.isEmpty()) {
            m_builder->appendLiteralText(t);
        }

        if (!it.atEnd()) {
            return ++it;
        }
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
                // Bug fixing : add missing single break line
                m_builder->addSingleBreakLine();
            }
        }
    }
    if (!it.atEnd()) {
        ++it;
    }

    processClosingElements(it);

    return it;
}

void MarkupDirector::processDocumentContents(QTextFrame::iterator start, const QTextFrame::iterator &end)
{
    while (!start.atEnd() && start != end) {
        auto frame = start.currentFrame();
        if (frame) {
            auto table = qobject_cast<QTextTable *>(frame);
            if (table) {
                start = processTable(start, table);
            } else {
                start = processFrame(start, frame);
            }
        } else {
            auto block = start.currentBlock();
            Q_ASSERT(block.isValid());
            start = processBlock(start, block);
        }
    }
}

QTextFrame::iterator MarkupDirector::processFrame(QTextFrame::iterator it, QTextFrame *frame)
{
    if (frame) {
        processDocumentContents(frame->begin(), frame->end());
    }
    if (!it.atEnd())
        return ++it;
    return it;
}

QTextFrame::iterator MarkupDirector::processBlock(QTextFrame::iterator it, const QTextBlock &block)
{
    if (block.isValid()) {
        auto fmt = block.blockFormat();
        auto object = block.document()->objectForFormat(fmt);
        if (object) {
            return processObject(it, block, object);
        } else {
            return processBlockContents(it, block);
        }
    }

    if (!it.atEnd())
        return ++it;
    return it;
}

QTextFrame::iterator MarkupDirector::processTable(QTextFrame::iterator it, QTextTable *table)
{
    const auto format = table->format();

    const auto colLengths = format.columnWidthConstraints();

    const auto tableWidth = format.width();
    QString sWidth;

    if (tableWidth.type() == QTextLength::PercentageLength) {
        sWidth = QStringLiteral("%1%");
        sWidth = sWidth.arg(tableWidth.rawValue());
    } else if (tableWidth.type() == QTextLength::FixedLength) {
        sWidth = QStringLiteral("%1");
        sWidth = sWidth.arg(tableWidth.rawValue());
    }

    m_builder->beginTable(format.cellPadding(), format.cellSpacing(), sWidth);

    const auto headerRowCount = format.headerRowCount();

    QVector<QTextTableCell> alreadyProcessedCells;

    for (int row = 0, total = table->rows(); row < total; ++row) {
        // Put a thead element around here somewhere?
        // if (row < headerRowCount)
        // {
        // beginTableHeader();
        // }

        m_builder->beginTableRow();

        // Header attribute should really be on cells, not determined by number
        // of
        // rows.
        // http://www.webdesignfromscratch.com/html-tables.cfm

        for (int column = 0, total = table->columns(); column < total; ++column) {
            auto tableCell = table->cellAt(row, column);

            auto columnSpan = tableCell.columnSpan();
            auto rowSpan = tableCell.rowSpan();
            if ((rowSpan > 1) || (columnSpan > 1)) {
                if (alreadyProcessedCells.contains(tableCell)) {
                    // Already processed this cell. Move on.
                    continue;
                } else {
                    alreadyProcessedCells.append(tableCell);
                }
            }

            auto cellWidth = colLengths.at(column);

            QString sCellWidth;

            if (cellWidth.type() == QTextLength::PercentageLength) {
                sCellWidth = QStringLiteral("%1%").arg(cellWidth.rawValue());
            } else if (cellWidth.type() == QTextLength::FixedLength) {
                sCellWidth = QStringLiteral("%1").arg(cellWidth.rawValue());
            }

            // TODO: Use THEAD instead
            if (row < headerRowCount) {
                m_builder->beginTableHeaderCell(sCellWidth, columnSpan, rowSpan);
            } else {
                m_builder->beginTableCell(sCellWidth, columnSpan, rowSpan);
            }

            processTableCell(tableCell, table);

            if (row < headerRowCount) {
                m_builder->endTableHeaderCell();
            } else {
                m_builder->endTableCell();
            }
        }
        m_builder->endTableRow();
    }
    m_builder->endTable();

    if (!it.atEnd())
        return ++it;
    return it;
}

void MarkupDirector::processTableCell(const QTextTableCell &tableCell, QTextTable *table)
{
    Q_UNUSED(table)
    processDocumentContents(tableCell.begin(), tableCell.end());
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::processList(QTextFrame::iterator it, const QTextBlock &_block, QTextList *list)
{
    const auto style = list->format().style();
    m_builder->beginList(style);
    auto block = _block;
    while (block.isValid() && block.textList()) {
        m_builder->beginListItem();
        processBlockContents(it, block);
        m_builder->endListItem();

        if (!it.atEnd())
            ++it;
        block = block.next();
        if (block.isValid()) {
            auto obj = block.document()->objectForFormat(block.blockFormat());
            auto group = qobject_cast<QTextBlockGroup *>(obj);
            if (group && group != list) {
                auto pair = processBlockGroup(it, block, group);
                it = pair.first;
                block = pair.second;
            }
        }
    }
    m_builder->endList();
    return qMakePair(it, block);
}

void MarkupDirector::processCustomFragment(const QTextFragment &fragment, const QTextDocument *doc)
{
    Q_UNUSED(fragment)
    Q_UNUSED(doc)
}

QTextFrame::iterator MarkupDirector::processObject(QTextFrame::iterator it, const QTextBlock &block, QTextObject *object)
{
    auto group = qobject_cast<QTextBlockGroup *>(object);
    if (group) {
        return processBlockGroup(it, block, group).first;
    }
    if (!it.atEnd())
        return ++it;
    return it;
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::skipBlockGroup(QTextFrame::iterator it, const QTextBlock &_block, QTextBlockGroup *blockGroup)
{
    auto block = _block;
    auto lastBlock = _block;
    auto lastIt = it;
    auto obj = block.document()->objectForFormat(block.blockFormat());
    QTextBlockGroup *nextGroup;

    if (!obj)
        return qMakePair(lastIt, lastBlock);

    auto group = qobject_cast<QTextBlockGroup *>(obj);
    if (!group)
        return qMakePair(lastIt, lastBlock);

    while (block.isValid()) {
        if (!group)
            break;

        block = block.next();
        if (!it.atEnd())
            ++it;

        obj = block.document()->objectForFormat(block.blockFormat());
        if (obj)
            continue;

        nextGroup = qobject_cast<QTextBlockGroup *>(obj);

        if (group == blockGroup || !nextGroup) {
            lastBlock = block;
            lastIt = it;
        }
        group = nextGroup;
    }
    return qMakePair(lastIt, lastBlock);
}

QPair<QTextFrame::iterator, QTextBlock> MarkupDirector::processBlockGroup(const QTextFrame::iterator &it, const QTextBlock &block, QTextBlockGroup *blockGroup)
{
    const auto list = qobject_cast<QTextList *>(blockGroup);
    if (list) {
        return processList(it, block, list);
    }
    return skipBlockGroup(it, block, blockGroup);
}

void MarkupDirector::processDocument(QTextDocument *doc)
{
    processFrame(QTextFrame::iterator(), doc->rootFrame());
}

QTextBlock::iterator MarkupDirector::processCharTextObject(QTextBlock::iterator it, const QTextFragment &fragment, QTextObject *textObject)
{
    const auto fragmentFormat = fragment.charFormat();
    if (fragmentFormat.isImageFormat()) {
        const auto imageFormat = fragmentFormat.toImageFormat();
        return processImage(it, imageFormat, textObject->document());
    }
    if (!it.atEnd())
        return ++it;
    return it;
}

QTextBlock::iterator MarkupDirector::processImage(QTextBlock::iterator it, const QTextImageFormat &imageFormat, const QTextDocument *doc)
{
    Q_UNUSED(doc)
    // TODO: Close any open format elements?
    m_builder->insertImage(imageFormat.name(), imageFormat.width(), imageFormat.height());
    if (!it.atEnd())
        return ++it;
    return it;
}

void MarkupDirector::processClosingElements(const QTextBlock::iterator &it)
{
    Q_D(MarkupDirector);
    // The order of closing elements is determined by the order they were opened
    // in.
    // The order of opened elements is in the openElements member list.
    // see testDifferentStartDoubleFinish and
    // testDifferentStartDoubleFinishReverseOrder

    if (d->m_openElements.isEmpty())
        return;

    auto elementsToClose = getElementsToClose(it);

    int previousSize;
    auto remainingSize = elementsToClose.size();
    while (!elementsToClose.isEmpty()) {
        auto tag = d->m_openElements.last();
        if (elementsToClose.contains(tag)) {
            switch (tag) {
            case Strong:
                m_builder->endStrong();
                break;
            case Emph:
                m_builder->endEmph();
                break;
            case Underline:
                m_builder->endUnderline();
                break;
            case StrikeOut:
                m_builder->endStrikeout();
                break;
            case SpanFontPointSize:
                m_builder->endFontPointSize();
                break;
            case SpanFontFamily:
                m_builder->endFontFamily();
                break;
            case SpanBackground:
                m_builder->endBackground();
                break;
            case SpanForeground:
                m_builder->endForeground();
                break;
            case Anchor:
                m_builder->endAnchor();
                break;
            case SubScript:
                m_builder->endSubscript();
                break;
            case SuperScript:
                m_builder->endSuperscript();
                break;

            default:
                break;
            }
            d->m_openElements.removeLast();
            elementsToClose.remove(tag);
        }
        previousSize = remainingSize;
        remainingSize = elementsToClose.size();

        if (previousSize == remainingSize) {
            // Iterated once through without closing any tags.
            // This means that there's overlap in the tags, such as
            // 'text with <b>some <i>formatting</i></b><i> tags</i>'
            // See testOverlap.
            // The top element in openElements must be a blocker, so close it on
            // next
            // iteration.
            elementsToClose.insert(d->m_openElements.last());
        }
    }
}

void MarkupDirector::processOpeningElements(const QTextBlock::iterator &it)
{
    Q_D(MarkupDirector);
    auto fragment = it.fragment();

    if (!fragment.isValid())
        return;

    const auto fragmentFormat = fragment.charFormat();
    const auto elementsToOpenList = getElementsToOpen(it);

    for (int tag : elementsToOpenList) {
        switch (tag) {
        case Strong:
            m_builder->beginStrong();
            break;
        case Emph:
            m_builder->beginEmph();
            break;
        case Underline:
            m_builder->beginUnderline();
            break;
        case StrikeOut:
            m_builder->beginStrikeout();
            break;
        case SpanFontPointSize:
            m_builder->beginFontPointSize(fragmentFormat.font().pointSize());
            d->m_openFontPointSize = fragmentFormat.font().pointSize();
            break;
        case SpanFontFamily:
            m_builder->beginFontFamily(fragmentFormat.fontFamily());
            d->m_openFontFamily = fragmentFormat.fontFamily();
            break;
        case SpanBackground:
            m_builder->beginBackground(fragmentFormat.background());
            d->m_openBackground = fragmentFormat.background();
            break;
        case SpanForeground:
            m_builder->beginForeground(fragmentFormat.foreground());
            d->m_openForeground = fragmentFormat.foreground();
            break;
        case Anchor: {
            // TODO: Multiple anchor names here.
            auto anchorNames = fragmentFormat.anchorNames();
            if (!anchorNames.isEmpty()) {
                while (!anchorNames.isEmpty()) {
                    auto n = anchorNames.last();
                    anchorNames.removeLast();
                    if (anchorNames.isEmpty()) {
                        // Doesn't matter if anchorHref is empty.
                        m_builder->beginAnchor(fragmentFormat.anchorHref(), n);
                        break;
                    } else {
                        // Empty <a> tags allow multiple names for the same
                        // section.
                        m_builder->beginAnchor(QString(), n);
                        m_builder->endAnchor();
                    }
                }
            } else {
                m_builder->beginAnchor(fragmentFormat.anchorHref());
            }
            d->m_openAnchorHref = fragmentFormat.anchorHref();
            break;
        }
        case SuperScript:
            m_builder->beginSuperscript();
            break;
        case SubScript:
            m_builder->beginSubscript();
            break;
        default:
            break;
        }
        d->m_openElements.append(tag);
        d->m_elementsToOpen.remove(tag);
    }
}

QSet<int> MarkupDirector::getElementsToClose(const QTextBlock::iterator &it) const
{
    Q_D(const MarkupDirector);
    QSet<int> closedElements;

    if (it.atEnd()) {
        // End of block?. Close all open tags.

        const QList<int> openElement = d->m_openElements;
        auto elementsToClose = QSet<int>(openElement.begin(), openElement.end());
        return elementsToClose.unite(d->m_elementsToOpen);
    }

    auto fragment = it.fragment();

    if (!fragment.isValid())
        return closedElements;

    const auto fragmentFormat = fragment.charFormat();

    const auto fontWeight = fragmentFormat.fontWeight();
    const auto fontItalic = fragmentFormat.fontItalic();
    const auto fontUnderline = fragmentFormat.fontUnderline();
    const auto fontStrikeout = fragmentFormat.fontStrikeOut();

    const auto fontForeground = fragmentFormat.foreground();
    const auto fontBackground = fragmentFormat.background();

    const auto fontFamily = fragmentFormat.fontFamily();
    const auto fontPointSize = fragmentFormat.font().pointSize();
    const auto anchorHref = fragmentFormat.anchorHref();

    const auto vAlign = fragmentFormat.verticalAlignment();
    const auto superscript = (vAlign == QTextCharFormat::AlignSuperScript);
    const auto subscript = (vAlign == QTextCharFormat::AlignSubScript);

    if (!fontStrikeout && (d->m_openElements.contains(StrikeOut) || d->m_elementsToOpen.contains(StrikeOut))) {
        closedElements.insert(StrikeOut);
    }

    if (!fontUnderline && (d->m_openElements.contains(Underline) || d->m_elementsToOpen.contains(Underline))
        && !(d->m_openElements.contains(Anchor) || d->m_elementsToOpen.contains(Anchor))) {
        closedElements.insert(Underline);
    }

    if (!fontItalic && (d->m_openElements.contains(Emph) || d->m_elementsToOpen.contains(Emph))) {
        closedElements.insert(Emph);
    }

    if (fontWeight != QFont::Bold && (d->m_openElements.contains(Strong) || d->m_elementsToOpen.contains(Strong))) {
        closedElements.insert(Strong);
    }

    if ((d->m_openElements.contains(SpanFontPointSize) || d->m_elementsToOpen.contains(SpanFontPointSize)) && (d->m_openFontPointSize != fontPointSize)) {
        closedElements.insert(SpanFontPointSize);
    }

    if ((d->m_openElements.contains(SpanFontFamily) || d->m_elementsToOpen.contains(SpanFontFamily)) && (d->m_openFontFamily != fontFamily)) {
        closedElements.insert(SpanFontFamily);
    }

    if ((d->m_openElements.contains(SpanBackground) && (d->m_openBackground != fontBackground))
        || (d->m_elementsToOpen.contains(SpanBackground) && (d->m_backgroundToOpen != fontBackground))) {
        closedElements.insert(SpanBackground);
    }

    if ((d->m_openElements.contains(SpanForeground) && (d->m_openForeground != fontForeground))
        || (d->m_elementsToOpen.contains(SpanForeground) && (d->m_foregroundToOpen != fontForeground))) {
        closedElements.insert(SpanForeground);
    }

    if ((d->m_openElements.contains(Anchor) && (d->m_openAnchorHref != anchorHref))
        || (d->m_elementsToOpen.contains(Anchor) && (d->m_anchorHrefToOpen != anchorHref))) {
        closedElements.insert(Anchor);
    }

    if (!subscript && (d->m_openElements.contains(SubScript) || d->m_elementsToOpen.contains(SubScript))) {
        closedElements.insert(SubScript);
    }

    if (!superscript && (d->m_openElements.contains(SuperScript) || d->m_elementsToOpen.contains(SuperScript))) {
        closedElements.insert(SuperScript);
    }
    return closedElements;
}

QList<int> MarkupDirector::getElementsToOpen(const QTextBlock::iterator &it)
{
    Q_D(MarkupDirector);
    auto fragment = it.fragment();
    if (!fragment.isValid()) {
        return QList<int>();
    }
    const auto fragmentFormat = fragment.charFormat();

    const auto fontWeight = fragmentFormat.fontWeight();
    const auto fontItalic = fragmentFormat.fontItalic();
    const auto fontUnderline = fragmentFormat.fontUnderline();
    const auto fontStrikeout = fragmentFormat.fontStrikeOut();

    const auto fontForeground = fragmentFormat.foreground();
    const auto fontBackground = fragmentFormat.background();

    const auto fontFamily = fragmentFormat.fontFamily();
    const auto fontPointSize = fragmentFormat.font().pointSize();
    const auto anchorHref = fragmentFormat.anchorHref();

    const auto vAlign = fragmentFormat.verticalAlignment();
    const auto superscript = (vAlign == QTextCharFormat::AlignSuperScript);
    const auto subscript = (vAlign == QTextCharFormat::AlignSubScript);

    if (superscript && !(d->m_openElements.contains(SuperScript))) {
        d->m_elementsToOpen.insert(SuperScript);
    }

    if (subscript && !(d->m_openElements.contains(SubScript))) {
        d->m_elementsToOpen.insert(SubScript);
    }

    if (!anchorHref.isEmpty() && !(d->m_openElements.contains(Anchor)) && (d->m_openAnchorHref != anchorHref)) {
        d->m_elementsToOpen.insert(Anchor);
        d->m_anchorHrefToOpen = anchorHref;
    }

    if (fontForeground != Qt::NoBrush
        && !(d->m_openElements.contains(SpanForeground)) // Can only open one
        // foreground element
        // at a time.
        && (fontForeground != d->m_openForeground)
        && !((d->m_openElements.contains(Anchor) // Links can't have a foreground color.
              || d->m_elementsToOpen.contains(Anchor)))) {
        d->m_elementsToOpen.insert(SpanForeground);
        d->m_foregroundToOpen = fontForeground;
    }

    if (fontBackground != Qt::NoBrush && !(d->m_openElements.contains(SpanBackground)) && (fontBackground != d->m_openBackground)) {
        d->m_elementsToOpen.insert(SpanBackground);
        d->m_backgroundToOpen = fontBackground;
    }

    if (!fontFamily.isEmpty() && !(d->m_openElements.contains(SpanFontFamily)) && (fontFamily != d->m_openFontFamily)) {
        d->m_elementsToOpen.insert(SpanFontFamily);
        d->m_fontFamilyToOpen = fontFamily;
    }

    if ((QTextCharFormat().font().pointSize() != fontPointSize) // Different from the default.
        && !(d->m_openElements.contains(SpanFontPointSize)) && (fontPointSize != d->m_openFontPointSize)) {
        d->m_elementsToOpen.insert(SpanFontPointSize);
        d->m_fontPointSizeToOpen = fontPointSize;
    }

    //   Only open a new bold tag if one is not already open.
    //   eg, <b>some <i>mixed</i> format</b> should be as is, rather than
    //   <b>some </b><b><i>mixed</i></b><b> format</b>

    if (fontWeight == QFont::Bold && !(d->m_openElements.contains(Strong))) {
        d->m_elementsToOpen.insert(Strong);
    }

    if (fontItalic && !(d->m_openElements.contains(Emph))) {
        d->m_elementsToOpen.insert(Emph);
    }

    if (fontUnderline && !(d->m_openElements.contains(Underline))
        && !(d->m_openElements.contains(Anchor) || d->m_elementsToOpen.contains(Anchor)) // Can't change the underline state of a link.
    ) {
        d->m_elementsToOpen.insert(Underline);
    }

    if (fontStrikeout && !(d->m_openElements.contains(StrikeOut))) {
        d->m_elementsToOpen.insert(StrikeOut);
    }

    if (d->m_elementsToOpen.size() <= 1) {
        return d->m_elementsToOpen.values();
    }
    return sortOpeningOrder(d->m_elementsToOpen, it);
}

QList<int> MarkupDirector::sortOpeningOrder(QSet<int> openingOrder, QTextBlock::iterator it) const
{
    QList<int> sortedOpenedElements;

    // This is an insertion sort in a way. elements in openingOrder are assumed
    // to
    // be out of order.
    // The rest of the block is traversed until there are no more elements to
    // sort, or the end is reached.
    while (!openingOrder.isEmpty()) {
        if (!it.atEnd()) {
            it++;

            if (!it.atEnd()) {
                // Because I've iterated, this returns the elements that will
                // be closed by the next fragment.
                const auto elementsToClose = getElementsToClose(it);

                // The exact order these are opened in is irrelevant, as all
                // will be
                // closed on the same block.
                // See testDoubleFormat.
                for (int tag : elementsToClose) {
                    if (openingOrder.remove(tag)) {
                        sortedOpenedElements.prepend(tag);
                    }
                }
            }
        } else {
            // End of block. Need to close all open elements.
            // Order irrelevant in this case.
            for (int tag : std::as_const(openingOrder)) {
                sortedOpenedElements.prepend(tag);
            }
            break;
        }
    }
    return sortedOpenedElements;
}
