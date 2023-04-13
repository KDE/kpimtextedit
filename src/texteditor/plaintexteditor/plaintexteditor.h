/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <QPlainTextEdit>

namespace Sonnet
{
class Highlighter;
class SpellCheckDecorator;
}
namespace KPIMTextEdit
{
/**
 * @brief The PlainTextEditor class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT PlainTextEditor : public QPlainTextEdit
{
    Q_OBJECT
    Q_PROPERTY(bool searchSupport READ searchSupport WRITE setSearchSupport)
    Q_PROPERTY(bool spellCheckingSupport READ spellCheckingSupport WRITE setSpellCheckingSupport)
    Q_PROPERTY(bool textToSpeechSupport READ textToSpeechSupport WRITE setTextToSpeechSupport)
    Q_PROPERTY(bool webShortcutSupport READ webShortcutSupport WRITE setWebShortcutSupport)
    Q_PROPERTY(bool emojiSupport READ emojiSupport WRITE setEmojiSupport)
public:
    explicit PlainTextEditor(QWidget *parent = nullptr);
    ~PlainTextEditor() override;

    enum SupportFeature {
        None = 0,
        Search = 1,
        SpellChecking = 2,
        TextToSpeech = 4,
        AllowWebShortcut = 8,
        Emoji = 16,
    };
    Q_DECLARE_FLAGS(SupportFeatures, SupportFeature)

    void setSearchSupport(bool b);
    Q_REQUIRED_RESULT bool searchSupport() const;

    Q_REQUIRED_RESULT bool spellCheckingSupport() const;
    void setSpellCheckingSupport(bool check);

    virtual void setReadOnly(bool readOnly);

    void setTextToSpeechSupport(bool b);
    Q_REQUIRED_RESULT bool textToSpeechSupport() const;

    void setWebShortcutSupport(bool b);
    Q_REQUIRED_RESULT bool webShortcutSupport() const;

    virtual void createHighlighter();

    void addIgnoreWords(const QStringList &lst);
    Q_REQUIRED_RESULT bool activateLanguageMenu() const;
    void setActivateLanguageMenu(bool activate);
    Q_REQUIRED_RESULT Sonnet::Highlighter *highlighter() const;
    Q_REQUIRED_RESULT bool checkSpellingEnabled() const;
    void setCheckSpellingEnabled(bool check);
    void setSpellCheckingConfigFileName(const QString &_fileName);
    Q_REQUIRED_RESULT QString spellCheckingConfigFileName() const;
    Q_REQUIRED_RESULT const QString &spellCheckingLanguage() const;
    void setSpellCheckingLanguage(const QString &_language);

    void setEmojiSupport(bool b);
    Q_REQUIRED_RESULT bool emojiSupport() const;

public Q_SLOTS:
    void slotDisplayMessageIndicator(const QString &message);
    void slotCheckSpelling();
    void slotSpeakText();
    void slotZoomReset();

protected:
    virtual void addExtraMenuEntry(QMenu *menu, QPoint pos);
    void contextMenuEvent(QContextMenuEvent *event) override;
    Q_REQUIRED_RESULT bool event(QEvent *ev) override;
    void keyPressEvent(QKeyEvent *event) override;
    Q_REQUIRED_RESULT bool overrideShortcut(QKeyEvent *event);
    Q_REQUIRED_RESULT bool handleShortcut(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event) override;

    virtual Sonnet::SpellCheckDecorator *createSpellCheckDecorator();
    void setHighlighter(Sonnet::Highlighter *_highLighter);
    void focusInEvent(QFocusEvent *event) override;
    virtual void updateHighLighter();
    virtual void clearDecorator();

Q_SIGNALS:
    void findText();
    void replaceText();
    void spellCheckerAutoCorrect(const QString &currentWord, const QString &autoCorrectWord);
    void checkSpellingChanged(bool);
    void languageChanged(const QString &);
    void spellCheckStatus(const QString &);
    void say(const QString &text);

private:
    KPIMTEXTEDIT_NO_EXPORT void slotUndoableClear();
    KPIMTEXTEDIT_NO_EXPORT void slotSpellCheckerMisspelling(const QString &text, int pos);
    KPIMTEXTEDIT_NO_EXPORT void slotSpellCheckerCorrected(const QString &, int, const QString &);
    KPIMTEXTEDIT_NO_EXPORT void slotSpellCheckerAutoCorrect(const QString &, const QString &);
    KPIMTEXTEDIT_NO_EXPORT void slotSpellCheckerCanceled();
    KPIMTEXTEDIT_NO_EXPORT void slotSpellCheckerFinished();

    KPIMTEXTEDIT_NO_EXPORT void slotLanguageSelected();
    KPIMTEXTEDIT_NO_EXPORT void slotToggleAutoSpellCheck();
    KPIMTEXTEDIT_NO_EXPORT void addIgnoreWordsToHighLighter();
    KPIMTEXTEDIT_NO_EXPORT void deleteWordBack();
    KPIMTEXTEDIT_NO_EXPORT void deleteWordForward();
    KPIMTEXTEDIT_NO_EXPORT void highlightWord(int length, int pos);
    KPIMTEXTEDIT_NO_EXPORT void deleteEndOfLine();
    KPIMTEXTEDIT_NO_EXPORT void moveLineUpDown(bool moveUp);
    KPIMTEXTEDIT_NO_EXPORT void moveCursorBeginUpDown(bool moveUp);
    KPIMTEXTEDIT_NO_EXPORT void regenerateColorScheme();
    KPIMTEXTEDIT_NO_EXPORT void updateReadOnlyColor();
    KPIMTEXTEDIT_NO_EXPORT void slotInsertEmoticon(const QString &str);
    class PlainTextEditorPrivate;
    std::unique_ptr<PlainTextEditorPrivate> const d;
};
}
