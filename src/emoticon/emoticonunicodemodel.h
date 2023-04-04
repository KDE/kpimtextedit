/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticonunicodeutils.h"
#include "kpimtextedit_private_export.h"
#include <QAbstractListModel>
namespace KPIMTextEdit
{
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonUnicodeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum EmoticonsRoles {
        UnicodeEmoji = Qt::UserRole + 1,
        Identifier,
        Category,
    };
    Q_ENUM(EmoticonsRoles)

    explicit EmoticonUnicodeModel(QObject *parent = nullptr);
    ~EmoticonUnicodeModel() override;

    Q_REQUIRED_RESULT int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_REQUIRED_RESULT QVariant data(const QModelIndex &index, int role) const override;

    Q_REQUIRED_RESULT const QList<EmoticonUnicodeUtils::EmoticonStruct> &emoticonList() const;
    void setEmoticonList(const QList<EmoticonUnicodeUtils::EmoticonStruct> &newEmoticonList);

private:
    Q_DISABLE_COPY(EmoticonUnicodeModel)
    QList<EmoticonUnicodeUtils::EmoticonStruct> mEmoticonList;
};
}
