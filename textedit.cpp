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
     * Returns the text of the signature. If the signature is HTML, the HTML
     * tags will be stripped.
     */
    QString plainSignatureText( const KPIMIdentities::Signature &signature ) const;

    /**
     * Replaces each text which matches the regular expression with another text.
     * Text inside quotes or the given signature will be ignored.
     */
    void cleanWhitespaceHelper( const QRegExp &regExp, const QString &newText,
                                const KPIMIdentities::Signature &sig );

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

QList< QPair<int,int> > TextEdit::signaturePositions( const KPIMIdentities::Signature &sig ) const
{
  QList< QPair<int,int> > signaturePositions;
  if ( !sig.rawText().isEmpty() ) {

    QString sigText = d->plainSignatureText( sig );

    int currentSearchPosition = 0;
    forever {

      // Find the next occurrence of the signature text
      QString text = document()->toPlainText();
      int currentMatch = text.indexOf( sigText, currentSearchPosition );
      currentSearchPosition = currentMatch + sigText.length();
      if ( currentMatch == -1 )
        break;

      signaturePositions.append( QPair<int,int>( currentMatch,
                                 currentMatch + sigText.length() ) );
    }
  }
  return signaturePositions;
}

QString TextEditPrivate::plainSignatureText( const KPIMIdentities::Signature &signature ) const
{
  QString sigText = signature.rawText();
  if ( signature.isInlinedHtml() &&
       signature.type() == KPIMIdentities::Signature::Inlined ) {

    // Use a QTextDocument as a helper, it does all the work for us and
    // strips all HTML tags.
    QTextDocument helper;
    QTextCursor helperCursor( &helper );
    helperCursor.insertHtml( sigText );
    sigText = helper.toPlainText();
  }
  return sigText;
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

void TextEdit::insertFromMimeData( const QMimeData * source )
{
  // Attempt to paste HTML contents into the text edit in plain text mode,
  // prevent this and prevent plain text instead.
  if ( textMode() == KRichTextEdit::Plain && source->hasHtml() ) {
    if ( source->hasText() ) {
      insertPlainText( source->text() );
    }
  }
  else
    KRichTextWidget::insertFromMimeData( source );
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
    return startOfText + 1;
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

void TextEdit::insertSignature( const KPIMIdentities::Signature &sig,
                                Placement placement, bool addSeparator )
{
  QString signature;
  if ( addSeparator )
    signature = sig.withSeparator();
  else
    signature = sig.rawText();

  insertSignature( signature, placement,
                   ( sig.isInlinedHtml() &&
                     sig.type() == KPIMIdentities::Signature::Inlined ) );
}

void TextEdit::insertSignature( const QString &signature, Placement placement,
                                bool isHtml )
{
  if ( !signature.isEmpty() ) {

    // Save the modified state of the document, as inserting a signature
    // shouldn't change this. Restore it at the end of this function.
    bool isModified = document()->isModified();

    // Move to the desired position, where the signature should be inserted
    QTextCursor cursor = textCursor();
    QTextCursor oldCursor = cursor;
    cursor.beginEditBlock();

    if ( placement == End )
      cursor.movePosition( QTextCursor::End );
    else if ( placement == Start )
      cursor.movePosition( QTextCursor::Start );
    setTextCursor( cursor );

    // Insert the signature and newlines depending on where it was inserted.
    bool hackForCursorsAtEnd = false;
    int oldCursorPos = -1;
    if ( placement == End ) {

      if ( oldCursor.position() == toPlainText().length() ) {
        hackForCursorsAtEnd = true;
        oldCursorPos = oldCursor.position();
      }

      if ( isHtml ) {
        insertHtml( QLatin1String( "<br>" ) + signature );
      } else {
        insertPlainText( QLatin1Char( '\n' ) + signature );
      }
    } else if ( placement == Start || placement == AtCursor ) {
      if ( isHtml ) {
        insertHtml( QLatin1String( "<br>" ) + signature + QLatin1String( "<br>" ) );
      } else {
        insertPlainText( QLatin1Char( '\n' ) + signature + QLatin1Char( '\n' ) );
      }
    }

    cursor.endEditBlock();

    // There is one special case when re-setting the old cursor: The cursor
    // was at the end. In this case, QTextEdit has no way to know
    // if the signature was added before or after the cursor, and just decides
    // that it was added before (and the cursor moves to the end, but it should
    // not when appending a signature). See bug 167961
    if ( hackForCursorsAtEnd )
      oldCursor.setPosition( oldCursorPos );

    setTextCursor( oldCursor );
    ensureCursorVisible();

    document()->setModified( isModified );

    if ( isHtml )
      enableRichTextMode();
  }
}

void TextEdit::replaceSignature( const KPIMIdentities::Signature &oldSig,
                                 const KPIMIdentities::Signature &newSig )
{
  QTextCursor cursor( document() );
  cursor.beginEditBlock();

  QString oldSigText = d->plainSignatureText( oldSig );

  int currentSearchPosition = 0;
  forever {

    // Find the next occurrence of the signature text
    QString text = document()->toPlainText();
    int currentMatch = text.indexOf( oldSigText, currentSearchPosition );
    currentSearchPosition = currentMatch;
    if ( currentMatch == -1 )
      break;

    // Select the signature
    QTextCursor cursor( document() );
    cursor.setPosition( currentMatch );

    // If the new signature is completely empty, we also want to remove the
    // signature separator, so include it in the selection
    int additionalMove = 0;
    if ( newSig.rawText().isEmpty() &&
         text.mid( currentMatch - 4, 4) == QLatin1String( "-- \n" ) ) {
      cursor.movePosition( QTextCursor::PreviousCharacter,
                           QTextCursor::MoveAnchor, 4 );
      additionalMove = 4;
    }
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                         oldSigText.length() + additionalMove );


    // Skip quoted signatures
    if ( isLineQuoted( cursor.block().text() ) ) {
      currentSearchPosition += d->plainSignatureText( oldSig ).length();
      continue;
    }

    // Remove the old and instert the new signature
    cursor.removeSelectedText();
    if ( newSig.isInlinedHtml() &&
         newSig.type() == KPIMIdentities::Signature::Inlined ) {
      cursor.insertHtml( newSig.rawText() );
      enableRichTextMode();
    }
    else
      cursor.insertText( newSig.rawText() );

    currentSearchPosition += d->plainSignatureText( newSig ).length();
  }

  cursor.endEditBlock();
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
    imageNameToAdd = imageName + QString::number( imageNumber++ );
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
  KFileDialog fdlg( QString(), QString(), q );
  fdlg.setOperationMode( KFileDialog::Other );
  fdlg.setCaption( i18n("Add Image") );
  fdlg.okButton()->setGuiItem( KGuiItem( i18n("&Add"), QLatin1String( "document-open" ) ) );
  fdlg.setMode( KFile::Files );
  if ( fdlg.exec() != KDialog::Accepted )
    return;

  const KUrl::List files = fdlg.selectedUrls();
  foreach ( const KUrl& url, files ) {
    q->addImage( url );
  }
}



void TextEditPrivate::cleanWhitespaceHelper( const QRegExp &regExp,
                                             const QString &newText,
                                             const KPIMIdentities::Signature &sig )
{
  int currentSearchPosition = 0;

  forever {

    // Find the text
    QString text = q->document()->toPlainText();
    int currentMatch = regExp.indexIn( text, currentSearchPosition );
    currentSearchPosition = currentMatch;
    if ( currentMatch == -1 )
      break;

    // Select the text
    QTextCursor cursor( q->document() );
    cursor.setPosition( currentMatch );
    cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                         regExp.matchedLength() );

    // Skip quoted text
    if ( q->isLineQuoted( cursor.block().text() ) ) {
      currentSearchPosition += regExp.matchedLength();
      continue;
    }

    // Skip text inside signatures
    bool insideSignature = false;
    QList< QPair<int,int> > sigPositions = q->signaturePositions( sig );
    QPair<int,int> position;
    foreach( position, sigPositions ) { //krazy:exclude=foreach
      if ( cursor.position() >= position.first &&
           cursor.position() <= position.second )
        insideSignature = true;
    }
    if ( insideSignature ) {
      currentSearchPosition += regExp.matchedLength();
      continue;
    }

    // Replace the text
    cursor.removeSelectedText();
    cursor.insertText( newText );
    currentSearchPosition += newText.length();
  }
}

void TextEdit::cleanWhitespace( const KPIMIdentities::Signature &sig )
{
  QTextCursor cursor( document() );
  cursor.beginEditBlock();

  // Squeeze tabs and spaces
  d->cleanWhitespaceHelper( QRegExp( QLatin1String( "[\t ]+" ) ),
                            QString( QLatin1Char( ' ' ) ), sig );

  // Remove trailing whitespace
  d->cleanWhitespaceHelper( QRegExp( QLatin1String( "[\t ][\n]" ) ),
                            QString( QLatin1Char( '\n' ) ), sig );

  // Single space lines
  d->cleanWhitespaceHelper( QRegExp( QLatin1String( "[\n]{3,}" ) ),
                            QLatin1String( "\n\n" ), sig );

  if ( !textCursor().hasSelection() ) {
    textCursor().clearSelection();
  }

  cursor.endEditBlock();
}

void KPIMTextEdit::TextEdit::enableImageActions()
{
  d->imageSupportEnabled = true;
}


#include "textedit.moc"
