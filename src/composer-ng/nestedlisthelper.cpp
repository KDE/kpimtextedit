/**
 * Nested list helper
 *
 * SPDX-FileCopyrightText: 2008 Stephen Kelly <steveire@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "nestedlisthelper_p.h"

#include <QKeyEvent>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextEdit>
#include <QTextList>
using namespace KPIMTextEdit;
NestedListHelper::NestedListHelper(QTextEdit *te)
{
    textEdit = te;
    listBottomMargin = 12;
    listTopMargin = 12;
    listNoMargin = 0;
}

bool NestedListHelper::handleBeforeKeyPressEvent(QKeyEvent *event)
{
    QTextCursor cursor = textEdit->textCursor();

    // Only attempt to handle Backspace while on a list
    if ((event->key() != Qt::Key_Backspace) || (!cursor.currentList())) {
        return false;
    }

    bool handled = false;

    if (!cursor.hasSelection() && cursor.currentList() && event->key() == Qt::Key_Backspace && cursor.atBlockStart()) {
        handleOnIndentLess();
        handled = true;
    }

    return handled;
}

bool NestedListHelper::canIndent() const
{
    if ((textEdit->textCursor().block().isValid())
        //            && (  textEdit->textCursor().block().previous().isValid() )
    ) {
        const QTextBlock block = textEdit->textCursor().block();
        const QTextBlock prevBlock = textEdit->textCursor().block().previous();
        if (block.textList()) {
            if (prevBlock.textList()) {
                return block.textList()->format().indent() <= prevBlock.textList()->format().indent();
            }
        } else {
            return true;
        }
    }
    return false;
}

bool NestedListHelper::canDedent() const
{
    QTextBlock thisBlock = textEdit->textCursor().block();
    QTextBlock nextBlock = thisBlock.next();
    if (thisBlock.isValid()) {
        int nextBlockIndent = 0;
        if (nextBlock.isValid() && nextBlock.textList()) {
            nextBlockIndent = nextBlock.textList()->format().indent();
        }
        if (thisBlock.textList()) {
            const int thisBlockIndent = thisBlock.textList()->format().indent();
            if (thisBlockIndent >= nextBlockIndent) {
                return thisBlockIndent > 0;
            }
        }
    }
    return false;
}

bool NestedListHelper::handleAfterKeyPressEvent(QKeyEvent *event)
{
    // Only attempt to handle Backspace and Return
    if ((event->key() != Qt::Key_Backspace) && (event->key() != Qt::Key_Return)) {
        return false;
    }

    QTextCursor cursor = textEdit->textCursor();
    bool handled = false;

    if (!cursor.hasSelection() && cursor.currentList()) {
        // Check if we're on the last list item.
        // itemNumber is zero indexed
        QTextBlock currentBlock = cursor.block();
        if (cursor.currentList()->count() == cursor.currentList()->itemNumber(currentBlock) + 1) {
            // Last block in this list, but may have just gained another list below.
            if (currentBlock.next().textList()) {
                reformatList();
            }

            // No need to reformatList in this case. reformatList is slow.
            if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Backspace)) {
                handled = true;
            }
        } else {
            reformatList();
        }
    }
    return handled;
}

bool NestedListHelper::handleAfterDropEvent(QDropEvent *dropEvent)
{
    Q_UNUSED(dropEvent)
    QTextCursor cursor = topOfSelection();

    QTextBlock droppedBlock = cursor.block();
    const int firstDroppedItemIndent = droppedBlock.textList()->format().indent();

    const int minimumIndent = droppedBlock.previous().textList()->format().indent();

    if (firstDroppedItemIndent < minimumIndent) {
        cursor = QTextCursor(droppedBlock);
        QTextListFormat fmt = droppedBlock.textList()->format();
        fmt.setIndent(minimumIndent);
        QTextList *list = cursor.createList(fmt);

        int endOfDrop = bottomOfSelection().position();
        while (droppedBlock.next().position() < endOfDrop) {
            droppedBlock = droppedBlock.next();
            if (droppedBlock.textList()->format().indent() != firstDroppedItemIndent) {
                // new list?
            }
            list->add(droppedBlock);
        }
        //         list.add( droppedBlock );
    }

    return true;
}

void NestedListHelper::processList(QTextList *list)
{
    QTextBlock block = list->item(0);
    const int thisListIndent = list->format().indent();

    QTextCursor cursor = QTextCursor(block);
    list = cursor.createList(list->format());
    bool processingSubList = false;
    while (block.next().textList() != nullptr) {
        block = block.next();

        QTextList *nextList = block.textList();
        const int nextItemIndent = nextList->format().indent();
        if (nextItemIndent < thisListIndent) {
            return;
        } else if (nextItemIndent > thisListIndent) {
            if (processingSubList) {
                continue;
            }
            processingSubList = true;
            processList(nextList);
        } else {
            processingSubList = false;
            list->add(block);
        }
    }
    //     delete nextList;
    //     nextList = 0;
}

void NestedListHelper::reformatList(QTextBlock block)
{
    if (block.textList()) {
        const int minimumIndent = block.textList()->format().indent();

        // Start at the top of the list
        while (block.previous().textList() != nullptr) {
            if (block.previous().textList()->format().indent() < minimumIndent) {
                break;
            }
            block = block.previous();
        }

        processList(block.textList());
    }
}

void NestedListHelper::reformatList()
{
    const QTextCursor cursor = textEdit->textCursor();
    reformatList(cursor.block());
}

QTextCursor NestedListHelper::topOfSelection()
{
    QTextCursor cursor = textEdit->textCursor();

    if (cursor.hasSelection()) {
        cursor.setPosition(qMin(cursor.position(), cursor.anchor()));
    }
    return cursor;
}

QTextCursor NestedListHelper::bottomOfSelection()
{
    QTextCursor cursor = textEdit->textCursor();

    if (cursor.hasSelection()) {
        cursor.setPosition(qMax(cursor.position(), cursor.anchor()));
    }
    return cursor;
}

void NestedListHelper::handleOnIndentMore()
{
    QTextCursor cursor = textEdit->textCursor();

    QTextListFormat listFmt;
    if (!cursor.currentList()) {
        QTextListFormat::Style style;
        cursor = topOfSelection();
        cursor.movePosition(QTextCursor::PreviousBlock);
        if (cursor.currentList()) {
            style = cursor.currentList()->format().style();
        } else {
            cursor = bottomOfSelection();
            cursor.movePosition(QTextCursor::NextBlock);

            if (cursor.currentList()) {
                style = cursor.currentList()->format().style();
            } else {
                style = QTextListFormat::ListDisc;
            }
        }
        handleOnBulletType(style);
    } else {
        listFmt = cursor.currentList()->format();
        listFmt.setIndent(listFmt.indent() + 1);

        cursor.createList(listFmt);
        reformatList();
    }
}

void NestedListHelper::handleOnIndentLess()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextList *currentList = cursor.currentList();
    if (!currentList) {
        return;
    }
    QTextListFormat listFmt = currentList->format();
    if (listFmt.indent() > 1) {
        listFmt.setIndent(listFmt.indent() - 1);
        cursor.createList(listFmt);
        reformatList(cursor.block());
    } else {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.setBlockFormat(bfmt);
        reformatList(cursor.block().next());
    }
}

void NestedListHelper::handleOnBulletType(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();
    if (styleIndex != 0) {
        auto style = static_cast<QTextListFormat::Style>(styleIndex);
        QTextList *currentList = cursor.currentList();
        QTextListFormat listFmt;

        cursor.beginEditBlock();

        if (currentList) {
            listFmt = currentList->format();
            listFmt.setStyle(style);
            currentList->setFormat(listFmt);
        } else {
            listFmt.setStyle(style);
            cursor.createList(listFmt);
        }

        cursor.endEditBlock();
    } else {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.setBlockFormat(bfmt);
    }

    reformatList();
}
