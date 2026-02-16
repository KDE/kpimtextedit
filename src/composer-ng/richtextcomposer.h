/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <TextCustomEditor/RichTextEditor>
class KActionCollection;

namespace KPIMTextEdit
{
class RichTextComposerControler;
class RichTextComposerActions;
class RichTextExternalComposer;
class RichTextComposerEmailQuoteHighlighter;
/*!
 * \class KPIMTextEdit::RichTextComposer
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposer
 *
 * \brief The RichTextComposer class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposer : public TextCustomEditor::RichTextEditor
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposer with the given \a parent.
     */
    explicit RichTextComposer(QWidget *parent = nullptr);
    /*!
     * Destroys the RichTextComposer.
     */
    ~RichTextComposer() override;

    enum Mode {
        Plain, ///< Plain text mode
        Rich ///< Rich text mode
    };

    /*!
     * Returns The current text mode
     */
    [[nodiscard]] Mode textMode() const;

    /*!
     * Enables word wrap. Words will be wrapped at the specified column.
     *
     * \a wrapColumn the column where words will be wrapped
     */
    void enableWordWrap(int wrapColumn);

    /*!
     * Disables word wrap.
     * Note that words are still wrapped at the end of the editor; no scrollbar
     * will appear.
     */
    void disableWordWrap();

    /*!
     * Returns the line number where the cursor is. This takes word-wrapping
     *         into account. Line numbers start at 0.
     */
    [[nodiscard]] int linePosition() const;

    /*!
     * Returns the column number where the cursor is.
     */
    [[nodiscard]] int columnNumber() const;

    /*!
     * Forces plain text markup style based on the \a force parameter.
     */
    void forcePlainTextMarkup(bool force);

    /*!
     * Activates rich text mode.
     */
    void activateRichText();
    /*!
     * Switches to plain text mode.
     */
    void switchToPlainText();

    /*!
     * Sets the text content from the given \a text, interpreting it as either plain text or HTML.
     */
    void setTextOrHtml(const QString &text);
    /*!
     * Returns the text content as either plain text or HTML based on the current mode.
     */
    [[nodiscard]] QString textOrHtml() const;

    /*!
     * Sets the highlighter colors using the given email quote \a highlighter.
     * This is a virtual method that can be overridden by subclasses.
     */
    virtual void setHighlighterColors(KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *highlighter);

    /*!
     * Enables or disables the use of an external editor based on the \a use parameter.
     */
    void setUseExternalEditor(bool use);
    /*!
     * Sets the path to the external editor executable.
     */
    void setExternalEditorPath(const QString &path);
    /*!
     * Checks if the external editor process has finished.
     * Returns true if the editor has finished, false otherwise.
     */
    [[nodiscard]] bool checkExternalEditorFinished();
    /*!
     * Terminates the external editor process.
     */
    void killExternalEditor();

    /*!
     * Applies smart quoting to the given \a msg.
     * This is a virtual method that can be overridden by subclasses.
     */
    virtual QString smartQuote(const QString &msg);

    /*!
     * Sets the quote prefix name used for identifying quoted text.
     */
    void setQuotePrefixName(const QString &quotePrefix);
    /*!
     * Returns the current quote prefix name.
     */
    [[nodiscard]] QString quotePrefixName() const;

    /*!
     * Sets the cursor position to \a pos characters from the start of the document.
     */
    void setCursorPositionFromStart(unsigned int pos);
    /*!
     * Calculates and returns the quote level of the given \a line.
     * If \a oneQuote is true, only single quotes are counted.
     */
    [[nodiscard]] int quoteLength(const QString &line, bool oneQuote = false) const;
    /*!
     * Returns true if the given \a line is quoted.
     */
    [[nodiscard]] bool isLineQuoted(const QString &line) const;
    /*!
     * Returns the default quote sign for this composer.
     */
    const QString defaultQuoteSign() const;
    /*!
     * Creates actions and adds them to the given \a ac action collection.
     */
    void createActions(KActionCollection *ac);

    /*!
     * Returns the list of rich text actions.
     */
    [[nodiscard]] QList<QAction *> richTextActionList() const;
    /*!
     * Enables or disables actions based on the \a state parameter.
     */
    void setEnableActions(bool state);

    /*!
     * Returns the composer controller.
     */
    KPIMTextEdit::RichTextComposerControler *composerControler() const;
    /*!
     * Returns the external composer for handling external editor operations.
     */
    KPIMTextEdit::RichTextExternalComposer *externalComposer() const;
    /*!
     * Returns the composer actions manager.
     */
    KPIMTextEdit::RichTextComposerActions *composerActions() const;
    /*!
     * Creates a new highlighter for email quotes.
     */
    void createHighlighter() override;
    /*!
     * Processes a modify text key event. This is a virtual method that can be overridden by subclasses.
     * Returns true if the event was processed, false otherwise.
     */
    [[nodiscard]] virtual bool processModifyText(QKeyEvent *event);

public Q_SLOTS:
    /*!
     * Inserts plain text from the clipboard.
     */
    void insertPlainTextImplementation();
    /*!
     * Changes the insert mode between insert and overwrite.
     */
    void slotChangeInsertMode();

Q_SIGNALS:
    /*!
     * Emitted when the insert mode changes.
     */
    void insertModeChanged();
    /*!
     * Emitted whenever the text mode is changed.
     *
     * \a mode The new text mode
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    /*!
     * Emitted when the user uses the up arrow in the first line. The application
     * should then put the focus on the widget above the text edit.
     */
    void focusUp();

    /*!
     * Emitted when the external editor is started.
     */
    void externalEditorStarted();
    /*!
     * Emitted when the external editor is closed.
     */
    void externalEditorClosed();
    /*!
     * Emitted when an emoticon is inserted.
     */
    void insertEmoticon(const QString &str);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    Sonnet::SpellCheckDecorator *createSpellCheckDecorator() override;
    void insertFromMimeData(const QMimeData *source) override;
    bool canInsertFromMimeData(const QMimeData *source) const override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void clearDecorator() override;
    void updateHighLighter() override;
    bool processKeyEvent(QKeyEvent *e);

private:
    KPIMTEXTEDIT_NO_EXPORT void slotTextModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    KPIMTEXTEDIT_NO_EXPORT void evaluateListSupport(QKeyEvent *event);
    KPIMTEXTEDIT_NO_EXPORT void evaluateReturnKeySupport(QKeyEvent *event);
    class RichTextComposerPrivate;
    std::unique_ptr<RichTextComposerPrivate> const d;
};
}
