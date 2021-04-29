/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "tableactionmenu.h"
#include "inserttabledialog.h"
#include "tablecellformatdialog.h"
#include "tableformatdialog.h"

#include <KLocalizedString>
#include <QAction>
#include <QIcon>

#include <QPointer>
#include <QTextEdit>
#include <QTextTable>

namespace KPIMTextEdit
{
class TableActionMenuPrivate
{
public:
    TableActionMenuPrivate(QTextEdit *edit, TableActionMenu *qq)
        : textEdit(edit)
        , q(qq)
    {
    }

    void _k_slotInsertRowBelow();
    void _k_slotInsertRowAbove();
    void _k_slotInsertColumnBefore();
    void _k_slotInsertColumnAfter();

    void _k_slotInsertTable();

    void _k_slotRemoveRowBelow();
    void _k_slotRemoveRowAbove();
    void _k_slotRemoveColumnBefore();
    void _k_slotRemoveColumnAfter();
    void _k_slotRemoveCellContents();

    void _k_slotMergeCell();
    void _k_slotMergeSelectedCells();
    void _k_slotTableFormat();
    void _k_slotTableCellFormat();
    void _k_slotSplitCell();
    void _k_updateActions(bool forceUpdate = false);

    QAction *actionInsertTable = nullptr;

    QAction *actionInsertRowBelow = nullptr;
    QAction *actionInsertRowAbove = nullptr;

    QAction *actionInsertColumnBefore = nullptr;
    QAction *actionInsertColumnAfter = nullptr;

    QAction *actionRemoveRowBelow = nullptr;
    QAction *actionRemoveRowAbove = nullptr;

    QAction *actionRemoveColumnBefore = nullptr;
    QAction *actionRemoveColumnAfter = nullptr;

    QAction *actionMergeCell = nullptr;
    QAction *actionMergeSelectedCells = nullptr;
    QAction *actionSplitCell = nullptr;

    QAction *actionTableFormat = nullptr;
    QAction *actionTableCellFormat = nullptr;

    QAction *actionRemoveCellContents = nullptr;

    QTextEdit *const textEdit;
    TableActionMenu *const q;
    bool richTextMode = false;
};

void TableActionMenuPrivate::_k_slotRemoveCellContents()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        const QTextTableCell cell = table->cellAt(textEdit->textCursor());
        if (cell.isValid()) {
            const QTextCursor firstCursor = cell.firstCursorPosition();
            const QTextCursor endCursor = cell.lastCursorPosition();
            QTextCursor cursor = textEdit->textCursor();
            cursor.beginEditBlock();
            cursor.setPosition(firstCursor.position());
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, endCursor.position() - firstCursor.position());
            cursor.removeSelectedText();
            cursor.endEditBlock();
        }
    }
}

void TableActionMenuPrivate::_k_slotRemoveRowBelow()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.row() < table->rows() - 1) {
                table->removeRows(cell.row(), 1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotRemoveRowAbove()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.row() >= 1) {
                table->removeRows(cell.row() - 1, 1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotRemoveColumnBefore()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.column() > 0) {
                table->removeColumns(cell.column() - 1, 1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotRemoveColumnAfter()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.column() < table->columns() - 1) {
                table->removeColumns(cell.column(), 1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertRowBelow()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.row() < table->rows()) {
                table->insertRows(cell.row() + 1, 1);
            } else {
                table->appendRows(1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertRowAbove()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->insertRows(cell.row(), 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertColumnBefore()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->insertColumns(cell.column(), 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertColumnAfter()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.column() < table->columns()) {
                table->insertColumns(cell.column() + 1, 1);
            } else {
                table->appendColumns(1);
            }
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertTable()
{
    if (richTextMode) {
        QPointer<InsertTableDialog> dialog = new InsertTableDialog(textEdit);
        if (dialog->exec()) {
            QTextCursor cursor = textEdit->textCursor();
            QTextTableFormat tableFormat;
            tableFormat.setBorder(dialog->border());
            const int numberOfColumns(dialog->columns());
            QVector<QTextLength> constrains;
            constrains.reserve(numberOfColumns);
            const QTextLength::Type type = dialog->typeOfLength();
            const int length = dialog->length();

            const QTextLength textlength(type, length / numberOfColumns);
            for (int i = 0; i < numberOfColumns; ++i) {
                constrains.append(textlength);
            }
            tableFormat.setColumnWidthConstraints(constrains);
            tableFormat.setAlignment(Qt::AlignLeft);
            QTextTable *table = cursor.insertTable(dialog->rows(), numberOfColumns);
            table->setFormat(tableFormat);
        }
        delete dialog;
    }
}

void TableActionMenuPrivate::_k_slotMergeCell()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->mergeCells(cell.row(), cell.column(), 1, cell.columnSpan() + 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotMergeSelectedCells()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            table->mergeCells(textEdit->textCursor());
        }
    }
}

void TableActionMenuPrivate::_k_slotTableFormat()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            QPointer<TableFormatDialog> dialog = new TableFormatDialog(textEdit);
            const int numberOfColumn(table->columns());
            const int numberOfRow(table->rows());
            dialog->setColumns(numberOfColumn);
            dialog->setRows(numberOfRow);
            QTextTableFormat tableFormat = table->format();
            dialog->setBorder(tableFormat.border());
            dialog->setSpacing(tableFormat.cellSpacing());
            dialog->setPadding(tableFormat.cellPadding());
            dialog->setAlignment(tableFormat.alignment());
            if (tableFormat.hasProperty(QTextFormat::BackgroundBrush)) {
                dialog->setTableBackgroundColor(tableFormat.background().color());
            }
            QVector<QTextLength> constrains = tableFormat.columnWidthConstraints();
            if (!constrains.isEmpty()) {
                dialog->setTypeOfLength(constrains.at(0).type());
                dialog->setLength(constrains.at(0).rawValue() * numberOfColumn);
            }

            if (dialog->exec()) {
                const int newNumberOfColumns(dialog->columns());
                if ((newNumberOfColumns != numberOfColumn) || (dialog->rows() != numberOfRow)) {
                    table->resize(dialog->rows(), newNumberOfColumns);
                }
                tableFormat.setBorder(dialog->border());
                tableFormat.setCellPadding(dialog->padding());
                tableFormat.setCellSpacing(dialog->spacing());
                tableFormat.setAlignment(dialog->alignment());

                QVector<QTextLength> constrains;
                constrains.reserve(newNumberOfColumns);
                const QTextLength::Type type = dialog->typeOfLength();
                const int length = dialog->length();

                const QTextLength textlength(type, length / newNumberOfColumns);
                for (int i = 0; i < newNumberOfColumns; ++i) {
                    constrains.append(textlength);
                }
                tableFormat.setColumnWidthConstraints(constrains);
                const QColor tableBackgroundColor = dialog->tableBackgroundColor();
                if (dialog->useBackgroundColor()) {
                    if (tableBackgroundColor.isValid()) {
                        tableFormat.setBackground(tableBackgroundColor);
                    }
                } else {
                    tableFormat.clearBackground();
                }
                table->setFormat(tableFormat);
            }
            delete dialog;
        }
    }
}

void TableActionMenuPrivate::_k_slotTableCellFormat()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            QTextTableCell cell = table->cellAt(textEdit->textCursor());
            QPointer<TableCellFormatDialog> dialog = new TableCellFormatDialog(textEdit);
            QTextTableCellFormat format = cell.format().toTableCellFormat();
            if (format.hasProperty(QTextFormat::BackgroundBrush)) {
                dialog->setTableCellBackgroundColor(format.background().color());
            }
            dialog->setVerticalAlignment(format.verticalAlignment());
            if (dialog->exec()) {
                if (dialog->useBackgroundColor()) {
                    const QColor tableCellColor = dialog->tableCellBackgroundColor();
                    if (tableCellColor.isValid()) {
                        format.setBackground(tableCellColor);
                    }
                } else {
                    format.clearBackground();
                }
                format.setVerticalAlignment(dialog->verticalAlignment());
                cell.setFormat(format);
            }
            delete dialog;
        }
    }
}

void TableActionMenuPrivate::_k_slotSplitCell()
{
    if (richTextMode) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.columnSpan() > 1 || cell.rowSpan() > 1) {
                table->splitCell(cell.row(), cell.column(), qMax(1, cell.rowSpan() - 1), qMax(1, cell.columnSpan() - 1));
                _k_updateActions();
            }
        }
    }
}

void TableActionMenuPrivate::_k_updateActions(bool forceUpdate)
{
    if ((richTextMode) || forceUpdate) {
        QTextTable *table = textEdit->textCursor().currentTable();
        const bool isTable = (table != nullptr);
        actionInsertRowBelow->setEnabled(isTable);
        actionInsertRowAbove->setEnabled(isTable);

        actionInsertColumnBefore->setEnabled(isTable);
        actionInsertColumnAfter->setEnabled(isTable);

        actionRemoveRowBelow->setEnabled(isTable);
        actionRemoveRowAbove->setEnabled(isTable);

        actionRemoveColumnBefore->setEnabled(isTable);
        actionRemoveColumnAfter->setEnabled(isTable);

        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());

            int firstRow = -1;
            int numRows = -1;
            int firstColumn = -1;
            int numColumns = -1;
            textEdit->textCursor().selectedTableCells(&firstRow, &numRows, &firstColumn, &numColumns);
            const bool hasSelectedTableCell = (firstRow != -1) && (numRows != -1) && (firstColumn != -1) && (numColumns != -1);
            if (cell.column() > table->columns() - 2) {
                actionMergeCell->setEnabled(false);
            } else {
                actionMergeCell->setEnabled(true);
            }
            if (cell.columnSpan() > 1 || cell.rowSpan() > 1) {
                actionSplitCell->setEnabled(true);
            } else {
                actionSplitCell->setEnabled(false);
            }
            actionTableCellFormat->setEnabled(true);
            actionMergeSelectedCells->setEnabled(hasSelectedTableCell);
        } else {
            actionSplitCell->setEnabled(false);
            actionMergeCell->setEnabled(false);
            actionMergeSelectedCells->setEnabled(false);
        }
        actionTableFormat->setEnabled(isTable);
        actionTableCellFormat->setEnabled(isTable);
        actionRemoveCellContents->setEnabled(isTable);
    }
}

TableActionMenu::TableActionMenu(QTextEdit *textEdit)
    : KActionMenu(textEdit)
    , d(new TableActionMenuPrivate(textEdit, this))
{
    KActionMenu *insertMenu = new KActionMenu(i18n("Insert"), this);
    addAction(insertMenu);

    d->actionInsertTable = new QAction(QIcon::fromTheme(QStringLiteral("insert-table")), i18n("Table..."), this);
    d->actionInsertTable->setObjectName(QStringLiteral("insert_new_table"));
    insertMenu->addAction(d->actionInsertTable);
    connect(d->actionInsertTable, &QAction::triggered, this, [this]() {
        d->_k_slotInsertTable();
    });

    insertMenu->addSeparator();
    d->actionInsertRowBelow = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-insert-row-below")), i18n("Row Below"), this);
    insertMenu->addAction(d->actionInsertRowBelow);
    d->actionInsertRowBelow->setObjectName(QStringLiteral("insert_row_below"));
    connect(d->actionInsertRowBelow, &QAction::triggered, this, [this]() {
        d->_k_slotInsertRowBelow();
    });

    d->actionInsertRowAbove = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-insert-row-above")), i18n("Row Above"), this);
    insertMenu->addAction(d->actionInsertRowAbove);
    d->actionInsertRowAbove->setObjectName(QStringLiteral("insert_row_above"));
    connect(d->actionInsertRowAbove, &QAction::triggered, this, [this]() {
        d->_k_slotInsertRowAbove();
    });

    insertMenu->addSeparator();
    d->actionInsertColumnBefore = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-insert-column-left")), i18n("Column Before"), this);
    insertMenu->addAction(d->actionInsertColumnBefore);
    d->actionInsertColumnBefore->setObjectName(QStringLiteral("insert_column_before"));

    connect(d->actionInsertColumnBefore, &QAction::triggered, this, [this]() {
        d->_k_slotInsertColumnBefore();
    });

    d->actionInsertColumnAfter = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-insert-column-right")), i18n("Column After"), this);
    insertMenu->addAction(d->actionInsertColumnAfter);
    d->actionInsertColumnAfter->setObjectName(QStringLiteral("insert_column_after"));
    connect(d->actionInsertColumnAfter, &QAction::triggered, this, [this]() {
        d->_k_slotInsertColumnAfter();
    });

    KActionMenu *removeMenu = new KActionMenu(i18n("Delete"), this);
    addAction(removeMenu);

    d->actionRemoveRowBelow = new QAction(i18n("Row Below"), this);
    removeMenu->addAction(d->actionRemoveRowBelow);
    d->actionRemoveRowBelow->setObjectName(QStringLiteral("remove_row_below"));
    connect(d->actionRemoveRowBelow, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveRowBelow();
    });

    d->actionRemoveRowAbove = new QAction(i18n("Row Above"), this);
    removeMenu->addAction(d->actionRemoveRowAbove);
    d->actionRemoveRowAbove->setObjectName(QStringLiteral("remove_row_above"));
    connect(d->actionRemoveRowAbove, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveRowAbove();
    });

    removeMenu->addSeparator();
    d->actionRemoveColumnBefore = new QAction(i18n("Column Before"), this);
    removeMenu->addAction(d->actionRemoveColumnBefore);
    d->actionRemoveColumnBefore->setObjectName(QStringLiteral("remove_column_before"));

    connect(d->actionRemoveColumnBefore, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveColumnBefore();
    });

    d->actionRemoveColumnAfter = new QAction(i18n("Column After"), this);
    removeMenu->addAction(d->actionRemoveColumnAfter);
    d->actionRemoveColumnAfter->setObjectName(QStringLiteral("remove_column_after"));
    connect(d->actionRemoveColumnAfter, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveColumnAfter();
    });

    removeMenu->addSeparator();
    d->actionRemoveCellContents = new QAction(i18n("Cell Contents"), this);
    removeMenu->addAction(d->actionRemoveCellContents);
    d->actionRemoveCellContents->setObjectName(QStringLiteral("remove_cell_contents"));
    connect(d->actionRemoveCellContents, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveCellContents();
    });

    addSeparator();

    d->actionMergeCell = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-cell-merge")), i18n("Join With Cell to the Right"), this);
    d->actionMergeCell->setObjectName(QStringLiteral("join_cell_to_the_right"));
    connect(d->actionMergeCell, &QAction::triggered, this, [this]() {
        d->_k_slotMergeCell();
    });
    addAction(d->actionMergeCell);

    d->actionMergeSelectedCells = new QAction(i18n("Join Selected Cells"), this);
    d->actionMergeSelectedCells->setObjectName(QStringLiteral("join_cell_selected_cells"));
    connect(d->actionMergeSelectedCells, &QAction::triggered, this, [this]() {
        d->_k_slotMergeSelectedCells();
    });
    addAction(d->actionMergeSelectedCells);

    d->actionSplitCell = new QAction(QIcon::fromTheme(QStringLiteral("edit-table-cell-split")), i18n("Split cells"), this);
    d->actionSplitCell->setObjectName(QStringLiteral("split_cells"));
    connect(d->actionSplitCell, &QAction::triggered, this, [this]() {
        d->_k_slotSplitCell();
    });
    addAction(d->actionSplitCell);

    addSeparator();

    d->actionTableFormat = new QAction(i18n("Table Format..."), this);
    d->actionTableFormat->setObjectName(QStringLiteral("table_format"));
    connect(d->actionTableFormat, &QAction::triggered, this, [this]() {
        d->_k_slotTableFormat();
    });
    addAction(d->actionTableFormat);

    d->actionTableCellFormat = new QAction(i18n("Table Cell Format..."), this);
    d->actionTableCellFormat->setObjectName(QStringLiteral("table_cell_format"));
    connect(d->actionTableCellFormat, &QAction::triggered, this, [this]() {
        d->_k_slotTableCellFormat();
    });
    addAction(d->actionTableCellFormat);

    connect(textEdit, &QTextEdit::cursorPositionChanged, this, [this]() {
        d->_k_updateActions(false);
    });
    d->_k_updateActions(true);
}

TableActionMenu::~TableActionMenu()
{
    delete d;
}

void TableActionMenu::setRichTextMode(bool richTextMode)
{
    d->richTextMode = richTextMode;
}
}

#include "moc_tableactionmenu.cpp"
