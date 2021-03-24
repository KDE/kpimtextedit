/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include "richtextcomposer.h"
#include <QObject>
namespace KPIMTextEdit
{
class RichTextComposer;
class NestedListHelper;
class RichTextComposerImages;
/**
 * @brief The RichTextComposerControler class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerControler : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerControler(RichTextComposer *richtextComposer, QObject *parent = nullptr);
    ~RichTextComposerControler() override;

    Q_REQUIRED_RESULT RichTextComposer *richTextComposer() const;

    Q_REQUIRED_RESULT QString currentLinkUrl() const;

    Q_REQUIRED_RESULT QString currentLinkText() const;
    void selectLinkText() const;
    Q_REQUIRED_RESULT QString toCleanHtml() const;

    Q_REQUIRED_RESULT bool canIndentList() const;
    Q_REQUIRED_RESULT bool canDedentList() const;

    Q_REQUIRED_RESULT NestedListHelper *nestedListHelper() const;
    void insertLink(const QString &url);
    void setCursorPositionFromStart(unsigned int pos);
    void ensureCursorVisible();

    RichTextComposerImages *composerImages() const;
    Q_REQUIRED_RESULT bool painterActive() const;
    void disablePainter();
    Q_REQUIRED_RESULT bool isFormattingUsed() const;

    void setFontForWholeText(const QFont &font);
    void textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode);
    void updateLink(const QString &linkUrl, const QString &linkText);
    Q_REQUIRED_RESULT QString toWrappedPlainText(QTextDocument *doc) const;
    Q_REQUIRED_RESULT QString toWrappedPlainText() const;
    Q_REQUIRED_RESULT QString toCleanPlainText(const QString &plainText = QString()) const;
    void addQuotes(const QString &defaultQuote);
public Q_SLOTS:
    void addCheckbox(bool add = true);
    void insertHorizontalRule();
    void setHeadingLevel(int level);
    void alignLeft();
    void alignCenter();
    void alignRight();
    void alignJustify();
    void makeRightToLeft();
    void makeLeftToRight();
    void setTextBold(bool bold);
    void setTextItalic(bool italic);
    void setTextUnderline(bool underline);
    void setTextStrikeOut(bool strikeOut);
    void setTextForegroundColor(const QColor &color);
    void setTextBackgroundColor(const QColor &color);
    void setFontFamily(const QString &fontFamily);
    void setFontSize(int size);
    void setFont(const QFont &font);
    void setTextSuperScript(bool superscript);
    void setTextSubScript(bool subscript);
    void setChangeTextForegroundColor();
    void setChangeTextBackgroundColor();
    void manageLink();
    void indentListMore();
    void indentListLess();
    void setListStyle(int styleIndex);
    void slotAddEmoticon(const QString &text);
    void slotInsertHtml();
    void slotFormatReset();
    void slotDeleteLine();
    void slotPasteWithoutFormatting();
    void slotPasteAsQuotation();
    void slotRemoveQuotes();
    void slotAddQuotes();
    void slotAddImage();
    void slotFormatPainter(bool active);
    void ensureCursorVisibleDelayed();

private:
    class RichTextComposerControlerPrivate;
    RichTextComposerControlerPrivate *const d;
};
}
