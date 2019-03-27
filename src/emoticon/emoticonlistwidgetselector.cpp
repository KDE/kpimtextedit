/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

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
#include <KEmoticons>
#include <kemoticonstheme.h>
// Use a static for this as calls to the KEmoticons constructor are expensive.
Q_GLOBAL_STATIC(KEmoticons, sEmoticons)

using namespace KPIMTextEdit;

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, QListWidget *parent)
    : QListWidgetItem(parent)
{
    mText = emoticonText;
    setText(mText);
    setToolTip(mText);
}

EmoticonTextEditItem::EmoticonTextEditItem(const QString &emoticonText, const QString &pixmapPath, QListWidget *parent)
    : QListWidgetItem(parent)
{
    mText = emoticonText;
    mPixmapPath = pixmapPath;
    QPixmap p(mPixmapPath);
    // Some of the custom icons are rather large
    // so lets limit them to a maximum size for this display panel
    //

    //TODO need to fix hdpi support here.
    if (p.width() > 32 || p.height() > 32) {
        p = p.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    }

    setIcon(p);
    setToolTip(mText);
}

QString EmoticonTextEditItem::text() const
{
    return mText;
}

QString EmoticonTextEditItem::pixmapPath() const
{
    return mPixmapPath;
}

EmoticonListWidgetSelector::EmoticonListWidgetSelector(QWidget *parent)
    : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    connect(this, &EmoticonListWidgetSelector::itemEntered, this, &EmoticonListWidgetSelector::slotMouseOverItem);
    connect(this, &EmoticonListWidgetSelector::itemClicked, this, &EmoticonListWidgetSelector::slotEmoticonClicked);
    setIconSize(QSize(32, 32));
}

EmoticonListWidgetSelector::~EmoticonListWidgetSelector()
{
}

void EmoticonListWidgetSelector::slotMouseOverItem(QListWidgetItem *item)
{
    item->setSelected(true);
    if (!hasFocus()) {
        setFocus();
    }
}

void EmoticonListWidgetSelector::loadEmotionsFromTheme()
{
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
            new EmoticonTextEditItem(str, it.key(), this);
        }
    }
}

void EmoticonListWidgetSelector::setEmoticons(const QList<uint> &lst)
{
    for (uint emoji : lst) {
        const QString str = QString::fromUcs4(&emoji, 1);
        new KPIMTextEdit::EmoticonTextEditItem(str, this);
    }
}

void EmoticonListWidgetSelector::slotEmoticonClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    EmoticonTextEditItem *itemEmoticon = static_cast<EmoticonTextEditItem *>(item);

    Q_EMIT itemSelected(itemEmoticon->text());
}
