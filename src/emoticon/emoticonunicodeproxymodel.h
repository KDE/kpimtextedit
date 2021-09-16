/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

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
    const QStringList &categories() const;
    void setCategories(const QStringList &newCategories);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QStringList mCategories;
};
}
