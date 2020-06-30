/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPIMTEXTEDIT_EMOTICONTEXTEDITACTION_H
#define KPIMTEXTEDIT_EMOTICONTEXTEDITACTION_H

#include "kpimtextedit_export.h"

#include <KActionMenu>

namespace KPIMTextEdit {
class EmoticonTextEditAction : public KActionMenu
{
    Q_OBJECT
public:
    explicit EmoticonTextEditAction(QObject *parent);
    ~EmoticonTextEditAction();

Q_SIGNALS:
    void emoticonActivated(const QString &);

private:
    class EmoticonTextEditActionPrivate;
    EmoticonTextEditActionPrivate *const d;
};
}

#endif /* KPIMTEXTEDIT_EMOTICONTEXTEDITACTION_H */
