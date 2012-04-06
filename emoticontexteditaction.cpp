/* -*- mode: C++; c-file-style: "gnu" -*-

  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "emoticontexteditaction.h"
#include "emoticontexteditselector.h"
#include <KLocale>

#include <KMenu>

#include <QWidgetAction>

using namespace KPIMTextEdit;
class EmoticonTextEditAction::EmoticonTextEditActionPrivate
{
public:
  EmoticonTextEditActionPrivate() {
    emoticonMenu = new KMenu();
    selector = new EmoticonTextEditSelector( emoticonMenu );
    QWidgetAction *action = new QWidgetAction(emoticonMenu);
    action->setDefaultWidget(selector);
    emoticonMenu->addAction(action);
    connect( emoticonMenu, SIGNAL(aboutToShow()), selector, SLOT(slotCreateEmoticonList()) );

  }
  ~EmoticonTextEditActionPrivate() {
    delete emoticonMenu;
  }

  KMenu *emoticonMenu;
  EmoticonTextEditSelector *selector;
};

EmoticonTextEditAction::EmoticonTextEditAction( QObject * parent )
  : KActionMenu( i18n( "Add Smiley" ), parent ), d( new EmoticonTextEditActionPrivate() )
{
  setMenu( d->emoticonMenu );
  setIcon( KIcon(QLatin1String( "emoticon" )) );
  setShortcutConfigurable( false );
  connect( d->selector, SIGNAL(itemSelected(QString)),
           this, SIGNAL(emoticonActivated(QString)) );

}

EmoticonTextEditAction::~EmoticonTextEditAction()
{
  delete d;
}

#include "emoticontexteditaction.moc"
