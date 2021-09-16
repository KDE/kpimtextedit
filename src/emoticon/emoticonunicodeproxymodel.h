/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "emoticonunicodeutils.h"
#include "kpimtextedit_private_export.h"
#include <QSortFilterProxyModel>
namespace KPIMTextEdit
{
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonUnicodeProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeProxyModel(QObject *parent = nullptr);
    ~EmoticonUnicodeProxyModel() override;
    EmoticonUnicodeUtils::EmoticonStruct::EmoticonType categories() const;
    void setCategories(EmoticonUnicodeUtils::EmoticonStruct::EmoticonType newCategories);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    EmoticonUnicodeUtils::EmoticonStruct::EmoticonType mCategories = EmoticonUnicodeUtils::EmoticonStruct::Unknown;
};
}
