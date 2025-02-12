/*
  SPDX-FileCopyrightText: 2014-2025 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "inserttablewidget.h"
#include "kpimtextedit_debug.h"
#include <KLocalizedString>
#include <QComboBox>

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

using namespace KPIMTextEdit;

class InsertTableWidget::InsertTableWidgetPrivate
{
public:
    explicit InsertTableWidgetPrivate(InsertTableWidget *qq)
        : q(qq)
    {
        mRows = new QSpinBox;
        mRows->setMinimum(1);
        mRows->setValue(2);

        mColumns = new QSpinBox;
        mColumns->setMinimum(1);
        mColumns->setValue(2);

        mBorder = new QSpinBox;
        mBorder->setMinimum(0);
        mBorder->setValue(1);
        mBorder->setSuffix(i18n(" px"));

        auto gridLayout = new QGridLayout;
        gridLayout->setContentsMargins({});
        gridLayout->addWidget(new QLabel(i18nc("@label:textbox", "Rows:")), 0, 0);
        gridLayout->addWidget(mRows, 0, 1);

        gridLayout->addWidget(new QLabel(i18nc("@label:textbox", "Columns:")), 1, 0);
        gridLayout->addWidget(mColumns, 1, 1);

        gridLayout->addWidget(new QLabel(i18nc("@label:textbox", "Border:")), 2, 0);
        gridLayout->addWidget(mBorder, 2, 1);

        mTypeOfLength = new QComboBox;
        q->connect(mTypeOfLength, &QComboBox::activated, q, &InsertTableWidget::slotTypeOfLengthChanged);
        // xgettext: no-c-format
        mTypeOfLength->addItem(i18n("% of windows"), QTextLength::PercentageLength);
        mTypeOfLength->addItem(i18n("pixels"), QTextLength::FixedLength);
        mLength = new QSpinBox;
        mLength->setMinimum(1);
        mLength->setMaximum(100);
        mLength->setValue(100);

        gridLayout->addWidget(new QLabel(i18nc("@label:textbox", "Width:")), 3, 0);
        gridLayout->addWidget(mLength, 3, 1);
        gridLayout->addWidget(mTypeOfLength, 3, 2);
        q->setLayout(gridLayout);
    }

    QSpinBox *mColumns = nullptr;
    QSpinBox *mRows = nullptr;
    QSpinBox *mBorder = nullptr;
    QSpinBox *mLength = nullptr;
    QComboBox *mTypeOfLength = nullptr;

    InsertTableWidget *const q;
};

InsertTableWidget::InsertTableWidget(QWidget *parent)
    : QWidget(parent)
    , d(new InsertTableWidgetPrivate(this))
{
}

InsertTableWidget::~InsertTableWidget() = default;

void InsertTableWidget::slotTypeOfLengthChanged(int index)
{
    switch (index) {
    case 0:
        d->mLength->setMaximum(100);
        d->mLength->setValue(qMin(d->mLength->value(), 100));
        break;
    case 1:
        d->mLength->setMaximum(9999);
        break;
    default:
        qCDebug(KPIMTEXTEDIT_LOG) << " index not defined " << index;
        break;
    }
}

QTextLength::Type InsertTableWidget::typeOfLength() const
{
    return static_cast<QTextLength::Type>(d->mTypeOfLength->itemData(d->mTypeOfLength->currentIndex()).toInt());
}

void InsertTableWidget::setTypeOfLength(QTextLength::Type type)
{
    const int index = d->mTypeOfLength->findData(QVariant(type));
    d->mTypeOfLength->setCurrentIndex(index);
    slotTypeOfLengthChanged(index);
}

int InsertTableWidget::length() const
{
    return d->mLength->value();
}

void InsertTableWidget::setLength(int val)
{
    d->mLength->setValue(val);
}

void InsertTableWidget::setColumns(int col)
{
    d->mColumns->setValue(col);
}

void InsertTableWidget::setRows(int rows)
{
    d->mRows->setValue(rows);
}

void InsertTableWidget::setBorder(int border)
{
    d->mBorder->setValue(border);
}

int InsertTableWidget::columns() const
{
    return d->mColumns->value();
}

int InsertTableWidget::rows() const
{
    return d->mRows->value();
}

int InsertTableWidget::border() const
{
    return d->mBorder->value();
}

#include "moc_inserttablewidget.cpp"
