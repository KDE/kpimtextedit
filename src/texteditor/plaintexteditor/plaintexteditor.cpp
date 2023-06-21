/*
   SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "plaintexteditor.h"

#include "texteditor/commonwidget/textmessageindicator.h"
#include <KConfig>
#include <KConfigGroup>
#include <KCursor>
#include <KIO/KUriFilterSearchProviderActions>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KStandardAction>
#include <KStandardGuiItem>
#include <QActionGroup>
#include <QIcon>

#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
#include <TextEditTextToSpeech/TextToSpeech>
#endif
#include <Sonnet/Dialog>
#include <TextEmoticonsWidgets/EmoticonTextEditAction>
#include <sonnet/backgroundchecker.h>

#include <KColorScheme>
#include <QApplication>
#include <QClipboard>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QMenu>
#include <QScrollBar>
#include <QShortcut>
#include <QTextDocumentFragment>

#include <sonnet/spellcheckdecorator.h>

#include <sonnet/speller.h>

#include <Sonnet/Highlighter>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN PlainTextEditor::PlainTextEditorPrivate
{
public:
    PlainTextEditorPrivate(PlainTextEditor *qq)
        : q(qq)
        , mTextIndicator(new KPIMTextEdit::TextMessageIndicator(q))
        , webshortcutMenuManager(new KIO::KUriFilterSearchProviderActions(q))
    {
        KConfig sonnetKConfig(QStringLiteral("sonnetrc"));
        KConfigGroup group(&sonnetKConfig, "Spelling");
        checkSpellingEnabled = group.readEntry("checkerEnabledByDefault", false);
        supportFeatures |= PlainTextEditor::Search;
        supportFeatures |= PlainTextEditor::SpellChecking;
        supportFeatures |= PlainTextEditor::TextToSpeech;
        supportFeatures |= PlainTextEditor::AllowWebShortcut;
    }

    ~PlainTextEditorPrivate()
    {
        delete richTextDecorator;
        delete speller;
    }

    QStringList ignoreSpellCheckingWords;
    PlainTextEditor *const q;
    KPIMTextEdit::TextMessageIndicator *const mTextIndicator;
    KIO::KUriFilterSearchProviderActions *const webshortcutMenuManager;
    Sonnet::SpellCheckDecorator *richTextDecorator = nullptr;
    Sonnet::Speller *speller = nullptr;

    QString spellCheckingConfigFileName;
    QString spellCheckingLanguage;
    QTextDocumentFragment originalDoc;
    PlainTextEditor::SupportFeatures supportFeatures;
    QColor mReadOnlyBackgroundColor;
    int mInitialFontSize = 0;
    bool customPalette = false;
    bool activateLanguageMenu = true;
    bool checkSpellingEnabled = false;
};

PlainTextEditor::PlainTextEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , d(new PlainTextEditor::PlainTextEditorPrivate(this))
{
    KCursor::setAutoHideCursor(this, true, false);
    setSpellCheckingConfigFileName(QString());
    d->mInitialFontSize = font().pointSize();
    regenerateColorScheme();
}

PlainTextEditor::~PlainTextEditor() = default;

void PlainTextEditor::regenerateColorScheme()
{
    d->mReadOnlyBackgroundColor = KColorScheme(QPalette::Disabled, KColorScheme::View).background().color();
    updateReadOnlyColor();
}

void PlainTextEditor::addIgnoreWords(const QStringList &lst)
{
    d->ignoreSpellCheckingWords = lst;
    addIgnoreWordsToHighLighter();
}

void PlainTextEditor::slotDisplayMessageIndicator(const QString &message)
{
    d->mTextIndicator->display(message);
}

void PlainTextEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popup = createStandardContextMenu();
    if (popup) {
        const bool emptyDocument = document()->isEmpty();
        if (!isReadOnly()) {
            const QList<QAction *> actionList = popup->actions();
            enum { UndoAct, RedoAct, CutAct, CopyAct, PasteAct, ClearAct, SelectAllAct, NCountActs };
            QAction *separatorAction = nullptr;
            const int idx = actionList.indexOf(actionList[SelectAllAct]) + 1;
            if (idx < actionList.count()) {
                separatorAction = actionList.at(idx);
            }
            if (separatorAction) {
                if (!emptyDocument) {
                    QAction *clearAllAction = KStandardAction::clear(this, &PlainTextEditor::slotUndoableClear, popup);
                    popup->insertAction(separatorAction, clearAllAction);
                }
            }
        }
        if (d->supportFeatures & Search) {
            popup->addSeparator();
            if (!emptyDocument) {
                popup->addAction(KStandardGuiItem::find().icon(), KStandardGuiItem::find().text(), Qt::Key_F | Qt::CTRL, this, &PlainTextEditor::findText);
                popup->addSeparator();
            }
            if (!isReadOnly()) {
                if (!emptyDocument) {
                    popup->addAction(i18n("Replace..."), Qt::Key_R | Qt::CTRL, this, &PlainTextEditor::replaceText);
                    popup->addSeparator();
                }
            }
        } else {
            popup->addSeparator();
        }

        if (!isReadOnly() && spellCheckingSupport()) {
            if (!d->speller) {
                d->speller = new Sonnet::Speller();
            }
            if (!d->speller->availableBackends().isEmpty()) {
                if (!emptyDocument) {
                    popup->addAction(QIcon::fromTheme(QStringLiteral("tools-check-spelling")),
                                     i18n("Check Spelling..."),
                                     this,
                                     &PlainTextEditor::slotCheckSpelling);
                    popup->addSeparator();
                }
                QAction *autoSpellCheckAction = popup->addAction(i18n("Auto Spell Check"), this, &PlainTextEditor::slotToggleAutoSpellCheck);
                autoSpellCheckAction->setCheckable(true);
                autoSpellCheckAction->setChecked(checkSpellingEnabled());
                popup->addAction(autoSpellCheckAction);

                if (checkSpellingEnabled() && d->activateLanguageMenu) {
                    auto languagesMenu = new QMenu(i18n("Spell Checking Language"), popup);
                    auto languagesGroup = new QActionGroup(languagesMenu);
                    languagesGroup->setExclusive(true);

                    QString defaultSpellcheckingLanguage = spellCheckingLanguage();
                    if (defaultSpellcheckingLanguage.isEmpty()) {
                        // TODO fix default value
                        defaultSpellcheckingLanguage = d->speller->defaultLanguage();
                    }

                    QMapIterator<QString, QString> i(d->speller->availableDictionaries());
                    while (i.hasNext()) {
                        i.next();
                        QAction *languageAction = languagesMenu->addAction(i.key());
                        languageAction->setCheckable(true);
                        languageAction->setChecked(defaultSpellcheckingLanguage == i.value());
                        languageAction->setData(i.value());
                        languageAction->setActionGroup(languagesGroup);
                        connect(languageAction, &QAction::triggered, this, &PlainTextEditor::slotLanguageSelected);
                    }
                    popup->addMenu(languagesMenu);
                }
                popup->addSeparator();
            }
        }
        if (d->supportFeatures & TextToSpeech) {
#ifdef HAVE_KTEXTADDONS_TEXT_TO_SPEECH_SUPPORT
            if (!emptyDocument) {
                QAction *speakAction = popup->addAction(i18n("Speak Text"));
                speakAction->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")));
                connect(speakAction, &QAction::triggered, this, &PlainTextEditor::slotSpeakText);
            }
#endif
        }
        if (webShortcutSupport() && textCursor().hasSelection()) {
            popup->addSeparator();
            const QString selectedText = textCursor().selectedText();
            d->webshortcutMenuManager->setSelectedText(selectedText);
            d->webshortcutMenuManager->addWebShortcutsToMenu(popup);
        }
        if (emojiSupport()) {
            popup->addSeparator();
            auto action = new TextEmoticonsWidgets::EmoticonTextEditAction(this);
            popup->addAction(action);
            connect(action, &TextEmoticonsWidgets::EmoticonTextEditAction::insertEmoticon, this, &PlainTextEditor::slotInsertEmoticon);
        }
        addExtraMenuEntry(popup, event->pos());
        popup->exec(event->globalPos());

        delete popup;
    }
}

void PlainTextEditor::slotInsertEmoticon(const QString &str)
{
    insertPlainText(str);
}
void PlainTextEditor::setEmojiSupport(bool b)
{
    if (b) {
        d->supportFeatures |= Emoji;
    } else {
        d->supportFeatures = (d->supportFeatures & ~Emoji);
    }
}

bool PlainTextEditor::emojiSupport() const
{
    return d->supportFeatures & Emoji;
}
void PlainTextEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    Q_UNUSED(menu)
    Q_UNUSED(pos)
}

void PlainTextEditor::slotSpeakText()
{
    QString text;
    if (textCursor().hasSelection()) {
        text = textCursor().selectedText();
    } else {
        text = toPlainText();
    }
    // qCDebug(KPIMTEXTEDIT_LOG) << " KPIMTextEdit::TextToSpeech::self()->isReady() :" << KPIMTextEdit::TextToSpeech::self()->isReady();
    Q_EMIT say(text);
}

void PlainTextEditor::slotUndoableClear()
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    cursor.endEditBlock();
}

void PlainTextEditor::setSearchSupport(bool b)
{
    if (b) {
        d->supportFeatures |= Search;
    } else {
        d->supportFeatures = (d->supportFeatures & ~Search);
    }
}

bool PlainTextEditor::searchSupport() const
{
    return d->supportFeatures & Search;
}

void PlainTextEditor::setTextToSpeechSupport(bool b)
{
    if (b) {
        d->supportFeatures |= TextToSpeech;
    } else {
        d->supportFeatures = (d->supportFeatures & ~TextToSpeech);
    }
}

bool PlainTextEditor::textToSpeechSupport() const
{
    return d->supportFeatures & TextToSpeech;
}

bool PlainTextEditor::spellCheckingSupport() const
{
    return d->supportFeatures & SpellChecking;
}

void PlainTextEditor::setSpellCheckingSupport(bool check)
{
    if (check) {
        d->supportFeatures |= SpellChecking;
    } else {
        d->supportFeatures = (d->supportFeatures & ~SpellChecking);
    }
}

void PlainTextEditor::setWebShortcutSupport(bool b)
{
    if (b) {
        d->supportFeatures |= AllowWebShortcut;
    } else {
        d->supportFeatures = (d->supportFeatures & ~AllowWebShortcut);
    }
}

bool PlainTextEditor::webShortcutSupport() const
{
    return d->supportFeatures & AllowWebShortcut;
}

void PlainTextEditor::updateReadOnlyColor()
{
    if (isReadOnly()) {
        QPalette p = palette();
        p.setColor(QPalette::Base, d->mReadOnlyBackgroundColor);
        p.setColor(QPalette::Window, d->mReadOnlyBackgroundColor);
        setPalette(p);
    }
}

void PlainTextEditor::setReadOnly(bool readOnly)
{
    if (!readOnly && hasFocus() && d->checkSpellingEnabled && !d->richTextDecorator) {
        createHighlighter();
    }

    if (readOnly == isReadOnly()) {
        return;
    }

    if (readOnly) {
        clearDecorator();
        d->customPalette = testAttribute(Qt::WA_SetPalette);
        updateReadOnlyColor();
    } else {
        if (d->customPalette && testAttribute(Qt::WA_SetPalette)) {
            QPalette p = palette();
            QColor color = p.color(QPalette::Normal, QPalette::Base);
            p.setColor(QPalette::Base, color);
            p.setColor(QPalette::Window, color);
            setPalette(p);
        } else {
            setPalette(QPalette());
        }
    }

    QPlainTextEdit::setReadOnly(readOnly);
}

void PlainTextEditor::slotCheckSpelling()
{
    if (document()->isEmpty()) {
        slotDisplayMessageIndicator(i18n("Nothing to spell check."));
        return;
    }
    auto backgroundSpellCheck = new Sonnet::BackgroundChecker;
    if (backgroundSpellCheck->speller().availableBackends().isEmpty()) {
        slotDisplayMessageIndicator(i18n("No backend available for spell checking."));
        delete backgroundSpellCheck;
        return;
    }
    if (!d->spellCheckingLanguage.isEmpty()) {
        backgroundSpellCheck->changeLanguage(d->spellCheckingLanguage);
    }
    if (!d->ignoreSpellCheckingWords.isEmpty()) {
        for (const QString &word : std::as_const(d->ignoreSpellCheckingWords)) {
            backgroundSpellCheck->speller().addToSession(word);
        }
    }
    auto spellDialog = new Sonnet::Dialog(backgroundSpellCheck, nullptr);
    backgroundSpellCheck->setParent(spellDialog);
    spellDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(spellDialog, &Sonnet::Dialog::replace, this, &PlainTextEditor::slotSpellCheckerCorrected);
    connect(spellDialog, &Sonnet::Dialog::misspelling, this, &PlainTextEditor::slotSpellCheckerMisspelling);
    connect(spellDialog, &Sonnet::Dialog::autoCorrect, this, &PlainTextEditor::slotSpellCheckerAutoCorrect);
    connect(spellDialog, &Sonnet::Dialog::spellCheckDone, this, &PlainTextEditor::slotSpellCheckerFinished);
    connect(spellDialog, &Sonnet::Dialog::cancel, this, &PlainTextEditor::slotSpellCheckerCanceled);
    connect(spellDialog, &Sonnet::Dialog::spellCheckStatus, this, &PlainTextEditor::spellCheckStatus);
    connect(spellDialog, &Sonnet::Dialog::languageChanged, this, &PlainTextEditor::languageChanged);
    d->originalDoc = QTextDocumentFragment(document());
    spellDialog->setBuffer(toPlainText());
    spellDialog->show();
}

void PlainTextEditor::slotSpellCheckerCanceled()
{
    QTextDocument *doc = document();
    doc->clear();
    QTextCursor cursor(doc);
    cursor.insertFragment(d->originalDoc);
    slotSpellCheckerFinished();
}

void PlainTextEditor::slotSpellCheckerAutoCorrect(const QString &currentWord, const QString &autoCorrectWord)
{
    Q_EMIT spellCheckerAutoCorrect(currentWord, autoCorrectWord);
}

void PlainTextEditor::slotSpellCheckerMisspelling(const QString &text, int pos)
{
    highlightWord(text.length(), pos);
}

void PlainTextEditor::slotSpellCheckerCorrected(const QString &oldWord, int pos, const QString &newWord)
{
    if (oldWord != newWord) {
        QTextCursor cursor(document());
        cursor.setPosition(pos);
        cursor.setPosition(pos + oldWord.length(), QTextCursor::KeepAnchor);
        cursor.insertText(newWord);
    }
}

void PlainTextEditor::slotSpellCheckerFinished()
{
    QTextCursor cursor(document());
    cursor.clearSelection();
    setTextCursor(cursor);
}

void PlainTextEditor::highlightWord(int length, int pos)
{
    QTextCursor cursor(document());
    cursor.setPosition(pos);
    cursor.setPosition(pos + length, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
    ensureCursorVisible();
}

static void deleteWord(QTextCursor cursor, QTextCursor::MoveOperation op)
{
    cursor.clearSelection();
    cursor.movePosition(op, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
}

void PlainTextEditor::deleteWordBack()
{
    deleteWord(textCursor(), QTextCursor::PreviousWord);
}

void PlainTextEditor::deleteWordForward()
{
    deleteWord(textCursor(), QTextCursor::WordRight);
}

bool PlainTextEditor::event(QEvent *ev)
{
    if (ev->type() == QEvent::ShortcutOverride) {
        auto e = static_cast<QKeyEvent *>(ev);
        if (overrideShortcut(e)) {
            e->accept();
            return true;
        }
    } else if (ev->type() == QEvent::ApplicationPaletteChange) {
        regenerateColorScheme();
    }

    return QPlainTextEdit::event(ev);
}

bool PlainTextEditor::overrideShortcut(QKeyEvent *event)
{
    const int key = event->key() | event->modifiers();
    if (KStandardShortcut::copy().contains(key)) {
        return true;
    } else if (KStandardShortcut::paste().contains(key)) {
        return true;
    } else if (KStandardShortcut::cut().contains(key)) {
        return true;
    } else if (KStandardShortcut::undo().contains(key)) {
        return true;
    } else if (KStandardShortcut::redo().contains(key)) {
        return true;
    } else if (KStandardShortcut::deleteWordBack().contains(key)) {
        return true;
    } else if (KStandardShortcut::deleteWordForward().contains(key)) {
        return true;
    } else if (KStandardShortcut::backwardWord().contains(key)) {
        return true;
    } else if (KStandardShortcut::forwardWord().contains(key)) {
        return true;
    } else if (KStandardShortcut::next().contains(key)) {
        return true;
    } else if (KStandardShortcut::prior().contains(key)) {
        return true;
    } else if (KStandardShortcut::begin().contains(key)) {
        return true;
    } else if (KStandardShortcut::end().contains(key)) {
        return true;
    } else if (KStandardShortcut::beginningOfLine().contains(key)) {
        return true;
    } else if (KStandardShortcut::endOfLine().contains(key)) {
        return true;
    } else if (KStandardShortcut::pasteSelection().contains(key)) {
        return true;
    } else if (searchSupport() && KStandardShortcut::find().contains(key)) {
        return true;
    } else if (searchSupport() && KStandardShortcut::replace().contains(key)) {
        return true;
    } else if (searchSupport() && KStandardShortcut::findNext().contains(key)) {
        return true;
    } else if (event->matches(QKeySequence::SelectAll)) { // currently missing in QTextEdit
        return true;
    } else if (event == QKeySequence::DeleteEndOfLine) {
        return true;
    }
    return false;
}

bool PlainTextEditor::handleShortcut(QKeyEvent *event)
{
    const int key = event->key() | event->modifiers();

    if (KStandardShortcut::copy().contains(key)) {
        copy();
        return true;
    } else if (KStandardShortcut::paste().contains(key)) {
        paste();
        return true;
    } else if (KStandardShortcut::cut().contains(key)) {
        cut();
        return true;
    } else if (KStandardShortcut::undo().contains(key)) {
        if (!isReadOnly()) {
            undo();
        }
        return true;
    } else if (KStandardShortcut::redo().contains(key)) {
        if (!isReadOnly()) {
            redo();
        }
        return true;
    } else if (KStandardShortcut::deleteWordBack().contains(key)) {
        if (!isReadOnly()) {
            deleteWordBack();
        }
        return true;
    } else if (KStandardShortcut::deleteWordForward().contains(key)) {
        if (!isReadOnly()) {
            deleteWordForward();
        }
        return true;
    } else if (KStandardShortcut::backwardWord().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::PreviousWord);
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::forwardWord().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::NextWord);
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::next().contains(key)) {
        QTextCursor cursor = textCursor();
        bool moved = false;
        qreal lastY = cursorRect(cursor).bottom();
        qreal distance = 0;
        do {
            qreal y = cursorRect(cursor).bottom();
            distance += qAbs(y - lastY);
            lastY = y;
            moved = cursor.movePosition(QTextCursor::Down);
        } while (moved && distance < viewport()->height());

        if (moved) {
            cursor.movePosition(QTextCursor::Up);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
        }
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::prior().contains(key)) {
        QTextCursor cursor = textCursor();
        bool moved = false;
        qreal lastY = cursorRect(cursor).bottom();
        qreal distance = 0;
        do {
            qreal y = cursorRect(cursor).bottom();
            distance += qAbs(y - lastY);
            lastY = y;
            moved = cursor.movePosition(QTextCursor::Up);
        } while (moved && distance < viewport()->height());

        if (moved) {
            cursor.movePosition(QTextCursor::Down);
            verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
        }
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::begin().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Start);
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::end().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::beginningOfLine().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);
        setTextCursor(cursor);
        return true;
    } else if (KStandardShortcut::endOfLine().contains(key)) {
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        return true;
    } else if (searchSupport() && KStandardShortcut::find().contains(key)) {
        Q_EMIT findText();
        return true;
    } else if (searchSupport() && KStandardShortcut::replace().contains(key)) {
        if (!isReadOnly()) {
            Q_EMIT replaceText();
        }
        return true;
    } else if (KStandardShortcut::pasteSelection().contains(key)) {
        QString text = QApplication::clipboard()->text(QClipboard::Selection);
        if (!text.isEmpty()) {
            insertPlainText(text); // TODO: check if this is html? (MiB)
        }
        return true;
    } else if (event == QKeySequence::DeleteEndOfLine) {
        deleteEndOfLine();
        return true;
    }
    return false;
}

void PlainTextEditor::deleteEndOfLine()
{
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    if (cursor.position() == block.position() + block.length() - 2) {
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    } else {
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    }
    cursor.removeSelectedText();
    setTextCursor(cursor);
}

void PlainTextEditor::moveCursorBeginUpDown(bool moveUp)
{
    QTextCursor cursor = textCursor();
    QTextCursor move = cursor;
    move.beginEditBlock();
    cursor.clearSelection();
    move.movePosition(QTextCursor::StartOfBlock);
    move.movePosition(moveUp ? QTextCursor::PreviousBlock : QTextCursor::NextBlock);
    move.endEditBlock();
    setTextCursor(move);
}

void PlainTextEditor::moveLineUpDown(bool moveUp)
{
    QTextCursor cursor = textCursor();
    QTextCursor move = cursor;
    move.beginEditBlock();

    const bool hasSelection = cursor.hasSelection();

    if (hasSelection) {
        move.setPosition(cursor.selectionStart());
        move.movePosition(QTextCursor::StartOfBlock);
        move.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
        move.movePosition(move.atBlockStart() ? QTextCursor::Left : QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    } else {
        move.movePosition(QTextCursor::StartOfBlock);
        move.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    }
    const QString text = move.selectedText();

    move.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
    move.removeSelectedText();

    if (moveUp) {
        move.movePosition(QTextCursor::PreviousBlock);
        move.insertBlock();
        move.movePosition(QTextCursor::Left);
    } else {
        move.movePosition(QTextCursor::EndOfBlock);
        if (move.atBlockStart()) { // empty block
            move.movePosition(QTextCursor::NextBlock);
            move.insertBlock();
            move.movePosition(QTextCursor::Left);
        } else {
            move.insertBlock();
        }
    }

    int start = move.position();
    move.clearSelection();
    move.insertText(text);
    int end = move.position();

    if (hasSelection) {
        move.setPosition(end);
        move.setPosition(start, QTextCursor::KeepAnchor);
    } else {
        move.setPosition(start);
    }
    move.endEditBlock();

    setTextCursor(move);
}

void PlainTextEditor::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            zoomIn();
        } else if (event->angleDelta().y() < 0) {
            zoomOut();
        }
        event->accept();
        return;
    }
    QPlainTextEdit::wheelEvent(event);
}

void PlainTextEditor::keyPressEvent(QKeyEvent *event)
{
    const bool isControlClicked = event->modifiers() & Qt::ControlModifier;
    const bool isShiftClicked = event->modifiers() & Qt::ShiftModifier;
    if (handleShortcut(event)) {
        event->accept();
    } else if (event->key() == Qt::Key_Up && isControlClicked && isShiftClicked) {
        moveLineUpDown(true);
        event->accept();
    } else if (event->key() == Qt::Key_Down && isControlClicked && isShiftClicked) {
        moveLineUpDown(false);
        event->accept();
    } else if (event->key() == Qt::Key_Up && isControlClicked) {
        moveCursorBeginUpDown(true);
        event->accept();
    } else if (event->key() == Qt::Key_Down && isControlClicked) {
        moveCursorBeginUpDown(false);
        event->accept();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}

bool PlainTextEditor::activateLanguageMenu() const
{
    return d->activateLanguageMenu;
}

void PlainTextEditor::setActivateLanguageMenu(bool activate)
{
    d->activateLanguageMenu = activate;
}

Sonnet::Highlighter *PlainTextEditor::highlighter() const
{
    if (d->richTextDecorator) {
        return d->richTextDecorator->highlighter();
    } else {
        return nullptr;
    }
}

Sonnet::SpellCheckDecorator *PlainTextEditor::createSpellCheckDecorator()
{
    return new Sonnet::SpellCheckDecorator(this);
}

void PlainTextEditor::addIgnoreWordsToHighLighter()
{
    if (d->ignoreSpellCheckingWords.isEmpty()) {
        return;
    }
    if (d->richTextDecorator) {
        Sonnet::Highlighter *_highlighter = d->richTextDecorator->highlighter();
        for (const QString &word : std::as_const(d->ignoreSpellCheckingWords)) {
            _highlighter->ignoreWord(word);
        }
    }
}

void PlainTextEditor::setHighlighter(Sonnet::Highlighter *_highLighter)
{
    Sonnet::SpellCheckDecorator *decorator = createSpellCheckDecorator();
    delete decorator->highlighter();
    decorator->setHighlighter(_highLighter);

    d->richTextDecorator = decorator;
    addIgnoreWordsToHighLighter();
}

void PlainTextEditor::focusInEvent(QFocusEvent *event)
{
    if (checkSpellingEnabled() && !isReadOnly() && !d->richTextDecorator && spellCheckingSupport()) {
        createHighlighter();
    }

    QPlainTextEdit::focusInEvent(event);
}

bool PlainTextEditor::checkSpellingEnabled() const
{
    return d->checkSpellingEnabled;
}

void PlainTextEditor::setCheckSpellingEnabled(bool check)
{
    if (check == d->checkSpellingEnabled) {
        return;
    }
    d->checkSpellingEnabled = check;
    Q_EMIT checkSpellingChanged(check);
    // From the above statement we know that if we're turning checking
    // on that we need to create a new highlighter and if we're turning it
    // off we should remove the old one.
    if (check) {
        if (hasFocus()) {
            if (!d->richTextDecorator) {
                createHighlighter();
            }
            if (!d->spellCheckingLanguage.isEmpty()) {
                setSpellCheckingLanguage(spellCheckingLanguage());
            }
        }
    } else {
        clearDecorator();
    }
    updateHighLighter();
}

void PlainTextEditor::updateHighLighter()
{
}

void PlainTextEditor::clearDecorator()
{
    delete d->richTextDecorator;
    d->richTextDecorator = nullptr;
}

void PlainTextEditor::createHighlighter()
{
    auto highlighter = new Sonnet::Highlighter(this);
    highlighter->setCurrentLanguage(spellCheckingLanguage());
    setHighlighter(highlighter);
}

void PlainTextEditor::setSpellCheckingConfigFileName(const QString &_fileName)
{
    d->spellCheckingConfigFileName = _fileName;
    KSharedConfig::Ptr config = KSharedConfig::openConfig(d->spellCheckingConfigFileName);
    if (config->hasGroup("Spelling")) {
        KConfigGroup group(config, "Spelling");
        d->checkSpellingEnabled = group.readEntry("checkerEnabledByDefault", false);
        d->spellCheckingLanguage = group.readEntry("Language", QString());
    }
    setCheckSpellingEnabled(checkSpellingEnabled());

    if (!d->spellCheckingLanguage.isEmpty() && highlighter()) {
        highlighter()->setCurrentLanguage(d->spellCheckingLanguage);
        highlighter()->rehighlight();
    }
}

QString PlainTextEditor::spellCheckingConfigFileName() const
{
    return d->spellCheckingConfigFileName;
}

void PlainTextEditor::slotLanguageSelected()
{
    auto languageAction = static_cast<QAction *>(QObject::sender());
    setSpellCheckingLanguage(languageAction->data().toString());
}

const QString &PlainTextEditor::spellCheckingLanguage() const
{
    return d->spellCheckingLanguage;
}

void PlainTextEditor::setSpellCheckingLanguage(const QString &_language)
{
    if (highlighter()) {
        highlighter()->setCurrentLanguage(_language);
        highlighter()->rehighlight();
    }

    if (_language != d->spellCheckingLanguage) {
        d->spellCheckingLanguage = _language;
        KSharedConfig::Ptr config = KSharedConfig::openConfig(d->spellCheckingConfigFileName);
        KConfigGroup group(config, "Spelling");
        group.writeEntry("Language", d->spellCheckingLanguage);
        setCheckSpellingEnabled(checkSpellingEnabled());

        Q_EMIT languageChanged(_language);
    }
}

void PlainTextEditor::slotToggleAutoSpellCheck()
{
    setCheckSpellingEnabled(!checkSpellingEnabled());
    KSharedConfig::Ptr config = KSharedConfig::openConfig(d->spellCheckingConfigFileName);
    KConfigGroup group(config, "Spelling");
    group.writeEntry("checkerEnabledByDefault", d->checkSpellingEnabled);
}

void PlainTextEditor::slotZoomReset()
{
    QFont f = font();
    if (d->mInitialFontSize != f.pointSize()) {
        f.setPointSize(d->mInitialFontSize);
        setFont(f);
    }
}

#include "moc_plaintexteditor.cpp"
