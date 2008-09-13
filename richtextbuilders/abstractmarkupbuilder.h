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

// Any class for which you cannot create instances is an abstract class.

#ifndef ABSTRACTMARKUPBUILDER_H
#define ABSTRACTMARKUPBUILDER_H


#include <QString>
#include <QBrush>
#include <QTextList>

/**
@brief The AbstractMarkupBuilder class serves as a base class for creating marked up plain text output.

The AbstractMarkupBuilder is used by the MarkupDirector to create marked up output such as html or markdown.

Subclasses can reimplement whichever methods they choose. None of the methods are pure virtual and all default to an empty function
to allow a clean fall-through. The exception is appendLiteralText, which appends its argument to the text being built.

See PlainTextMarkupBuilder and HTMLBuilder for example imlpementations.

@todo Move this to kdelibs when tested.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2
*/
class AbstractMarkupBuilder
{
public:
    /** Destructor */
    virtual ~AbstractMarkupBuilder() {}

    /** Begin a bold element in the markup */
    virtual void beginStrong() { }
    /** Close the bold element in the markup */
    virtual void endStrong() { }
    /** Begin an emphasised element in the markup */
    virtual void beginEmph() { }
    /** Close the emphasised element in the markup */
    virtual void endEmph() { }
    /** Begin an underlined element in the markup */
    virtual void beginUnderline() { }
    /** Close the underlined element in the markup */
    virtual void endUnderline() { }
    /** Begin a striked out element in the markup */
    virtual void beginStrikeout() { }
    /** Close the striked out element in the markup */
    virtual void endStrikeout() { }
    /** Begin a decorarated foreground element in the markup (A text color) */
    virtual void beginForeground(const QBrush &) { }
    /** Close the decorarated foreground element in the markup */
    virtual void endForeground() { }
    /** Begin a decorarated background element in the markup (A text background color) */
    virtual void beginBackground(const QBrush &) { }
    /** Close the decorarated background element in the markup */
    virtual void endBackground() { }
    /** Begin a url anchor element in the markup     
    @param The href of the link.    
    */
    virtual void beginAnchor(const QString &) { }
    /** Close the anchor element */
    virtual void endAnchor() { }

    /** Begin a new font familiy element in the markup     
    @param The name of the font familiy to begin.
    */
    virtual void beginFontFamily(const QString &) { }
    /** End font family element */
    virtual void endFontFamily() { }
    
    /** Begin a new font point size element in the markup
    @param the point size to begin.
    */
    virtual void beginFontPointSize( int ) { }
    /** End font point size element */
    virtual void endFontPointSize() { }

//    /** Begin a new paragraph in the markup
//    @param The alignment of the new paragraph.
//     */
//     virtual void beginParagraph(Qt::Alignment) { }

    /** Begin a new paragraph in the markup
    @param The alignment of the new paragraph.
    @param The top margin of the new paragraph.
    @param The bottom margin of the new paragraph.
     */
    virtual void beginParagraph(Qt::Alignment, qreal, qreal) { }
    
    /** Close the paragraph in the markup. */
    virtual void endParagraph() { }
    /** Add a newline to the markup. */
    virtual void addNewline() { }

    /** Insert a horizontal rule into the markup.
    @warning This is never called by the MarkupDirector as there is no way to know if a QTextDocument contains a horizontal rule.
    Just a placeholder until a workaround is written.
    */
    virtual void insertHorizontalRule() { }
    
    /**
    Insert a new image element into the markup.
    @param The url of the image
    @param The width of the image
    @param The height of the image.
    */
    virtual void insertImage(const QString &, qreal, qreal) { }

    /**
    Begin a new list element in the markup.
    A list element contains list items, and may contain other lists.
    @param The style of list to create.
    */
    virtual void beginList(QTextListFormat::Style) { }
    
    /**
    Close the list.
    */
    virtual void endList() { }
    /** Begin a new list item in the markup */
    virtual void beginListItem() { }
    /** End the list item */
    virtual void endListItem() { }

    /** Begin a superscript element */
    virtual void beginSuperscript() { }

    /** End superscript element */
    virtual void endSuperscript() { }
    
    /** Begin a subscript element */
    virtual void beginSubscript() { }
    
    /** End subscript element */
    virtual void endSubscript() { }

    virtual void beginTable(qreal, qreal, const QString &) { }
    virtual void beginTableRow() { }
    virtual void beginTableHeaderCell(QString, int, int) { }
    virtual void beginTableCell(QString, int, int) { }

    virtual void endTable() { }
    virtual void endTableRow() { }
    virtual void endTableHeaderCell() { }
    virtual void endTableCell() { }




            

    /**
    Append the plain text @p text to the markup.
    
    @param The text to append.
    */
    virtual void appendLiteralText(const QString &text ) = 0; 

    /** Return the fully marked up result of the building process. This may contain metadata etc, such as a head element in html.
    
    @return The fully marked up text.
    */
    virtual QString& getResult() = 0;

};

#endif
