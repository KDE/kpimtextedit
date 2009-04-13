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
 * A list with all images can be retrieved with KMComposerEditor::embeddedImages().
 */
struct EmbeddedImage
{
  QByteArray image;   ///< The image, encoded as PNG with base64 encoding
  QString contentID;  ///< The content id of the embedded image
  QString imageName;  ///< Name of the image as it is available as a resource in the editor
};

// TODO:

// APIDOX - Also mainpage.dox!
// messages.sh
// Proofreading
// coding style
// FIXMEs (!!)
// GUI test app
// clean up mime insert/paste mess
// clean up quoting mess
// bugzilla entry

// Unittests:
// inssert signature at various places
// replace signature at various places + not inside qutoes
// ^ same for HTML signature
// ^ same for multiline sigs
// test inserting images -> embeddedImages() returns the correct thing? images there?
// toWrappedPlainText()
// toCleanPlainText()
// enter key: normal + in quote

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
     */
    explicit TextEdit( const QString& text, QWidget *parent = 0 );

    /**
     * Constructs a TextEdit object.
     */
    explicit TextEdit( QWidget *parent = 0 );

    /**
     * Reimplemented from KMEditor, to support more actions.
     *
     * The additional action XML names are:
     * - add_image
     *
     * @reimp
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
    QList< QSharedPointer<EmbeddedImage> > embeddedImages() const;

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
     * Returns the text of the editor as plain text, with linebreaks inserted
     * where word-wrapping occurred.
     */
    QString toWrappedPlainText() const;

    /**
     * Same as toPlainText() from QTextEdit, only that it removes embedded images.
     */
    QString toCleanPlainText() const;

    ~TextEdit();

    virtual void changeHighlighterColors( EMailQuoteHighlighter * );

  protected:

    virtual void dropEvent( QDropEvent *e ) {
      //FIXME !!
    }
    virtual bool canInsertFromMimeData( const QMimeData *source ) const {
      // FIXME
      return false;
    }
    virtual void insertFromMimeData( const QMimeData *source );
    virtual bool eventFilter( QObject*o, QEvent* e );
    virtual void keyPressEvent ( QKeyEvent * e );

    // For the explaination for these three methods, see the comment at the
    // spellCheckingEnabled variable of the private class.

    /**
     * Reimplemented from KTextEditSpellInterface
     * @reimp
     */
    virtual bool isSpellCheckingEnabled() const;

    /**
     * Reimplemented from KTextEditSpellInterface
     * @reimp
     */
    virtual void setSpellCheckingEnabled( bool enable );

    /**
     * Reimplemented from KTextEditSpellInterface, to avoid spellchecking
     * quoted text.
     * @reimp
     */
    virtual bool shouldBlockBeSpellChecked( const QString& block ) const;

    /**
     * Reimplemented to create our own highlighter which does quote and
     * spellcheck highlighting
     * @reimp
     */
    virtual void createHighlighter();

  public Q_SLOTS:

    /**
     * Pastes the content of the clipboard into the editor, if the
     * mime type of the clipboard's contents in supported.
     */
    virtual void paste() {
      // FIXME !!
    }

  private:

    std::auto_ptr<TextEditPrivate> const d;
    friend class TextEditPrivate;
    Q_PRIVATE_SLOT( d, void _k_slotAddImage() )
};

} // namespace

#endif
