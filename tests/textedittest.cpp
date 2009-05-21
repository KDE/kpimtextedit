/*
    Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>

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
#include "qtest_kde.h"
#include "textedittest.h"

#include "../textedit.h"

#include <KIconLoader>
#include <KStandardDirs>
#include <QTextCursor>

using namespace KPIMTextEdit;

QTEST_KDEMAIN( TextEditTester, GUI )

void TextEditTester::test_FormattingUsed()
{
  // This method tries to test everything that krichtextedit makes available, so
  // we can sure that in KMail, when the user uses some formatting, the mail is actually
  // sent as HTML mail

  TextEdit textEdit;

  QVERIFY( !textEdit.isFormattingUsed() );

  // Insert some text.
  QTextCursor cursor( textEdit.document() );
  cursor.insertText( QLatin1String( "Hello World!!" ) );
  QVERIFY( !textEdit.isFormattingUsed() );
  cursor.setPosition( 1 );
  textEdit.setTextCursor( cursor );

  //
  // Test link
  //
  QString someUrl = QLatin1String( "www.test.de" );
  QString altText = QLatin1String( "Hello" );
  textEdit.updateLink( someUrl, altText );
  QVERIFY( textEdit.isFormattingUsed() );
  QCOMPARE( textEdit.currentLinkUrl(), someUrl );
  QCOMPARE( textEdit.currentLinkText(), altText );

  cursor.setPosition( 1 );
  textEdit.setTextCursor( cursor );
  textEdit.updateLink( QString(), QLatin1String( "Hello" ) );
  QVERIFY( textEdit.currentLinkUrl().isEmpty() );
  QVERIFY( !textEdit.currentLinkText().isEmpty() );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Test alignment
  //
  cursor.setPosition( 1 );
  textEdit.setTextCursor( cursor );
  textEdit.alignRight();
  QVERIFY( textEdit.isFormattingUsed() );
  QCOMPARE( textEdit.alignment(), Qt::AlignRight );
  textEdit.alignLeft();
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.alignCenter();
  QCOMPARE( textEdit.alignment(), Qt::AlignHCenter );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.alignJustify();
  QCOMPARE( textEdit.alignment(), Qt::AlignJustify );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.alignLeft();
  QCOMPARE( textEdit.alignment(), Qt::AlignLeft );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Test lists
  //
  textEdit.setListStyle( QTextListFormat::ListCircle );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setListStyle( 0 );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Test font attributes
  //
  textEdit.setFontFamily( QLatin1String( "Times" ) );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFontFamily( textEdit.document()->defaultFont().family() );
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.setFontSize( 48 );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFontSize( textEdit.document()->defaultFont().pointSize() );
  QVERIFY( !textEdit.isFormattingUsed() );
  QFont myFont = textEdit.document()->defaultFont();
  myFont.setStyle( QFont::StyleOblique );
  textEdit.setFont( myFont );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFont( textEdit.document()->defaultFont() );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Test bold, italic, underline and strikeout
  //
  textEdit.setTextBold( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextBold( false );
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.setTextUnderline( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextUnderline( false );
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.setTextItalic( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextItalic( false );
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.setTextStrikeOut( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextStrikeOut( false );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Color
  //
  QColor oldForeground = textEdit.document()->firstBlock().charFormat().foreground().color();
  textEdit.setTextForegroundColor( Qt::red );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextForegroundColor( oldForeground );
  QVERIFY( !textEdit.isFormattingUsed() );
  QColor oldBackground = textEdit.document()->firstBlock().charFormat().background().color();
  textEdit.setTextBackgroundColor( Qt::red );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextBackgroundColor( oldBackground );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Horizontal rule
  //
  textEdit.insertHorizontalRule();
  QVERIFY( textEdit.isFormattingUsed() );
  // No way to easily remove the horizontal line, so clear the text edit and start over
  textEdit.clear();
  cursor.insertText( QLatin1String( "Hello World!!" ) );
  QVERIFY( !textEdit.isFormattingUsed() );
  cursor.setPosition( 1 );
  textEdit.setTextCursor( cursor );

  //
  // Sub and superscript
  //
  textEdit.setTextSuperScript( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextSuperScript( false );
  QVERIFY( !textEdit.isFormattingUsed() );
  textEdit.setTextSubScript( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextSubScript( false );
  QVERIFY( !textEdit.isFormattingUsed() );

  //
  // Image
  //
  QString imagePath = KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false );
  textEdit.addImage( imagePath );
  QVERIFY( textEdit.isFormattingUsed() );
  cursor = textEdit.textCursor();
  cursor.movePosition( QTextCursor::Left, QTextCursor::KeepAnchor, 1 );
  cursor.removeSelectedText();
  QVERIFY( !textEdit.isFormattingUsed() );
}


