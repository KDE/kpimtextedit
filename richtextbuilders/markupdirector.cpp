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
#include <QStack>
#include <QTextFrame>
#include <QTextTable>
#include <QTextList>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QMap>
#include <QColor>
#include <QBrush>

#include "markupdirector_p.h"


MarkupDirector::MarkupDirector( AbstractMarkupBuilder* builder ) :
    d( new Private( this ) )
{
  d->builder = builder;
}

MarkupDirector::~MarkupDirector()
{
  delete d;
}

void MarkupDirector::processDocumentContents( QTextFrame::iterator start, QTextFrame::iterator end )
{

  for ( QTextFrame::iterator it = start; (( !it.atEnd() ) && ( it != end ) ); ++it ) {
    QTextFrame *frame = it.currentFrame();
    if ( frame ) {
      QTextTable *table = dynamic_cast<QTextTable*>( frame );
      if ( table ) {
        processTable( table );
      } else {
        processFrame( frame );
      }
    } else {
      processBlock( it.currentBlock() );
    }
  }
}

void MarkupDirector::processFrame( QTextFrame* frame )
{
  processDocumentContents( frame->begin(), frame->end() );
}

void MarkupDirector::processBlock( const QTextBlock &block )
{
  if ( block.isValid() ) {
    QTextList *list = block.textList();
    if ( list ) {
      // An entire list is processed when first found.
      // Just skip over if not the first item in a list.
      if (( list->item( 0 ) == block ) && ( !block.previous().textList() ) ) {
        processList( block );
      }
    } else {
      processBlockContents( block );
    }
  }
}

void MarkupDirector::processTable( QTextTable *table )
{
  QTextTableFormat format = table->format();
  QVector<QTextLength> colLengths = format.columnWidthConstraints();

  QTextLength tableWidth = format.width();
  QString sWidth;

  if ( tableWidth.type() == QTextLength::PercentageLength ) {
    sWidth = "%1%";
    sWidth = sWidth.arg( tableWidth.rawValue() );
  } else if ( tableWidth.type() == QTextLength::FixedLength ) {
    sWidth = "%1";
    sWidth = sWidth.arg( tableWidth.rawValue() );
  }

  d->builder->beginTable( format.cellPadding(), format.cellSpacing(), sWidth );

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


      QTextTableCell tableCell = table->cellAt( row, column );

      int columnSpan = tableCell.columnSpan();
      int rowSpan = tableCell.rowSpan();
      if ( ( rowSpan > 1 ) || ( columnSpan > 1 ) ) {
        if ( alreadyProcessedCells.contains( tableCell ) ) {
          // Already processed this cell. Move on.
          continue;
        } else {
          alreadyProcessedCells.append( tableCell );
        }
      }

      QTextLength cellWidth = colLengths.at( column );

      QString sCellWidth;

      if ( cellWidth.type() == QTextLength::PercentageLength ) {
        sCellWidth = "%1%";
        sCellWidth = sCellWidth.arg( cellWidth.rawValue() );
      } else if ( cellWidth.type() == QTextLength::FixedLength ) {
        sCellWidth = "%1";
        sCellWidth = sCellWidth.arg( cellWidth.rawValue() );
      }

      // TODO: Use THEAD instead
      if ( row < headerRowCount ) {
        d->builder->beginTableHeaderCell( sCellWidth, columnSpan, rowSpan );
      } else {
        d->builder->beginTableCell( sCellWidth, columnSpan, rowSpan );
      }

      processTableCell( tableCell );

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

void MarkupDirector::processTableCell( const QTextTableCell &cell )
{
  processDocumentContents( cell.begin(), cell.end() );
}

void MarkupDirector::processList( const QTextBlock &ablock )
{
  QTextBlock block( ablock );

  QTextList *list = block.textList();
  if ( !list ) {
    return;
  }

  QList<QTextList*> lists;

  while ( block.isValid() && block.textList() ) {
    if ( list->item( 0 ) == block ) {
      // Item zero in a list is the first block in the list of blocks that make up a list.
      QTextListFormat::Style style = list->format().style();
      d->builder->beginList( style );

      lists.append( list );
    }

    d->builder->beginListItem();
    processBlockContents( block );
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
        if ( newList->item( 0 ) == block ) {
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

void MarkupDirector::processBlockContents( const QTextBlock &block )
{
  QTextBlockFormat blockFormat = block.blockFormat();
  Qt::Alignment blockAlignment = blockFormat.alignment();

  // TODO: decide when to use <h1> etc.

  if ( blockFormat.hasProperty( QTextFormat::BlockTrailingHorizontalRulerWidth ) ) {
    d->builder->insertHorizontalRule();
    return;
  }

  QTextBlock::iterator it;
  it = block.begin();

  // The beginning is the end. This is an empty block. Insert a newline and move on.
  // This is what gets generated by a QTextEdit...
  if ( it.atEnd() )
  {
//     kDebug() << "The beginning is the end";
    d->builder->addNewline();
    return;
  }

  QTextFragment fragment = it.fragment();

  // .. but if a sequence such as '<br /><br />' is imported into a document with setHtml, Separator_Line
  // characters are inserted here within one block. See testNewlines and testNewlinesThroughQTextEdit.
  if ( fragment.isValid() ) {
    QTextCharFormat fragmentFormat = fragment.charFormat();

    if ( !fragmentFormat.isImageFormat() && fragment.text().at(0).category() == QChar::Separator_Line ) {

      // Consecutive newlines in a qtextdocument are in a single fragment if inserted with setHtml.
      foreach (QChar c, fragment.text() )
      {
//         kDebug() << c;
        if ( c.category() == QChar::Separator_Line )
        {
          d->builder->addNewline();
        }
      }
      return;
    }
  }
  d->builder->beginParagraph( blockAlignment,
                              blockFormat.topMargin(),
                              blockFormat.bottomMargin(),
                              blockFormat.leftMargin(),
                              blockFormat.rightMargin()
                            );
  while ( !it.atEnd() ) {
    fragment = it.fragment();
    if ( fragment.isValid() ) {
      QTextCharFormat fragmentFormat = fragment.charFormat();

      if ( fragmentFormat.isImageFormat() ) {
        // TODO: Close any open format elements?
        QTextImageFormat imageFormat = fragmentFormat.toImageFormat();
        d->builder->insertImage( imageFormat.name(), imageFormat.width(), imageFormat.height() );
      } else {
        d->processOpeningElements( it );
        d->builder->appendLiteralText( fragment.text() );
      }
      ++it;
      d->processClosingElements( it );
    }
  }
  d->builder->endParagraph();

}

void MarkupDirector::constructContent( QTextDocument* doc )
{
  QTextFrame *rootFrame = doc->rootFrame();
  processFrame( rootFrame );
}
