/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "tableformatdialog.h"
#include "inserttablewidget.h"

#include <KColorButton>
#include <KLocalizedString>
#include <KSeparator>
#include <QComboBox>

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace KPIMTextEdit;

class TableFormatDialog::TableFormatDialogPrivate
{
public:
    TableFormatDialogPrivate(TableFormatDialog *qq)
        : q(qq)
    {
        q->setWindowTitle(i18nc("@title:window", "Table Format"));
        auto mainLayout = new QVBoxLayout(q);

        QWidget *page = new QWidget(q);
        auto lay = new QVBoxLayout(page);
        lay->setContentsMargins(0, 0, 0, 0);
        tableWidget = new InsertTableWidget;
        lay->addWidget(tableWidget);

        KSeparator *sep = new KSeparator;
        lay->addWidget(sep);

        auto hbox = new QHBoxLayout;
        QLabel *lab = new QLabel(i18n("Spacing:"));
        hbox->addWidget(lab);
        spacing = new QSpinBox;
        spacing->setMinimum(0);
        hbox->addWidget(spacing);
        lab = new QLabel(i18n("pixels between cells"));
        hbox->addWidget(lab);
        lay->addLayout(hbox);

        hbox = new QHBoxLayout;
        lab = new QLabel(i18n("Padding:"));
        hbox->addWidget(lab);
        padding = new QSpinBox;
        padding->setMinimum(0);
        hbox->addWidget(padding);
        lab = new QLabel(i18n("pixels between cell border and content"));
        hbox->addWidget(lab);
        lay->addLayout(hbox);

        sep = new KSeparator;
        lay->addWidget(sep);

        alignment = new QComboBox;
        alignment->addItem(i18n("Left"), Qt::AlignLeft);
        alignment->addItem(i18n("Right"), Qt::AlignRight);
        alignment->addItem(i18n("Center"), Qt::AlignHCenter);
        alignment->addItem(i18n("Justify"), Qt::AlignJustify);

        hbox = new QHBoxLayout;
        lab = new QLabel(i18n("Table Alignment:"));
        hbox->addWidget(lab);
        hbox->addWidget(alignment);

        lay->addLayout(hbox);

        sep = new KSeparator;
        lay->addWidget(sep);

        hbox = new QHBoxLayout;
        useBackgroundColor = new QCheckBox(i18n("Background Color:"));

        hbox->addWidget(useBackgroundColor);
        backgroundColor = new KColorButton;
        backgroundColor->setDefaultColor(Qt::white);
        hbox->addWidget(backgroundColor);
        lay->addLayout(hbox);

        sep = new KSeparator;
        lay->addWidget(sep);
        backgroundColor->setEnabled(false);
        q->connect(useBackgroundColor, &QCheckBox::toggled, backgroundColor, &KColorButton::setEnabled);
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, q);
        connect(buttonBox, &QDialogButtonBox::accepted, q, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, q, &QDialog::reject);

        mainLayout->addWidget(page);
        mainLayout->addWidget(buttonBox);
    }

    QCheckBox *useBackgroundColor = nullptr;
    KColorButton *backgroundColor = nullptr;
    QComboBox *alignment = nullptr;
    QSpinBox *spacing = nullptr;
    QSpinBox *padding = nullptr;
    KPIMTextEdit::InsertTableWidget *tableWidget = nullptr;
    TableFormatDialog *const q;
};

TableFormatDialog::TableFormatDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TableFormatDialogPrivate(this))
{
}

TableFormatDialog::~TableFormatDialog()
{
    delete d;
}

int TableFormatDialog::columns() const
{
    return d->tableWidget->columns();
}

int TableFormatDialog::rows() const
{
    return d->tableWidget->rows();
}

int TableFormatDialog::border() const
{
    return d->tableWidget->border();
}

void TableFormatDialog::setColumns(int col)
{
    d->tableWidget->setColumns(col);
}

void TableFormatDialog::setRows(int row)
{
    d->tableWidget->setRows(row);
}

void TableFormatDialog::setBorder(int border)
{
    d->tableWidget->setBorder(border);
}

int TableFormatDialog::padding() const
{
    return d->padding->value();
}

void TableFormatDialog::setPadding(int value)
{
    d->padding->setValue(value);
}

int TableFormatDialog::spacing() const
{
    return d->spacing->value();
}

void TableFormatDialog::setSpacing(int value)
{
    d->spacing->setValue(value);
}

void TableFormatDialog::setAlignment(Qt::Alignment alignment)
{
    d->alignment->setCurrentIndex(d->alignment->findData(QVariant(alignment)));
}

Qt::Alignment TableFormatDialog::alignment() const
{
    return static_cast<Qt::Alignment>(d->alignment->itemData(d->alignment->currentIndex()).toInt());
}

QTextLength::Type TableFormatDialog::typeOfLength() const
{
    return d->tableWidget->typeOfLength();
}

int TableFormatDialog::length() const
{
    return d->tableWidget->length();
}

void TableFormatDialog::setLength(int val)
{
    d->tableWidget->setLength(val);
}

void TableFormatDialog::setTypeOfLength(QTextLength::Type type)
{
    d->tableWidget->setTypeOfLength(type);
}

QColor TableFormatDialog::tableBackgroundColor() const
{
    return d->backgroundColor->color();
}

void TableFormatDialog::setTableBackgroundColor(const QColor &col)
{
    d->backgroundColor->setColor(col);
    d->useBackgroundColor->setChecked(true);
}

bool TableFormatDialog::useBackgroundColor() const
{
    return d->useBackgroundColor->isChecked();
}
