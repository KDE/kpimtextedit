/*
   SPDX-FileCopyrightText: 2020 Igor Poboiko <igor.poboiko@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findutils.h"
#include "textfindreplacewidget.h"

#include <QDebug>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextEdit>

using namespace KPIMTextEdit;
QTextDocument::FindFlags FindUtils::convertTextEditFindFlags(TextEditFindBarBase::FindFlags textEditFlags)
{
    QTextDocument::FindFlags flags;
    if (textEditFlags & TextEditFindBarBase::FindBackward) {
        flags |= QTextDocument::FindBackward;
    }
    if (textEditFlags & TextEditFindBarBase::FindCaseSensitively) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (textEditFlags & TextEditFindBarBase::FindWholeWords) {
        flags |= QTextDocument::FindWholeWords;
    }
    return flags;
}

int FindUtils::replaceAll(QTextEdit *view, const QString &str, const QString &replaceStr, TextEditFindBarBase::FindFlags searchOptions)
{
    auto document = view->document();
    int count = 0;
    // Ignoring FindBackward when replacing all
    const QTextDocument::FindFlags flags = FindUtils::convertTextEditFindFlags(searchOptions) & ~QTextDocument::FindBackward;
    if (searchOptions & TextEditFindBarBase::FindFlag::FindRespectDiacritics) {
        view->textCursor().beginEditBlock();
        QTextCursor c(document);
        while (!c.isNull()) {
            c = document->find(str, c, flags);
            if (!c.isNull()) {
                // find() selects found text, and insertText() replaces selection
                c.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    } else {
        const QString toPlainTextWithoutRespectDiacritics{FindUtils::normalize(view->toPlainText())};
        const QString searchStrWithoutRespectDiacritics{FindUtils::normalize(str)};

        QTextDocument documentWithoutRespectDiacritics(toPlainTextWithoutRespectDiacritics);
        QTextCursor documentWithoutRespectDiacriticsTextCursor(&documentWithoutRespectDiacritics);
        QTextCursor docCusor(view->textCursor());
        documentWithoutRespectDiacriticsTextCursor.setPosition(0);

        view->textCursor().beginEditBlock();
        QTextCursor c(document);

        while (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
            documentWithoutRespectDiacriticsTextCursor =
                documentWithoutRespectDiacritics.find(searchStrWithoutRespectDiacritics, documentWithoutRespectDiacriticsTextCursor, flags);
            if (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionStart());
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionEnd(), QTextCursor::KeepAnchor);
                // find() selects found text, and insertText() replaces selection
                c.insertText(replaceStr);
                documentWithoutRespectDiacriticsTextCursor.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    }
    return count;
}

int FindUtils::replaceAll(QPlainTextEdit *view, const QString &str, const QString &replaceStr, TextEditFindBarBase::FindFlags searchOptions)
{
    auto document = view->document();
    int count = 0;
    // Ignoring FindBackward when replacing all
    const QTextDocument::FindFlags flags = FindUtils::convertTextEditFindFlags(searchOptions) & ~QTextDocument::FindBackward;
    if (searchOptions & TextEditFindBarBase::FindFlag::FindRespectDiacritics) {
        view->textCursor().beginEditBlock();
        QTextCursor c(document);
        while (!c.isNull()) {
            c = document->find(str, c, flags);
            if (!c.isNull()) {
                // find() selects found text, and insertText() replaces selection
                c.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    } else {
        const QString toPlainTextWithoutRespectDiacritics{FindUtils::normalize(view->toPlainText())};
        const QString searchStrWithoutRespectDiacritics{FindUtils::normalize(str)};

        QTextDocument documentWithoutRespectDiacritics(toPlainTextWithoutRespectDiacritics);
        QTextCursor documentWithoutRespectDiacriticsTextCursor(&documentWithoutRespectDiacritics);
        QTextCursor docCusor(view->textCursor());
        documentWithoutRespectDiacriticsTextCursor.setPosition(0);

        view->textCursor().beginEditBlock();
        QTextCursor c(document);

        while (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
            documentWithoutRespectDiacriticsTextCursor =
                documentWithoutRespectDiacritics.find(searchStrWithoutRespectDiacritics, documentWithoutRespectDiacriticsTextCursor, flags);
            if (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionStart());
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionEnd(), QTextCursor::KeepAnchor);
                // find() selects found text, and insertText() replaces selection
                c.insertText(replaceStr);
                documentWithoutRespectDiacriticsTextCursor.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    }
    return count;
}

int FindUtils::replaceAll(QTextDocument *document, const QRegularExpression &regExp, const QString &replaceWidget, TextEditFindBarBase::FindFlags searchOptions)
{
    QTextCursor c(document);
    c.beginEditBlock();
    int count = 0;
    // Ignoring FindBackward when replacing all
    const QTextDocument::FindFlags flags = FindUtils::convertTextEditFindFlags(searchOptions) & ~QTextDocument::FindBackward;
    while (!c.isNull()) {
        c = document->find(regExp, c, flags);
        if (!c.isNull()) {
            // find() selects found text, and insertText() replaces selection
            c.insertText(replaceWidget);
            count++;
        } else {
            break;
        }
    }
    c.endEditBlock();
    return count;
}

bool FindUtils::find(QPlainTextEdit *view, const QString &searchText, QTextDocument::FindFlags searchOptions)
{
    const QString text = FindUtils::normalize(view->document()->toPlainText());
    QTextDocument doc(text);
    QTextCursor c(&doc);
    QTextCursor docCusor(view->textCursor());
    c.setPosition(docCusor.position());
    // qDebug() << " docCusor.position() " << docCusor.position();
    c = doc.find(FindUtils::normalize(searchText), c, searchOptions);
    if (!c.isNull()) {
        // qDebug() << " c.selectionStart() " << c.selectionStart() << "c.selectionEnd() " << c.selectionEnd();
        if (searchOptions & QTextDocument::FindBackward) {
            docCusor.setPosition(c.selectionEnd());
            docCusor.setPosition(c.selectionStart(), QTextCursor::KeepAnchor);
        } else {
            docCusor.setPosition(c.selectionStart());
            docCusor.setPosition(c.selectionEnd(), QTextCursor::KeepAnchor);
        }
        view->setTextCursor(docCusor);
        view->ensureCursorVisible();
        return true;
    }
    return false;
}

bool FindUtils::find(QTextEdit *view, const QString &searchText, QTextDocument::FindFlags searchOptions)
{
    const QString text = FindUtils::normalize(view->document()->toPlainText());
    QTextDocument doc(text);
    QTextCursor c(&doc);
    QTextCursor docCusor(view->textCursor());
    c.setPosition(docCusor.position());
    // qDebug() << " docCusor.position() " << docCusor.position();
    c = doc.find(FindUtils::normalize(searchText), c, searchOptions);
    if (!c.isNull()) {
        // qDebug() << " c.selectionStart() " << c.selectionStart() << "c.selectionEnd() " << c.selectionEnd();
        if (searchOptions & QTextDocument::FindBackward) {
            docCusor.setPosition(c.selectionEnd());
            docCusor.setPosition(c.selectionStart(), QTextCursor::KeepAnchor);
        } else {
            docCusor.setPosition(c.selectionStart());
            docCusor.setPosition(c.selectionEnd(), QTextCursor::KeepAnchor);
        }
        view->setTextCursor(docCusor);
        view->ensureCursorVisible();
        return true;
    }
    return false;
}

// code from kitinerary/src/lib/stringutil.cpp
QString FindUtils::normalize(QStringView str)
{
    QString out;
    out.reserve(str.size());
    for (const auto c : str) {
        // case folding
        const auto n = c.toCaseFolded();

        // if the character has a canonical decomposition use that and skip the
        // combining diacritic markers following it
        // see https://en.wikipedia.org/wiki/Unicode_equivalence
        // see https://en.wikipedia.org/wiki/Combining_character
        if (n.decompositionTag() == QChar::Canonical) {
            out.push_back(n.decomposition().at(0));
        }
        // handle compatibility compositions such as ligatures
        // see https://en.wikipedia.org/wiki/Unicode_compatibility_characters
        else if (n.decompositionTag() == QChar::Compat && n.isLetter() && n.script() == QChar::Script_Latin) {
            out.append(n.decomposition());
        } else {
            out.push_back(n);
        }
    }
    return out;
}
