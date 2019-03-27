/*
  Copyright (c) 2012-2019 Montel Laurent <montel@kde.org>
  based on code from kopete

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#include "emoticontexteditselector.h"
#ifdef USE_UNICODE_TAB
#include "emoticonunicodetab.h"
#else
#include "emoticonlistwidgetselector.h"
#endif
#include "textutils.h"
#include <KEmoticons>
#include <kemoticonstheme.h>

#include <QListWidget>
#include <QPixmap>
#include <QHBoxLayout>

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
#ifdef USE_UNICODE_TAB
    mUnicodeTab = new EmoticonUnicodeTab(this);
    lay->addWidget(mUnicodeTab);
    connect(mUnicodeTab, &EmoticonUnicodeTab::itemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
#else
    mListEmoticon = new EmoticonListWidgetSelector(this);
    lay->addWidget(mListEmoticon);
    connect(mListEmoticon, &EmoticonListWidgetSelector::itemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
#endif
}

EmoticonTextEditSelector::~EmoticonTextEditSelector()
{
}

void EmoticonTextEditSelector::slotItemSelected(const QString &str)
{
    Q_EMIT itemSelected(str);
    if (isVisible() && parentWidget()
        && parentWidget()->inherits("QMenu")) {
        parentWidget()->close();
    }
}

void EmoticonTextEditSelector::loadEmoticons()
{
#ifdef USE_UNICODE_TAB
    if (mUnicodeTab->count() == 0) {
        mUnicodeTab->loadEmoticons();
    }
#else
    if (mListEmoticon->count() == 0) {
        if (mEmojiPlainText) {
            mListEmoticon->setEmoticons(TextUtils::unicodeFullEmoji());
        } else {
            mListEmoticon->loadEmotionsFromTheme();
        }
    }
#endif
}

bool EmoticonTextEditSelector::emojiPlainText() const
{
#ifdef USE_UNICODE_TAB
    return mUnicodeTab->emojiPlainText();
#else
    return mEmojiPlainText;
#endif
}

void EmoticonTextEditSelector::setEmojiPlainText(bool emojiPlainText)
{
#ifdef USE_UNICODE_TAB
    mUnicodeTab->setEmojiPlainText(emojiPlainText);
#else
    if (mEmojiPlainText != emojiPlainText) {
        mEmojiPlainText = emojiPlainText;
        mListEmoticon->clear();
    }
#endif
}
