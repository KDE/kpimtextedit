/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plaintexteditfindbartest.h"
#include "texteditor/commonwidget/findutils.h"
#include "texteditor/plaintexteditor/plaintexteditfindbar.h"
#include <QPlainTextEdit>
#include <QTest>

QTEST_MAIN(PlainTextEditFindBarTest)
Q_DECLARE_METATYPE(KPIMTextEdit::TextEditFindBarBase::FindFlags)
PlainTextEditFindBarTest::PlainTextEditFindBarTest(QObject *parent)
    : QObject{parent}
{
}

void PlainTextEditFindBarTest::shouldHaveDefaultValues()
{
    QPlainTextEdit edit;
    KPIMTextEdit::PlainTextEditFindBar w(&edit);
    QVERIFY(w.text().isEmpty());
}

class PlainTextEditFindBarExample : public KPIMTextEdit::PlainTextEditFindBar
{
public:
    explicit PlainTextEditFindBarExample(QPlainTextEdit *view, QWidget *parent = nullptr)
        : PlainTextEditFindBar(view, parent)
    {
    }
    ~PlainTextEditFindBarExample() override = default;
    Q_REQUIRED_RESULT bool searchInDocument(const QString &text, TextEditFindBarBase::FindFlags searchOptions) override
    {
        return KPIMTextEdit::PlainTextEditFindBar::searchInDocument(text, searchOptions);
    }

    Q_REQUIRED_RESULT bool searchInDocument(const QRegularExpression &regExp, TextEditFindBarBase::FindFlags searchOptions) override
    {
        return KPIMTextEdit::PlainTextEditFindBar::searchInDocument(regExp, searchOptions);
    }
};

void PlainTextEditFindBarTest::shouldSearchText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("searchText");
    QTest::addColumn<KPIMTextEdit::TextEditFindBarBase::FindFlags>("flags");
    QTest::addColumn<bool>("found");

    QTest::newRow("empty") << QString() << QStringLiteral("blabla") << KPIMTextEdit::TextEditFindBarBase::FindFlags() << false;
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindRespectDiacritics;
        QTest::newRow("empty-1") << QString() << QStringLiteral("blabla") << flags << false;
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindCaseSensitively;
        QTest::newRow("empty-2") << QString() << QStringLiteral("blabla") << flags << false;
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("findwholewords-1") << QStringLiteral("bla bla") << QStringLiteral("bla") << flags << true;

        QTest::newRow("findwholewords-2") << QStringLiteral("blabla") << QStringLiteral("bla") << flags << false;
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindRespectDiacritics;
        QTest::newRow("find-diacritics-1") << QStringLiteral("numéro numéro") << QStringLiteral("numero") << flags << false;

        QTest::newRow("find-diacritics-2") << QStringLiteral("numéro numéro") << QStringLiteral("numéro") << flags << true;
        QTest::newRow("find-diacritics-3") << QStringLiteral("numéro numéro il est là. Numero numéro") << QStringLiteral("numero") << flags << true;
    }
}

void PlainTextEditFindBarTest::shouldSearchText()
{
    QFETCH(QString, text);
    QFETCH(QString, searchText);
    QFETCH(KPIMTextEdit::TextEditFindBarBase::FindFlags, flags);
    QFETCH(bool, found);

    QPlainTextEdit edit;
    PlainTextEditFindBarExample w(&edit);
    edit.setPlainText(text);

    QCOMPARE(w.searchInDocument(searchText, flags), found);
}

void PlainTextEditFindBarTest::shouldReplaceAllText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("searchText");
    QTest::addColumn<QString>("replaceText");
    QTest::addColumn<KPIMTextEdit::TextEditFindBarBase::FindFlags>("flags");
    QTest::addColumn<int>("nbElement");
    QTest::addColumn<QString>("resultStr");

    QTest::newRow("empty") << QString() << QStringLiteral("blabla") << QStringLiteral("replace") << KPIMTextEdit::TextEditFindBarBase::FindFlags() << 0
                           << QString();

    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords") << QStringLiteral("bla bla") << QStringLiteral("bla") << QStringLiteral("replace") << flags << 2
                                    << QStringLiteral("replace replace");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindRespectDiacritics;
        QTest::newRow("wholewords-diacritics") << QStringLiteral("réunion reunion réunion") << QStringLiteral("réunion") << QStringLiteral("replace") << flags
                                               << 2 << QStringLiteral("replace reunion replace");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords-no-diacritics") << QStringLiteral("réunion reunion réunion") << QStringLiteral("réunion") << QStringLiteral("replace")
                                                  << flags << 3 << QStringLiteral("replace replace replace");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords-no-diacritics-2") << QStringLiteral("réunion réunion réunion") << QStringLiteral("réunion") << QStringLiteral("replace")
                                                    << flags << 3 << QStringLiteral("replace replace replace");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords-no-diacritics-3") << QStringLiteral("voiture voiture voiture") << QStringLiteral("réunion") << QStringLiteral("replace")
                                                    << flags << 0 << QStringLiteral("voiture voiture voiture");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords-no-diacritics-3") << QStringLiteral("voiture voiture il est là. voiture") << QStringLiteral("voiture")
                                                    << QStringLiteral("replace") << flags << 3 << QStringLiteral("replace replace il est là. replace");
    }
    {
        KPIMTextEdit::TextEditFindBarBase::FindFlags flags;
        flags |= KPIMTextEdit::TextEditFindBarBase::FindWholeWords;
        QTest::newRow("wholewords-no-diacritics-4") << QStringLiteral("réunion réunion . Test... réunion") << QStringLiteral("réunion")
                                                    << QStringLiteral("replace") << flags << 3 << QStringLiteral("replace replace . Test... replace");
    }
}

void PlainTextEditFindBarTest::shouldReplaceAllText()
{
    QFETCH(QString, text);
    QFETCH(QString, searchText);
    QFETCH(QString, replaceText);
    QFETCH(KPIMTextEdit::TextEditFindBarBase::FindFlags, flags);
    QFETCH(int, nbElement);
    QFETCH(QString, resultStr);

    QPlainTextEdit edit;
    edit.setPlainText(text);
    PlainTextEditFindBarExample w(&edit);
    const int nbElementResult = KPIMTextEdit::FindUtils::replaceAll(&edit, searchText, replaceText, flags);
    QCOMPARE(edit.toPlainText(), resultStr);
    QCOMPARE(nbElementResult, nbElement);
}

#include "moc_plaintexteditfindbartest.cpp"
