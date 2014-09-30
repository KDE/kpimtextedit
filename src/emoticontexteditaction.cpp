/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
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
#include "emoticontexteditaction.h"
#include "emoticontexteditselector.h"

#include <KLocalizedString>
#include <QMenu>
#include <QIcon>

#include <QWidgetAction>

using namespace KPIMTextEdit;

class EmoticonTextEditAction::EmoticonTextEditActionPrivate
{
public:
    EmoticonTextEditActionPrivate()
    {
        emoticonMenu = new QMenu();
        selector = new EmoticonTextEditSelector(emoticonMenu);
        QWidgetAction *action = new QWidgetAction(emoticonMenu);
        action->setDefaultWidget(selector);
        emoticonMenu->addAction(action);
        connect(emoticonMenu, &QMenu::aboutToShow, selector, &EmoticonTextEditSelector::slotCreateEmoticonList);
    }

    ~EmoticonTextEditActionPrivate()
    {
        delete emoticonMenu;
    }

    QMenu *emoticonMenu;
    EmoticonTextEditSelector *selector;
};

EmoticonTextEditAction::EmoticonTextEditAction(QObject *parent)
    : KActionMenu(i18n("Add Smiley"), parent), d(new EmoticonTextEditActionPrivate())
{
    setMenu(d->emoticonMenu);
    setIcon(QIcon::fromTheme(QLatin1String("face-smile")));
    setDelayed(false);
    connect(d->selector, &EmoticonTextEditSelector::itemSelected, this, &EmoticonTextEditAction::emoticonActivated);
}

EmoticonTextEditAction::~EmoticonTextEditAction()
{
    delete d;
}
