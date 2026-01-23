/*
  SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/
#pragma once

#include "abstractmarkupbuilder.h"
#include "kpimtextedit_export.h"
#include <QTextDocument>
#include <QTextFrame>
class QTextTable;
class QTextTableCell;
class QTextList;
class QTextCharFormat;

namespace KPIMTextEdit
{
class MarkupDirectorPrivate;
class AbstractMarkupBuilder;

/// @headerfile markupdirector.h grantlee/markupdirector.h

/*!
 * \class KPIMTextEdit::MarkupDirector
 * \inmodule KPIMTextEdit
 * \inheaderfile KPIMTextEdit/MarkupDirector
 *
  \brief Instructs a builder object to create markup output

  The **%MarkupDirector** is used with an implementation of
  AbstractMarkupBuilder to create a marked up document output.

  Usage can be quite simple.

  \code
    auto doc = editor->document(); // editor is a QTextEdit

    auto builder = new HTMLBuilder();
    auto md = new MarkupDirector(builder);
    md->processDocument(doc);
    browser->setHtml(builder->getResult()); // browser is a QTextBrowser.
  \endcode

  Or with a different builder:

  \code
    auto builder = new PlainTextMarkupBuilder();
    auto md = new MarkupDirector(builder);
    md->processDocument(doc);
    browser->setPlainText(builder->getResult());
  \endcode

  The **%MarkupDirector** also provides API for processing just part of a
  QTextDocument, such as a QTextFrame or a QTextBlock. The appropriate method
  may then be called with an invalid iterator as appropriate.

  \code
    // ... Do some processing to get a QTextFrame.
    auto frame = getFrame();

    auto builder = new PlainTextMarkupBuilder();
    auto md = new MarkupDirector(builder);

    // Create output from only the frame.
    md->processFrame(QTextFrame::iterator(), frame);
    browser->setPlainText(builder->getResult());
  \endcode

  The behaviour of the **%MarkupDirector** can be customized by subclassing.
  Support for custom types can also be added by implementing the \
  processCustomFragment method.

  \sa \ custom_qtextobject

  \class KPIMTextEdit::MarkupDirector
  \inmodule KPIMTextedit
  \inheaderfile KPIMTextEdit/MarkupDirector
  \author Stephen Kelly <steveire@gmail.com>
*/
class KPIMTEXTEDIT_EXPORT MarkupDirector
{
public:
    /*!
    Constructor
  */
    explicit MarkupDirector(KPIMTextEdit::AbstractMarkupBuilder *builder);

    /*!
    Destructor
  */
    virtual ~MarkupDirector();

    /*!
    Constructs the output by directing the builder to create the markup.
  */
    virtual void processDocument(QTextDocument *doc);

    /*!
    Directs the builder to create output for the single \a frame. If calling
    this method directly, an invalid QTextFrame::iterator may be used.
  */
    [[nodiscard]] virtual QTextFrame::iterator processFrame(QTextFrame::iterator it, QTextFrame *frame);

    /*!
    Directs the builder to create output for the single \a block. If calling
    this method directly, an invalid QTextFrame::iterator may be used.

    This method does not process the contents of the \a block, but uses the
    \ processBlockContents method to do so.
  */
    [[nodiscard]] virtual QTextFrame::iterator processBlock(QTextFrame::iterator it, const QTextBlock &block);

    /*!
    Directs the builder to create output for the single \a textObject. If
    calling this method directly, an invalid QTextFrame::iterator may be used.

    The block \a block is the container of the \a textObject.
  */
    [[nodiscard]] virtual QTextFrame::iterator processObject(QTextFrame::iterator it, const QTextBlock &block, QTextObject *textObject);

    /*!
    Directs the builder to create output for the single \a textBlockGroup. If
    calling this method directly, an invalid QTextFrame::iterator may be used.

    The block \a block is the first block in the \a textBlockGroup.
  */
    [[nodiscard]] virtual QPair<QTextFrame::iterator, QTextBlock>
    processBlockGroup(const QTextFrame::iterator &it, const QTextBlock &block, QTextBlockGroup *textBlockGroup);

    /*!
    Directs the builder to create output for the single \a textList. If
    calling this method directly, an invalid QTextFrame::iterator may be used.

    The block \a block is the first block in the \a textList.
  */
    [[nodiscard]] virtual QPair<QTextFrame::iterator, QTextBlock> processList(QTextFrame::iterator it, const QTextBlock &block, QTextList *textList);

    /*!
    Directs the builder to create output for the contents of the single @p
    block. If calling this method directly, an invalid QTextFrame::iterator
    may be used.
  */
    virtual QTextFrame::iterator processBlockContents(QTextFrame::iterator it, const QTextBlock &block);

    /*!
    Hook for instructing the builder to create output for the \a fragemnt with
    a custom type. \a doc is the document the fragment is in.
  */
    virtual void processCustomFragment(const QTextFragment &fragment, QTextDocument const *doc);

    /*!
    Directs the builder to create output for the contents of the single @p
    fragment. If calling this method directly, an invalid QTextBlock::iterator
    may be used. \a doc is the document the fragment is in.
  */
    [[nodiscard]] virtual QTextBlock::iterator processFragment(QTextBlock::iterator it, const QTextFragment &fragment, QTextDocument const *doc);

    /*!
    Directs the builder to create output for the contents of the single @p
    textObject. The \a textObject is represented in the QTextDocument with the
    QTextFragment \a fragment.

    If calling this method directly, an invalid QTextBlock::iterator may be
    used.
  */
    [[nodiscard]] virtual QTextBlock::iterator processCharTextObject(QTextBlock::iterator it, const QTextFragment &fragment, QTextObject *textObject);

    /*!
    Directs the builder to create output for the image represented by the @p
    imageFormat.

    If calling this method directly, an invalid QTextBlock::iterator may be
    used. \a doc is the document the fragment is in.
  */
    [[nodiscard]] virtual QTextBlock::iterator processImage(QTextBlock::iterator it, const QTextImageFormat &imageFormat, QTextDocument const *doc);

    /*!
    Directs the builder to create output for the contents of the single @p
    table.

    If calling this method directly, an invalid QTextFrame::iterator may be
    used.
  */
    [[nodiscard]] virtual QTextFrame::iterator processTable(QTextFrame::iterator it, QTextTable *table);

    /*!
    Directs the builder to create output for the contents of the single @p
    tableCell. The tableCell is in the \a table.
  */
    virtual void processTableCell(const QTextTableCell &tableCell, QTextTable *table);

protected:
    /*!
    Processes the document between \a begin and \a end
  */
    void processDocumentContents(QTextFrame::iterator begin, const QTextFrame::iterator &end);

    /*!
    Iterates the iterator \a it to the first block after \a blockGroup. @p
    _block is any block in the \a blockGroup.

    The return pair is the iterator pointing after the end of \a blockGroup
    and the first block after \a blockGroup.
  */
    [[nodiscard]] QPair<QTextFrame::iterator, QTextBlock> skipBlockGroup(QTextFrame::iterator it, const QTextBlock &_block, QTextBlockGroup *blockGroup);

    /*!
    Returns a list of tags contained in \a openingTags sorted so they can be
    opened in order and will be closed in the correct order.

    \a openingTags should be a set of tags opened at the fragment pointed to
    by \a it.
  */
    [[nodiscard]] QList<int> sortOpeningOrder(QSet<int> openingTags, QTextBlock::iterator it) const;

    /*!
    Directs the builder to close the appropriate tags at the position of @p
    it.
  */
    virtual void processClosingElements(const QTextBlock::iterator &it);

    /*!
    Directs the builder to open the appropriate tags at the position of \a it.
  */
    virtual void processOpeningElements(const QTextBlock::iterator &it);

    /*!
    Returns the tags that should be closed at the position of \a it.
  */
    [[nodiscard]] virtual QSet<int> getElementsToClose(const QTextBlock::iterator &it) const;

    /*!
    Returns the tags that should be opened at the position of \a it.
  */
    [[nodiscard]] virtual QList<int> getElementsToOpen(const QTextBlock::iterator &it);

    /*!
    Flags for the tags that may be open.
  */
    enum OpenElementValues {
        None = 0x0, /// No tags are open
        SuperScript = 0x01, /// A superscript tag is open
        SubScript = 0x02, /// A subscript tag is open
        Anchor = 0x04, /// An anchor tag is open
        SpanForeground = 0x08, /// A foreground altering span tag is open.
        SpanBackground = 0x10, /// A background altering span tag is open.
        SpanFontFamily = 0x20, /// A font family altering span tag is open.
        SpanFontPointSize = 0x40, /// A font size altering span tag is open.
        Strong = 0x80, /// A strong tag is open.
        Emph = 0x100, /// A emphasis tag is open.
        Underline = 0x200, /// An underline tag is open.
        StrikeOut = 0x400 /// A strikeout tag is open.
    };

protected:
#ifndef Q_QDOC
    MarkupDirectorPrivate *const d_ptr;
#endif

    /*!
    The builder this MarkupDirector is operating on. This is available when
    subclassing to customize behaviour.
  */
    KPIMTextEdit::AbstractMarkupBuilder *const m_builder;

#ifndef Q_QDOC
private:
    Q_DECLARE_PRIVATE(MarkupDirector)
#endif
};
}
