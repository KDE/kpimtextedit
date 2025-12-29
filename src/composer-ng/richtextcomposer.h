/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

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
 * \inheaderfile KPIMTextEdit/RichTextComposer
 *
 * \brief The RichTextComposer class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposer : public TextCustomEditor::RichTextEditor
{
    Q_OBJECT
public:
    explicit RichTextComposer(QWidget *parent = nullptr);
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

    void forcePlainTextMarkup(bool force);

    void activateRichText();
    void switchToPlainText();

    void setTextOrHtml(const QString &text);
    [[nodiscard]] QString textOrHtml() const;

    virtual void setHighlighterColors(KPIMTextEdit::RichTextComposerEmailQuoteHighlighter *highlighter);

    void setUseExternalEditor(bool use);
    void setExternalEditorPath(const QString &path);
    [[nodiscard]] bool checkExternalEditorFinished();
    void killExternalEditor();

    // Redefine it for each apps
    virtual QString smartQuote(const QString &msg); // need by kmail

    void setQuotePrefixName(const QString &quotePrefix);
    [[nodiscard]] QString quotePrefixName() const;

    void setCursorPositionFromStart(unsigned int pos);
    [[nodiscard]] int quoteLength(const QString &line, bool oneQuote = false) const;
    [[nodiscard]] bool isLineQuoted(const QString &line) const;
    const QString defaultQuoteSign() const;
    void createActions(KActionCollection *ac);

    [[nodiscard]] QList<QAction *> richTextActionList() const;
    void setEnableActions(bool state);

    KPIMTextEdit::RichTextComposerControler *composerControler() const;
    KPIMTextEdit::RichTextExternalComposer *externalComposer() const;
    KPIMTextEdit::RichTextComposerActions *composerActions() const;
    void createHighlighter() override;
    [[nodiscard]] virtual bool processModifyText(QKeyEvent *event);

public Q_SLOTS:
    void insertPlainTextImplementation();
    void slotChangeInsertMode();

Q_SIGNALS:
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

    void externalEditorStarted();
    void externalEditorClosed();
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
