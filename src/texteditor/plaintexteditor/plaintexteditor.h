/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

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
public:
    explicit PlainTextEditor(QWidget *parent = nullptr);
    ~PlainTextEditor() override;

    enum SupportFeature { None = 0, Search = 1, SpellChecking = 2, TextToSpeech = 4, AllowWebShortcut = 8 };
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
    bool checkSpellingEnabled() const;
    void setCheckSpellingEnabled(bool check);
    void setSpellCheckingConfigFileName(const QString &_fileName);
    Q_REQUIRED_RESULT QString spellCheckingConfigFileName() const;
    const QString &spellCheckingLanguage() const;
    void setSpellCheckingLanguage(const QString &_language);
public Q_SLOTS:
    void slotDisplayMessageIndicator(const QString &message);
    void slotCheckSpelling();
    void slotSpeakText();
    void slotZoomReset();
private Q_SLOTS:
    void slotUndoableClear();
    void slotSpellCheckerMisspelling(const QString &text, int pos);
    void slotSpellCheckerCorrected(const QString &, int, const QString &);
    void slotSpellCheckerAutoCorrect(const QString &, const QString &);
    void slotSpellCheckerCanceled();
    void slotSpellCheckerFinished();

    void slotLanguageSelected();
    void slotToggleAutoSpellCheck();

protected:
    virtual void addExtraMenuEntry(QMenu *menu, QPoint pos);

protected:
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
    void addIgnoreWordsToHighLighter();
    void deleteWordBack();
    void deleteWordForward();
    void highlightWord(int length, int pos);
    void deleteEndOfLine();
    void moveLineUpDown(bool moveUp);
    void moveCursorBeginUpDown(bool moveUp);
    class PlainTextEditorPrivate;
    PlainTextEditorPrivate *const d;
};
}
