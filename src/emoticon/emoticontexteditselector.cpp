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

#include "emoticonlistwidgetselector.h"
#include "emoticontexteditselector.h"
#include "textutils.h"
#include <KEmoticons>
#include <kemoticonstheme.h>

#include <QListWidget>
#include <QPixmap>
#include <QHBoxLayout>

// Use a static for this as calls to the KEmoticons constructor are expensive.
Q_GLOBAL_STATIC(KEmoticons, sEmoticons)

using namespace KPIMTextEdit;

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setSpacing(0);
    lay->setContentsMargins(0, 0, 0, 0);
    mListEmoticon = new EmoticonListWidgetSelector(this);
    lay->addWidget(mListEmoticon);
    connect(mListEmoticon, &EmoticonListWidgetSelector::itemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
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

void EmoticonTextEditSelector::slotCreateEmoticonList()
{
    if (mListEmoticon->count() == 0) {
        if (mEmojiPlainText) {
            const QList<uint> lstEmoji = TextUtils::unicodeFullEmoji();
            for (uint emoji : lstEmoji) {
                const QString str = QString::fromUcs4(&emoji, 1);
                new EmoticonTextEditItem(str, mListEmoticon);
            }
        } else {
            static QString cachedEmoticonsThemeName;
            if (cachedEmoticonsThemeName.isEmpty()) {
                cachedEmoticonsThemeName = KEmoticons::currentThemeName();
            }
            const QHash<QString, QStringList> list
                    = sEmoticons->theme(cachedEmoticonsThemeName).emoticonsMap();

            //Keep in sync with linklocator.cpp
            QStringList exclude;
            exclude << QStringLiteral("(c)") << QStringLiteral("(C)") << QStringLiteral("&gt;:-(") << QStringLiteral("&gt;:(") << QStringLiteral("(B)") << QStringLiteral("(b)") << QStringLiteral("(P)")
                    << QStringLiteral("(p)");
            exclude << QStringLiteral("(O)") << QStringLiteral("(o)") << QStringLiteral("(D)") << QStringLiteral("(d)") << QStringLiteral("(E)") << QStringLiteral("(e)") << QStringLiteral("(K)")
                    << QStringLiteral("(k)");
            exclude << QStringLiteral("(I)") << QStringLiteral("(i)") << QStringLiteral("(L)") << QStringLiteral("(l)") << QStringLiteral("(8)") << QStringLiteral("(T)") << QStringLiteral("(t)")
                    << QStringLiteral("(G)");
            exclude << QStringLiteral("(g)") << QStringLiteral("(F)") << QStringLiteral("(f)") << QStringLiteral("(H)");
            exclude << QStringLiteral("8)") << QStringLiteral("(N)") << QStringLiteral("(n)") << QStringLiteral("(Y)") << QStringLiteral("(y)") << QStringLiteral("(U)") << QStringLiteral("(u)")
                    << QStringLiteral("(W)") << QStringLiteral("(w)");

            const QHash<QString, QStringList>::const_iterator end = list.constEnd();
            for (QHash<QString, QStringList>::const_iterator it = list.constBegin(); it != end; ++it) {
                const QString str = it.value().first();
                if (!exclude.contains(str)) {
                    new EmoticonTextEditItem(str, it.key(), mListEmoticon);
                }
            }
        }
        mListEmoticon->setIconSize(QSize(32, 32));
    }
}

bool EmoticonTextEditSelector::emojiPlainText() const
{
    return mEmojiPlainText;
}

void EmoticonTextEditSelector::setEmojiPlainText(bool emojiPlainText)
{
    mEmojiPlainText = emojiPlainText;
    mListEmoticon->clear();
}
