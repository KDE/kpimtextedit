/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  based on code from qt-labs-graphics-dojo/htmleditor/highlighter.*

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

#ifndef KPIMTEXTEDIT_HTMLHIGHLIGHTER_H
#define KPIMTEXTEDIT_HTMLHIGHLIGHTER_H

#include "kpimtextedit_export.h"

#include <QSyntaxHighlighter>

/*
 * @since 4.10
 */
namespace KPIMTextEdit {

class HtmlHighlighterPrivate;

class KPIMTEXTEDIT_EXPORT HtmlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit HtmlHighlighter( QTextDocument *document );
    ~HtmlHighlighter();

protected:
    void highlightBlock( const QString &text );

private:
    friend class HtmlHighlighterPrivate;
    HtmlHighlighterPrivate * const d;

};

}

#endif // KPIMTEXTEDIT_HTMLHIGHLIGHTER_H
