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

    This is some <strong>formatted content</strong> in a paragraph.
    
instead of: 

    <head>
    <title>Some text</title>
    <head>
    <body>
        This is some <strong>formatted content</strong> in a paragraph.
    </body>
    </html>

Such tags should be created separately. For example:

        AbstractMarkupBuilder *b = new HTMLBuilder();        
        MarkupDirector *md = new MarkupDirector(b);
        md->constructContent();        
        QString cleanHtml("<head>\n<title>%1</title>\n</head>\n<body>%2</body>\n</html>")
                    .arg(document.metaInformation(QTextDocument::DocumentTitle))
                    .arg(b->getOutput());   
        QFile.write(cleanHtml);

Font formatting information on elements is represented by individual span elements.
eg:
    <span style"color:blue;"><span style="background-color:red;">Blue text on red background</span></span>
instead of 
    <span style="color:blue;background-color:red;">Blue text on red background</span>
    
 It my be possible to change this if neccessary.

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
    HTMLBuilder() {}

    virtual void beginStrong() { m_text.append("<strong>"); }
    virtual void endStrong() { m_text.append("</strong>"); }
    virtual void beginEmph() { m_text.append("<em>"); }
    virtual void endEmph() { m_text.append("</em>"); }
    virtual void beginUnderline() { m_text.append("<u>"); }
    virtual void endUnderline() { m_text.append("</u>"); }
    virtual void beginStrikeout() { m_text.append("<s>"); } 
    virtual void endStrikeout() { m_text.append("</s>"); }
    virtual void beginForeground(const QBrush &brush) { m_text.append(QString("<span style=\"color:%1;\">").arg(brush.color().name())); }
    virtual void endForeground() { m_text.append("</span>"); }
    virtual void beginBackground(const QBrush &brush) { m_text.append(QString("<span style=\"background-color:%1;\">").arg(brush.color().name())); }
    virtual void endBackground() { m_text.append("</span>"); }
    virtual void beginAnchor(const QString &href) { m_text.append(QString("<a href=\"%1\">").arg(href)); }
    virtual void endAnchor() { m_text.append("</a>"); }

    // Maybe this stuff should just be added to a list, and then when I add literal text,
    // add some kind of style attribute in one span instead of many.
    virtual void beginFontFamily(const QString &family) { m_text.append(QString("<span style=\"font-family:%1;\">").arg(family)); }
    virtual void endFontFamily() { m_text.append("</span>"); }
    virtual void beginFontPointSize(int size) { m_text.append(QString("<span style=\"font-size:%1pt;\">").arg(QString::number(size))); }
    virtual void endFontPointSize() { m_text.append("</span>"); }

    virtual void beginParagraph(Qt::Alignment al, qreal topMargin, qreal bottomMargin)
    {
        QString styleString("style=\"margin-top:%1;margin-bottom:%2;\">\n");
        styleString = styleString.arg(topMargin).arg(bottomMargin);
        if (al & Qt::AlignRight){
            m_text.append("<p align=\"right\" ");
        }
        else if (al & Qt::AlignHCenter){
            m_text.append("<p align=\"center\" ");
        }
        else if (al & Qt::AlignJustify){
            m_text.append("<p align=\"justify\" ");
        }
        else if (al & Qt::AlignLeft){
            m_text.append("<p ");
        }
        else{
            m_text.append("<p ");
        }
        m_text.append(styleString);
    }
    
    virtual void endParagraph() { m_text.append("\n</p>\n"); }
    virtual void addNewline() { m_text.append("<br />\n"); }

    virtual void insertHorizontalRule() { m_text.append("<hr />\n"); }

    virtual void insertImage(const QString &src, qreal width, qreal height) {
        
        m_text.append(QString("<img src=\"%1\" width=\"%2\" height=\"%3\" />").arg(src).arg(width).arg(height));
    }

    virtual void beginList(QTextListFormat::Style type) {
        currentListItemStyles.append(type);
        switch(type){
        case QTextListFormat::ListDisc:
            m_text.append("\n<ul type=\"disc\">\n");
            break;
        case QTextListFormat::ListCircle:
            m_text.append("\n<ul type=\"circle\">\n");
            break;
        case QTextListFormat::ListSquare:
            m_text.append("\n<ul type=\"square\">\n");
            break;
        case QTextListFormat::ListDecimal:
            m_text.append("\n<ol type=\"1\">\n");
            break;
        case QTextListFormat::ListLowerAlpha:
            m_text.append("\n<ol type=\"a\">\n");
            break;
        case QTextListFormat::ListUpperAlpha:
            m_text.append("\n<ol type=\"A\">\n");
            break;
        default:
            break;
        }
    }
    virtual void endList() {
        switch(currentListItemStyles.last()){
        case QTextListFormat::ListDisc:
        case QTextListFormat::ListCircle:
        case QTextListFormat::ListSquare:
            m_text.append("\n</ul>\n");
            break;
        case QTextListFormat::ListDecimal:
        case QTextListFormat::ListLowerAlpha:
        case QTextListFormat::ListUpperAlpha:
            m_text.append("\n</ol>\n");
            break;
        default:
            break;
        }
        currentListItemStyles.removeLast();
    }
    virtual void beginListItem() { m_text.append("<li>"); }
    virtual void endListItem() { m_text.append("</li>\n"); }

    virtual void beginSuperscript() { m_text.append("<sup>"); }

    virtual void endSuperscript() { m_text.append("</sup>"); }
    
    virtual void beginSubscript() { m_text.append("<sub>"); }
    
    virtual void endSubscript() { m_text.append("</sub>"); }

    
    virtual void beginTable(qreal cellpadding, qreal cellspacing, const QString &width) {
        m_text.append(QString("<table cellpadding=\"%1\" cellspacing=\"%2\" width=\"%3\" border=\"1\">")
                .arg(cellpadding)
                .arg(cellspacing)
                .arg(width));
    }
    
    virtual void beginTableRow() { m_text.append("<tr>"); }
    virtual void beginTableHeaderCell(QString width, int colspan, int rowspan) {
        m_text.append(QString("<th width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
    }
    
    virtual void beginTableCell(QString width, int colspan, int rowspan) {
        m_text.append(QString("<td width=\"%1\" colspan=\"%2\" rowspan=\"%3\">").arg(width).arg(colspan).arg(rowspan));
    }
    
    virtual void endTable() { m_text.append("</table>"); }
    virtual void endTableRow() { m_text.append("</tr>"); }
    virtual void endTableHeaderCell() { m_text.append("</th>"); }
    virtual void endTableCell() { m_text.append("</td>"); }

    /**
    Reimplemented from AbstractMarkupBuilder.
    
    This implementation escapes the text before appending so that 
    
        A sample <b>bold</b> word.
        
    becomes 
    
         A sample &lt;b&gt;bold&lt;/b&gt; word.       
    
    */
    virtual void appendLiteralText(const QString &text) { m_text.append(Qt::escape(text)); }

    
    virtual QString& getResult() { return m_text; }

private:
    QList<QTextListFormat::Style> currentListItemStyles;

    QString m_text;

};

#endif
