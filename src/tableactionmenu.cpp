/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later

*/

#include "tableactionmenu.h"
using namespace Qt::Literals::StringLiterals;

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
    explicit TableActionMenuPrivate(QTextEdit *edit, TableActionMenu *qq)
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
            QList<QTextLength> constrains;
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
            QList<QTextLength> constrains = tableFormat.columnWidthConstraints();
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

                QList<QTextLength> constrainsText;
                constrainsText.reserve(newNumberOfColumns);
                const QTextLength::Type type = dialog->typeOfLength();
                const int length = dialog->length();

                const QTextLength textlength(type, length / newNumberOfColumns);
                for (int i = 0; i < newNumberOfColumns; ++i) {
                    constrainsText.append(textlength);
                }
                tableFormat.setColumnWidthConstraints(constrainsText);
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
    auto insertMenu = new KActionMenu(i18n("Insert"), this);
    addAction(insertMenu);

    d->actionInsertTable = new QAction(QIcon::fromTheme(u"insert-table"_s), i18n("Table…"), this);
    d->actionInsertTable->setObjectName("insert_new_table"_L1);
    insertMenu->addAction(d->actionInsertTable);
    connect(d->actionInsertTable, &QAction::triggered, this, [this]() {
        d->_k_slotInsertTable();
    });

    insertMenu->addSeparator();
    d->actionInsertRowBelow = new QAction(QIcon::fromTheme(u"edit-table-insert-row-below"_s), i18n("Row Below"), this);
    insertMenu->addAction(d->actionInsertRowBelow);
    d->actionInsertRowBelow->setObjectName("insert_row_below"_L1);
    connect(d->actionInsertRowBelow, &QAction::triggered, this, [this]() {
        d->_k_slotInsertRowBelow();
    });

    d->actionInsertRowAbove = new QAction(QIcon::fromTheme(u"edit-table-insert-row-above"_s), i18n("Row Above"), this);
    insertMenu->addAction(d->actionInsertRowAbove);
    d->actionInsertRowAbove->setObjectName("insert_row_above"_L1);
    connect(d->actionInsertRowAbove, &QAction::triggered, this, [this]() {
        d->_k_slotInsertRowAbove();
    });

    insertMenu->addSeparator();
    d->actionInsertColumnBefore = new QAction(QIcon::fromTheme(u"edit-table-insert-column-left"_s), i18n("Column Before"), this);
    insertMenu->addAction(d->actionInsertColumnBefore);
    d->actionInsertColumnBefore->setObjectName("insert_column_before"_L1);

    connect(d->actionInsertColumnBefore, &QAction::triggered, this, [this]() {
        d->_k_slotInsertColumnBefore();
    });

    d->actionInsertColumnAfter = new QAction(QIcon::fromTheme(u"edit-table-insert-column-right"_s), i18n("Column After"), this);
    insertMenu->addAction(d->actionInsertColumnAfter);
    d->actionInsertColumnAfter->setObjectName("insert_column_after"_L1);
    connect(d->actionInsertColumnAfter, &QAction::triggered, this, [this]() {
        d->_k_slotInsertColumnAfter();
    });

    auto removeMenu = new KActionMenu(i18n("Delete"), this);
    addAction(removeMenu);

    d->actionRemoveRowBelow = new QAction(i18nc("@action", "Row Below"), this);
    removeMenu->addAction(d->actionRemoveRowBelow);
    d->actionRemoveRowBelow->setObjectName("remove_row_below"_L1);
    connect(d->actionRemoveRowBelow, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveRowBelow();
    });

    d->actionRemoveRowAbove = new QAction(i18nc("@action", "Row Above"), this);
    removeMenu->addAction(d->actionRemoveRowAbove);
    d->actionRemoveRowAbove->setObjectName("remove_row_above"_L1);
    connect(d->actionRemoveRowAbove, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveRowAbove();
    });

    removeMenu->addSeparator();
    d->actionRemoveColumnBefore = new QAction(i18nc("@action", "Column Before"), this);
    removeMenu->addAction(d->actionRemoveColumnBefore);
    d->actionRemoveColumnBefore->setObjectName("remove_column_before"_L1);

    connect(d->actionRemoveColumnBefore, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveColumnBefore();
    });

    d->actionRemoveColumnAfter = new QAction(i18nc("@action", "Column After"), this);
    removeMenu->addAction(d->actionRemoveColumnAfter);
    d->actionRemoveColumnAfter->setObjectName("remove_column_after"_L1);
    connect(d->actionRemoveColumnAfter, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveColumnAfter();
    });

    removeMenu->addSeparator();
    d->actionRemoveCellContents = new QAction(i18nc("@action", "Cell Contents"), this);
    removeMenu->addAction(d->actionRemoveCellContents);
    d->actionRemoveCellContents->setObjectName("remove_cell_contents"_L1);
    connect(d->actionRemoveCellContents, &QAction::triggered, this, [this]() {
        d->_k_slotRemoveCellContents();
    });

    addSeparator();

    d->actionMergeCell = new QAction(QIcon::fromTheme(u"edit-table-cell-merge"_s), i18n("Join With Cell to the Right"), this);
    d->actionMergeCell->setObjectName("join_cell_to_the_right"_L1);
    connect(d->actionMergeCell, &QAction::triggered, this, [this]() {
        d->_k_slotMergeCell();
    });
    addAction(d->actionMergeCell);

    d->actionMergeSelectedCells = new QAction(i18nc("@action", "Join Selected Cells"), this);
    d->actionMergeSelectedCells->setObjectName("join_cell_selected_cells"_L1);
    connect(d->actionMergeSelectedCells, &QAction::triggered, this, [this]() {
        d->_k_slotMergeSelectedCells();
    });
    addAction(d->actionMergeSelectedCells);

    d->actionSplitCell = new QAction(QIcon::fromTheme(u"edit-table-cell-split"_s), i18n("Split cells"), this);
    d->actionSplitCell->setObjectName("split_cells"_L1);
    connect(d->actionSplitCell, &QAction::triggered, this, [this]() {
        d->_k_slotSplitCell();
    });
    addAction(d->actionSplitCell);

    addSeparator();

    d->actionTableFormat = new QAction(i18nc("@action", "Table Format…"), this);
    d->actionTableFormat->setObjectName("table_format"_L1);
    connect(d->actionTableFormat, &QAction::triggered, this, [this]() {
        d->_k_slotTableFormat();
    });
    addAction(d->actionTableFormat);

    d->actionTableCellFormat = new QAction(i18nc("@action", "Table Cell Format…"), this);
    d->actionTableCellFormat->setObjectName("table_cell_format"_L1);
    connect(d->actionTableCellFormat, &QAction::triggered, this, [this]() {
        d->_k_slotTableCellFormat();
    });
    addAction(d->actionTableCellFormat);

    connect(textEdit, &QTextEdit::cursorPositionChanged, this, [this]() {
        d->_k_updateActions(false);
    });
    d->_k_updateActions(true);
}

TableActionMenu::~TableActionMenu() = default;

void TableActionMenu::setRichTextMode(bool richTextMode)
{
    d->richTextMode = richTextMode;
}
}

#include "moc_tableactionmenu.cpp"
