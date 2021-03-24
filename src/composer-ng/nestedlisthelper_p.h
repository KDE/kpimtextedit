/**
 * Nested list helper
 *
 * SPDX-FileCopyrightText: 2008 Stephen Kelly <steveire@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

//@cond PRIVATE
#include <QObject>
class QTextEdit;

class QKeyEvent;
class QDropEvent;
class QTextCursor;
class QTextList;
class QTextBlock;
namespace KPIMTextEdit
{
/**
 *
 * @short Helper class for automatic handling of nested lists in a text edit
 *
 *
 * @author Stephen Kelly
 * @since 4.1
 * @internal
 */
class NestedListHelper
{
public:
    /**
     * Create a helper
     *
     * @param te The text edit object to handle lists in.
     */
    explicit NestedListHelper(QTextEdit *te);

    /**
     *
     * Handles a key press before it is processed by the text edit widget.
     *
     * Currently this causes a backspace at the beginning of a line or with a
     * multi-line selection to decrease the nesting level of the list.
     *
     * @param event The event to be handled
     * @return Whether the event was completely handled by this method.
     */
    bool handleBeforeKeyPressEvent(QKeyEvent *event);

    /**
     *
     * Handles a key press after it is processed by the text edit widget.
     *
     * Currently this causes a Return at the end of the last list item, or
     * a Backspace after the last list item to recalculate the spacing
     * between the list items.
     *
     * @param event The event to be handled
     * @return Whether the event was completely handled by this method.
     */
    bool handleAfterKeyPressEvent(QKeyEvent *event);

    bool handleAfterDropEvent(QDropEvent *event);

    /**
     * Increases the indent (nesting level) on the current list item or selection.
     */
    void handleOnIndentMore();

    /**
     * Decreases the indent (nesting level) on the current list item or selection.
     */
    void handleOnIndentLess();

    /**
     * Changes the style of the current list or creates a new list with
     * the specified style.
     *
     * @param styleIndex The QTextListStyle of the list.
     */
    void handleOnBulletType(int styleIndex);

    /**
     * @brief Check whether the current item in the list may be indented.
     *
     * An list item must have an item above it on the same or greater level
     * if it can be indented.
     *
     * Also, a block which is currently part of a list can be indented.
     *
     * @sa canDedent
     *
     * @return Whether the item can be indented.
     */
    Q_REQUIRED_RESULT bool canIndent() const;

    /**
     * \brief Check whether the current item in the list may be dedented.
     *
     * An item may be dedented if it is part of a list. Otherwise it can't be.
     *
     * @sa canIndent
     *
     * @return Whether the item can be dedented.
     */
    Q_REQUIRED_RESULT bool canDedent() const;

private:
    QTextCursor topOfSelection();
    QTextCursor bottomOfSelection();
    void processList(QTextList *list);
    void reformatList(QTextBlock block);
    void reformatList();

    QTextEdit *textEdit = nullptr;

    int listBottomMargin;
    int listTopMargin;
    int listNoMargin;
};
}
//@endcond

