/*
   SPDX-FileCopyrightText: 2013-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QTextEdit>

class QContextMenuEvent;
class QMenu;
namespace Sonnet
{
class Highlighter;
class SpellCheckDecorator;
}
namespace KPIMTextEdit
{
/**
 * @brief The RichTextEditor class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT RichTextEditor : public QTextEdit
{
    Q_OBJECT
    Q_PROPERTY(bool searchSupport READ searchSupport WRITE setSearchSupport)
    Q_PROPERTY(bool spellCheckingSupport READ spellCheckingSupport WRITE setSpellCheckingSupport)
    Q_PROPERTY(bool textToSpeechSupport READ textToSpeechSupport WRITE setTextToSpeechSupport)
    Q_PROPERTY(bool activateLanguageMenu READ activateLanguageMenu WRITE setActivateLanguageMenu)
    Q_PROPERTY(bool tabSupport READ allowTabSupport WRITE setAllowTabSupport)
    Q_PROPERTY(bool webShortcutSupport READ webShortcutSupport WRITE setWebShortcutSupport)
public:
    explicit RichTextEditor(QWidget *parent = nullptr);
    ~RichTextEditor() override;
    enum SupportFeature { None = 0, Search = 1, SpellChecking = 2, TextToSpeech = 4, AllowTab = 8, AllowWebShortcut = 16 };
    Q_DECLARE_FLAGS(SupportFeatures, SupportFeature)

    void setSearchSupport(bool b);
    Q_REQUIRED_RESULT bool searchSupport() const;

    Q_REQUIRED_RESULT bool spellCheckingSupport() const;
    void setSpellCheckingSupport(bool check);

    void setSpellCheckingConfigFileName(const QString &_fileName);

    Q_REQUIRED_RESULT bool checkSpellingEnabled() const;
    void setCheckSpellingEnabled(bool check);

    void setSpellCheckingLanguage(const QString &_language);
    const QString &spellCheckingLanguage() const;

    virtual void setReadOnly(bool readOnly);
    virtual void createHighlighter();

    Q_REQUIRED_RESULT bool textToSpeechSupport() const;
    void setTextToSpeechSupport(bool b);
    Q_REQUIRED_RESULT Sonnet::Highlighter *highlighter() const;

    Q_REQUIRED_RESULT bool activateLanguageMenu() const;
    void setActivateLanguageMenu(bool activate);

    void setAllowTabSupport(bool b);
    Q_REQUIRED_RESULT bool allowTabSupport() const;

    void setShowAutoCorrectButton(bool b);
    Q_REQUIRED_RESULT bool showAutoCorrectButton() const;

    void forceSpellChecking();
    Q_REQUIRED_RESULT QString spellCheckingConfigFileName() const;

    void setWebShortcutSupport(bool b);
    Q_REQUIRED_RESULT bool webShortcutSupport() const;

    void addIgnoreWords(const QStringList &lst);

    // Reimplement it in subclass
    virtual void forceAutoCorrection(bool selectedText = false);

    void setDefaultFontSize(int val);
    int zoomFactor() const;
Q_SIGNALS:
    void say(const QString &text);

public Q_SLOTS:
    void slotDisplayMessageIndicator(const QString &message);
    void slotSpeakText();
    void slotCheckSpelling();
    void slotZoomReset();

protected:
    virtual void addExtraMenuEntry(QMenu *menu, QPoint pos);
    void contextMenuEvent(QContextMenuEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    bool event(QEvent *ev) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;

    QMenu *mousePopupMenu(QPoint pos);
    virtual Sonnet::SpellCheckDecorator *createSpellCheckDecorator();
    void setHighlighter(Sonnet::Highlighter *_highLighter);

    virtual void updateHighLighter();
    virtual void clearDecorator();
Q_SIGNALS:
    void findText();
    void replaceText();
    void spellCheckerAutoCorrect(const QString &currentWord, const QString &autoCorrectWord);
    void checkSpellingChanged(bool);
    void languageChanged(const QString &);
    void spellCheckStatus(const QString &);
    void spellCheckingFinished();
    void spellCheckingCanceled();

private:
    void slotUndoableClear();
    void slotSpellCheckerMisspelling(const QString &text, int pos);
    void slotSpellCheckerCorrected(const QString &, int, const QString &);
    void slotSpellCheckerAutoCorrect(const QString &, const QString &);
    void slotSpellCheckerCanceled();
    void slotSpellCheckerFinished();
    void slotToggleAutoSpellCheck();
    void slotLanguageSelected();
    void slotAllowTab();

    void addIgnoreWordsToHighLighter();
    bool handleShortcut(QKeyEvent *event);
    bool overrideShortcut(QKeyEvent *event);
    void deleteWordBack();
    void deleteWordForward();
    void highlightWord(int length, int pos);
    void checkSpelling(bool force);
    void moveLineUpDown(bool moveUp);
    void moveCursorBeginUpDown(bool moveUp);
    class RichTextEditorPrivate;
    RichTextEditorPrivate *const d;
};
}

