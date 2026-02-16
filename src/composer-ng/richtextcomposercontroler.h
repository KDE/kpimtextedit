/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include "richtextcomposer.h"
#include <QObject>
#include <memory>
namespace KPIMTextEdit
{
class RichTextComposer;
class NestedListHelper;
class RichTextComposerImages;
/*!
 * \class KPIMTextEdit::RichTextComposerControler
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerControler
 *
 * \brief The RichTextComposerControler class
 * \author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerControler : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposerControler with the given \a richtextComposer and \a parent.
     */
    explicit RichTextComposerControler(RichTextComposer *richtextComposer, QObject *parent = nullptr);
    /*!
     * Destroys the RichTextComposerControler.
     */
    ~RichTextComposerControler() override;

    /*!
     * Returns the RichTextComposer managed by this controller.
     */
    [[nodiscard]] RichTextComposer *richTextComposer() const;

    /*!
     * Returns the URL of the current link at the cursor position.
     */
    [[nodiscard]] QString currentLinkUrl() const;

    /*!
     * Returns the text of the current link at the cursor position.
     */
    [[nodiscard]] QString currentLinkText() const;
    /*!
     * Selects the text of the current link.
     */
    void selectLinkText() const;
    /*!
     * Returns the HTML content as a clean string.
     */
    [[nodiscard]] QString toCleanHtml() const;

    /*!
     * Returns true if the current list can be indented further.
     */
    [[nodiscard]] bool canIndentList() const;
    /*!
     * Returns true if the current list can be dedented.
     */
    [[nodiscard]] bool canDedentList() const;

    /*!
     * Returns the helper for managing nested lists.
     */
    [[nodiscard]] NestedListHelper *nestedListHelper() const;
    /*!
     * Inserts a link with the given \a url at the cursor position.
     */
    void insertLink(const QString &url);
    /*!
     * Sets the cursor position to \a pos characters from the start of the document.
     */
    void setCursorPositionFromStart(unsigned int pos);
    /*!
     * Ensures the cursor is visible by scrolling if necessary.
     */
    void ensureCursorVisible();

    /*!
     * Returns the composer images helper.
     */
    RichTextComposerImages *composerImages() const;
    /*!
     * Returns true if the painter is currently active.
     */
    [[nodiscard]] bool painterActive() const;
    /*!
     * Disables the painter.
     */
    void disablePainter();
    /*!
     * Returns true if formatting has been used in the text.
     */
    [[nodiscard]] bool isFormattingUsed() const;

    /*!
     * Sets the font for the entire text content.
     */
    void setFontForWholeText(const QFont &font);
    /*!
     * This signal is emitted when the text mode changes.
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    /*!
     * Updates the link at the cursor position with the given \a linkUrl and \a linkText.
     */
    void updateLink(const QString &linkUrl, const QString &linkText);
    /*!
     * Returns the plain text wrapped according to the document's settings.
     */
    [[nodiscard]] QString toWrappedPlainText(QTextDocument *doc) const;
    /*!
     * Returns the wrapped plain text of the current document.
     */
    [[nodiscard]] QString toWrappedPlainText() const;
    /*!
     * Returns clean plain text, optionally from the given \a plainText.
     */
    [[nodiscard]] QString toCleanPlainText(const QString &plainText = QString()) const;
    /*!
     * Adds quotes to the given \a defaultQuote.
     */
    void addQuotes(const QString &defaultQuote);
public Q_SLOTS:
    /*!
     * Adds or removes a checkbox from the current line based on the \a add parameter.
     */
    void addCheckbox(bool add = true);
    /*!
     * Inserts a horizontal rule at the cursor position.
     */
    void insertHorizontalRule();
    /*!
     * Sets the heading level to the given \a level.
     */
    void setHeadingLevel(int level);
    /*!
     * Aligns the current text or selection to the left.
     */
    void alignLeft();
    /*!
     * Centers the current text or selection.
     */
    void alignCenter();
    /*!
     * Aligns the current text or selection to the right.
     */
    void alignRight();
    /*!
     * Justifies the current text or selection.
     */
    void alignJustify();
    /*!
     * Sets the text direction to right-to-left.
     */
    void makeRightToLeft();
    /*!
     * Sets the text direction to left-to-right.
     */
    void makeLeftToRight();
    /*!
     * Sets or removes bold formatting based on the \a bold parameter.
     */
    void setTextBold(bool bold);
    /*!
     * Sets or removes italic formatting based on the \a italic parameter.
     */
    void setTextItalic(bool italic);
    /*!
     * Sets or removes underline formatting based on the \a underline parameter.
     */
    void setTextUnderline(bool underline);
    /*!
     * Sets or removes strikethrough formatting based on the \a strikeOut parameter.
     */
    void setTextStrikeOut(bool strikeOut);
    /*!
     * Sets the text foreground color to the given \a color.
     */
    void setTextForegroundColor(const QColor &color);
    /*!
     * Sets the text background color to the given \a color.
     */
    void setTextBackgroundColor(const QColor &color);
    /*!
     * Sets the font family to the given \a fontFamily.
     */
    void setFontFamily(const QString &fontFamily);
    /*!
     * Sets the font size to the given \a size.
     */
    void setFontSize(int size);
    /*!
     * Sets the font to the given \a font.
     */
    void setFont(const QFont &font);
    /*!
     * Sets or removes superscript formatting based on the \a superscript parameter.
     */
    void setTextSuperScript(bool superscript);
    /*!
     * Sets or removes subscript formatting based on the \a subscript parameter.
     */
    void setTextSubScript(bool subscript);
    /*!
     * Opens a dialog to change the text foreground color.
     */
    void setChangeTextForegroundColor();
    /*!
     * Opens a dialog to change the text background color.
     */
    void setChangeTextBackgroundColor();
    /*!
     * Opens a dialog to manage links.
     */
    void manageLink();
    /*!
     * Increases the indentation of the current list.
     */
    void indentListMore();
    /*!
     * Decreases the indentation of the current list.
     */
    void indentListLess();
    /*!
     * Sets the list style based on the given \a styleIndex.
     */
    void setListStyle(int styleIndex);
    /*!
     * Inserts an emoticon from the given \a text.
     */
    void slotAddEmoticon(const QString &text);
    /*!
     * Opens a dialog to insert HTML.
     */
    void slotInsertHtml();
    /*!
     * Resets all formatting in the current selection.
     */
    void slotFormatReset();
    /*!
     * Pastes content without formatting.
     */
    void slotPasteWithoutFormatting();
    /*!
     * Pastes content as a quotation.
     */
    void slotPasteAsQuotation();
    /*!
     * Removes quotation formatting from the current text.
     */
    void slotRemoveQuotes();
    /*!
     * Adds quotation formatting to the current text.
     */
    void slotAddQuotes();
    /*!
     * Opens a dialog to add an image.
     */
    void slotAddImage();
    /*!
     * Activates or deactivates the format painter based on the \a active parameter.
     */
    void slotFormatPainter(bool active);
    /*!
     * Ensures the cursor is visible with a delay.
     */
    void ensureCursorVisibleDelayed();

private:
    class RichTextComposerControllerPrivate;
    std::unique_ptr<RichTextComposerControllerPrivate> const d;
};
}
