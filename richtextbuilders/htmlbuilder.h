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

#ifndef HTMLBUILDER_H
#define HTMLBUILDER_H

#include "abstractmarkupbuilder.h"

/**
@brief The HTMLBuilder creates a clean html markup output.

This class creates html output which is as minimal as possible and restricted to the rich text features supported in Qt. (http://doc.trolltech.com/4.4/richtext-html-subset.htm)

The output contains only the body content, not the head element or other metadata.

eg:

@code
    This is some <strong>formatted content</strong> in a paragraph.
@endcode

instead of:

@code
    <head>
    <title>Some text</title>
    <head>
    <body>
        This is some <strong>formatted content</strong> in a paragraph.
    </body>
    </html>
@endcode

Such tags should be created separately. For example:

@code
        AbstractMarkupBuilder *b = new HTMLBuilder();
        MarkupDirector *md = new MarkupDirector(b);
        md->constructContent();
        QString cleanHtml("<head>\n<title>%1</title>\n</head>\n<body>%2</body>\n</html>")
                    .arg(document.metaInformation(QTextDocument::DocumentTitle))
                    .arg(b->getOutput());
        QFile.write(cleanHtml);
@endcode

Font formatting information on elements is represented by individual span elements.
eg:
@code
    <span style"color:blue;"><span style="background-color:red;">Blue text on red background</span></span>
@endcode
instead of
@code
    <span style="color:blue;background-color:red;">Blue text on red background</span>
@endcode
 It my be possible to change this if necessary.

@todo Move this to kdelibs when tested and prooven.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class HTMLBuilder : public AbstractMarkupBuilder
{
public:

    /**
    Creates a new HTMLBuilder.
    */
    HTMLBuilder();
    virtual ~HTMLBuilder();

    virtual void beginStrong();
    virtual void endStrong();
    virtual void beginEmph();
    virtual void endEmph();
    virtual void beginUnderline();
    virtual void endUnderline();
    virtual void beginStrikeout();
    virtual void endStrikeout();
    virtual void beginForeground ( const QBrush &brush );
    virtual void endForeground();
    virtual void beginBackground ( const QBrush &brush );
    virtual void endBackground();
    virtual void beginAnchor ( const QString &href= QString(), const QString &name= QString() ) {
        m_text.append ( QString ( "<a href=\"%1\">" ).arg ( href ) );
    }
    virtual void endAnchor();

    // Maybe this stuff should just be added to a list, and then when I add literal text,
    // add some kind of style attribute in one span instead of many.
    virtual void beginFontFamily ( const QString &family );
    virtual void endFontFamily();
    virtual void beginFontPointSize ( int size );
    virtual void endFontPointSize();

    virtual void beginParagraph ( Qt::Alignment al, qreal topMargin, qreal bottomMargin, qreal leftMargin, qreal rightMargin );

    virtual void beginHeader1();
    virtual void beginHeader2();
    virtual void beginHeader3();
    virtual void beginHeader4();
    virtual void beginHeader5();
    virtual void beginHeader6();

    virtual void endHeader1();
    virtual void endHeader2();
    virtual void endHeader3();
    virtual void endHeader4();
    virtual void endHeader5();
    virtual void endHeader6();

    virtual void endParagraph();
    virtual void addNewline();

    virtual void insertHorizontalRule ( int width = -1 );

    virtual void insertImage ( const QString &src, qreal width, qreal height );

    virtual void beginList ( QTextListFormat::Style type );

    virtual void endList();

    virtual void beginListItem();
    virtual void endListItem();

    virtual void beginSuperscript();

    virtual void endSuperscript();

    virtual void beginSubscript();

    virtual void endSubscript();


    virtual void beginTable ( qreal cellpadding, qreal cellspacing, const QString &width );

    virtual void beginTableRow();
    virtual void beginTableHeaderCell ( QString width, int colspan, int rowspan );

    virtual void beginTableCell ( QString width, int colspan, int rowspan );

    virtual void endTable();
    virtual void endTableRow();
    virtual void endTableHeaderCell();
    virtual void endTableCell();

    /**
    Reimplemented from AbstractMarkupBuilder.

    This implementation escapes the text before appending so that

    @verbatim
        A sample <b>bold</b> word.
    @endverbatim

    becomes

    @verbatim
         A sample &lt;b&gt;bold&lt;/b&gt; word.
    @endverbatim

    */
    virtual void appendLiteralText ( const QString &text );


    virtual QString& getResult();

private:
    QList<QTextListFormat::Style> currentListItemStyles;

    QString m_text;

};

#endif
