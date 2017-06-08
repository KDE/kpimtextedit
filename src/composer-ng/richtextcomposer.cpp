/*
   Copyright (c) 2015-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "richtextcomposer.h"
#include "richtextcomposercontroler.h"
#include "richtextcomposeractions.h"
#include "richtextcomposerimages.h"
#include "richtextcomposeremailquotehighlighter.h"
#include "nestedlisthelper_p.h"
#include "richtextexternalcomposer.h"
#include <KLocalizedString>
#include <QTextBlock>
#include <QTextLayout>
#include <QApplication>
#include <QClipboard>

#include "richtextcomposeremailquotedecorator.h"

#include <grantlee/plaintextmarkupbuilder.h>

#include <KActionCollection>
#include <QAction>
#include <QFileInfo>
#include <QMimeData>
#include <QDebug>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposer::RichTextComposerPrivate
{
public:
    RichTextComposerPrivate(RichTextComposer *qq)
        : q(qq)
        , forcePlainTextMarkup(false)
        , mode(RichTextComposer::Plain)
    {
        composerControler = new RichTextComposerControler(q, q);
        richTextComposerActions = new RichTextComposerActions(composerControler, q);
        externalComposer = new KPIMTextEdit::RichTextExternalComposer(q);
        q->connect(externalComposer, &RichTextExternalComposer::externalEditorClosed, qq, &RichTextComposer::externalEditorClosed);
        q->connect(externalComposer, &RichTextExternalComposer::externalEditorStarted, qq, &RichTextComposer::externalEditorStarted);
        q->connect(q, &RichTextComposer::textModeChanged, q, &RichTextComposer::slotTextModeChanged);
    }

    QString quotePrefix;
    RichTextComposerControler *composerControler;
    RichTextComposerActions *richTextComposerActions;
    KPIMTextEdit::RichTextExternalComposer *externalComposer;
    RichTextComposer *q;
    bool forcePlainTextMarkup;
    RichTextComposer::Mode mode;
};

RichTextComposer::RichTextComposer(QWidget *parent)
    : KPIMTextEdit::RichTextEditor(parent)
    , d(new RichTextComposerPrivate(this))
{
    setAcceptRichText(false);
}

RichTextComposer::~RichTextComposer()
{
    delete d;
}

KPIMTextEdit::RichTextExternalComposer *RichTextComposer::externalComposer() const
{
    return d->externalComposer;
}

KPIMTextEdit::RichTextComposerControler *RichTextComposer::composerControler() const
{
    return d->composerControler;
}

KPIMTextEdit::RichTextComposerActions *RichTextComposer::composerActions() const
{
    return d->richTextComposerActions;
}

QList<QAction *> RichTextComposer::richTextActionList() const
{
    return d->richTextComposerActions->richTextActionList();
}

void RichTextComposer::setEnableActions(bool state)
{
    Q_FOREACH (QAction *act, richTextActionList()) {
        act->setEnabled(state);
    }
}

void RichTextComposer::createActions(KActionCollection *ac)
{
    d->richTextComposerActions->createActions(ac);
}

void RichTextComposer::updateHighLighter()
{
    KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *hlighter = qobject_cast<KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *>(highlighter());
    if (hlighter) {
        hlighter->toggleSpellHighlighting(checkSpellingEnabled());
    }
}

void RichTextComposer::clearDecorator()
{
    //Nothing
}

void RichTextComposer::createHighlighter()
{
    KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *highlighter = new KPIMTextEdit::RichTextComposerEmailQuoteHighlighter(this);
    highlighter->toggleSpellHighlighting(checkSpellingEnabled());
    setHighlighterColors(highlighter);
    setHighlighter(highlighter);
}

void RichTextComposer::setHighlighterColors(KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *highlighter)
{
    Q_UNUSED(highlighter);
}

void RichTextComposer::setUseExternalEditor(bool use)
{
    d->externalComposer->setUseExternalEditor(use);
}

void RichTextComposer::setExternalEditorPath(const QString &path)
{
    d->externalComposer->setExternalEditorPath(path);
}

bool RichTextComposer::checkExternalEditorFinished()
{
    return d->externalComposer->checkExternalEditorFinished();
}

void RichTextComposer::killExternalEditor()
{
    d->externalComposer->killExternalEditor();
}

RichTextComposer::Mode RichTextComposer::textMode() const
{
    return d->mode;
}

void RichTextComposer::enableWordWrap(int wrapColumn)
{
    setWordWrapMode(QTextOption::WordWrap);
    setLineWrapMode(QTextEdit::FixedColumnWidth);
    setLineWrapColumnOrWidth(wrapColumn);
}

void RichTextComposer::disableWordWrap()
{
    setLineWrapMode(QTextEdit::WidgetWidth);
}

int RichTextComposer::linePosition() const
{
    const QTextCursor cursor = textCursor();
    const QTextDocument *doc = document();
    QTextBlock block = doc->begin();
    int lineCount = 0;

    // Simply using cursor.block.blockNumber() would not work since that does not
    // take word-wrapping into account, i.e. it is possible to have more than one
    // line in a block.
    //
    // What we have to do therefore is to iterate over the blocks and count the
    // lines in them. Once we have reached the block where the cursor is, we have
    // to iterate over each line in it, to find the exact line in the block where
    // the cursor is.
    while (block.isValid()) {
        const QTextLayout *layout = block.layout();

        // If the current block has the cursor in it, iterate over all its lines
        if (block == cursor.block()) {
            // Special case: Cursor at end of single non-wrapped line, exit early
            // in this case as the logic below can't handle it
            if (block.lineCount() == layout->lineCount()) {
                return lineCount;
            }

            const int cursorBasePosition = cursor.position() - block.position();
            const int numberOfLine(layout->lineCount());
            for (int i = 0; i < numberOfLine; ++i) {
                QTextLine line = layout->lineAt(i);
                if (cursorBasePosition >= line.textStart()
                    && cursorBasePosition < line.textStart() + line.textLength()) {
                    break;
                }
                lineCount++;
            }
            return lineCount;
        } else {
            // No, cursor is not in the current block
            lineCount += layout->lineCount();
        }

        block = block.next();
    }

    // Only gets here if the cursor block can't be found, shouldn't happen except
    // for an empty document maybe
    return lineCount;
}

int RichTextComposer::columnNumber() const
{
    const QTextCursor cursor = textCursor();
    return cursor.columnNumber();
}

void RichTextComposer::forcePlainTextMarkup(bool force)
{
    d->forcePlainTextMarkup = force;
}

void RichTextComposer::insertPlainTextImplementation()
{
    if (d->forcePlainTextMarkup) {
        Grantlee::PlainTextMarkupBuilder *pb = new Grantlee::PlainTextMarkupBuilder();

        Grantlee::MarkupDirector *pmd = new Grantlee::MarkupDirector(pb);
        pmd->processDocument(document());
        const QString plainText = pb->getResult();
        document()->setPlainText(plainText);
        delete pmd;
        delete pb;
    } else {
        document()->setPlainText(document()->toPlainText());
    }
}

void RichTextComposer::slotChangeInsertMode()
{
    setOverwriteMode(!overwriteMode());
    Q_EMIT insertModeChanged();
}

void RichTextComposer::activateRichText()
{
    if (d->mode == RichTextComposer::Plain) {
        setAcceptRichText(true);
        d->mode = RichTextComposer::Rich;
        Q_EMIT textModeChanged(d->mode);
    }
}

void RichTextComposer::switchToPlainText()
{
    if (d->mode == RichTextComposer::Rich) {
        d->mode = RichTextComposer::Plain;
        // TODO: Warn the user about this?
        insertPlainTextImplementation();
        setAcceptRichText(false);
        Q_EMIT textModeChanged(d->mode);
    }
}

QString RichTextComposer::textOrHtml() const
{
    if (textMode() == Rich) {
        return d->composerControler->toCleanHtml();
    } else {
        return toPlainText();
    }
}

void RichTextComposer::setTextOrHtml(const QString &text)
{
    // might be rich text
    if (Qt::mightBeRichText(text)) {
        if (d->mode == RichTextComposer::Plain) {
            activateRichText();
        }
        setHtml(text);
    } else {
        setPlainText(text);
    }
}

void RichTextComposer::evaluateReturnKeySupport(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = textCursor();
        int oldPos = cursor.position();
        int blockPos = cursor.block().position();

        //selection all the line.
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        QString lineText = cursor.selectedText();
        if (((oldPos - blockPos) > 0)
            && ((oldPos - blockPos) < int(lineText.length()))) {
            bool isQuotedLine = false;
            int bot = 0; // bot = begin of text after quote indicators
            while (bot < lineText.length()) {
                if ((lineText[bot] == QChar::fromLatin1('>'))
                    || (lineText[bot] == QChar::fromLatin1('|'))) {
                    isQuotedLine = true;
                    ++bot;
                } else if (lineText[bot].isSpace()) {
                    ++bot;
                } else {
                    break;
                }
            }
            evaluateListSupport(event);
            // duplicate quote indicators of the previous line before the new
            // line if the line actually contained text (apart from the quote
            // indicators) and the cursor is behind the quote indicators
            if (isQuotedLine
                && (bot != lineText.length())
                && ((oldPos - blockPos) >= int(bot))) {
                // The cursor position might have changed unpredictably if there was selected
                // text which got replaced by a new line, so we query it again:
                cursor.movePosition(QTextCursor::StartOfBlock);
                cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
                QString newLine = cursor.selectedText();

                // remove leading white space from the new line and instead
                // add the quote indicators of the previous line
                int leadingWhiteSpaceCount = 0;
                while ((leadingWhiteSpaceCount < newLine.length())
                       && newLine[leadingWhiteSpaceCount].isSpace()) {
                    ++leadingWhiteSpaceCount;
                }
                newLine = newLine.replace(0, leadingWhiteSpaceCount, lineText.left(bot));
                cursor.insertText(newLine);
                //cursor.setPosition( cursor.position() + 2 );
                cursor.movePosition(QTextCursor::StartOfBlock);
                setTextCursor(cursor);
            }
        } else {
            evaluateListSupport(event);
        }
    } else {
        evaluateListSupport(event);
    }
}

void RichTextComposer::evaluateListSupport(QKeyEvent *event)
{
    bool handled = false;
    if (textCursor().currentList()) {
        // handled is False if the key press event was not handled or not completely
        // handled by the Helper class.
        handled = d->composerControler->nestedListHelper()->handleBeforeKeyPressEvent(event);
    }

    if (!handled) {
        KPIMTextEdit::RichTextEditor::keyPressEvent(event);
    }

    if (textCursor().currentList()) {
        d->composerControler->nestedListHelper()->handleAfterKeyPressEvent(event);
    }
    Q_EMIT cursorPositionChanged();
}

bool RichTextComposer::processKeyEvent(QKeyEvent *e)
{
    if (d->externalComposer->useExternalEditor()
        && (e->key() != Qt::Key_Shift)
        && (e->key() != Qt::Key_Control)
        && (e->key() != Qt::Key_Meta)
        && (e->key() != Qt::Key_CapsLock)
        && (e->key() != Qt::Key_NumLock)
        && (e->key() != Qt::Key_ScrollLock)
        && (e->key() != Qt::Key_Alt)
        && (e->key() != Qt::Key_AltGr)) {
        if (!d->externalComposer->isInProgress()) {
            d->externalComposer->startExternalEditor();
        }
        return true;
    }

    if (e->key() == Qt::Key_Up && e->modifiers() != Qt::ShiftModifier
        && textCursor().block().position() == 0
        && textCursor().block().layout()->lineForTextPosition(textCursor().position()).lineNumber() == 0) {
        textCursor().clearSelection();
        Q_EMIT focusUp();
    } else if (e->key() == Qt::Key_Backtab && e->modifiers() == Qt::ShiftModifier) {
        textCursor().clearSelection();
        Q_EMIT focusUp();
    } else {
        if (!processAutoCorrection(e)) {
            evaluateReturnKeySupport(e);
        }
    }
    return true;
}

bool RichTextComposer::processAutoCorrection(QKeyEvent *event)
{
    Q_UNUSED(event);
    return false;
}

void RichTextComposer::keyPressEvent(QKeyEvent *e)
{
    processKeyEvent(e);
}

Sonnet::SpellCheckDecorator *RichTextComposer::createSpellCheckDecorator()
{
    return new KPIMTextEdit::RichTextComposerEmailQuoteDecorator(this);
}

QString RichTextComposer::smartQuote(const QString &msg)
{
    return msg;
}

void RichTextComposer::setQuotePrefixName(const QString &quotePrefix)
{
    d->quotePrefix = quotePrefix;
}

QString RichTextComposer::quotePrefixName() const
{
    if (!d->quotePrefix.simplified().isEmpty()) {
        return d->quotePrefix;
    } else {
        return QStringLiteral(">");
    }
}

int RichTextComposer::quoteLength(const QString &line, bool oneQuote) const
{
    if (!d->quotePrefix.simplified().isEmpty()) {
        if (line.startsWith(d->quotePrefix)) {
            return d->quotePrefix.length();
        } else {
            return 0;
        }
    } else {
        bool quoteFound = false;
        int startOfText = -1;
        const int lineLength(line.length());
        for (int i = 0; i < lineLength; ++i) {
            if (line[i] == QLatin1Char('>') || line[i] == QLatin1Char('|')) {
                if (quoteFound && oneQuote) {
                    break;
                }
                quoteFound = true;
            } else if (line[i] != QLatin1Char(' ')) {
                startOfText = i;
                break;
            }
        }
        if (quoteFound) {
            //We found a quote but it's just quote element => 1 => remove 1 char.
            if (startOfText == -1) {
                startOfText = 1;
            }
            return startOfText;
        } else {
            return 0;
        }
    }
}

void RichTextComposer::setCursorPositionFromStart(unsigned int pos)
{
    d->composerControler->setCursorPositionFromStart(pos);
}

bool RichTextComposer::isLineQuoted(const QString &line) const
{
    return quoteLength(line) > 0;
}

const QString RichTextComposer::defaultQuoteSign() const
{
    if (!d->quotePrefix.simplified().isEmpty()) {
        return d->quotePrefix;
    } else {
        return QStringLiteral("> ");
    }
}

void RichTextComposer::insertFromMimeData(const QMimeData *source)
{
    // Add an image if that is on the clipboard
    if (textMode() == RichTextComposer::Rich && source->hasImage()) {
        const QImage image = qvariant_cast<QImage>(source->imageData());
        QFileInfo fi;
        d->composerControler->composerImages()->insertImage(image, fi);
        return;
    }

    // Attempt to paste HTML contents into the text edit in plain text mode,
    // prevent this and prevent plain text instead.
    if (textMode() == RichTextComposer::Plain && source->hasHtml()) {
        if (source->hasText()) {
            insertPlainText(source->text());
            return;
        }
    }

    KPIMTextEdit::RichTextEditor::insertFromMimeData(source);
}

bool RichTextComposer::canInsertFromMimeData(const QMimeData *source) const
{
    if (source->hasHtml() && textMode() == RichTextComposer::Rich) {
        return true;
    }

    if (source->hasText()) {
        return true;
    }

    if (textMode() == RichTextComposer::Rich && source->hasImage()) {
        return true;
    }

    return KPIMTextEdit::RichTextEditor::canInsertFromMimeData(source);
}

void RichTextComposer::mouseReleaseEvent(QMouseEvent *event)
{
    if (d->composerControler->painterActive()) {
        d->composerControler->disablePainter();
        d->richTextComposerActions->uncheckActionFormatPainter();
    }
    KPIMTextEdit::RichTextEditor::mouseReleaseEvent(event);
}

void RichTextComposer::slotTextModeChanged(KPIMTextEdit::RichTextComposer::Mode mode)
{
    d->composerControler->textModeChanged(mode);
    d->richTextComposerActions->textModeChanged(mode);
}
