/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

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
    Q_REQUIRED_RESULT QString category() const;
    void setCategory(const QString &newCategories);

protected:
    Q_REQUIRED_RESULT bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mCategorie;
};
}
