/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.

*/

#include "tableactionmenu.h"
#include "textedit.h"
#include "inserttabledialog.h"
#include "tableformatdialog.h"
#include "tablecellformatdialog.h"

#include <QAction>
#include <QIcon>
#include <KLocalizedString>

#include <QTextTable>
#include <QPointer>

namespace KPIMTextEdit
{

class TableActionMenuPrivate
{
public:
    TableActionMenuPrivate(TextEdit *edit, TableActionMenu *qq)
        : textEdit(edit), q(qq)
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

    QAction *actionInsertTable;

    QAction *actionInsertRowBelow;
    QAction *actionInsertRowAbove;

    QAction *actionInsertColumnBefore;
    QAction *actionInsertColumnAfter;

    QAction *actionRemoveRowBelow;
    QAction *actionRemoveRowAbove;

    QAction *actionRemoveColumnBefore;
    QAction *actionRemoveColumnAfter;

    QAction *actionMergeCell;
    QAction *actionMergeSelectedCells;
    QAction *actionSplitCell;

    QAction *actionTableFormat;
    QAction *actionTableCellFormat;

    QAction *actionRemoveCellContents;

    QList<QAction *> listAction;
    TextEdit *textEdit;
    TableActionMenu *q;
};

void TableActionMenuPrivate::_k_slotRemoveCellContents()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        const QTextTableCell cell = table->cellAt(textEdit->textCursor());
        if (cell.isValid()) {
            const QTextCursor firstCursor = cell.firstCursorPosition();
            const QTextCursor endCursor = cell.lastCursorPosition();
            QTextCursor cursor = textEdit->textCursor();
            cursor.beginEditBlock();
            cursor.setPosition(firstCursor.position());
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                                endCursor.position() - firstCursor.position());
            cursor.removeSelectedText();
            cursor.endEditBlock();
        }
    }
}

void TableActionMenuPrivate::_k_slotRemoveRowBelow()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->insertRows(cell.row(), 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertColumnBefore()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->insertColumns(cell.column(), 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotInsertColumnAfter()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QPointer<InsertTableDialog> dialog = new InsertTableDialog(textEdit);
        if (dialog->exec()) {
            QTextCursor cursor = textEdit->textCursor();
            QTextTableFormat tableFormat;
            tableFormat.setBorder(dialog->border());
            const int numberOfColumns(dialog->columns());
            QVector<QTextLength> contrains;
            const QTextLength::Type type = dialog->typeOfLength();
            const int length = dialog->length();

            const QTextLength textlength(type, length / numberOfColumns);
            for (int i = 0; i < numberOfColumns; ++i) {
                contrains.append(textlength);
            }
            tableFormat.setColumnWidthConstraints(contrains);
            tableFormat.setAlignment(Qt::AlignLeft);
            QTextTable *table = cursor.insertTable(dialog->rows(), numberOfColumns);
            table->setFormat(tableFormat);
        }
        delete dialog;
    }
}

void TableActionMenuPrivate::_k_slotMergeCell()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            table->mergeCells(cell.row(), cell.column(), 1, cell.columnSpan() + 1);
        }
    }
}

void TableActionMenuPrivate::_k_slotMergeSelectedCells()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            table->mergeCells(textEdit->textCursor());
        }
    }
}

void TableActionMenuPrivate::_k_slotTableFormat()
{
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
            QVector<QTextLength> contrains = tableFormat.columnWidthConstraints();
            if (!contrains.isEmpty()) {
                dialog->setTypeOfLength(contrains.at(0).type());
                dialog->setLength(contrains.at(0).rawValue() * numberOfColumn);
            }

            if (dialog->exec()) {
                const int newNumberOfColumns(dialog->columns());
                if ((newNumberOfColumns != numberOfColumn) ||
                        (dialog->rows() != numberOfRow)) {
                    table->resize(dialog->rows(), newNumberOfColumns);
                }
                tableFormat.setBorder(dialog->border());
                tableFormat.setCellPadding(dialog->padding());
                tableFormat.setCellSpacing(dialog->spacing());
                tableFormat.setAlignment(dialog->alignment());

                QVector<QTextLength> contrains;
                const QTextLength::Type type = dialog->typeOfLength();
                const int length = dialog->length();

                const QTextLength textlength(type, length / newNumberOfColumns);
                for (int i = 0; i < newNumberOfColumns; ++i) {
                    contrains.append(textlength);
                }
                tableFormat.setColumnWidthConstraints(contrains);
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
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
    if (textEdit->textMode() == KRichTextEdit::Rich) {
        QTextTable *table = textEdit->textCursor().currentTable();
        if (table) {
            const QTextTableCell cell = table->cellAt(textEdit->textCursor());
            if (cell.columnSpan() > 1 || cell.rowSpan() > 1) {
                table->splitCell(cell.row(), cell.column(),
                                 qMax(1, cell.rowSpan() - 1),
                                 qMax(1, cell.columnSpan() - 1));
                _k_updateActions();
            }
        }
    }
}

void TableActionMenuPrivate::_k_updateActions(bool forceUpdate)
{
    if ((textEdit->textMode() == KRichTextEdit::Rich) || forceUpdate) {
        QTextTable *table = textEdit->textCursor().currentTable();
        const bool isTable = (table != 0);
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
            const bool hasSelectedTableCell =
                (firstRow != -1) && (numRows != -1) &&
                (firstColumn != -1) && (numColumns != -1);
            if (cell.column() > table->columns() - 2) {
                actionMergeCell->setEnabled(false);
            } else {
                actionMergeCell->setEnabled(true);
            }
            if (cell.columnSpan() > 1 ||  cell.rowSpan() > 1) {
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

TableActionMenu::TableActionMenu(TextEdit *textEdit)
    : KActionMenu(textEdit), d(new TableActionMenuPrivate(textEdit, this))
{
    KActionMenu *insertMenu = new KActionMenu(i18n("Insert"), this);
    addAction(insertMenu);

    d->actionInsertTable = new QAction(QIcon::fromTheme(QLatin1String("insert-table")), i18n("Table..."), this);
    d->actionInsertTable->setObjectName(QLatin1String("insert_new_table"));
    d->listAction << d->actionInsertTable;
    insertMenu->addAction(d->actionInsertTable);
    connect(d->actionInsertTable, SIGNAL(triggered(bool)),
            SLOT(_k_slotInsertTable()));

    insertMenu->addSeparator();
    d->actionInsertRowBelow =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-insert-row-below")),
                    i18n("Row Below"), this);
    insertMenu->addAction(d->actionInsertRowBelow);
    d->actionInsertRowBelow->setObjectName(QLatin1String("insert_row_below"));
    d->listAction << d->actionInsertRowBelow;
    connect(d->actionInsertRowBelow, SIGNAL(triggered(bool)),
            SLOT(_k_slotInsertRowBelow()));

    d->actionInsertRowAbove =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-insert-row-above")),
                    i18n("Row Above"), this);
    insertMenu->addAction(d->actionInsertRowAbove);
    d->actionInsertRowAbove->setObjectName(QLatin1String("insert_row_above"));
    d->listAction << d->actionInsertRowAbove;
    connect(d->actionInsertRowAbove, SIGNAL(triggered(bool)),
            SLOT(_k_slotInsertRowAbove()));

    insertMenu->addSeparator();
    d->actionInsertColumnBefore =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-insert-column-left")),
                    i18n("Column Before"), this);
    insertMenu->addAction(d->actionInsertColumnBefore);
    d->listAction << d->actionInsertColumnBefore;
    d->actionInsertColumnBefore->setObjectName(QLatin1String("insert_column_before"));

    connect(d->actionInsertColumnBefore, SIGNAL(triggered(bool)),
            SLOT(_k_slotInsertColumnBefore()));

    d->actionInsertColumnAfter =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-insert-column-right")),
                    i18n("Column After"), this);
    insertMenu->addAction(d->actionInsertColumnAfter);
    d->actionInsertColumnAfter->setObjectName(QLatin1String("insert_column_after"));
    d->listAction << d->actionInsertColumnAfter;
    connect(d->actionInsertColumnAfter, SIGNAL(triggered(bool)),
            SLOT(_k_slotInsertColumnAfter()));

    KActionMenu *removeMenu = new KActionMenu(i18n("Delete"), this);
    addAction(removeMenu);

    d->actionRemoveRowBelow = new QAction(i18n("Row Below"), this);
    removeMenu->addAction(d->actionRemoveRowBelow);
    d->actionRemoveRowBelow->setObjectName(QLatin1String("remove_row_below"));
    d->listAction << d->actionRemoveRowBelow;
    connect(d->actionRemoveRowBelow, SIGNAL(triggered(bool)),
            SLOT(_k_slotRemoveRowBelow()));

    d->actionRemoveRowAbove = new QAction(i18n("Row Above"), this);
    removeMenu->addAction(d->actionRemoveRowAbove);
    d->listAction << d->actionRemoveRowAbove;
    d->actionRemoveRowAbove->setObjectName(QLatin1String("remove_row_above"));
    connect(d->actionRemoveRowAbove, SIGNAL(triggered(bool)),
            SLOT(_k_slotRemoveRowAbove()));

    removeMenu->addSeparator();
    d->actionRemoveColumnBefore = new QAction(i18n("Column Before"), this);
    d->listAction << d->actionRemoveColumnBefore;
    removeMenu->addAction(d->actionRemoveColumnBefore);
    d->actionRemoveColumnBefore->setObjectName(QLatin1String("remove_column_before"));

    connect(d->actionRemoveColumnBefore, SIGNAL(triggered(bool)),
            SLOT(_k_slotRemoveColumnBefore()));

    d->actionRemoveColumnAfter = new QAction(i18n("Column After"), this);
    d->listAction << d->actionRemoveColumnAfter;
    removeMenu->addAction(d->actionRemoveColumnAfter);
    d->actionRemoveColumnAfter->setObjectName(QLatin1String("remove_column_after"));
    connect(d->actionRemoveColumnAfter, SIGNAL(triggered(bool)),
            SLOT(_k_slotRemoveColumnAfter()));

    removeMenu->addSeparator();
    d->actionRemoveCellContents = new QAction(i18n("Cell Contents"), this);
    removeMenu->addAction(d->actionRemoveCellContents);
    d->listAction << d->actionRemoveCellContents;
    d->actionRemoveCellContents->setObjectName(QLatin1String("remove_cell_contents"));
    connect(d->actionRemoveCellContents, SIGNAL(triggered(bool)),
            SLOT(_k_slotRemoveCellContents()));

    addSeparator();

    d->actionMergeCell =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-cell-merge")),
                    i18n("Join With Cell to the Right"), this);
    d->actionMergeCell->setObjectName(QLatin1String("join_cell_to_the_right"));
    d->listAction << d->actionMergeCell;
    connect(d->actionMergeCell, SIGNAL(triggered(bool)),
            SLOT(_k_slotMergeCell()));
    addAction(d->actionMergeCell);

    d->actionMergeSelectedCells = new QAction(i18n("Join Selected Cells"), this);
    d->actionMergeSelectedCells->setObjectName(QLatin1String("join_cell_selected_cells"));
    d->listAction << d->actionMergeSelectedCells;
    connect(d->actionMergeSelectedCells, SIGNAL(triggered(bool)),
            SLOT(_k_slotMergeSelectedCells()));
    addAction(d->actionMergeSelectedCells);

    d->actionSplitCell =
        new QAction(QIcon::fromTheme(QLatin1String("edit-table-cell-split")),
                    i18n("Split cells"), this);
    d->actionSplitCell->setObjectName(QLatin1String("split_cells"));
    d->listAction << d->actionSplitCell;
    connect(d->actionSplitCell, SIGNAL(triggered(bool)),
            SLOT(_k_slotSplitCell()));
    addAction(d->actionSplitCell);

    addSeparator();

    d->actionTableFormat = new QAction(i18n("Table Format..."), this);
    d->actionTableFormat->setObjectName(QLatin1String("table_format"));
    d->listAction << d->actionTableFormat;
    connect(d->actionTableFormat, SIGNAL(triggered(bool)),
            SLOT(_k_slotTableFormat()));
    addAction(d->actionTableFormat);

    d->actionTableCellFormat = new QAction(i18n("Table Cell Format..."), this);
    d->actionTableCellFormat->setObjectName(QLatin1String("table_cell_format"));
    d->listAction << d->actionTableCellFormat;
    connect(d->actionTableCellFormat, SIGNAL(triggered(bool)),
            SLOT(_k_slotTableCellFormat()));
    addAction(d->actionTableCellFormat);

    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(_k_updateActions()));
    d->_k_updateActions(true);
}

TableActionMenu::~TableActionMenu()
{
    delete d;
}

QList<QAction *> TableActionMenu::listAction() const
{
    return d->listAction;
}

}

#include "moc_tableactionmenu.cpp"
