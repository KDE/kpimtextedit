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
#ifndef KPIMTEXTEDIT_TEXTEDIT_H
#define KPIMTEXTEDIT_TEXTEDIT_H

#include "kpimtextedit_export.h"

#include <kpimidentities/signature.h>

#include <KDE/KRichTextWidget>

#include <QtCore/QSharedPointer>

#include <memory>

class KUrl;

namespace KPIMTextEdit {

class TextEditPrivate;
class EMailQuoteHighlighter;

/**
 * Holds information about an embedded HTML image.
 * A list with all images can be retrieved with TextEdit::embeddedImages().
 */
struct EmbeddedImage
{
  QByteArray image;   ///< The image, encoded as PNG with base64 encoding
  QString contentID;  ///< The content id of the embedded image
  QString imageName;  ///< Name of the image as it is available as a resource in the editor
};

typedef QList< QSharedPointer<EmbeddedImage> > ImageList;

// TODO:

// Proofreading / API review
// isHtmlUsed() function

// Unittests:
// inssert signature at various places
// replace signature at various places + not inside qutoes
// ^ same for HTML signature
// ^ same for multiline sigs
// test inserting images -> embeddedImages() returns the correct thing? images there?
// toWrappedPlainText()
// toCleanPlainText()
// enter key: normal + in quote

/**
 * Special textedit that provides additional features which are useful for PIM applications
 * like mail clients.
 * Additional features this class provides:
 *   - Highlighting quoted text
 *   - Handling of inline images
 *   - Handling of signatures
 *   - Auto-Hiding the cursor
 *   - Handling of pastes and drops
 *
 * @since 4.3
 */
class KPIMTEXTEDIT_EXPORT TextEdit : public KRichTextWidget,
                                     protected KTextEditSpellInterface // TODO: KDE5: get rid of the spell interface
{
  Q_OBJECT

  public:

    /**
     * Describes the placement of a text which is to be inserted into this
     * textedit.
     */
    enum Placement {
      Start,                   ///< The text is placed at the start of the textedit
      End,                     ///< The text is placed at the end of the textedit
      AtCursor                 ///< The text is placed at the current cursor position
    };

    /**
     * Constructs a TextEdit object
     * @param text the inital plain text of the text edit, interpreted as HTML
     * @param parent the parent widget
     */
    explicit TextEdit( const QString& text, QWidget *parent = 0 );

    /**
     * Constructs a TextEdit object.
     * @param parent the parent widget
     */
    explicit TextEdit( QWidget *parent = 0 );

    /**
     * Calling this allows createActions() to create the add image actions.
     * Call this method before callilng createActions(), otherwise the action
     * will not be added.
     * Also, if image actions is enabled, the user can paste PNG images.
     *
     * Don't call this if you don't want to support adding images.
     */
    void enableImageActions();

    /**
     * Destructor
     */
    ~TextEdit();

    /**
     * Reimplemented from KMEditor, to support more actions.
     *
     * The additional action XML names are:
     * - add_image
     * 
     * The add_image actions is only added if enableImageActions() is called before.
     */
    virtual void createActions( KActionCollection *actionCollection );

    /**
     * Adds an image. The image is loaded from file and then pasted to the current
     * cursor position.
     *
     * @param url The URL of the file which contains the image
     */
    void addImage( const KUrl &url );

    /**
     * Get a list with all embedded HTML images.
     * If the same image is contained twice or more in the editor, it will have only
     * one entry in this list.
     *
     * @return a list of embedded HTML images of the editor.
     */
    ImageList embeddedImages() const;

    /**
     * Cleans the whitespace of the edit's text.
     * Adjacent tabs and spaces will be converted to a single space.
     * Trailing whitespace will be removed.
     * More than 2 newlines in a row will be changed to 2 newlines.
     * Text in quotes or text inside of the given signature will not be
     * cleaned.
     * For undo/redo, this is treated as one operation.
     *
     * @param sig text inside this signature will not be cleaned
     */
    void cleanWhitespace( const KPIMIdentities::Signature &sig );

    /**
     * Inserts the signature @p sig into the textedit.
     * The cursor position is preserved.
     * A leading or trailing newline is also added automatically, depending on
     * the placement.
     * For undo/redo, this is treated as one operation.
     *
     * Rich text mode will be enabled if the signature is in inlined HTML format.
     *
     * @param placement defines where in the textedit the signature should be
     *                  inserted.
     * @param addSeparator if true, the separator '-- \n' will be added in front
     *                     of the signature
     */
    void insertSignature( const KPIMIdentities::Signature &sig,
                          Placement placement = End, bool addSeparator = true );

    /**
     * Inserts the signature @p sig into the textedit.
     * The cursor position is preserved.
     * A leading or trailing newline is also added automatically, depending on
     * the placement.
     * For undo/redo, this is treated as one operation.
     * A separator is not added.
     *
     * Use the other insertSignature() function if possible, as it has support
     * for separators and does HTML detection automatically.
     *
     * Rich text mode will be enabled if @p isHtml is true.
     *
     * @param placement defines where in the textedit the signature should be
     *                  inserted.
     * @param isHtml defines whether the signature should be inserted as text or html
     */
    void insertSignature( const QString &signature, Placement placement = End,
                          bool isHtml = false );

    /**
     * Replaces all occurrences of the old signature with the new signature.
     * Text in quotes will be ignored.
     * For undo/redo, this is treated as one operation.
     * If the old signature is empty, nothing is done.
     * If the new signature is empty, the old signature including the
     * separator is removed.
     *
     * @param oldSig the old signature, which will be replaced
     * @param newSig the new signature
     */
    void replaceSignature( const KPIMIdentities::Signature &oldSig,
                           const KPIMIdentities::Signature &newSig );

    /**
     * Returns a list of all occurences of the given signature.
     * The list contains pairs which consists of the starting position and the end
     * of the signature.
     * 
     * @param sig this signature will be searched for
     * @return a list of pairs of start and end positions of the signature
     */
    QList< QPair<int,int> > signaturePositions( const KPIMIdentities::Signature &sig ) const;

    /**
     * Returns the text of the editor as plain text, with linebreaks inserted
     * where word-wrapping occurred.
     */
    QString toWrappedPlainText() const;

    /**
     * Same as toPlainText() from QTextEdit, only that it removes embedded images and
     * converts non-breaking space characters to normal spaces.
     */
    QString toCleanPlainText() const;

    /**
     * This method is called after the highlighter is created.
     * If you use custom colors for highlighting, override this method and set the colors
     * to the highlighter in it.
     *
     * The default implementation does nothing, therefore the default colors of the
     * EMailQuoteHighlighter class will be used.
     *
     * @param highlighter the highlighter that was just created. You need to set the colors
     *                    of this highlighter.
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
     * For all given embedded images, this function replace the image name in the <img> tag of the
     * HTML body with cid:content-id,
     * so that the HTML references the image body parts, see RFC 2557.
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
    static QByteArray imageNamesToContentIds( const QByteArray &htmlBody, const ImageList &imageList );

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
    virtual bool eventFilter( QObject*o, QEvent* e );

    /**
     * Reimplemented to add qoute signs when the user presses enter
     * on a quoted line.
     */
    virtual void keyPressEvent ( QKeyEvent * e );

    // For the explaination for these four methods, see the comment at the
    // spellCheckingEnabled variable of the private class.

    /**
     * Reimplemented from KTextEditSpellInterface
     */
    virtual bool isSpellCheckingEnabled() const;

    /**
     * Reimplemented from KTextEditSpellInterface
     */
    virtual void setSpellCheckingEnabled( bool enable );

    /**
     * Reimplemented from KTextEditSpellInterface, to avoid spellchecking
     * quoted text.
     */
    virtual bool shouldBlockBeSpellChecked( const QString& block ) const;

    /**
     * Reimplemented to create our own highlighter which does quote and
     * spellcheck highlighting
     */
    virtual void createHighlighter();

  private:

    std::auto_ptr<TextEditPrivate> const d;
    friend class TextEditPrivate;
    Q_PRIVATE_SLOT( d, void _k_slotAddImage() )
};

} // namespace

#endif
