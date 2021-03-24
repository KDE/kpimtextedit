/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"

#include <KActionMenu>

namespace KPIMTextEdit
{
class EmoticonTextEditAction : public KActionMenu
{
    Q_OBJECT
public:
    explicit EmoticonTextEditAction(QObject *parent);
    ~EmoticonTextEditAction() override;

Q_SIGNALS:
    void emoticonActivated(const QString &);

private:
    class EmoticonTextEditActionPrivate;
    EmoticonTextEditActionPrivate *const d;
};
}

