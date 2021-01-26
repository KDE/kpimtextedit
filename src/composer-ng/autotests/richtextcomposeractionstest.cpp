/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposeractionstest.h"
#include "../richtextcomposer.h"
#include "../richtextcomposeractions.h"
#include "../richtextcomposercontroler.h"
#include <KActionCollection>
#include <QAction>
#include <QTest>

RichTextComposerActionsTest::RichTextComposerActionsTest(QObject *parent)
    : QObject(parent)
{
}

RichTextComposerActionsTest::~RichTextComposerActionsTest()
{
}

void RichTextComposerActionsTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controler);

    KActionCollection *actionCollection = new KActionCollection(&composerActions);
    QVERIFY(actionCollection->actions().isEmpty());

    composerActions.createActions(actionCollection);

    QVERIFY(!actionCollection->actions().isEmpty());
    QCOMPARE(composerActions.numberOfActions(), actionCollection->actions().count() - 5);
}

void RichTextComposerActionsTest::shouldHaveActions()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controler);

    KActionCollection *actionCollection = new KActionCollection(&composerActions);
    composerActions.createActions(actionCollection);

    QStringList lst;
    lst << QStringLiteral("format_align_left") << QStringLiteral("format_align_center") << QStringLiteral("format_align_right")
        << QStringLiteral("format_align_justify") << QStringLiteral("direction_ltr") << QStringLiteral("direction_rtl")
        << QStringLiteral("format_text_subscript") << QStringLiteral("format_text_superscript") << QStringLiteral("format_text_bold")
        << QStringLiteral("format_text_italic") << QStringLiteral("format_text_underline") << QStringLiteral("format_text_strikeout")
        << QStringLiteral("format_font_family") << QStringLiteral("format_font_size") << QStringLiteral("insert_horizontal_rule")
        << QStringLiteral("format_text_foreground_color") << QStringLiteral("format_text_background_color") << QStringLiteral("manage_link")
        << QStringLiteral("format_list_indent_less") << QStringLiteral("format_list_indent_more") << QStringLiteral("format_list_style")
        << QStringLiteral("add_image") << QStringLiteral("insert_html") << QStringLiteral("insert_table") << QStringLiteral("delete_line")
        << QStringLiteral("format_reset") << QStringLiteral("format_painter") << QStringLiteral("format_heading_level")
        << QStringLiteral("format_list_checkbox");

    QStringList actionNoRichText;
    actionNoRichText << QStringLiteral("paste_quoted") << QStringLiteral("tools_quote") << QStringLiteral("tools_unquote") << QStringLiteral("add_emoticon")
                     << QStringLiteral("paste_without_formatting");

    // QCOMPARE(lst.count(), composerActions.numberOfActions());
    for (QAction *act : actionCollection->actions()) {
        const QString actionName = act->objectName();
        if (!actionNoRichText.contains(actionName)) {
            const bool hasActionName = lst.contains(actionName);
            if (!hasActionName) {
                qDebug() << " actionName " << actionName;
            }
            QVERIFY(hasActionName);
        }
    }
}

void RichTextComposerActionsTest::shouldChangeEnableState()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controler(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controler);

    KActionCollection *actionCollection = new KActionCollection(&composerActions);
    composerActions.createActions(actionCollection);

    QStringList actionNoRichText;
    actionNoRichText << QStringLiteral("paste_quoted") << QStringLiteral("tools_quote") << QStringLiteral("tools_unquote") << QStringLiteral("add_emoticon")
                     << QStringLiteral("paste_without_formatting");

    composerActions.setActionsEnabled(false);
    for (QAction *act : actionCollection->actions()) {
        if (!actionNoRichText.contains(act->objectName())) {
            QVERIFY(!act->isEnabled());
        }
    }
    composerActions.setActionsEnabled(true);
    for (QAction *act : actionCollection->actions()) {
        if (!actionNoRichText.contains(act->objectName())) {
            QVERIFY(act->isEnabled());
        }
    }
}

QTEST_MAIN(RichTextComposerActionsTest)
