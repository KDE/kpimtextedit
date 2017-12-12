/*
   Copyright (C) 2013-2017 Laurent Montel <montel@kde.org>

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
#include "plaintexteditor.h"
#include "kpimtextedit_debug.h"

#include "texteditor/commonwidget/textmessageindicator.h"
#include <KIO/KUriFilterSearchProviderActions>
#include <KLocalizedString>
#include <KIconTheme>
#include <KStandardGuiItem>
#include <KStandardAction>
#include <KCursor>
#include <QIcon>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KConfig>

#include <sonnet/backgroundchecker.h>
#include <Sonnet/Dialog>
#include "texttospeech/texttospeech.h"

#include <QMenu>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QTextDocumentFragment>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>
#include <QShortcut>

#include <sonnet/spellcheckdecorator.h>

#include <SonnetCore/sonnet/speller.h>

#include <Sonnet/Highlighter>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN PlainTextEditor::PlainTextEditorPrivate
{
public:
    PlainTextEditorPrivate(PlainTextEditor *qq)
        : q(qq)
        , mTextIndicator(new KPIMTextEdit::TextMessageIndicator(q))
        , webshortcutMenuManager(new KIO::KUriFilterSearchProviderActions(q))
        , richTextDecorator(nullptr)
        , speller(nullptr)
        , mInitialFontSize(0)
        , customPalette(false)
        , activateLanguageMenu(true)
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
    PlainTextEditor *q;
    KPIMTextEdit::TextMessageIndicator *mTextIndicator = nullptr;
    KIO::KUriFilterSearchProviderActions *webshortcutMenuManager = nullptr;
    Sonnet::SpellCheckDecorator *richTextDecorator = nullptr;
    Sonnet::Speller *speller = nullptr;

    QString spellCheckingConfigFileName;
    QString spellCheckingLanguage;
    QTextDocumentFragment originalDoc;
    PlainTextEditor::SupportFeatures supportFeatures;
    int mInitialFontSize = 0;
    bool customPalette;
    bool activateLanguageMenu;
    bool checkSpellingEnabled;
};

PlainTextEditor::PlainTextEditor(QWidget *parent)
    : QPlainTextEdit(parent)
    , d(new PlainTextEditor::PlainTextEditorPrivate(this))
{
    KCursor::setAutoHideCursor(this, true, false);
    setSpellCheckingConfigFileName(QString());
    d->mInitialFontSize = font().pointSize();
}

PlainTextEditor::~PlainTextEditor()
{
    delete d;
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
            QList<QAction *> actionList = popup->actions();
            enum {
                UndoAct, RedoAct, CutAct, CopyAct, PasteAct, ClearAct, SelectAllAct, NCountActs
            };
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
        KIconTheme::assignIconsToContextMenu(isReadOnly() ? KIconTheme::ReadOnlyText
                                             : KIconTheme::TextEditor,
                                             popup->actions());
        if (d->supportFeatures & Search) {
            popup->addSeparator();
            if (!emptyDocument) {
                popup->addAction(KStandardGuiItem::find().icon(), KStandardGuiItem::find().text(), this, SIGNAL(findText()), Qt::Key_F + Qt::CTRL);
                popup->addSeparator();
            }
            if (!isReadOnly()) {
                if (!emptyDocument) {
                    popup->addAction(i18n("Replace..."), this, SIGNAL(replaceText()), Qt::Key_R + Qt::CTRL);
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
                    popup->addAction(QIcon::fromTheme(QStringLiteral("tools-check-spelling")), i18n("Check Spelling..."), this, &PlainTextEditor::slotCheckSpelling);
                    popup->addSeparator();
                }
                QAction *autoSpellCheckAction = popup->addAction(i18n("Auto Spell Check"), this, &PlainTextEditor::slotToggleAutoSpellCheck);
                autoSpellCheckAction->setCheckable(true);
                autoSpellCheckAction->setChecked(checkSpellingEnabled());
                popup->addAction(autoSpellCheckAction);

                if (checkSpellingEnabled() && d->activateLanguageMenu) {
                    QMenu *languagesMenu = new QMenu(i18n("Spell Checking Language"), popup);
                    QActionGroup *languagesGroup = new QActionGroup(languagesMenu);
                    languagesGroup->setExclusive(true);

                    QString defaultSpellcheckingLanguage = spellCheckingLanguage();
                    if (defaultSpellcheckingLanguage.isEmpty()) {
                        //TODO fix default value
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
            if (KPIMTextEdit::TextToSpeech::self()->isReady()) {
                if (!emptyDocument) {
                    QAction *speakAction = popup->addAction(i18n("Speak Text"));
                    speakAction->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-text-to-speech")));
                    connect(speakAction, &QAction::triggered, this, &PlainTextEditor::slotSpeakText);
                }
            }
        }
        if (webShortcutSupport() && textCursor().hasSelection()) {
            popup->addSeparator();
            const QString selectedText = textCursor().selectedText();
            d->webshortcutMenuManager->setSelectedText(selectedText);
            d->webshortcutMenuManager->addWebShortcutsToMenu(popup);
        }

        addExtraMenuEntry(popup, event->pos());
        popup->exec(event->globalPos());

        delete popup;
    }
}

void PlainTextEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    Q_UNUSED(menu);
    Q_UNUSED(pos);
}

void PlainTextEditor::slotSpeakText()
{
    QString text;
    if (textCursor().hasSelection()) {
        text = textCursor().selectedText();
    } else {
        text = toPlainText();
    }
    //qCDebug(KPIMTEXTEDIT_LOG) << " KPIMTextEdit::TextToSpeech::self()->isReady() :" << KPIMTextEdit::TextToSpeech::self()->isReady();
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
        QPalette p = palette();
        QColor color = p.color(QPalette::Disabled, QPalette::Background);
        p.setColor(QPalette::Base, color);
        p.setColor(QPalette::Background, color);
        setPalette(p);
    } else {
        if (d->customPalette && testAttribute(Qt::WA_SetPalette)) {
            QPalette p = palette();
            QColor color = p.color(QPalette::Normal, QPalette::Base);
            p.setColor(QPalette::Base, color);
            p.setColor(QPalette::Background, color);
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
    Sonnet::BackgroundChecker *backgroundSpellCheck = new Sonnet::BackgroundChecker;
    if (backgroundSpellCheck->speller().availableBackends().isEmpty()) {
        slotDisplayMessageIndicator(i18n("No backend available for spell checking."));
        delete backgroundSpellCheck;
        return;
    }
    if (!d->spellCheckingLanguage.isEmpty()) {
        backgroundSpellCheck->changeLanguage(d->spellCheckingLanguage);
    }
    if (!d->ignoreSpellCheckingWords.isEmpty()) {
        for (const QString &word : qAsConst(d->ignoreSpellCheckingWords)) {
            backgroundSpellCheck->speller().addToSession(word);
        }
    }
    Sonnet::Dialog *spellDialog = new Sonnet::Dialog(backgroundSpellCheck, nullptr);
    backgroundSpellCheck->setParent(spellDialog);
    spellDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(spellDialog, &Sonnet::Dialog::replace, this, &PlainTextEditor::slotSpellCheckerCorrected);
    connect(spellDialog, &Sonnet::Dialog::misspelling, this, &PlainTextEditor::slotSpellCheckerMisspelling);
    connect(spellDialog, &Sonnet::Dialog::autoCorrect, this, &PlainTextEditor::slotSpellCheckerAutoCorrect);
    connect(spellDialog, QOverload<const QString &>::of(&Sonnet::Dialog::done),
            this, &PlainTextEditor::slotSpellCheckerFinished);

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
        QKeyEvent *e = static_cast<QKeyEvent *>(ev);
        if (overrideShortcut(e)) {
            e->accept();
            return true;
        }
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
            insertPlainText(text);    // TODO: check if this is html? (MiB)
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

void PlainTextEditor::moveLineUpDown(bool moveUp)
{
    QTextCursor cursor = textCursor();
    QTextCursor move = cursor;
    move.beginEditBlock();

    bool hasSelection = cursor.hasSelection();

    if (hasSelection) {
        move.setPosition(cursor.selectionStart());
        move.movePosition(QTextCursor::StartOfBlock);
        move.setPosition(cursor.selectionEnd(), QTextCursor::KeepAnchor);
        move.movePosition(move.atBlockStart() ? QTextCursor::Left: QTextCursor::EndOfBlock,
                          QTextCursor::KeepAnchor);
    } else {
        move.movePosition(QTextCursor::StartOfBlock);
        move.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    }
    QString text = move.selectedText();

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
        if (event->delta() > 0) {
            zoomIn();
        } else if (event->delta() < 0) {
            zoomOut();
        }
        event->accept();
        return;
    }
    QPlainTextEdit::wheelEvent(event);
}

void PlainTextEditor::keyPressEvent(QKeyEvent *event)
{
    if (handleShortcut(event)) {
        event->accept();
    } else if (event->key() == Qt::Key_Up && event->modifiers() == Qt::CTRL) {
        moveLineUpDown(true);
        event->accept();
    } else if (event->key() == Qt::Key_Down && event->modifiers() == Qt::CTRL) {
        moveLineUpDown(false);
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
        for (const QString &word : qAsConst(d->ignoreSpellCheckingWords)) {
            _highlighter->ignoreWord(word);
        }
    }
}

void PlainTextEditor::setHighlighter(Sonnet::Highlighter *_highLighter)
{
    Sonnet::SpellCheckDecorator *decorator = createSpellCheckDecorator();
    delete decorator->highlighter();
    decorator->setHighlighter(_highLighter);

    //KTextEdit used to take ownership of the highlighter, Sonnet::SpellCheckDecorator does not.
    //so we reparent the highlighter so it will be deleted when the decorator is destroyed
    _highLighter->setParent(decorator);
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
    // From the above statment we know know that if we're turning checking
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
    Sonnet::Highlighter *highlighter = new Sonnet::Highlighter(this);
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
    QAction *languageAction = static_cast<QAction *>(QObject::sender());
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
