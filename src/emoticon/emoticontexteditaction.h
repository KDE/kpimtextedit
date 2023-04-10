/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

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
    void insertEmoticon(const QString &);

private:
    class EmoticonTextEditActionPrivate;
    std::unique_ptr<EmoticonTextEditActionPrivate> const d;
};
}
