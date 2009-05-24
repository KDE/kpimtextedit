/*
    Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>

    Based on KMail and libkdepim code by:
    Copyright 2007 Laurent Montel <montel@kde.org>

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
#include "textedit.h"

#include "emailquotehighlighter.h"

#include <kmime/kmime_codecs.h>

#include <KDE/KAction>
#include <KDE/KActionCollection>
#include <KDE/KCursor>
#include <KDE/KFileDialog>
#include <KDE/KLocalizedString>
#include <KDE/KMessageBox>
#include <KDE/KPushButton>
#include <KDE/KUrl>

#include <QtCore/QBuffer>
#include <QtCore/QDateTime>
#include <QtCore/QMimeData>
#include <QtCore/QFileInfo>
#include <QtCore/QPointer>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextLayout>

namespace KPIMTextEdit {

class TextEditPrivate
{
  public:

    TextEditPrivate( TextEdit *parent )
      : q( parent ),
        imageSupportEnabled( false )
    {
    }

    /**
     * Helper function for addImage(), which does the actual work of adding the QImage as a
     * resource to the document, pasting it and adding it to the image name list.
     *
     * @param imageName the desired image name. If it is already taken, a number will
     *                  be appended to it
     * @param image the actual image to add
     */
    void addImageHelper( const QString &imageName, const QImage &image );

    /**
     * Helper function to get the list of all QTextImageFormats in the document.
     */
    QList<QTextImageFormat> embeddedImageFormats() const;

    /**
     * Removes embedded image markers, converts non-breaking spaces to normal spaces
     * and other fixes for strings that came from toPlainText()/toHtml().
     */
    void fixupTextEditString( QString &text ) const;

    /**
     * Does the constructor work
     */
    void init();

    /**
     * Opens a file dialog to let the user choose an image and then pastes that
     * image to the editor
     */
    void _k_slotAddImage();

    /// The action that triggers _k_slotAddImage()
    KAction *actionAddImage;

    /// The parent class
    TextEdit *q;

    /// Whether or not adding or pasting images is supported
    bool imageSupportEnabled;

    /**
     * The names of embedded images.
     * Used to easily obtain the names of the images.
     * New images are compared to the the list and not added as resource if already present.
     */
    QStringList mImageNames;

    /**
     * Although KTextEdit keeps track of the spell checking state, we override
     * it here, because we have a highlighter which does quote highlighting.
     * And since disabling spellchecking in KTextEdit simply would turn off our
     * quote highlighter, we never actually deactivate spell checking in the
     * base class, but only tell our own email highlighter to not highlight
     * spelling mistakes.
     * For this, we use the KTextEditSpellInterface, which is basically a hack
     * that makes it possible to have our own enabled/disabled state in a binary
     * compatible way.
     */
    bool spellCheckingEnabled;
};

} // namespace

using namespace KPIMTextEdit;

void TextEditPrivate::fixupTextEditString( QString &text ) const
{
  // Remove line separators. Normal \n chars are still there, so no linebreaks get lost here
  text.remove( QChar::LineSeparator );

  // Get rid of embedded images, see QTextImageFormat documentation:
  // "Inline images are represented by an object replacement character (0xFFFC in Unicode) "
  text.remove( 0xFFFC );

  // In plaintext mode, each space is non-breaking.
  text.replace( QChar::Nbsp, QChar::fromAscii( ' ' ) );
}

TextEdit::TextEdit( const QString& text, QWidget *parent )
  : KRichTextWidget( text, parent ),
    d( new TextEditPrivate( this ) )
{
  d->init();
}

TextEdit::TextEdit( QWidget *parent )
  : KRichTextWidget( parent ),
    d( new TextEditPrivate( this ) )
{
  d->init();
}

TextEdit::~TextEdit()
{
}

bool TextEdit::eventFilter( QObject*o, QEvent* e )
{
  if ( o == this )
    KCursor::autoHideEventFilter( o, e );
  return KRichTextWidget::eventFilter( o, e );
}

void TextEditPrivate::init()
{
  q->setSpellInterface( q );
  // We tell the KRichTextWidget to enable spell checking, because only then it will
  // call createHighlighter() which will create our own highlighter which also
  // does quote highlighting.
  // However, *our* spellchecking is still disabled. Our own highlighter only
  // cares about our spellcheck status, it will not highlight missspelled words
  // if our spellchecking is disabled.
  // See also KEMailQuotingHighlighter::highlightBlock().
  spellCheckingEnabled = false;
  q->setCheckSpellingEnabledInternal( true );

  KCursor::setAutoHideCursor( q, true, true );
  q->installEventFilter( q );
}

void TextEdit::keyPressEvent ( QKeyEvent * e )
{
  if ( e->key() ==  Qt::Key_Return ) {
    QTextCursor cursor = textCursor();
    int oldPos = cursor.position();
    int blockPos = cursor.block().position();

    //selection all the line.
    cursor.movePosition( QTextCursor::StartOfBlock );
    cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
    QString lineText = cursor.selectedText();
    if ( ( ( oldPos -blockPos )  > 0 ) &&
         ( ( oldPos-blockPos ) < int( lineText.length() ) ) ) {
      bool isQuotedLine = false;
      int bot = 0; // bot = begin of text after quote indicators
      while ( bot < lineText.length() ) {
        if( ( lineText[bot] == QChar::fromAscii( '>' ) ) ||
            ( lineText[bot] == QChar::fromAscii( '|' ) ) ) {
          isQuotedLine = true;
          ++bot;
        }
        else if ( lineText[bot].isSpace() ) {
          ++bot;
        }
        else {
          break;
        }
      }
      KRichTextWidget::keyPressEvent( e );
      // duplicate quote indicators of the previous line before the new
      // line if the line actually contained text (apart from the quote
      // indicators) and the cursor is behind the quote indicators
      if ( isQuotedLine
           && ( bot != lineText.length() )
           && ( ( oldPos-blockPos ) >= int( bot ) ) ) {
        // The cursor position might have changed unpredictably if there was selected
        // text which got replaced by a new line, so we query it again:
        cursor.movePosition( QTextCursor::StartOfBlock );
        cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
        QString newLine = cursor.selectedText();

        // remove leading white space from the new line and instead
        // add the quote indicators of the previous line
        int leadingWhiteSpaceCount = 0;
        while ( ( leadingWhiteSpaceCount < newLine.length() )
                  && newLine[leadingWhiteSpaceCount].isSpace() ) {
          ++leadingWhiteSpaceCount;
        }
        newLine = newLine.replace( 0, leadingWhiteSpaceCount,
                                   lineText.left( bot ) );
        cursor.insertText( newLine );
        //cursor.setPosition( cursor.position() + 2);
        cursor.movePosition( QTextCursor::StartOfBlock );
        setTextCursor( cursor );
      }
    }
    else
      KRichTextWidget::keyPressEvent( e );
  }
  else
  {
    KRichTextWidget::keyPressEvent( e );
  }
}


bool TextEdit::isSpellCheckingEnabled() const
{
  return d->spellCheckingEnabled;
}

void TextEdit::setSpellCheckingEnabled( bool enable )
{
  EMailQuoteHighlighter *hlighter =
      dynamic_cast<EMailQuoteHighlighter*>( highlighter() );
  if ( hlighter )
    hlighter->toggleSpellHighlighting( enable );

  d->spellCheckingEnabled = enable;
  emit checkSpellingChanged( enable );
}

bool TextEdit::shouldBlockBeSpellChecked( const QString& block ) const
{
  return !isLineQuoted( block );
}

bool KPIMTextEdit::TextEdit::isLineQuoted( const QString& line ) const
{
  return quoteLength( line ) > 0;
}

int KPIMTextEdit::TextEdit::quoteLength( const QString& line ) const
{
  bool quoteFound = false;
  int startOfText = -1;
  for ( int i = 0; i < line.length(); i++ ) {
    if ( line[i] == QLatin1Char( '>' ) || line[i] == QLatin1Char( '|' ) )
      quoteFound = true;
    else if ( line[i] != QLatin1Char( ' ' ) ) {
      startOfText = i;
      break;
    }
  }
  if ( quoteFound ) {
    if ( startOfText == -1 )
      startOfText = line.length() - 1;
    return startOfText;
  }
  else
    return 0;
}

const QString KPIMTextEdit::TextEdit::defaultQuoteSign() const
{
  return QLatin1String( "> " );
}

void TextEdit::createHighlighter()
{
  EMailQuoteHighlighter *emailHighLighter =
      new EMailQuoteHighlighter( this );

  setHighlighterColors( emailHighLighter );

  //TODO change config
  KRichTextWidget::setHighlighter( emailHighLighter );

  if ( !spellCheckingLanguage().isEmpty() )
    setSpellCheckingLanguage( spellCheckingLanguage() );
  setSpellCheckingEnabled( isSpellCheckingEnabled() );
}

void TextEdit::setHighlighterColors( EMailQuoteHighlighter *highlighter )
{
  Q_UNUSED( highlighter );
}

QString TextEdit::toWrappedPlainText() const
{
  QString temp;
  QTextDocument* doc = document();
  QTextBlock block = doc->begin();
  while ( block.isValid() ) {
    QTextLayout* layout = block.layout();
    for ( int i = 0; i < layout->lineCount(); i++ ) {
      QTextLine line = layout->lineAt( i );
      temp += block.text().mid( line.textStart(), line.textLength() ) + QLatin1Char( '\n' );
    }
    block = block.next();
  }

  // Remove the last superfluous newline added above
  if ( temp.endsWith( QLatin1Char( '\n' ) ) )
    temp.chop( 1 );

  d->fixupTextEditString( temp );
  return temp;
}

QString TextEdit::toCleanPlainText() const
{
  QString temp = toPlainText();
  d->fixupTextEditString( temp );
  return temp;
}

void TextEdit::createActions( KActionCollection *actionCollection )
{
  KRichTextWidget::createActions( actionCollection );

  if ( d->imageSupportEnabled ) {
    d->actionAddImage = new KAction( KIcon( QLatin1String( "insert-image" ) ),
                                    i18n( "Add Image" ), this );
    actionCollection->addAction( QLatin1String( "add_image" ), d->actionAddImage );
    connect( d->actionAddImage, SIGNAL(triggered(bool) ), SLOT( _k_slotAddImage() ) );
  }
}

void TextEdit::addImage( const KUrl &url )
{
  QImage image;
  if ( !image.load( url.path() ) ) {
    KMessageBox::error( this,
                        i18nc( "@info", "Unable to load image <filename>%1</filename>.", url.path() ) );
    return;
  }
  QFileInfo fi( url.path() );
  QString imageName = fi.baseName().isEmpty() ? QLatin1String( "image.png" )
                                              : fi.baseName() + QLatin1String( ".png" );
  d->addImageHelper( imageName, image );
}

void TextEditPrivate::addImageHelper( const QString &imageName, const QImage &image )
{
  QString imageNameToAdd = imageName;
  QTextDocument *document = q->document();

  // determine the imageNameToAdd
  int imageNumber = 1;
  while ( mImageNames.contains( imageNameToAdd ) ) {
    QVariant qv = document->resource( QTextDocument::ImageResource, QUrl( imageNameToAdd ) );
    if ( qv == image ) {
      // use the same name
      break;
    }
    int firstDot = imageName.indexOf( QLatin1Char( '.' ) );
    if ( firstDot == -1 )
      imageNameToAdd = imageName + QString::number( imageNumber++ );
    else
      imageNameToAdd = imageName.left( firstDot ) + QString::number( imageNumber++ ) +
                       imageName.mid( firstDot );
  }

  if ( !mImageNames.contains( imageNameToAdd ) ) {
    document->addResource( QTextDocument::ImageResource, QUrl( imageNameToAdd ), image );
    mImageNames << imageNameToAdd;
  }
  q->textCursor().insertImage( imageNameToAdd );
  q->enableRichTextMode();
}

QList< QSharedPointer<EmbeddedImage> > TextEdit::embeddedImages() const
{
  QList< QSharedPointer<EmbeddedImage> > retImages;
  QStringList seenImageNames;
  QList<QTextImageFormat> imageFormats = d->embeddedImageFormats();
  foreach( const QTextImageFormat &imageFormat, imageFormats ) {
    if ( !seenImageNames.contains( imageFormat.name() ) ) {
      QVariant data = document()->resource( QTextDocument::ImageResource, QUrl( imageFormat.name() ) );
      QImage image = qvariant_cast<QImage>( data );
      QBuffer buffer;
      buffer.open( QIODevice::WriteOnly );
      image.save( &buffer, "PNG" );

      qsrand( QDateTime::currentDateTime().toTime_t() + qHash( imageFormat.name() ) );
      QSharedPointer<EmbeddedImage> embeddedImage( new EmbeddedImage() );
      retImages.append( embeddedImage );
      embeddedImage->image = KMime::Codec::codecForName( "base64" )->encode( buffer.buffer() );
      embeddedImage->imageName = imageFormat.name();
      embeddedImage->contentID = QString( QLatin1String( "%1" ) ).arg( qrand() );
      seenImageNames.append( imageFormat.name() );
    }
  }
  return retImages;
}

QList<QTextImageFormat> TextEditPrivate::embeddedImageFormats() const
{
  QTextDocument *doc = q->document();
  QList<QTextImageFormat> retList;

  QTextBlock currentBlock = doc->begin();
  while ( currentBlock.isValid() ) {
    QTextBlock::iterator it;
    for ( it = currentBlock.begin(); !it.atEnd(); ++it ) {
      QTextFragment fragment = it.fragment();
      if ( fragment.isValid() ) {
        QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
        if ( imageFormat.isValid() ) {
          retList.append( imageFormat );
        }
      }
    }
    currentBlock = currentBlock.next();
  }
  return retList;
}

void TextEditPrivate::_k_slotAddImage()
{
  QPointer<KFileDialog> fdlg = new KFileDialog( QString(), QString(), q );
  fdlg->setOperationMode( KFileDialog::Other );
  fdlg->setCaption( i18n("Add Image") );
  fdlg->okButton()->setGuiItem( KGuiItem( i18n("&Add"), QLatin1String( "document-open" ) ) );
  fdlg->setMode( KFile::Files );
  if ( fdlg->exec() != KDialog::Accepted ) {
    delete fdlg;
    return;
  }

  const KUrl::List files = fdlg->selectedUrls();
  foreach ( const KUrl& url, files ) {
    q->addImage( url );
  }
  delete fdlg;
}

void KPIMTextEdit::TextEdit::enableImageActions()
{
  d->imageSupportEnabled = true;
}

QByteArray KPIMTextEdit::TextEdit::imageNamesToContentIds( const QByteArray &htmlBody, const KPIMTextEdit::ImageList &imageList )
{
  QByteArray result = htmlBody;
  if ( imageList.size() > 0 ) {
    foreach( const QSharedPointer<EmbeddedImage> &image, imageList ) {
      const QString newImageName = QLatin1String( "cid:" ) + image->contentID;
      QByteArray quote( "\"" );
      result.replace( QByteArray( quote + image->imageName.toLocal8Bit() + quote ),
                      QByteArray( quote + newImageName.toLocal8Bit() + quote ) );
    }
  }
  return result;
}

void TextEdit::insertFromMimeData( const QMimeData *source )
{
  // Add an image if that is on the clipboard
  if ( textMode() == KRichTextEdit::Rich && source->hasImage() && d->imageSupportEnabled ) {
    QImage image = qvariant_cast<QImage>( source->imageData() );
    QFileInfo fi( source->text() );
    QString imageName = fi.baseName().isEmpty() ? i18nc( "Start of the filename for an image", "image" ) : fi.baseName();
    d->addImageHelper( imageName, image );
    return;
  }

  // Attempt to paste HTML contents into the text edit in plain text mode,
  // prevent this and prevent plain text instead.
  if ( textMode() == KRichTextEdit::Plain && source->hasHtml() ) {
    if ( source->hasText() ) {
      insertPlainText( source->text() );
      return;
    }
  }

  KRichTextWidget::insertFromMimeData( source );
}

bool KPIMTextEdit::TextEdit::canInsertFromMimeData( const QMimeData *source ) const
{
  if ( source->hasHtml() && textMode() == KRichTextEdit::Rich )
    return true;
  if ( source->hasText() )
    return true;
  if ( textMode() == KRichTextEdit::Rich && source->hasImage() && d->imageSupportEnabled )
    return true;

  return KRichTextWidget::canInsertFromMimeData( source );
}

static bool isCharFormatFormatted( const QTextCharFormat &format, const QFont &defaultFont,
                                   const QTextCharFormat &defaultBlockFormat )
{
  if ( !format.anchorHref().isEmpty() ||
       format.font() != defaultFont ||
       format.isAnchor() ||
       format.verticalAlignment() != defaultBlockFormat.verticalAlignment() ||
       format.underlineStyle() != defaultBlockFormat.underlineStyle() ||
       format.foreground().color() != defaultBlockFormat.foreground().color() ||
       format.background().color() != defaultBlockFormat.background().color() )
    return true;

  return false;
}

static bool isBlockFormatFormatted( const QTextBlockFormat &format,
                                    const QTextBlockFormat &defaultFormat )
{
  if ( format.alignment() != defaultFormat.alignment() ||
       format.indent() != defaultFormat.indent() ||
       format.textIndent() != defaultFormat.textIndent() )
    return true;

  return false;
}

/// @return true if the format represents a list, table, image or something like that.
static bool isSpecial( const QTextFormat &charFormat )
{
  return charFormat.isFrameFormat() || charFormat.isImageFormat() ||
         charFormat.isListFormat() || charFormat.isTableFormat();
}

bool TextEdit::isFormattingUsed() const
{
  if ( textMode() == Plain )
    return false;

  // Below, we walk through all text blocks and through all text fragments in them
  // and check if any of those has any formatting.
  // To check if they have formatting, we use the functions isBlockFormatFormatted() and
  // isCharFormatFormatted(). Those do not check all the exising formatting possibilities on
  // earth, but everything that KRichTextEdit supports at the moment.
  //
  // Also, we have to compare the formats against those of a default text edit. For example,
  // we can't compare the foreground color against black, because the user might have another
  // color scheme. Therefore we compare the foreground color against a default text edit.

  QTextEdit defaultTextEdit;
  QTextCharFormat defaultCharFormat = defaultTextEdit.document()->begin().charFormat();
  QTextBlockFormat defaultBlockFormat = defaultTextEdit.document()->begin().blockFormat();
  QFont defaultFont = document()->defaultFont();

  QTextBlock block = document()->firstBlock();
  while ( block.isValid() ) {

    if ( isBlockFormatFormatted( block.blockFormat(), defaultBlockFormat ) ) {
      return true;
    }

    if ( isSpecial( block.charFormat() ) || isSpecial( block.blockFormat() ) ||
         block.textList() ) {
      return true;
    }

    QTextBlock::iterator it = block.begin();
    while ( !it.atEnd() ) {
      QTextFragment fragment = it.fragment();
      QTextCharFormat charFormat = fragment.charFormat();
      if ( isSpecial( charFormat ) ) {
        return true;
      }
      if ( isCharFormatFormatted( fragment.charFormat(), defaultFont, defaultCharFormat ) ) {
        return true;
      }

      it++;
    }
    block = block.next();
  }

  if ( toHtml().contains( QLatin1String( "<hr />" ) ) )
    return true;

  return false;
}

#include "textedit.moc"
