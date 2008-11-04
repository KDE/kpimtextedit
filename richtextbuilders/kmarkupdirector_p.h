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

#include "kmarkupdirector.h"

#include <QSet>

//@cond PRIVATE

/**
 * @internal
 * Maintainability class for KMarkupDirector
 */
class KMarkupDirector::Private
{
  public:
    Private( KMarkupDirector *md )
        : q( md )
    {
    }

//     void closeFragmentElements( int ElementsToClose );

    void processClosingElements( QTextBlock::iterator it );
    void processOpeningElements( QTextBlock::iterator it );
    QSet< int > getElementsToClose( QTextBlock::iterator it );
    QList< int > getElementsToOpen( QTextBlock::iterator it );
    QList< int > sortOpeningOrder( QSet< int > openingOrder, QTextBlock::iterator it );

    KMarkupDirector *q;
    KAbstractMarkupBuilder *builder;

    enum OpenElementValues {
      None = 0x0,
      SuperScript = 0x01,
      SubScript = 0x02,
      Anchor = 0x04,
      SpanForeground = 0x08,
      SpanBackground = 0x10,
      SpanFontFamily = 0x20,
      SpanFontPointSize = 0x40,
      Strong = 0x80,
      Emph = 0x100,
      Underline = 0x200,
      StrikeOut = 0x400
    };
//     Q_DECLARE_FLAGS(OpenElements, OpenElementValues)

    QString openAnchorHref;
    QString anchorHrefToOpen;
//     QString openAnchorName;

    QBrush openForeground;
    QBrush foregroundToOpen;
    QBrush openBackground;
    QBrush backgroundToOpen;
    int openFontPointSize;
    int fontPointSizeToOpen;
    QString openFontFamily;
    QString fontFamilyToOpen;

//     /**
//     Holds the elements open while processing a fragment.
//     */
//     quint32 openElements;


//     QPair< QList< int > > changedElements;

    // An ordered list containing the order elements were opened in.
    QList< int > openElements;
    QSet< int > elementsToOpen;

//     QTextFragment currentFragment;
//     QTextFragment previousFragment;

    // Using the Qt flags system fails. Using an int instead.
//     OpenElements openElements;
};
// Q_DECLARE_OPERATORS_FOR_FLAGS(KMarkupDirector::Private::OpenElements)


//@endcond
