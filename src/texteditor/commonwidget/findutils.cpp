/*
   SPDX-FileCopyrightText: 2020 Igor Poboiko <igor.poboiko@gmail.com>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "findutils.h"
#include "textfindreplacewidget.h"
#include <TextUtils/ConvertText>

#include <QDebug>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QTextCursor>
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
                c.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    } else {
        const QString toPlainTextWithoutRespectDiacritics{TextUtils::ConvertText::normalize(view->toPlainText())};
        const QString searchStrWithoutRespectDiacritics{TextUtils::ConvertText::normalize(str)};

        QTextDocument documentWithoutRespectDiacritics(toPlainTextWithoutRespectDiacritics);
        QTextCursor documentWithoutRespectDiacriticsTextCursor(&documentWithoutRespectDiacritics);
        documentWithoutRespectDiacriticsTextCursor.setPosition(0);

        view->textCursor().beginEditBlock();
        QTextCursor c(document);

        while (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
            documentWithoutRespectDiacriticsTextCursor =
                documentWithoutRespectDiacritics.find(searchStrWithoutRespectDiacritics, documentWithoutRespectDiacriticsTextCursor, flags);
            if (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionStart());
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionEnd(), QTextCursor::KeepAnchor);
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
                c.insertText(replaceStr);
                count++;
            } else {
                break;
            }
        }
        view->textCursor().endEditBlock();
    } else {
        const QString toPlainTextWithoutRespectDiacritics{TextUtils::ConvertText::normalize(view->toPlainText())};
        const QString searchStrWithoutRespectDiacritics{TextUtils::ConvertText::normalize(str)};

        QTextDocument documentWithoutRespectDiacritics(toPlainTextWithoutRespectDiacritics);
        QTextCursor documentWithoutRespectDiacriticsTextCursor(&documentWithoutRespectDiacritics);
        documentWithoutRespectDiacriticsTextCursor.setPosition(0);

        view->textCursor().beginEditBlock();
        QTextCursor c(document);

        while (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
            documentWithoutRespectDiacriticsTextCursor =
                documentWithoutRespectDiacritics.find(searchStrWithoutRespectDiacritics, documentWithoutRespectDiacriticsTextCursor, flags);
            if (!documentWithoutRespectDiacriticsTextCursor.isNull()) {
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionStart());
                c.setPosition(documentWithoutRespectDiacriticsTextCursor.selectionEnd(), QTextCursor::KeepAnchor);
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
    const QString text = TextUtils::ConvertText::normalize(view->document()->toPlainText());
    QTextDocument doc(text);
    QTextCursor c(&doc);
    QTextCursor docCusor(view->textCursor());
    c.setPosition(docCusor.position());
    // qDebug() << " docCusor.position() " << docCusor.position();
    c = doc.find(TextUtils::ConvertText::normalize(searchText), c, searchOptions);
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
    const QString text = TextUtils::ConvertText::normalize(view->document()->toPlainText());
    QTextDocument doc(text);
    QTextCursor c(&doc);
    QTextCursor docCusor(view->textCursor());
    c.setPosition(docCusor.position());
    // qDebug() << " docCusor.position() " << docCusor.position();
    c = doc.find(TextUtils::ConvertText::normalize(searchText), c, searchOptions);
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
