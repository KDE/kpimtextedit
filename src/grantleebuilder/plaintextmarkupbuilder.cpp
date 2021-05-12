/*
  SPDX-FileCopyrightText: 2019-2020 Montel Laurent <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "plaintextmarkupbuilder.h"
#include <QDebug>
namespace KPIMTextEdit
{
class PlainTextMarkupBuilderPrivate
{
public:
    PlainTextMarkupBuilderPrivate(PlainTextMarkupBuilder *b)
        : q_ptr(b)
    {
    }

    /**
    Get a letter string to represent a number.

    The numbers 1-26 are represented by a-z, and 27-52 by aa-az, 53-79 by
    ba-bz
    etc.

    @param The number to convert
    @return The letter string representation of the number.
  */
    Q_REQUIRED_RESULT QString getLetterString(int itemNumber);

    Q_REQUIRED_RESULT QString getRomanString(int itemNumber);

    /**
    Gets a block of references in the body of the text.
    This is an ordered list of links and images in the text.
  */
    Q_REQUIRED_RESULT QString getReferences();

    QStringList m_urls;
    QList<QTextListFormat::Style> currentListItemStyles;
    QList<int> currentListItemNumbers;

    QString activeLink;

    QString m_text;

    QString m_quoteprefix;

    PlainTextMarkupBuilder *const q_ptr;

    Q_DECLARE_PUBLIC(PlainTextMarkupBuilder)
};
}

using namespace KPIMTextEdit;
PlainTextMarkupBuilder::PlainTextMarkupBuilder()
    : d_ptr(new PlainTextMarkupBuilderPrivate(this))
{
}

QString PlainTextMarkupBuilderPrivate::getRomanString(int item)
{
    QString result;
    // Code based to gui/text/qtextlist.cpp
    if (item < 5000) {
        QString romanNumeral;

        // works for up to 4999 items
        auto romanSymbols = QStringLiteral("iiivixxxlxcccdcmmmm");
        int c[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
        auto n = item;
        for (auto i = 12; i >= 0; n %= c[i], i--) {
            auto q = n / c[i];
            if (q > 0) {
                auto startDigit = i + (i + 3) / 4;
                int numDigits;
                if (i % 4) {
                    // c[i] == 4|5|9|40|50|90|400|500|900
                    if ((i - 2) % 4) {
                        // c[i] == 4|9|40|90|400|900 => with subtraction (IV,
                        // IX, XL, XC,
                        // ...)
                        numDigits = 2;
                    } else {
                        // c[i] == 5|50|500 (V, L, D)
                        numDigits = 1;
                    }
                } else {
                    // c[i] == 1|10|100|1000 (I, II, III, X, XX, ...)
                    numDigits = q;
                }

                romanNumeral.append(QStringView(romanSymbols).mid(startDigit, numDigits).toString());
            }
        }
        result = romanNumeral;
    } else {
        result = QStringLiteral("?");
    }
    return result;
}

QString PlainTextMarkupBuilderPrivate::getLetterString(int itemNumber)
{
    QString letterString;
    while (true) {
        // Create the letter string by prepending one char at a time.
        // The itemNumber is converted to a number in the base 36 (number of
        // letters
        // in the
        // alphabet plus 10) after being increased by 10 (to pass out the digits
        // 0
        // to 9).
        letterString.prepend(QStringLiteral("%1").arg((itemNumber % LETTERSINALPHABET) + DIGITSOFFSET,
                                                      0, // no padding while building this string.
                                                      LETTERSINALPHABET + DIGITSOFFSET));
        if ((itemNumber >= LETTERSINALPHABET)) {
            itemNumber = itemNumber / LETTERSINALPHABET;
            itemNumber--;
        } else {
            break;
        }
    }
    return letterString;
}

QString PlainTextMarkupBuilderPrivate::getReferences()
{
    QString refs;
    if (!m_urls.isEmpty()) {
        refs.append(QStringLiteral("\n--------\n"));

        int index = 1;
        while (!m_urls.isEmpty()) {
            refs.append(QStringLiteral("[%1] %2\n").arg(index++).arg(m_urls.takeFirst()));
        }
    }
    return refs;
}

PlainTextMarkupBuilder::~PlainTextMarkupBuilder()
{
    delete d_ptr;
}

void PlainTextMarkupBuilder::setQuotePrefix(const QString &prefix)
{
    Q_D(PlainTextMarkupBuilder);
    d->m_quoteprefix = prefix;
}

void PlainTextMarkupBuilder::beginStrong()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('*'));
}

void PlainTextMarkupBuilder::endStrong()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('*'));
}

void PlainTextMarkupBuilder::beginEmph()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('/'));
}

void PlainTextMarkupBuilder::endEmph()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('/'));
}

void PlainTextMarkupBuilder::beginUnderline()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('_'));
}

void PlainTextMarkupBuilder::endUnderline()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('_'));
}

void PlainTextMarkupBuilder::beginStrikeout()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('-'));
}

void PlainTextMarkupBuilder::endStrikeout()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('-'));
}

void PlainTextMarkupBuilder::beginAnchor(const QString &href, const QString &name)
{
    Q_D(PlainTextMarkupBuilder);
    Q_UNUSED(name)
    if (!d->m_urls.contains(href)) {
        d->m_urls.append(href);
    }
    d->activeLink = href;
}

void PlainTextMarkupBuilder::endAnchor()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QStringLiteral("[%1]").arg(d->m_urls.indexOf(d->activeLink) + 1));
}

void PlainTextMarkupBuilder::endParagraph()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('\n'));
}

void PlainTextMarkupBuilder::addNewline()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('\n'));
}

void PlainTextMarkupBuilder::insertHorizontalRule(int width)
{
    Q_UNUSED(width)
    Q_D(PlainTextMarkupBuilder);

    d->m_text.append(QStringLiteral("--------------------\n"));
}

int PlainTextMarkupBuilder::addReference(const QString &reference)
{
    Q_D(PlainTextMarkupBuilder);

    if (!d->m_urls.contains(reference))
        d->m_urls.append(reference);
    return d->m_urls.indexOf(reference) + 1;
}

void PlainTextMarkupBuilder::insertImage(const QString &src, qreal width, qreal height)
{
    Q_D(PlainTextMarkupBuilder);
    Q_UNUSED(width)
    Q_UNUSED(height)

    const auto ref = addReference(src);

    d->m_text.append(QStringLiteral("[%1]").arg(ref));
}

void PlainTextMarkupBuilder::beginList(QTextListFormat::Style style)
{
    Q_D(PlainTextMarkupBuilder);
    d->currentListItemStyles.append(style);
    d->currentListItemNumbers.append(0);
}

void PlainTextMarkupBuilder::endList()
{
    Q_D(PlainTextMarkupBuilder);
    if (!d->currentListItemNumbers.isEmpty()) {
        d->currentListItemStyles.removeLast();
        d->currentListItemNumbers.removeLast();
    }
}

void PlainTextMarkupBuilder::beginListItem()
{
    Q_D(PlainTextMarkupBuilder);
    for (int i = 0, total = d->currentListItemNumbers.size(); i < total; ++i) {
        d->m_text.append(QStringLiteral("    "));
    }

    auto itemNumber = d->currentListItemNumbers.last();

    switch (d->currentListItemStyles.last()) {
    case QTextListFormat::ListDisc:
        d->m_text.append(QStringLiteral(" *  "));
        break;
    case QTextListFormat::ListCircle:
        d->m_text.append(QStringLiteral(" o  "));
        break;
    case QTextListFormat::ListSquare:
        d->m_text.append(QStringLiteral(" -  "));
        break;
    case QTextListFormat::ListDecimal:
        d->m_text.append(QStringLiteral(" %1. ").arg(itemNumber + 1));
        break;
    case QTextListFormat::ListLowerAlpha:
        d->m_text.append(QStringLiteral(" %1. ").arg(d->getLetterString(itemNumber)));
        break;
    case QTextListFormat::ListUpperAlpha:
        d->m_text.append(QStringLiteral(" %1. ").arg(d->getLetterString(itemNumber).toUpper()));
        break;
    case QTextListFormat::ListLowerRoman:
        d->m_text.append(QStringLiteral(" %1. ").arg(d->getRomanString(itemNumber + 1)));
        break;
    case QTextListFormat::ListUpperRoman:
        d->m_text.append(QStringLiteral(" %1. ").arg(d->getRomanString(itemNumber + 1).toUpper()));
        break;
    default:
        break;
    }
}

void PlainTextMarkupBuilder::endListItem()
{
    Q_D(PlainTextMarkupBuilder);
    d->currentListItemNumbers.last() = d->currentListItemNumbers.last() + 1;
    d->m_text.append(QLatin1Char('\n'));
}

void PlainTextMarkupBuilder::beginSuperscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QStringLiteral("^{"));
}

void PlainTextMarkupBuilder::endSuperscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('}'));
}

void PlainTextMarkupBuilder::beginSubscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QStringLiteral("_{"));
}

void PlainTextMarkupBuilder::endSubscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('}'));
}

void PlainTextMarkupBuilder::appendLiteralText(const QString &text)
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(text);
}

void PlainTextMarkupBuilder::appendRawText(const QString &text)
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(text);
}

QString PlainTextMarkupBuilder::getResult()
{
    Q_D(PlainTextMarkupBuilder);
    auto ret = d->m_text;
    ret.append(d->getReferences());
    d->m_text.clear();
    return ret;
}

void PlainTextMarkupBuilder::beginParagraph(Qt::Alignment a, qreal top, qreal bottom, qreal left, qreal right, bool leftToRightText)
{
    Q_UNUSED(a)
    Q_UNUSED(leftToRightText)
    Q_D(PlainTextMarkupBuilder);
    if (isQuoteBlock(top, bottom, left, right)) {
        d->m_text.append(d->m_quoteprefix);
    }
}

bool PlainTextMarkupBuilder::isQuoteBlock(qreal top, qreal bottom, qreal left, qreal right) const
{
    Q_UNUSED(top)
    Q_UNUSED(bottom)
    return /*(top == 12) && (bottom == 12) &&*/ (left == 40) && (right == 40);
}

void PlainTextMarkupBuilder::beginBackground(const QBrush &brush)
{
    Q_UNUSED(brush)
}

void PlainTextMarkupBuilder::beginFontFamily(const QString &family)
{
    Q_UNUSED(family)
}

void PlainTextMarkupBuilder::beginFontPointSize(int size)
{
    Q_UNUSED(size)
}

void PlainTextMarkupBuilder::beginForeground(const QBrush &brush)
{
    Q_UNUSED(brush)
}

void PlainTextMarkupBuilder::beginHeader(int level)
{
    Q_D(PlainTextMarkupBuilder);
    switch (level) {
    case 1:
        d->m_text.append(QStringLiteral("# "));
        break;
    case 2:
        d->m_text.append(QStringLiteral("## "));
        break;
    case 3:
        d->m_text.append(QStringLiteral("### "));
        break;
    case 4:
        d->m_text.append(QStringLiteral("#### "));
        break;
    case 5:
        d->m_text.append(QStringLiteral("##### "));
        break;
    case 6:
        d->m_text.append(QStringLiteral("###### "));
        break;
    default:
        break;
    }
}

void PlainTextMarkupBuilder::beginTable(qreal cellpadding, qreal cellspacing, const QString &width)
{
    Q_UNUSED(cellpadding)
    Q_UNUSED(cellspacing)
    Q_UNUSED(width)
}

void PlainTextMarkupBuilder::beginTableCell(const QString &width, int colSpan, int rowSpan)
{
    Q_UNUSED(width)
    Q_UNUSED(colSpan)
    Q_UNUSED(rowSpan)
}

void PlainTextMarkupBuilder::beginTableHeaderCell(const QString &width, int colSpan, int rowSpan)
{
    Q_UNUSED(width)
    Q_UNUSED(colSpan)
    Q_UNUSED(rowSpan)
}

void PlainTextMarkupBuilder::beginTableRow()
{
}

void PlainTextMarkupBuilder::endBackground()
{
}

void PlainTextMarkupBuilder::endFontFamily()
{
}

void PlainTextMarkupBuilder::endFontPointSize()
{
}

void PlainTextMarkupBuilder::endForeground()
{
}

void PlainTextMarkupBuilder::endHeader(int level)
{
    Q_D(PlainTextMarkupBuilder);
    qDebug() << " void PlainTextMarkupBuilder::endHeader(int level)" << level;
    switch (level) {
    case 1:
        d->m_text.append(QStringLiteral(" #\n"));
        break;
    case 2:
        d->m_text.append(QStringLiteral(" ##\n"));
        break;
    case 3:
        d->m_text.append(QStringLiteral(" ###\n"));
        break;
    case 4:
        d->m_text.append(QStringLiteral(" ####\n"));
        break;
    case 5:
        d->m_text.append(QStringLiteral(" #####\n"));
        break;
    case 6:
        d->m_text.append(QStringLiteral(" ######\n"));
        break;
    default:
        break;
    }
}

void PlainTextMarkupBuilder::endTable()
{
}

void PlainTextMarkupBuilder::endTableCell()
{
}

void PlainTextMarkupBuilder::endTableHeaderCell()
{
}

void PlainTextMarkupBuilder::endTableRow()
{
}

void PlainTextMarkupBuilder::addSingleBreakLine()
{
    Q_D(PlainTextMarkupBuilder);
    d->m_text.append(QLatin1Char('\n'));
}
