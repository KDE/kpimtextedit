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
#include "qtest.h"
#include "textedittest.h"

#include "textedit.h"

#include <kmime/kmime_codecs.h>

#include <KIconLoader>

#include <QTextCursor>
#include <qtestevent.h>
#include <QBuffer>

using namespace KPIMTextEdit;

QTEST_MAIN( TextEditTester )

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
  // Test layout direction
  //
  textEdit.selectAll();
  QTextCharFormat direction;
  direction.setLayoutDirection( Qt::RightToLeft );
  textEdit.mergeCurrentCharFormat( direction );
  QVERIFY( textEdit.isFormattingUsed() );
  direction.setLayoutDirection( Qt::LeftToRight );
  textEdit.mergeCurrentCharFormat( direction );
  QVERIFY( textEdit.isFormattingUsed() );

  //
  // Test lists
  //
  textEdit.setListStyle( QTextListFormat::ListCircle );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setListStyle( 0 );
  QVERIFY( textEdit.isFormattingUsed() );

  //
  // Test font attributes
  //
  textEdit.setFontFamily( QLatin1String( "Times" ) );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFontFamily( textEdit.document()->defaultFont().family() );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFontSize( 48 );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFontSize( textEdit.document()->defaultFont().pointSize() );
  QVERIFY( textEdit.isFormattingUsed() );
  QFont myFont = textEdit.document()->defaultFont();
  myFont.setStyle( QFont::StyleOblique );
  textEdit.setFont( myFont );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setFont( textEdit.document()->defaultFont() );
  QVERIFY( textEdit.isFormattingUsed() );

  //
  // Test bold, italic, underline and strikeout
  //
  textEdit.setTextBold( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextBold( false );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextUnderline( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextUnderline( false );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextItalic( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextItalic( false );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextStrikeOut( true );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextStrikeOut( false );
  QVERIFY( textEdit.isFormattingUsed() );

  //
  // Color
  //
  QColor oldForeground = textEdit.document()->firstBlock().charFormat().foreground().color();
  textEdit.setTextForegroundColor( Qt::red );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextForegroundColor( oldForeground );
  QVERIFY( textEdit.isFormattingUsed() );
  QColor oldBackground = textEdit.document()->firstBlock().charFormat().background().color();
  textEdit.setTextBackgroundColor( Qt::red );
  QVERIFY( textEdit.isFormattingUsed() );
  textEdit.setTextBackgroundColor( oldBackground );
  QVERIFY( textEdit.isFormattingUsed() );

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
  textEdit.addImage( QUrl(imagePath) );
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
  edit.addImage( QUrl(KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false )) );
  QVERIFY( edit.textMode() == TextEdit::Rich );
  QCOMPARE( edit.toCleanPlainText(), plain );

  edit.show(); // < otherwise toWrappedPlainText can't work, it needs a layout
  QCOMPARE( edit.toWrappedPlainText(), plain );
}

void TextEditTester::testEnter_data()
{
  QTest::addColumn<QString>( "initalText" );
  QTest::addColumn<QString>( "expectedText" );
  QTest::addColumn<int>( "cursorPos" );

  QTest::newRow( "" ) << QString::fromLatin1( "> Hello World" )
                      << QString::fromLatin1( "> Hello \n> World" )
                      << 8;
  QTest::newRow( "" ) << QString::fromLatin1( "Hello World" )
                      << QString::fromLatin1( "Hello \nWorld" )
                      << 6;
  QTest::newRow( "" ) << QString::fromLatin1( "> Hello World" )
                      << QString::fromLatin1( "> Hello World\n" )
                      << 13;
  QTest::newRow( "" ) << QString::fromLatin1( ">Hello World" )
                      << QString::fromLatin1( ">Hello \n>World" )
                      << 7;
  QTest::newRow( "" ) << QString::fromLatin1( "> > Hello World" )
                      << QString::fromLatin1( "> > Hello \n> > World" )
                      << 10;
  QTest::newRow( "" ) << QString::fromLatin1( "| | Hello World" )
                      << QString::fromLatin1( "| | Hello \n| | World" )
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
  edit.addImage( QUrl(image1Path) );
  KPIMTextEdit::ImageList images = edit.embeddedImages();
  KPIMTextEdit::ImageWithNameList imagesWithNames = edit.imagesWithName();
  QCOMPARE( images.size(), 1 );
  QCOMPARE( imagesWithNames.size(), 1 );
  EmbeddedImage *image = images.first().data();
  ImageWithName *imageWithName = imagesWithNames.first().data();
  QCOMPARE( image->imageName, QString::fromLatin1( "folder-new.png" ) );
  QCOMPARE( imageWithName->name, QString::fromLatin1( "folder-new.png" ) );

  // Also check that it loads the correct image
  QImage diskImage( image1Path );
  QBuffer buffer;
  buffer.open( QIODevice::WriteOnly );
  diskImage.save( &buffer, "PNG" );
  QBuffer imageWithNameBuffer;
  imageWithNameBuffer.open( QIODevice::WriteOnly );
  imageWithName->image.save( &imageWithNameBuffer, "PNG" );
  QByteArray encodedImage = KMime::Codec::codecForName( "base64" )->encode( buffer.buffer() );
  QCOMPARE( image->image, encodedImage );
  QCOMPARE( buffer.buffer(), imageWithNameBuffer.buffer() );

  // No image should be there after clearing
  edit.clear();
  QVERIFY( edit.embeddedImages().isEmpty() );
  QVERIFY( edit.imagesWithName().isEmpty() );

  // Check that manually removing the image also empties the image list
  edit.addImage( QUrl(image1Path) );
  QCOMPARE( edit.embeddedImages().size(), 1 );
  QCOMPARE( edit.imagesWithName().size(), 1 );
  QTextCursor cursor = edit.textCursor();
  cursor.setPosition( 0, QTextCursor::MoveAnchor );
  cursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, 1 );
  cursor.removeSelectedText();
  QVERIFY( edit.embeddedImages().isEmpty() );
  QVERIFY( edit.imagesWithName().isEmpty() );

  // Check that adding the identical image two times only adds the image once
  edit.addImage( QUrl(image1Path) );
  edit.addImage( QUrl(image1Path) );
  QCOMPARE( edit.embeddedImages().size(), 1 );
  QCOMPARE( edit.imagesWithName().size(), 1 );

  // Another different image added, and we should have two images
  edit.clear();
  edit.addImage( QUrl(image1Path) );
  edit.addImage( QUrl(image2Path) );
  images = edit.embeddedImages();
  imagesWithNames = edit.imagesWithName();
  QCOMPARE( images.size(), 2 );
  QCOMPARE( imagesWithNames.size(), 2 );
  KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
  KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
  KPIMTextEdit::ImageWithName *imageWithName1 = imagesWithNames.first().data();
  KPIMTextEdit::ImageWithName *imageWithName2 = imagesWithNames.last().data();
  QCOMPARE( image1->imageName, QString::fromLatin1( "folder-new2.png" ) ); // ### FIXME: should be folder-new.png, but QTextEdit provides no way to remove cached resources!
  QCOMPARE( imageWithName1->name, QString::fromLatin1( "folder-new2.png" ) );
  QCOMPARE( image2->imageName, QString::fromLatin1( "arrow-up.png" ) );
  QCOMPARE( imageWithName2->name, QString::fromLatin1( "arrow-up.png" ) );
  QVERIFY( image1->contentID != image2->contentID );
}

void TextEditTester::testImageHtmlCode()
{
  TextEdit edit;
  QString image1Path = KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false );
  QString image2Path = KIconLoader::global()->iconPath( QLatin1String( "arrow-up" ), KIconLoader::Small, false );
  edit.addImage( QUrl(image1Path) );
  edit.addImage( QUrl(image2Path) );
  KPIMTextEdit::ImageList images = edit.embeddedImages();
  QCOMPARE( images.size(), 2 );
  KPIMTextEdit::EmbeddedImage *image1 = images.first().data();
  KPIMTextEdit::EmbeddedImage *image2 = images.last().data();
  QString startHtml = QLatin1String( "<img src=\"arrow-up.png\"><img src=\"folder-new.png\">Bla<b>Blub</b>" );
  QString endHtml = QString( QLatin1String( "<img src=\"cid:%1\"><img src=\"cid:%2\">Bla<b>Blub</b>" ) )
                        .arg( image2->contentID ).arg( image1->contentID );
  QCOMPARE( TextEdit::imageNamesToContentIds( startHtml.toLatin1(), images ), endHtml.toAscii() );
}

void TextEditTester::testDeleteLine_data()
{
  QTest::addColumn<QString>( "initalText" );
  QTest::addColumn<QString>( "expectedText" );
  QTest::addColumn<int>( "cursorPos" );

  QTest::newRow( "" ) << QString::fromLatin1( "line1\nline2\nline3" )
                      << QString::fromLatin1( "line1\nline3" )
                      << 6;
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nline2\nline3" )
                      << QString::fromLatin1( "line2\nline3" )
                      << 5;
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nline2\nline3" )
                      << QString::fromLatin1( "line1\nline3" )
                      << 11;
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nline2\nline3" )
                      << QString::fromLatin1( "line2\nline3" )
                      << 0;
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nline2\nline3" )
                      << QString::fromLatin1( "line1\nline2" )
                      << 17;
  QTest::newRow( "" ) << QString::fromLatin1( "line1" )
                      << QString::fromLatin1( "" )
                      << 0;
  QTest::newRow( "" ) << QString::fromLatin1( "line1" )
                      << QString::fromLatin1( "" )
                      << 5;

  // Now, test deletion with word wrapping. The line with the Ms is so long that it will get wrapped
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << QString::fromLatin1( "line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << 6;
  QTest::newRow( "" ) << QString::fromLatin1( "line1\nMMMMMMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
                      << QString::fromLatin1( "line1\nMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\nline3" )
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

void TextEditTester::testLoadImage()
{
  TextEdit edit;
  QString image1Path = KIconLoader::global()->iconPath( QLatin1String( "folder-new" ), KIconLoader::Small, false );
  QString image2Path = KIconLoader::global()->iconPath( QLatin1String( "arrow-up" ), KIconLoader::Small, false );
  QImage image1, image2;
  QVERIFY( image1.load( image1Path ) );
  QVERIFY( image2.load( image1Path ) );

  edit.setHtml( QLatin1String( "Bla<img src=\"folder-new.png\">Bla" ) );

  // First try to load an image with a name that doesn't match, it should fail
  edit.loadImage( image1, QString::fromLatin1( "doesntmatch" ), QString::fromLatin1( "folder-new" ) );
  QVERIFY( !edit.document()->resource( QTextDocument::ImageResource, QUrl( QLatin1String( "folder-new" ) ) ).isValid() );

  // Now, load the image for real
  edit.loadImage( image1, QString::fromLatin1( "folder-new.png" ), QString::fromLatin1( "folder-new" ) );
  QVERIFY( edit.document()->resource( QTextDocument::ImageResource, QUrl( QLatin1String( "folder-new" ) ) ).isValid() );

  // New test with a new textedit (so that we don't use the cached resources
  // This example has two images in the same text block, make sure that doesn't crash (bug 204214)
  TextEdit edit2;
  edit2.setHtml( QLatin1String( "<img src=\"folder-new.png\"><img src=\"folder-new.png\">" ) );
  edit2.loadImage( image1, QString::fromLatin1( "folder-new.png" ), QString::fromLatin1( "folder-new" ) );
  QVERIFY( edit.document()->resource( QTextDocument::ImageResource, QUrl( QLatin1String( "folder-new" ) ) ).isValid() );
  QCOMPARE( edit.embeddedImages().size(), 1 );
}

void TextEditTester::testWrappedPlainText()
{
  TextEdit edit;
  QLatin1String text( "http://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\n  https://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\ntest ftp://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\nftps://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test\n  ldap://example.org/test-test-test-test-test-test-test-test-test-test-test-test-test" );
  edit.setPlainText( text );

  edit.show(); // < otherwise toWrappedPlainText can't work, it needs a layout

  QCOMPARE( edit.toWrappedPlainText(), text );
}
