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

#include "kmime/kmime_codecs.h"

#include <KIconLoader>
#include <KStandardDirs>

#include <QTextCursor>
#include <qtestevent.h>

using namespace KPIMTextEdit;

QTEST_KDEMAIN( TextEditTester, GUI )

void TextEditTester::testFormattingUsed()
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

void TextEditTester::testQuoting()
{
  TextEdit edit;
  QVERIFY( edit.isLineQuoted( QLatin1String( "> Hello" ) ) );
  QVERIFY( edit.isLineQuoted( QLatin1String( ">Hello" ) ) );
  QVERIFY( !edit.isLineQuoted( QLatin1String( "Hello" ) ) );
  QCOMPARE( edit.quoteLength( QLatin1String( "Hello" ) ), 0 );
  QCOMPARE( edit.quoteLength( QLatin1String( ">Hello" ) ), 1 );
  QCOMPARE( edit.quoteLength( QLatin1String( "> Hello" ) ), 2 );
  QCOMPARE( edit.quoteLength( QLatin1String( ">>>Hello" ) ), 3 );
  QCOMPARE( edit.quoteLength( QLatin1String( "> > > Hello" ) ), 6 );
  QCOMPARE( edit.quoteLength( QLatin1String( "|Hello" ) ), 1 );
  QCOMPARE( edit.quoteLength( QLatin1String( "| |Hello" ) ), 3 );
}

void TextEditTester::testCleanText()
{
  TextEdit edit;
  QLatin1String html( "<html><head></head><body>Heelllo&nbsp;World<br>Bye!</body></html>" );
  QLatin1String plain( "Heelllo World\nBye!" );
  edit.setTextOrHtml( html );
  edit.addImage( KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false ) );
  QVERIFY( edit.textMode() == TextEdit::Rich );
  QCOMPARE( edit.toCleanPlainText(), plain );

  edit.show(); // < otherwise toWrappedPlainText can't work, it needs a layout
  QCOMPARE( edit.toWrappedPlainText(), plain );
}

void TextEditTester::testEnter_data()
{
  QTest::addColumn<QString>("initalText");
  QTest::addColumn<QString>("expectedText");
  QTest::addColumn<int>("cursorPos");

  QTest::newRow( "" ) << QString::fromAscii( "> Hello World" )
                      << QString::fromAscii( "> Hello \n> World" )
                      << 8;
  QTest::newRow( "" ) << QString::fromAscii( "Hello World" )
                      << QString::fromAscii( "Hello \nWorld" )
                      << 6;
  QTest::newRow( "" ) << QString::fromAscii( "> Hello World" )
                      << QString::fromAscii( "> Hello World\n" )
                      << 13;
  QTest::newRow( "" ) << QString::fromAscii( ">Hello World" )
                      << QString::fromAscii( ">Hello \n>World" )
                      << 7;
  QTest::newRow( "" ) << QString::fromAscii( "> > Hello World" )
                      << QString::fromAscii( "> > Hello \n> > World" )
                      << 10;
  QTest::newRow( "" ) << QString::fromAscii( "| | Hello World" )
                      << QString::fromAscii( "| | Hello \n| | World" )
                      << 10;
}

void TextEditTester::testEnter()
{
  QFETCH( QString, initalText );
  QFETCH( QString, expectedText );
  QFETCH( int, cursorPos );

  TextEdit edit;
  edit.setPlainText( initalText );
  QTextCursor textCursor( edit.document() );
  textCursor.setPosition( cursorPos );
  edit.setTextCursor( textCursor );

  QTest::keyClick( &edit, Qt::Key_Return );
  QCOMPARE( edit.toPlainText(), expectedText );
}

void TextEditTester::testImages()
{
  TextEdit edit;
  QString image1Path = KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false );
  QString image2Path = KIconLoader::global()->iconPath( QLatin1String( "arrow-up" ), KIconLoader::Small, false );

  // Add one image, check that embeddedImages() returns the right stuff
  edit.addImage( image1Path );
  KPIMTextEdit::ImageList images = edit.embeddedImages();
  QCOMPARE( images.size(), 1 );
  EmbeddedImage *image = images.first().data();
  QCOMPARE( image->imageName, QString::fromAscii( "folder-new.png" ) );

  // Also check that it loads the correct image
  QImage diskImage( image1Path );
  QBuffer buffer;
  buffer.open( QIODevice::WriteOnly );
  diskImage.save( &buffer, "PNG" );
  QByteArray encodedImage = KMime::Codec::codecForName( "base64" )->encode( buffer.buffer() );
  QCOMPARE( image->image, encodedImage );

  // No image should be there after clearing
  edit.clear();
  QVERIFY( edit.embeddedImages().isEmpty() );

  // Check that manually removing the image also empties the image list
  edit.addImage( image1Path );
  QCOMPARE( edit.embeddedImages().size(), 1 );
  QTextCursor cursor = edit.textCursor();
  cursor.setPosition( 0, QTextCursor::MoveAnchor );
  cursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, 1 );
  cursor.removeSelectedText();
  QVERIFY( edit.embeddedImages().isEmpty() );

  // Check that adding the identical image two times only adds the image once
  edit.addImage( image1Path );
  edit.addImage( image1Path );
  QCOMPARE( edit.embeddedImages().size(), 1 );

  // Another different image added, and we should have two images
  edit.clear();
  edit.addImage( image1Path );
  edit.addImage( image2Path );
  images = edit.embeddedImages();
  QCOMPARE( images.size(), 2 );
  KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
  KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
  QCOMPARE( image1->imageName, QString::fromAscii( "folder-new2.png" ) ); // ### FIXME: should be folder-new.png, but QTextEdit provides no way to remove cached resources!
  QCOMPARE( image2->imageName, QString::fromAscii( "arrow-up.png" ) );
  QVERIFY( image1->contentID != image2->contentID );
}

void TextEditTester::testImageHtmlCode()
{
  TextEdit edit;
  QString image1Path = KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false );
  QString image2Path = KIconLoader::global()->iconPath( QLatin1String( "arrow-up" ), KIconLoader::Small, false );
  edit.addImage( image1Path );
  edit.addImage( image2Path );
  KPIMTextEdit::ImageList images = edit.embeddedImages();
  QCOMPARE( images.size(), 2 );
  KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
  KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
  QString startHtml = QLatin1String( "<img src=\"arrow-up.png\"><img src=\"folder-new.png\">Bla<b>Blub</b>" );
  QString endHtml = QString( QLatin1String( "<img src=\"cid:%1\"><img src=\"cid:%2\">Bla<b>Blub</b>" ) )
                        .arg( image2->contentID ).arg( image1->contentID );
  QCOMPARE( TextEdit::imageNamesToContentIds( startHtml.toAscii(), images ), endHtml.toAscii() );
}

void TextEditTester::testDeleteLine_data()
{
  QTest::addColumn<QString>("initalText");
  QTest::addColumn<QString>("expectedText");
  QTest::addColumn<int>("cursorPos");

  QTest::newRow( "" ) << QString::fromAscii( "line1\nline2\nline3" )
                      << QString::fromAscii( "line1\nline3" )
                      << 6;
  QTest::newRow( "" ) << QString::fromAscii( "line1\nline2\nline3" )
                      << QString::fromAscii( "line2\nline3" )
                      << 5;
  QTest::newRow( "" ) << QString::fromAscii( "line1\nline2\nline3" )
                      << QString::fromAscii( "line1\nline3" )
                      << 11;
  QTest::newRow( "" ) << QString::fromAscii( "line1\nline2\nline3" )
                      << QString::fromAscii( "line2\nline3" )
                      << 0;
  QTest::newRow( "" ) << QString::fromAscii( "line1\nline2\nline3" )
                      << QString::fromAscii( "line1\nline2" )
                      << 17;
  QTest::newRow( "" ) << QString::fromAscii( "line1" )
                      << QString::fromAscii( "" )
                      << 0;
  QTest::newRow( "" ) << QString::fromAscii( "line1" )
                      << QString::fromAscii( "" )
                      << 5;

  // Now, test deletion with word wrapping. The line with the Ms is so long that it will get wrapped
  QTest::newRow( "" ) << QString::fromAscii( "line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << QString::fromAscii( "line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << 6;
  QTest::newRow( "" ) << QString::fromAscii( "line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << QString::fromAscii( "line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << 13;
}

void TextEditTester::testDeleteLine()
{
  QFETCH( QString, initalText );
  QFETCH( QString, expectedText );
  QFETCH( int, cursorPos );

  TextEdit edit;
  edit.setPlainText( initalText );
  QTextCursor cursor = edit.textCursor();
  cursor.setPosition( cursorPos );
  edit.setTextCursor( cursor );

  edit.show(); // we need a layout for this to work

  edit.deleteCurrentLine();
  QCOMPARE( edit.toPlainText(), expectedText );
}

