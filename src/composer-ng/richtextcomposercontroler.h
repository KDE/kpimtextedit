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
     */
    explicit RichTextComposerControler(RichTextComposer *richtextComposer, QObject *parent = nullptr);
    /*!
     */
    ~RichTextComposerControler() override;

    /*!
     */
    [[nodiscard]] RichTextComposer *richTextComposer() const;

    /*!
     */
    [[nodiscard]] QString currentLinkUrl() const;

    /*!
     */
    [[nodiscard]] QString currentLinkText() const;
    /*!
     */
    void selectLinkText() const;
    /*!
     */
    [[nodiscard]] QString toCleanHtml() const;

    /*!
     */
    [[nodiscard]] bool canIndentList() const;
    /*!
     */
    [[nodiscard]] bool canDedentList() const;

    /*!
     */
    [[nodiscard]] NestedListHelper *nestedListHelper() const;
    /*!
     */
    void insertLink(const QString &url);
    /*!
     */
    void setCursorPositionFromStart(unsigned int pos);
    /*!
     */
    void ensureCursorVisible();

    /*!
     */
    RichTextComposerImages *composerImages() const;
    /*!
     */
    [[nodiscard]] bool painterActive() const;
    /*!
     */
    void disablePainter();
    /*!
     */
    [[nodiscard]] bool isFormattingUsed() const;

    /*!
     */
    void setFontForWholeText(const QFont &font);
    /*!
     */
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    /*!
     */
    void updateLink(const QString &linkUrl, const QString &linkText);
    /*!
     */
    [[nodiscard]] QString toWrappedPlainText(QTextDocument *doc) const;
    /*!
     */
    [[nodiscard]] QString toWrappedPlainText() const;
    /*!
     */
    [[nodiscard]] QString toCleanPlainText(const QString &plainText = QString()) const;
    /*!
     */
    void addQuotes(const QString &defaultQuote);
public Q_SLOTS:
    /*!
     */
    void addCheckbox(bool add = true);
    /*!
     */
    void insertHorizontalRule();
    /*!
     */
    void setHeadingLevel(int level);
    /*!
     */
    void alignLeft();
    /*!
     */
    void alignCenter();
    /*!
     */
    void alignRight();
    /*!
     */
    void alignJustify();
    /*!
     */
    void makeRightToLeft();
    /*!
     */
    void makeLeftToRight();
    /*!
     */
    void setTextBold(bool bold);
    /*!
     */
    void setTextItalic(bool italic);
    /*!
     */
    void setTextUnderline(bool underline);
    /*!
     */
    void setTextStrikeOut(bool strikeOut);
    /*!
     */
    void setTextForegroundColor(const QColor &color);
    /*!
     */
    void setTextBackgroundColor(const QColor &color);
    /*!
     */
    void setFontFamily(const QString &fontFamily);
    /*!
     */
    void setFontSize(int size);
    /*!
     */
    void setFont(const QFont &font);
    /*!
     */
    void setTextSuperScript(bool superscript);
    /*!
     */
    void setTextSubScript(bool subscript);
    /*!
     */
    void setChangeTextForegroundColor();
    /*!
     */
    void setChangeTextBackgroundColor();
    /*!
     */
    void manageLink();
    /*!
     */
    void indentListMore();
    /*!
     */
    void indentListLess();
    /*!
     */
    void setListStyle(int styleIndex);
    /*!
     */
    void slotAddEmoticon(const QString &text);
    /*!
     */
    void slotInsertHtml();
    /*!
     */
    void slotFormatReset();
    /*!
     */
    void slotPasteWithoutFormatting();
    /*!
     */
    void slotPasteAsQuotation();
    /*!
     */
    void slotRemoveQuotes();
    /*!
     */
    void slotAddQuotes();
    /*!
     */
    void slotAddImage();
    /*!
     */
    void slotFormatPainter(bool active);
    /*!
     */
    void ensureCursorVisibleDelayed();

private:
    class RichTextComposerControllerPrivate;
    std::unique_ptr<RichTextComposerControllerPrivate> const d;
};
}
