/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

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


#include <kdebug.h>

#include "markupdirector.h"
#include "abstractmarkupbuilder.h"
#include <QFlags>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QString>
#include <QTextFrame>
#include <QTextTable>
#include <QTextList>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QMap>
#include <QColor>
#include <QBrush>


//@cond PRIVATE

/**
 * @internal
 * Maintainability class for MarkupDirector
 */
class MarkupDirector::Private
{
public:
    Private ( MarkupDirector *md ) : q ( md ) {
    }

    void closeFragmentElements();

    MarkupDirector *q;
    AbstractMarkupBuilder *builder;

    enum OpenElementValues {
        None = 0x0,
        Strong = 0x01,
        Emph = 0x02,
        Underline = 0x04,
        StrikeOut = 0x08,
        SuperScript = 0x10,
        SubScript = 0x20,
        Anchor = 0x40,
        SpanForeground = 0x80,
        SpanBackground = 0x100,
        SpanFontFamily = 0x200,
        SpanFontPointSize = 0x400
    };
//     Q_DECLARE_FLAGS(OpenElements, OpenElementValues)

    QString openAnchorHref;
    QString openAnchorName;

    QBrush openForeground;
    QBrush openBackground;
    int openFontPointSize;
    QString openFontFamily;

    /**
    Holds the elements open while processing a fragment.
    */
    quint32 openElements;

    // Using the Qt flags system fails. Using an int instead.
//     OpenElements openElements;
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(MarkupDirector::Private::OpenElements)

void MarkupDirector::Private::closeFragmentElements()
{
    if ( openElements & Strong ) {
        builder->endStrong();
        openElements = openElements & ~Strong;
    }

    if ( openElements & Emph ) {
        builder->endEmph();
        openElements = openElements & ~Emph;
    }

}

//@endcond


MarkupDirector::MarkupDirector ( AbstractMarkupBuilder* builder ) :
        d ( new Private ( this ) )
{
    d->builder = builder;
}

MarkupDirector::~MarkupDirector()
{
    delete d;
}

void MarkupDirector::processDocumentContents ( QTextFrame::iterator start, QTextFrame::iterator end )
{

    for ( QTextFrame::iterator it = start; ( ( !it.atEnd() ) && ( it != end ) ); ++it ) {
        QTextFrame *frame = it.currentFrame();
        if ( frame ) {
            QTextTable *table = dynamic_cast<QTextTable*> ( frame );
            if ( table ) {
                processTable ( table );
            } else {
                processFrame ( frame );
            }
        } else {
            processBlock ( it.currentBlock() );
        }
    }
}

void MarkupDirector::processFrame ( QTextFrame* frame )
{
    processDocumentContents ( frame->begin(), frame->end() );
}

void MarkupDirector::processBlock ( const QTextBlock &block )
{
    if ( block.isValid() ) {
        QTextList *list = block.textList();
        if ( list ) {
            // Just skip over if not the first item in a list.
            if ( ( list->item ( 0 ) == block ) && ( !block.previous().textList() ) ) {
                processList ( block );
            }
        } else {
            processBlockContents ( block );
        }
    }
}

void MarkupDirector::processTable ( QTextTable *table )
{
    QTextTableFormat format = table->format();
    QVector<QTextLength> colLengths = format.columnWidthConstraints();

    QTextLength tableWidth = format.width();
    QString sWidth;

    if ( tableWidth.type() == QTextLength::PercentageLength ) {
        sWidth = "%1%";
        sWidth = sWidth.arg ( tableWidth.rawValue() );
    } else if ( tableWidth.type() == QTextLength::FixedLength ) {
        sWidth = "%1";
        sWidth = sWidth.arg ( tableWidth.rawValue() );
    }

    d->builder->beginTable ( format.cellPadding(), format.cellSpacing(), sWidth );

    int headerRowCount = format.headerRowCount();

    QList<QTextTableCell> alreadyProcessedCells;

    for ( int row = 0; row < table->rows(); ++row ) {
        // Put a thead element around here somewhere?
        // if (row < headerRowCount)
        // {
        // d->builder->beginTableHeader();
        // }

        d->builder->beginTableRow();

        // Header attribute should really be on cells, not determined by number of rows.
        //http://www.webdesignfromscratch.com/html-tables.cfm


        for ( int column = 0; column < table->columns(); ++column ) {

            // TODO: Cells should support alignment.


            QTextTableCell tableCell = table->cellAt ( row, column );

            int columnSpan = tableCell.columnSpan();
            int rowSpan = tableCell.rowSpan();
            if ( ( rowSpan > 1 ) || ( columnSpan > 1 ) ) {
                if ( alreadyProcessedCells.contains ( tableCell ) ) {
                    // Already processed this cell. Move on.
                    continue;
                } else {
                    alreadyProcessedCells.append ( tableCell );
                }
            }

            QTextLength cellWidth = colLengths.at ( column );

            QString sCellWidth;

            if ( cellWidth.type() == QTextLength::PercentageLength ) {
                sCellWidth = "%1%";
                sCellWidth = sCellWidth.arg ( cellWidth.rawValue() );
            } else if ( cellWidth.type() == QTextLength::FixedLength ) {
                sCellWidth = "%1";
                sCellWidth = sCellWidth.arg ( cellWidth.rawValue() );
            }

            // TODO: Use THEAD instead
            if ( row < headerRowCount ) {
                d->builder->beginTableHeaderCell ( sCellWidth, columnSpan, rowSpan );
            } else {
                d->builder->beginTableCell ( sCellWidth, columnSpan, rowSpan );
            }

            processTableCell ( tableCell );

            if ( row < headerRowCount ) {
                d->builder->endTableHeaderCell();
            } else {
                d->builder->endTableCell();
            }
        }
        d->builder->endTableRow();
    }
    d->builder->endTable();
}

void MarkupDirector::processTableCell ( const QTextTableCell &cell )
{
    processDocumentContents ( cell.begin(), cell.end() );
}

void MarkupDirector::processList ( const QTextBlock &ablock )
{
    QTextBlock block ( ablock );

    QTextList *list = block.textList();
    if ( !list ) {
        return;
    }

    QList<QTextList*> lists;

    while ( block.isValid() && block.textList() ) {
        if ( list->item ( 0 ) == block ) {
            // Item zero in a list is the first block in the list of blocks that make up a list.
            QTextListFormat::Style style = list->format().style();
            d->builder->beginList ( style );

            lists.append ( list );
        }

        d->builder->beginListItem();
        processBlockContents ( block );
        d->builder->endListItem();

        block = block.next();

        if ( block.isValid() ) {
            QTextList *newList = block.textList();

            if ( !newList ) {
                while ( !lists.isEmpty() ) {
                    lists.removeLast();
                    d->builder->endList();
                }
            } else if ( newList == list ) {
                //Next block is on the same list; Handled on next iteration.
                continue;
            } else if ( newList != list ) {
                if ( newList->item ( 0 ) == block ) {
                    list = newList;
                    continue;
                } else {
                    while ( !lists.isEmpty() ) {
                        if ( block.textList() != lists.last() ) {
                            lists.removeLast();
                            d->builder->endList();
                        } else {
                            break;
                        }
                    }
                    continue;
                }
            }
        } else {
            // Next block is not valid. Maybe at EOF. Close all open lists.
            while ( !lists.isEmpty() ) {
                lists.removeLast();
                d->builder->endList();
            }
        }
    }
}

void MarkupDirector::processBlockContents ( const QTextBlock &block )
{
    QTextBlockFormat blockFormat = block.blockFormat();
    Qt::Alignment blockAlignment = blockFormat.alignment();

    // TODO: decide when to use <h1> etc.

    if ( blockFormat.hasProperty ( QTextFormat::BlockTrailingHorizontalRulerWidth ) ) {
        d->builder->insertHorizontalRule();
        return;
    }

    if ( block.length() == 1 ) {
        // If the length is one, it is only the newline and the block is empty.
        // TODO: Find a better way of determining this.
        d->builder->addNewline();

    } else {

        d->builder->beginParagraph ( blockAlignment,
                                     blockFormat.topMargin(),
                                     blockFormat.bottomMargin(),
                                     blockFormat.leftMargin(),
                                     blockFormat.rightMargin()
                                   );

        QTextBlock::iterator it;
        // Reset the opened and closed tags to none open at the beginnning of the paragraph.
        d->openElements = 0;
//         d->openedElements = 0;
//         d->closedElements = 0;
        for ( it = block.begin(); !it.atEnd(); ++it ) {
            QTextFragment currentFragment = it.fragment();
            if ( currentFragment.isValid() ) {
                processFragment ( currentFragment );
            }
        }
        d->closeFragmentElements();

        d->builder->endParagraph();
    }
}

void MarkupDirector::processFragment ( const QTextFragment &fragment )
{
    QTextCharFormat fragmentFormat = fragment.charFormat();

    if ( fragmentFormat.isImageFormat() ) {
        QTextImageFormat imageFormat = fragmentFormat.toImageFormat();
        d->builder->insertImage ( imageFormat.name(), imageFormat.width(), imageFormat.height() );
    } else {
        int fontWeight = fragmentFormat.fontWeight();
        bool fontItalic = fragmentFormat.fontItalic();
        bool fontUnderline = fragmentFormat.fontUnderline();
        bool fontStrikeout = fragmentFormat.fontStrikeOut();

        QBrush fontForeground = fragmentFormat.foreground();
        QBrush fontBackground = fragmentFormat.background();

        QString fontFamily = fragmentFormat.fontFamily();
        int fontPointSize = fragmentFormat.font().pointSize();
        QString anchorHref = fragmentFormat.anchorHref();

        QTextCharFormat::VerticalAlignment vAlign = fragmentFormat.verticalAlignment();
        bool superscript = ( vAlign == QTextCharFormat::AlignSuperScript );
        bool subscript = ( vAlign == QTextCharFormat::AlignSubScript );


        // ### Warning: If adding new tags, make sure to add them to
        // the MarkupDirector::Private::closeFragmentElements method too.


// Closing tags.

//         if ( QTextCharFormat().font().pointSize() != fontPointSize ) {
//             d->builder->endFontPointSize();
//         }
//


        if ( ( d->openElements & d->SpanFontPointSize )
                && ( d->openFontPointSize != fontPointSize )
//                 && ( d->openFontPointSize == -1 )
            )
        {
            d->builder->endFontPointSize();
            d->openElements = d->openElements & ~d->SpanFontPointSize;
            d->openFontPointSize = QTextCharFormat().font().pointSize();
        }


        if ( ( d->openElements & d->SpanFontFamily )
                && ( d->openFontFamily != fontFamily )
            )
        {
            d->builder->endFontFamily();
            d->openElements = d->openElements & ~d->SpanFontFamily;
            d->openFontFamily.clear();
        }

        if ( ( d->openElements & d->SpanBackground )
                && ( d->openBackground != fontBackground )
            )
        {
            d->builder->endBackground();
            d->openElements = d->openElements & ~d->SpanBackground;
            d->openBackground = Qt::NoBrush;
        }

        if ( ( d->openElements & d->SpanForeground )
                && ( d->openForeground != fontForeground )
            )
        {
            d->builder->endForeground();
            d->openElements = d->openElements & ~d->SpanForeground;
            d->openForeground = Qt::NoBrush;
        }

//

//         kDebug() << "processing: " << fragment.text() << "bold=" << (fontWeight == QFont::Bold)<< "boldOpen="
//             << ( d->openElements & d->Strong ) << "boldOpened="
//             << ( d->openedElements & d->Strong ) << "em=" << fontItalic << "emOpen="
//             << ( d->openElements & d->Emph );

//
        if ( !fontStrikeout && ( d->openElements & d->StrikeOut ) ) {
            d->builder->endStrikeout();
            d->openElements = d->openElements & ~d->StrikeOut;
        }

        if ( !fontUnderline && ( d->openElements & d->Underline ) ) {
            d->builder->endUnderline();
            d->openElements = d->openElements & ~d->Underline;
        }

        if ( !fontItalic && ( d->openElements & d->Emph ) ) {
            d->builder->endEmph();
            d->openElements = d->openElements & ~d->Emph;
        }

        if ( fontWeight != QFont::Bold && ( d->openElements & d->Strong ) ) {
            d->builder->endStrong();
            d->openElements = d->openElements & ~d->Strong;
        }

        if ( ( d->openElements & d->Anchor )
                && ( d->openAnchorHref != anchorHref ) // Could be adjacent different anchors.
            )
        {
            d->builder->endAnchor();
            d->openElements = d->openElements & ~d->Anchor;
            d->openAnchorHref.clear();
        }

        if ( !subscript && ( d->openElements & d->SubScript ) ) {
            d->builder->endSubscript();
            d->openElements = d->openElements & ~d->SubScript;
        }

        if ( !superscript && ( d->openElements & d->SuperScript ) ) {
            d->builder->endSuperscript();
            d->openElements = d->openElements & ~d->SuperScript;
        }


        // Opening tags

        if ( superscript && !(d->openElements & d->SuperScript) ) {
            d->builder->beginSuperscript();
            d->openElements |= d->SuperScript;
        }

        if ( subscript && !(d->openElements & d->SubScript) ) {
            d->builder->beginSubscript();
            d->openElements |= d->SubScript;
        }

        if ( !anchorHref.isEmpty()
                && !(d->openElements & d->Anchor)
                && (d->openAnchorHref != anchorHref)
                ) {
            d->builder->beginLinkedAnchor ( anchorHref );
            d->openElements |= d->Anchor;
//             d->openAnchorName = anchorName;
            d->openAnchorHref = anchorHref;
        }

        // Only open a new bold tag if one is not already open.
        // eg, <b>some <i>mixed</i> format</b> should be as is, rather than
        // <b>some </b><b><i>mixed</i></b><b> format</b>
        if ( fontWeight == QFont::Bold && !(d->openElements & d->Strong) ) {
            d->builder->beginStrong();
            d->openElements |= d->Strong;
        }

        if ( fontItalic && !(d->openElements & d->Emph) ) {
            d->builder->beginEmph();
            d->openElements |= d->Emph;
        }

        if ( fontUnderline
                && !(d->openElements & d->Underline)
                && !(d->openElements & d->Anchor ) // Can't change the underline or foreground colour of a link.
                ) {
            d->builder->beginUnderline();
            d->openElements |= d->Underline;
        }

        if ( fontStrikeout && !(d->openElements & d->StrikeOut) ) {
            d->builder->beginStrikeout();
            d->openElements |= d->StrikeOut;
        }

        if ( fontForeground != Qt::NoBrush
            && !( d->openElements & d->SpanForeground ) // Can only open one foreground element at a time.
            && ( fontForeground != d->openForeground )
            && !( d->openElements & d->Anchor ) // Can't change the underline or foreground colour of a link.
            )
        {
            d->builder->beginForeground ( fontForeground );
            d->openElements |= d->SpanForeground;
            d->openForeground = fontForeground;
        }

        if ( fontBackground != Qt::NoBrush
            && !( d->openElements & d->SpanBackground )
            && ( fontBackground != d->openBackground )
            )
        {
            d->builder->beginBackground ( fontBackground );
            d->openElements |= d->SpanBackground;
            d->openBackground = fontBackground;
        }


        if ( !fontFamily.isEmpty()
            && !( d->openElements & d->SpanFontFamily )
            && ( fontFamily != d->openFontFamily )
            )
        {
            d->builder->beginFontFamily ( fontFamily );
            d->openElements |= d->SpanFontFamily;
            d->openFontFamily = fontFamily;
        }

        if ( ( QTextCharFormat().font().pointSize() != fontPointSize ) // Different from the default.
            && !( d->openElements & d->SpanFontPointSize )
            && ( fontPointSize != d->openFontPointSize )
            )
        {
            d->builder->beginFontPointSize ( fontPointSize );
            d->openElements |= d->SpanFontPointSize;
            d->openFontPointSize = fontPointSize;
        }

//         if ( QTextCharFormat().font().pointSize() != fontPointSize ) {
//             d->builder->beginFontPointSize ( fontPointSize );
//         }

        d->builder->appendLiteralText ( fragment.text() );
    }
}

void MarkupDirector::constructContent ( QTextDocument* doc )
{
    QTextFrame *rootFrame = doc->rootFrame();
    processFrame ( rootFrame );
}
