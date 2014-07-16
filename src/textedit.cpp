/*
  Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>

  Based on KMail and libkdepim code by:
  Copyright 2007 - 2012 Laurent Montel <montel@kde.org>

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
#include "emoticontexteditaction.h"
#include "inserthtmldialog.h"
#include "tableactionmenu.h"
#include "insertimagedialog.h"

#include <kmime/kmime_codecs.h>

#include <QAction>
#include <KActionCollection>
#include <KCursor>
#include <KLocalizedString>
#include <KMessageBox>

#include <QtCore/QBuffer>
#include <QtCore/QDateTime>
#include <QtCore/QMimeData>
#include <QtCore/QFileInfo>
#include <QtCore/QPointer>
#include <QKeyEvent>
#include <QTextLayout>
#include <QUrl>

#include "textutils.h"

namespace KPIMTextEdit {

class TextEditPrivate
{
public:

    TextEditPrivate( TextEdit *parent )
        : actionAddImage( 0 ),
          actionDeleteLine( 0 ),
          actionAddEmoticon( 0 ),
          actionInsertHtml( 0 ),
          actionTable( 0 ),
          actionFormatReset( 0 ),
          q( parent ),
          imageSupportEnabled( false ),
          emoticonSupportEnabled( false ),
          insertHtmlSupportEnabled( false ),
          insertTableSupportEnabled( false ),
          spellCheckingEnabled( false )
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
    void addImageHelper( const QString &imageName, const QImage &image,
                         int width = -1, int height = -1 );

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

    void _k_slotDeleteLine();

    void _k_slotAddEmoticon( const QString & );

    void _k_slotInsertHtml();

    void _k_slotFormatReset();

    void _k_slotTextModeChanged( KRichTextEdit::Mode );

    /// The action that triggers _k_slotAddImage()
    QAction *actionAddImage;

    /// The action that triggers _k_slotDeleteLine()
    QAction *actionDeleteLine;

    EmoticonTextEditAction *actionAddEmoticon;

    QAction *actionInsertHtml;

    TableActionMenu *actionTable;

    QAction *actionFormatReset;

    /// The parent class
    TextEdit *q;

    /// Whether or not adding or pasting images is supported
    bool imageSupportEnabled;

    bool emoticonSupportEnabled;

    bool insertHtmlSupportEnabled;

    bool insertTableSupportEnabled;
    /**
     * The names of embedded images.
     * Used to easily obtain the names of the images.
     * New images are compared to the list and not added as resource if already present.
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

    QString configFile;
    QFont saveFont;
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
    text.replace( QChar::Nbsp, QChar::fromLatin1( ' ' ) );
}

TextEdit::TextEdit( const QString &text, QWidget *parent )
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

TextEdit::TextEdit( QWidget *parent, const QString &configFile )
    : KRichTextWidget( parent ),
      d( new TextEditPrivate( this ) )
{
    d->init();
    d->configFile = configFile;
}

TextEdit::~TextEdit()
{
}

bool TextEdit::eventFilter( QObject *o, QEvent *e )
{
#ifndef QT_NO_CURSOR
    if ( o == this ) {
        KCursor::autoHideEventFilter( o, e );
    }
#endif
    return KRichTextWidget::eventFilter( o, e );
}

void TextEditPrivate::init()
{
    q->connect( q, SIGNAL(textModeChanged(KRichTextEdit::Mode)),
                q, SLOT(_k_slotTextModeChanged(KRichTextEdit::Mode)) );

    // We tell the KRichTextWidget to enable spell checking, because only then it will
    // call createHighlighter() which will create our own highlighter which also
    // does quote highlighting.
    // However, *our* spellchecking is still disabled. Our own highlighter only
    // cares about our spellcheck status, it will not highlight missspelled words
    // if our spellchecking is disabled.
    // See also KEMailQuotingHighlighter::highlightBlock().
    spellCheckingEnabled = false;
    q->setCheckSpellingEnabled( true );

#ifndef QT_NO_CURSOR
    KCursor::setAutoHideCursor( q, true, true );
#endif
    q->installEventFilter( q );
}

QString TextEdit::configFile() const
{
    return d->configFile;
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
        if ( ( ( oldPos - blockPos )  > 0 ) &&
             ( ( oldPos - blockPos ) < int( lineText.length() ) ) ) {
            bool isQuotedLine = false;
            int bot = 0; // bot = begin of text after quote indicators
            while ( bot < lineText.length() ) {
                if ( ( lineText[bot] == QChar::fromLatin1( '>' ) ) ||
                     ( lineText[bot] == QChar::fromLatin1( '|' ) ) ) {
                    isQuotedLine = true;
                    ++bot;
                } else if ( lineText[bot].isSpace() ) {
                    ++bot;
                } else {
                    break;
                }
            }
            KRichTextWidget::keyPressEvent( e );
            // duplicate quote indicators of the previous line before the new
            // line if the line actually contained text (apart from the quote
            // indicators) and the cursor is behind the quote indicators
            if ( isQuotedLine &&
                 ( bot != lineText.length() ) &&
                 ( ( oldPos - blockPos ) >= int( bot ) ) ) {
                // The cursor position might have changed unpredictably if there was selected
                // text which got replaced by a new line, so we query it again:
                cursor.movePosition( QTextCursor::StartOfBlock );
                cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
                QString newLine = cursor.selectedText();

                // remove leading white space from the new line and instead
                // add the quote indicators of the previous line
                int leadingWhiteSpaceCount = 0;
                while ( ( leadingWhiteSpaceCount < newLine.length() ) &&
                        newLine[leadingWhiteSpaceCount].isSpace() ) {
                    ++leadingWhiteSpaceCount;
                }
                newLine = newLine.replace( 0, leadingWhiteSpaceCount, lineText.left( bot ) );
                cursor.insertText( newLine );
                //cursor.setPosition( cursor.position() + 2 );
                cursor.movePosition( QTextCursor::StartOfBlock );
                setTextCursor( cursor );
            }
        } else {
            KRichTextWidget::keyPressEvent( e );
        }
    } else {
        KRichTextWidget::keyPressEvent( e );
    }
}

bool TextEdit::checkSpellingEnabled() const
{
    return d->spellCheckingEnabled;
}

void TextEdit::setCheckSpellingEnabled(bool check)
{
    EMailQuoteHighlighter *hlighter = dynamic_cast<EMailQuoteHighlighter*>( highlighter() );
    if ( hlighter ) {
        hlighter->toggleSpellHighlighting( check );
    }

    d->spellCheckingEnabled = check;
    emit checkSpellingChanged( check );
}

bool TextEdit::shouldBlockBeSpellChecked( const QString &block ) const
{
    return !isLineQuoted( block );
}

bool KPIMTextEdit::TextEdit::isLineQuoted( const QString &line ) const
{
    return quoteLength( line ) > 0;
}

int KPIMTextEdit::TextEdit::quoteLength( const QString &line ) const
{
    bool quoteFound = false;
    int startOfText = -1;
    const int lineLength( line.length() );
    for ( int i = 0; i < lineLength; ++i ) {
        if ( line[i] == QLatin1Char( '>' ) || line[i] == QLatin1Char( '|' ) ) {
            quoteFound = true;
        } else if ( line[i] != QLatin1Char( ' ' ) ) {
            startOfText = i;
            break;
        }
    }
    if ( quoteFound ) {
        if ( startOfText == -1 ) {
            startOfText = line.length() - 1;
        }
        return startOfText;
    } else {
        return 0;
    }
}

const QString KPIMTextEdit::TextEdit::defaultQuoteSign() const
{
    return QLatin1String( "> " );
}

void TextEdit::createHighlighter()
{
    EMailQuoteHighlighter *emailHighLighter = new EMailQuoteHighlighter( this );

    setHighlighterColors( emailHighLighter );

    //TODO change config
    KRichTextWidget::setHighlighter( emailHighLighter );

    if ( !spellCheckingLanguage().isEmpty() ) {
        setSpellCheckingLanguage( spellCheckingLanguage() );
    }
}

void TextEdit::setHighlighterColors( EMailQuoteHighlighter *highlighter )
{
    Q_UNUSED( highlighter );
}

QString TextEdit::toWrappedPlainText() const
{
    QTextDocument *doc = document();
    return toWrappedPlainText( doc );
}

QString TextEdit::toWrappedPlainText( QTextDocument *doc ) const
{
    QString temp;
    QRegExp rx( QLatin1String( "(http|ftp|ldap)s?\\S+-$" ) );
    QTextBlock block = doc->begin();
    while ( block.isValid() ) {
        QTextLayout *layout = block.layout();
        const int numberOfLine( layout->lineCount() );
        bool urlStart = false;
        for ( int i = 0; i < numberOfLine; ++i ) {
            QTextLine line = layout->lineAt( i );
            QString lineText = block.text().mid( line.textStart(), line.textLength() );

            if ( lineText.contains( rx ) ||
                 ( urlStart && !lineText.contains( QLatin1Char( ' ' ) ) &&
                   lineText.endsWith( QLatin1Char( '-' ) ) ) ) {
                // don't insert line break in URL
                temp += lineText;
                urlStart = true;
            } else {
                temp += lineText + QLatin1Char( '\n' );
            }
        }
        block = block.next();
    }

    // Remove the last superfluous newline added above
    if ( temp.endsWith( QLatin1Char( '\n' ) ) ) {
        temp.chop( 1 );
    }

    d->fixupTextEditString( temp );
    return temp;
}

QString TextEdit::toCleanPlainText( const QString &plainText ) const
{
    QString temp = plainText.isEmpty() ? toPlainText() : plainText;
    d->fixupTextEditString( temp );
    return temp;
}

QList<QAction*> TextEdit::createActions()
{
    QList<QAction *> listAction = KRichTextWidget::createActions();

    if ( d->imageSupportEnabled ) {
        d->actionAddImage = new QAction( QIcon::fromTheme( QLatin1String( "insert-image" ) ),
                                         i18n( "Add Image" ), this );
        d->actionAddImage->setObjectName(QLatin1String( "add_image" ));
        connect( d->actionAddImage, SIGNAL(triggered(bool)), SLOT(_k_slotAddImage()) );
        listAction << d->actionAddImage;
    }
    if ( d->emoticonSupportEnabled ) {
        d->actionAddEmoticon = new EmoticonTextEditAction( this );
        d->actionAddEmoticon->setObjectName(QLatin1String( "add_emoticon" ));
        connect( d->actionAddEmoticon, SIGNAL(emoticonActivated(QString)),
                 SLOT(_k_slotAddEmoticon(QString)) );
        listAction << d->actionAddEmoticon;
    }

    if ( d->insertHtmlSupportEnabled ) {
        d->actionInsertHtml = new QAction( i18n( "Insert HTML" ), this );
        d->actionInsertHtml->setObjectName(QLatin1String( "insert_html" ));
        connect( d->actionInsertHtml, SIGNAL(triggered(bool)), SLOT(_k_slotInsertHtml()) );
        listAction << d->actionInsertHtml;
    }

    if ( d->insertTableSupportEnabled ) {
        d->actionTable = new TableActionMenu( this );
        d->actionTable->setIcon( QIcon::fromTheme( QLatin1String( "insert-table" ) ) );
        d->actionTable->setText( i18n( "Table" ) );
        d->actionTable->setDelayed( false );
        d->actionTable->setObjectName(QLatin1String( "insert_table" ));
        listAction << d->actionTable->listAction();
        listAction << d->actionTable;
    }
    d->actionDeleteLine = new QAction( i18n( "Delete Line" ), this );
    d->actionDeleteLine->setShortcut( QKeySequence( Qt::CTRL + Qt::Key_K ) );
    d->actionDeleteLine->setObjectName(QLatin1String( "delete_line" ));
    connect( d->actionDeleteLine, SIGNAL(triggered(bool)), SLOT(_k_slotDeleteLine()) );
    listAction << d->actionDeleteLine;

    d->actionFormatReset =
            new QAction( QIcon::fromTheme( QLatin1String( "draw-eraser" ) ), i18n( "Reset Font Settings" ), this );
    d->actionFormatReset->setIconText( i18n( "Reset Font" ) );
    d->actionFormatReset->setObjectName(QLatin1String( "format_reset" ));
    connect( d->actionFormatReset, SIGNAL(triggered(bool)), SLOT(_k_slotFormatReset()) );
    listAction << d->actionFormatReset;
    return listAction;
}

void TextEdit::addImage( const QUrl &url, int width, int height )
{
    addImageHelper( url, width, height );
}

void TextEdit::addImageHelper( const QUrl &url, int width, int height )
{
    QImage image;
    if ( !image.load( url.path() ) ) {
        KMessageBox::error(
                    this,
                    xi18nc( "@info",
                           "Unable to load image <filename>%1</filename>.",
                           url.path() ) );
        return;
    }
    QFileInfo fi( url.path() );
    QString imageName =
            fi.baseName().isEmpty() ?
                QLatin1String( "image.png" ) :
                QString( fi.baseName() + QLatin1String( ".png" ) );
    d->addImageHelper( imageName, image, width, height );
}

void TextEdit::loadImage ( const QImage &image, const QString &matchName,
                           const QString &resourceName )
{
    QSet<int> cursorPositionsToSkip;
    QTextBlock currentBlock = document()->begin();
    QTextBlock::iterator it;
    while ( currentBlock.isValid() ) {
        for ( it = currentBlock.begin(); !it.atEnd(); ++it ) {
            QTextFragment fragment = it.fragment();
            if ( fragment.isValid() ) {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                if ( imageFormat.isValid() && imageFormat.name() == matchName ) {
                    int pos = fragment.position();
                    if ( !cursorPositionsToSkip.contains( pos ) ) {
                        QTextCursor cursor( document() );
                        cursor.setPosition( pos );
                        cursor.setPosition( pos + 1, QTextCursor::KeepAnchor );
                        cursor.removeSelectedText();
                        document()->addResource( QTextDocument::ImageResource,
                                                 QUrl( resourceName ), QVariant( image ) );
                        QTextImageFormat format;
                        format.setName( resourceName );
                        if ( ( imageFormat.width() != 0 ) && ( imageFormat.height() != 0 ) ) {
                            format.setWidth( imageFormat.width() );
                            format.setHeight( imageFormat.height() );
                        }
                        cursor.insertImage( format );

                        // The textfragment iterator is now invalid, restart from the beginning
                        // Take care not to replace the same fragment again, or we would be in
                        // an infinite loop.
                        cursorPositionsToSkip.insert( pos );
                        it = currentBlock.begin();
                    }
                }
            }
        }
        currentBlock = currentBlock.next();
    }
}

void TextEditPrivate::addImageHelper( const QString &imageName, const QImage &image,
                                      int width, int height )
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
        if ( firstDot == -1 ) {
            imageNameToAdd = imageName + QString::number( imageNumber++ );
        } else {
            imageNameToAdd = imageName.left( firstDot ) + QString::number( imageNumber++ ) +
                    imageName.mid( firstDot );
        }
    }

    if ( !mImageNames.contains( imageNameToAdd ) ) {
        document->addResource( QTextDocument::ImageResource, QUrl( imageNameToAdd ), image );
        mImageNames << imageNameToAdd;
    }
    if ( width != -1 && height != -1 ) {
        QTextImageFormat format;
        format.setName( imageNameToAdd );
        format.setWidth( width );
        format.setHeight( height );
        q->textCursor().insertImage( format );
    } else {
        q->textCursor().insertImage( imageNameToAdd );
    }
    q->enableRichTextMode();
}

ImageWithNameList TextEdit::imagesWithName() const
{
    ImageWithNameList retImages;
    QStringList seenImageNames;
    QList<QTextImageFormat> imageFormats = d->embeddedImageFormats();
    foreach ( const QTextImageFormat &imageFormat, imageFormats ) {
        if ( !seenImageNames.contains( imageFormat.name() ) ) {
            QVariant resourceData = document()->resource( QTextDocument::ImageResource,
                                                          QUrl( imageFormat.name() ) );
            QImage image = qvariant_cast<QImage>( resourceData );
            QString name = imageFormat.name();
            ImageWithNamePtr newImage( new ImageWithName );
            newImage->image = image;
            newImage->name = name;
            retImages.append( newImage );
            seenImageNames.append( imageFormat.name() );
        }
    }
    return retImages;
}

QList< QSharedPointer<EmbeddedImage> > TextEdit::embeddedImages() const
{
    ImageWithNameList normalImages = imagesWithName();
    QList< QSharedPointer<EmbeddedImage> > retImages;
    foreach ( const ImageWithNamePtr &normalImage, normalImages ) {
        QBuffer buffer;
        buffer.open( QIODevice::WriteOnly );
        normalImage->image.save( &buffer, "PNG" );

        qsrand( QDateTime::currentDateTime().toTime_t() + qHash( normalImage->name ) );
        QSharedPointer<EmbeddedImage> embeddedImage( new EmbeddedImage() );
        retImages.append( embeddedImage );
        embeddedImage->image = KMime::Codec::codecForName( "base64" )->encode( buffer.buffer() );
        embeddedImage->imageName = normalImage->name;
        embeddedImage->contentID = QString( QLatin1String( "%1@KDE" ) ).arg( qrand() );
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
                    //TODO: Replace with a way to see if an image is an embedded image or a remote
                    QUrl url( imageFormat.name() );
                    if ( !url.isValid() || !url.scheme().startsWith( QLatin1String( "http" ) ) ) {
                        retList.append( imageFormat );
                    }
                }
            }
        }
        currentBlock = currentBlock.next();
    }
    return retList;
}

void TextEditPrivate::_k_slotAddEmoticon( const QString &text )
{
    QTextCursor cursor = q->textCursor();
    cursor.insertText( text );
}

void TextEditPrivate::_k_slotInsertHtml()
{
    if ( q->textMode() == KRichTextEdit::Rich ) {
        QPointer<InsertHtmlDialog> dialog = new InsertHtmlDialog( q );
        if ( dialog->exec() ) {
            const QString str = dialog->html();
            if ( !str.isEmpty() ) {
                QTextCursor cursor = q->textCursor();
                cursor.insertHtml( str );
            }
        }
        delete dialog;
    }
}

void TextEditPrivate::_k_slotAddImage()
{
    QPointer<InsertImageDialog> dlg = new InsertImageDialog( q );
    if ( dlg->exec() == QDialog::Accepted && dlg ) {
        const QUrl url = dlg->imageUrl();
        int imageWidth = -1;
        int imageHeight = -1;
        if ( !dlg->keepOriginalSize() ) {
            imageWidth = dlg->imageWidth();
            imageHeight = dlg->imageHeight();
        }
        q->addImage( url, imageWidth, imageHeight );
    }
    delete dlg;
}

void TextEditPrivate::_k_slotTextModeChanged( KRichTextEdit::Mode mode )
{
    if ( mode == KRichTextEdit::Rich ) {
        saveFont = q->currentFont();
    }
}

void TextEditPrivate::_k_slotFormatReset()
{
    q->setTextBackgroundColor( q->palette().highlightedText().color() );
    q->setTextForegroundColor( q->palette().text().color() );
    q->setFont( saveFont );

}

void KPIMTextEdit::TextEdit::enableImageActions()
{
    d->imageSupportEnabled = true;
}

bool KPIMTextEdit::TextEdit::isEnableImageActions() const
{
    return d->imageSupportEnabled;
}

void KPIMTextEdit::TextEdit::enableEmoticonActions()
{
    d->emoticonSupportEnabled = true;
}

bool KPIMTextEdit::TextEdit::isEnableEmoticonActions() const
{
    return d->emoticonSupportEnabled;
}

void KPIMTextEdit::TextEdit::enableInsertHtmlActions()
{
    d->insertHtmlSupportEnabled = true;
}

bool KPIMTextEdit::TextEdit::isEnableInsertHtmlActions() const
{
    return d->insertHtmlSupportEnabled;
}

bool KPIMTextEdit::TextEdit::isEnableInsertTableActions() const
{
    return d->insertTableSupportEnabled;
}

void KPIMTextEdit::TextEdit::enableInsertTableActions()
{
    d->insertTableSupportEnabled = true;
}

QByteArray KPIMTextEdit::TextEdit::imageNamesToContentIds(
        const QByteArray &htmlBody, const KPIMTextEdit::ImageList &imageList )
{
    QByteArray result = htmlBody;
    if ( !imageList.isEmpty() ) {
        foreach ( const QSharedPointer<EmbeddedImage> &image, imageList ) {
            const QString newImageName = QLatin1String( "cid:" ) + image->contentID;
            QByteArray quote( "\"" );
            result.replace( QByteArray( quote + image->imageName.toLocal8Bit() + quote ),
                            QByteArray( quote + newImageName.toLocal8Bit() + quote ) );
        }
    }
    return result;
}

void TextEdit::insertImage( const QImage &image, const QFileInfo &fileInfo )
{
    QString imageName = fileInfo.baseName().isEmpty() ?
                i18nc( "Start of the filename for an image", "image" ) :
                fileInfo.baseName();
    d->addImageHelper( imageName, image );
}

void TextEdit::insertFromMimeData( const QMimeData *source )
{
    // Add an image if that is on the clipboard
    if ( textMode() == KRichTextEdit::Rich && source->hasImage() && d->imageSupportEnabled ) {
        QImage image = qvariant_cast<QImage>( source->imageData() );
        QFileInfo fi;
        insertImage( image, fi );
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
    if ( source->hasHtml() && textMode() == KRichTextEdit::Rich ) {
        return true;
    }

    if ( source->hasText() ) {
        return true;
    }

    if ( textMode() == KRichTextEdit::Rich && source->hasImage() && d->imageSupportEnabled ) {
        return true;
    }

    return KRichTextWidget::canInsertFromMimeData( source );
}

bool TextEdit::isFormattingUsed() const
{
    if ( textMode() == Plain ) {
        return false;
    }

    return TextUtils::containsFormatting( document() );
}

void TextEditPrivate::_k_slotDeleteLine()
{
    if ( q->hasFocus() ) {
        q->deleteCurrentLine();
    }
}

void TextEdit::deleteCurrentLine()
{
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    const QTextLayout *layout = block.layout();

    // The current text block can have several lines due to word wrapping.
    // Search the line the cursor is in, and then delete it.
    for ( int lineNumber = 0; lineNumber < layout->lineCount(); lineNumber++ ) {
        QTextLine line = layout->lineAt( lineNumber );
        const bool lastLineInBlock = ( line.textStart() + line.textLength() == block.length() - 1 );
        const bool oneLineBlock = ( layout->lineCount() == 1 );
        const int startOfLine = block.position() + line.textStart();
        int endOfLine = block.position() + line.textStart() + line.textLength();
        if ( !lastLineInBlock ) {
            endOfLine -= 1;
        }

        // Found the line where the cursor is in
        if ( cursor.position() >= startOfLine && cursor.position() <= endOfLine ) {
            int deleteStart = startOfLine;
            int deleteLength = line.textLength();
            if ( oneLineBlock ) {
                deleteLength++; // The trailing newline
            }

            // When deleting the last line in the document,
            // remove the newline of the line before the last line instead
            if ( deleteStart + deleteLength >= document()->characterCount() &&
                 deleteStart > 0 ) {
                deleteStart--;
            }

            cursor.beginEditBlock();
            cursor.setPosition( deleteStart );
            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor, deleteLength );
            cursor.removeSelectedText();
            cursor.endEditBlock();
            return;
        }
    }
}

#include "moc_textedit.cpp"
