/*
  Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>

  Based on KMail and libkdepim code by:
  Copyright 2007 - 2010 Laurent Montel <montel@kde.org>

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
#ifndef KPIMTEXTEDIT_TEXTEDIT_H
#define KPIMTEXTEDIT_TEXTEDIT_H

#include "kpimtextedit_export.h"

#include <KRichTextWidget>
#include <KActionCollection>

#include <QtCore/QSharedPointer>

#include <memory>

class QUrl;
class QFileInfo;
class KActionCollection;

namespace KPIMTextEdit {

class TextEditPrivate;
class EMailQuoteHighlighter;

/**
 * Holds information about an embedded HTML image that will be useful for mail clients.
 * A list with all images can be retrieved with TextEdit::embeddedImages().
 */
struct EmbeddedImage
{
    QByteArray image;   ///< The image, encoded as PNG with base64 encoding
    QString contentID;  ///< The content id of the embedded image
    QString imageName;  ///< Name of the image as it is available as a resource in the editor
};

/**
 * Holds information about an embedded HTML image that will be generally useful.
 * A list with all images can be retrieved with TextEdit::imagesWithName().
 *
 * @since 4.4
 */
struct ImageWithName
{
    QImage image; ///< The image
    QString name; ///< The name of the image as it is available as a resource in the editor
};

typedef QSharedPointer<ImageWithName> ImageWithNamePtr;
typedef QList< ImageWithNamePtr > ImageWithNameList;
typedef QList< QSharedPointer<EmbeddedImage> > ImageList;

/**
 * Special textedit that provides additional features which are useful for PIM applications
 * like mail clients.
 * Additional features this class provides:
 *   - Highlighting quoted text
 *   - Handling of inline images
 *   - Auto-Hiding the cursor
 *   - Handling of pastes and drops of images
 *
 * @since 4.3
 */
class KPIMTEXTEDIT_EXPORT TextEdit : public KRichTextWidget
{
    Q_OBJECT

public:

    /**
     * Constructs a TextEdit object
     * @param text the initial plain text of the text edit, interpreted as HTML
     * @param parent the parent widget
     */
    explicit TextEdit( const QString &text, QWidget *parent = 0 );

    /**
     * Constructs a TextEdit object.
     * @param parent the parent widget
     */
    explicit TextEdit( QWidget *parent = 0 );

    /**
     * Constructs a TextEdit object
     * @param parent the parent widget
     * @param configFile the config file
     * @since 4.6
     *
     * TODO KDE-5 merge with other constructor
     */
    explicit TextEdit( QWidget *parent, const QString &configFile );

    /**
     * Calling this allows createActions() to create the add image actions.
     * Call this method before calling createActions(), otherwise the action
     * will not be added.
     * Also, if image actions is enabled, the user can paste PNG images.
     *
     * Don't call this if you don't want to support adding images.
     */
    void enableImageActions();
    /**
     * Calling this allows createActions() to create the add emoticons actions.
     * Call this method before calling createActions(), otherwise the action
     * will not be added.
     * Don't call this if you don't want to support emoticons actions.
     */
    void enableEmoticonActions();

    void enableInsertHtmlActions();

    void enableInsertTableActions();
    /**
     * Destructor
     */
    ~TextEdit();

    /**
     * Reimplemented from KMEditor, to support more actions.
     *
     * @param actionCollection the collection to put the new actions into
     * The additional action XML names are:
     * - add_image
     * - delete_line
     *
     * The add_image actions is only added if enableImageActions() is called before.
     */
    virtual QList<QAction *> createActions();

    /**
     * Adds an image. The image is loaded from file and then pasted to the current
     * cursor position with the given @p width and @p height.
     *
     * @param url The URL of the file which contains the image
     * @param width The width the inserted image will have.
     * @param height The height the inserted image will have.
     *
     * @since 4.10
     */
    void addImage( const QUrl &url, int width = -1, int height = -1 );

    /**
     * Loads an image into the textedit. The difference to addImage() is that this
     * function expects that the image tag is already present in the HTML source.
     *
     * @param image the image to load
     * @param matchName the name of tags to match image
     * @param resourceName the resource name of image
     * So what this message does is that it scans the HTML source for the image
     * tag that matches the @p matchName, and then inserts the @p image as a
     * resource, giving that resource the name @p resourceName.
     *
     * @since 4.4
     */
    void loadImage( const QImage &image, const QString &matchName, const QString &resourceName );

    /**
     * Deletes the line at the current cursor position.
     * @since 4.4
     */
    void deleteCurrentLine();

    /**
     * Get a list with all embedded HTML images.
     * If the same image is contained twice or more in the editor, it will have only
     * one entry in this list.
     *
     * @return a list of embedded HTML images of the editor.
     */
    ImageList embeddedImages() const;

    /**
     * Same as embeddedImages(), only that this returns a list of general purpose information,
     * whereas the embeddedImages() function returns a list with mail-specific information.
     *
     * @since 4.4
     */
    ImageWithNameList imagesWithName() const;

    /**
     * Returns the text of the editor as plain text, with linebreaks inserted
     * where word-wrapping occurred.
     */
    QString toWrappedPlainText() const;

    /**
     * @since 5.0
     */
    QString toWrappedPlainText( QTextDocument *document ) const;

    /**
     * Same as toPlainText() from QTextEdit, only that it removes embedded
     * images and converts non-breaking space characters to normal spaces.
     * @since 4.10
     */
    QString toCleanPlainText( const QString &plainText = QString() ) const;

    /**
     * This method is called after the highlighter is created.
     * If you use custom colors for highlighting, override this method and
     * set the colors to the highlighter in it.
     *
     * The default implementation does nothing, therefore the default colors of
     * the EMailQuoteHighlighter class will be used.
     *
     * @param highlighter the highlighter that was just created. You need to
     *   set the colors of this highlighter.
     */
    virtual void setHighlighterColors( EMailQuoteHighlighter *highlighter );

    /**
     * Convenience method for qouteLength( line ) > 0
     */
    bool isLineQuoted( const QString &line ) const;

    /**
     * This is called whenever the editor needs to find out the length of the quote,
     * i.e. the length of the quote prefix before the real text starts.
     * The default implementation counts the number of spaces, '>' and '|' chars in
     * front of the line.
     *
     * @param line the line of which the length of the quote prefix should be returned
     * @return 0 if the line is not quoted, the length of the quote prefix otherwise
     * FIXME: Not yet used in all places, e.g. keypressEvent() or the quote highlighter
     */
    virtual int quoteLength( const QString &line ) const;

    /**
     * Returns the prefix that is added to a line that is quoted.
     * By default, this is "> ".
     */
    virtual const QString defaultQuoteSign() const;

    /**
     * For all given embedded images, this function replace the image name
     * in the <img> tag of the HTML body with cid:content-id, so that the
     * HTML references the image body parts, see RFC 2557.
     *
     * This is useful when building a MIME message with inline images.
     *
     * Note that this function works on encoded content already.
     *
     * @param htmlBody the HTML code in which the <img> tag will be modified.
     *                 The HTML code here could come from toHtml(), for example.
     *
     * @param imageList the list of images of which the <img> tag will be modified.
     *                  You can get such a list from the embeddedImages() function.
     *
     * @return a modified HTML code, where the <img> tags got replaced
     */
    static QByteArray imageNamesToContentIds( const QByteArray &htmlBody,
                                              const ImageList &imageList );

    /**
     * Checks if rich text formatting is used anywhere.
     * This is not the same as checking whether textMode() returns "Rich",
     * since that only tells that rich text mode is enabled, but not if any
     * special formatting is actually used.
     *
     * @return true if formatting is used anywhere
     */
    bool isFormattingUsed() const;

    /**
     *  Return config file.
     *  @since 4.5
     */
    QString configFile() const;

    /**  Return true if richtext mode support image
     *  @since 4.6
     */
    bool isEnableImageActions() const;

    /** Return true if emoticons actions supported
     *  @since 4.9
     */
    bool isEnableEmoticonActions() const;

    /**
     * @param image the image to insert
     * @param info the info to supply with image
     * @since 4.6
     */
    void insertImage( const QImage &image, const QFileInfo &info );

    /**
     * @since 4.10
     */
    bool isEnableInsertHtmlActions() const;

    /**
     * @since 4.10
     */
    bool isEnableInsertTableActions() const;

protected:

    /**
     * Reimplemented for inline image support
     */
    virtual bool canInsertFromMimeData( const QMimeData *source ) const;

    /**
     * Reimplemented for inline image support
     */
    virtual void insertFromMimeData( const QMimeData *source );

    /**
     * Reimplemented from KRichTextWidget to hide the mouse cursor when there
     * was no mouse movement for some time, using KCursor
     */
    virtual bool eventFilter( QObject *o, QEvent *e );

    /**
     * Reimplemented to add qoute signs when the user presses enter
     * on a quoted line.
     */
    virtual void keyPressEvent ( QKeyEvent *e );

    // For the explaination for these four methods, see the comment at the
    // spellCheckingEnabled variable of the private class.

    /**
     * Reimplemented from KTextEdit
     */
    virtual bool checkSpellingEnabled() const;

    /**
     * Reimplemented from KTextEdit
     */
    virtual void setCheckSpellingEnabled( bool check );

    /**
     * Reimplemented from KTextEdit, to avoid spellchecking
     * quoted text.
     */
    virtual bool shouldBlockBeSpellChecked( const QString &block ) const;

    /**
     * Reimplemented to create our own highlighter which does quote and
     * spellcheck highlighting
     */
    virtual void createHighlighter();

private:
    void addImageHelper( const QUrl &url, int width = -1, int height = -1 );
    std::auto_ptr<TextEditPrivate> const d;
    friend class TextEditPrivate;
    Q_PRIVATE_SLOT( d, void _k_slotAddImage() )
    Q_PRIVATE_SLOT( d, void _k_slotDeleteLine() )
    Q_PRIVATE_SLOT( d, void _k_slotAddEmoticon( const QString & ) )
    Q_PRIVATE_SLOT( d, void _k_slotInsertHtml() )
    Q_PRIVATE_SLOT( d, void _k_slotFormatReset() )
    Q_PRIVATE_SLOT( d, void _k_slotTextModeChanged( KRichTextEdit::Mode ) )
};

} // namespace

#endif
