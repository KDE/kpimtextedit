/*
  SPDX-FileCopyrightText: 2019-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "plaintextmarkupbuilder.h"
using namespace Qt::Literals::StringLiterals;

#include <QDebug>
namespace KPIMTextEdit
{
class PlainTextMarkupBuilderPrivate
{
public:
    explicit PlainTextMarkupBuilderPrivate(PlainTextMarkupBuilder *b)
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
    [[nodiscard]] QString getLetterString(int itemNumber);

    [[nodiscard]] QString getRomanString(int itemNumber);

    /**
    Gets a block of references in the body of the text.
    This is an ordered list of links and images in the text.
  */
    [[nodiscard]] QString getReferences();

    QStringList mUrls;
    QList<QTextListFormat::Style> currentListItemStyles;
    QList<int> currentListItemNumbers;

    QString activeLink;

    QString mText;

    QString mQuoteprefix;

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
        auto romanSymbols = u"iiivixxxlxcccdcmmmm"_s;
        const int c[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
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
        result = u"?"_s;
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
        letterString.prepend(u"%1"_s.arg((itemNumber % LETTERSINALPHABET) + DIGITSOFFSET,
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
    if (!mUrls.isEmpty()) {
        refs.append(u"\n--------\n"_s);

        int index = 1;
        while (!mUrls.isEmpty()) {
            refs.append(u"[%1] %2\n"_s.arg(index++).arg(mUrls.takeFirst()));
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
    d->mQuoteprefix = prefix;
}

void PlainTextMarkupBuilder::beginStrong()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'*');
}

void PlainTextMarkupBuilder::endStrong()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'*');
}

void PlainTextMarkupBuilder::beginEmph()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'/');
}

void PlainTextMarkupBuilder::endEmph()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'/');
}

void PlainTextMarkupBuilder::beginUnderline()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'_');
}

void PlainTextMarkupBuilder::endUnderline()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'_');
}

void PlainTextMarkupBuilder::beginStrikeout()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'-');
}

void PlainTextMarkupBuilder::endStrikeout()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'-');
}

void PlainTextMarkupBuilder::beginAnchor(const QString &href, [[maybe_unused]] const QString &name)
{
    Q_D(PlainTextMarkupBuilder);
    if (!d->mUrls.contains(href)) {
        d->mUrls.append(href);
    }
    d->activeLink = href;
}

void PlainTextMarkupBuilder::endAnchor()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u"[%1]"_s.arg(d->mUrls.indexOf(d->activeLink) + 1));
}

void PlainTextMarkupBuilder::endParagraph()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'\n');
}

void PlainTextMarkupBuilder::addNewline()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'\n');
}

void PlainTextMarkupBuilder::insertHorizontalRule([[maybe_unused]] int width)
{
    Q_D(PlainTextMarkupBuilder);

    d->mText.append(u"--------------------\n"_s);
}

int PlainTextMarkupBuilder::addReference(const QString &reference)
{
    Q_D(PlainTextMarkupBuilder);

    if (!d->mUrls.contains(reference)) {
        d->mUrls.append(reference);
    }
    return d->mUrls.indexOf(reference) + 1;
}

void PlainTextMarkupBuilder::insertImage(const QString &src, [[maybe_unused]] qreal width, [[maybe_unused]] qreal height)
{
    Q_D(PlainTextMarkupBuilder);

    const auto ref = addReference(src);

    d->mText.append(u"[%1]"_s.arg(ref));
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
        d->mText.append(u"    "_s);
    }

    auto itemNumber = d->currentListItemNumbers.constLast();

    switch (d->currentListItemStyles.constLast()) {
    case QTextListFormat::ListDisc:
        d->mText.append(u" *  "_s);
        break;
    case QTextListFormat::ListCircle:
        d->mText.append(u" o  "_s);
        break;
    case QTextListFormat::ListSquare:
        d->mText.append(u" -  "_s);
        break;
    case QTextListFormat::ListDecimal:
        d->mText.append(u" %1. "_s.arg(itemNumber + 1));
        break;
    case QTextListFormat::ListLowerAlpha:
        d->mText.append(u" %1. "_s.arg(d->getLetterString(itemNumber)));
        break;
    case QTextListFormat::ListUpperAlpha:
        d->mText.append(u" %1. "_s.arg(d->getLetterString(itemNumber).toUpper()));
        break;
    case QTextListFormat::ListLowerRoman:
        d->mText.append(u" %1. "_s.arg(d->getRomanString(itemNumber + 1)));
        break;
    case QTextListFormat::ListUpperRoman:
        d->mText.append(u" %1. "_s.arg(d->getRomanString(itemNumber + 1).toUpper()));
        break;
    default:
        break;
    }
}

void PlainTextMarkupBuilder::endListItem()
{
    Q_D(PlainTextMarkupBuilder);
    d->currentListItemNumbers.last() = d->currentListItemNumbers.last() + 1;
    d->mText.append(u'\n');
}

void PlainTextMarkupBuilder::beginSuperscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u"^{"_s);
}

void PlainTextMarkupBuilder::endSuperscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'}');
}

void PlainTextMarkupBuilder::beginSubscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u"_{"_s);
}

void PlainTextMarkupBuilder::endSubscript()
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(u'}');
}

void PlainTextMarkupBuilder::appendLiteralText(const QString &text)
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(text);
}

void PlainTextMarkupBuilder::appendRawText(const QString &text)
{
    Q_D(PlainTextMarkupBuilder);
    d->mText.append(text);
}

QString PlainTextMarkupBuilder::getResult()
{
    Q_D(PlainTextMarkupBuilder);
    auto ret = d->mText;
    ret.append(d->getReferences());
    d->mText.clear();
    return ret;
}

void PlainTextMarkupBuilder::beginParagraph([[maybe_unused]] Qt::Alignment a,
                                            qreal top,
                                            qreal bottom,
                                            qreal left,
                                            qreal right,
                                            [[maybe_unused]] bool leftToRightText)
{
    Q_D(PlainTextMarkupBuilder);
    if (isQuoteBlock(top, bottom, left, right)) {
        d->mText.append(d->mQuoteprefix);
    }
}

bool PlainTextMarkupBuilder::isQuoteBlock([[maybe_unused]] qreal top, [[maybe_unused]] qreal bottom, qreal left, qreal right) const
{
    return /*(top == 12) && (bottom == 12) &&*/ (left == 40) && (right == 40);
}

void PlainTextMarkupBuilder::beginBackground([[maybe_unused]] const QBrush &brush)
{
}

void PlainTextMarkupBuilder::beginFontFamily([[maybe_unused]] const QString &family)
{
}

void PlainTextMarkupBuilder::beginFontPointSize([[maybe_unused]] int size)
{
}

void PlainTextMarkupBuilder::beginForeground([[maybe_unused]] const QBrush &brush)
{
}

void PlainTextMarkupBuilder::beginHeader(int level)
{
    Q_D(PlainTextMarkupBuilder);
    switch (level) {
    case 1:
        d->mText.append(u"# "_s);
        break;
    case 2:
        d->mText.append(u"## "_s);
        break;
    case 3:
        d->mText.append(u"### "_s);
        break;
    case 4:
        d->mText.append(u"#### "_s);
        break;
    case 5:
        d->mText.append(u"##### "_s);
        break;
    case 6:
        d->mText.append(u"###### "_s);
        break;
    default:
        break;
    }
}

void PlainTextMarkupBuilder::beginTable([[maybe_unused]] qreal cellpadding, [[maybe_unused]] qreal cellspacing, [[maybe_unused]] const QString &width)
{
}

void PlainTextMarkupBuilder::beginTableCell([[maybe_unused]] const QString &width, [[maybe_unused]] int colSpan, [[maybe_unused]] int rowSpan)
{
}

void PlainTextMarkupBuilder::beginTableHeaderCell([[maybe_unused]] const QString &width, [[maybe_unused]] int colSpan, [[maybe_unused]] int rowSpan)
{
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
        d->mText.append(u" #\n"_s);
        break;
    case 2:
        d->mText.append(u" ##\n"_s);
        break;
    case 3:
        d->mText.append(u" ###\n"_s);
        break;
    case 4:
        d->mText.append(u" ####\n"_s);
        break;
    case 5:
        d->mText.append(u" #####\n"_s);
        break;
    case 6:
        d->mText.append(u" ######\n"_s);
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
    d->mText.append(u'\n');
}
