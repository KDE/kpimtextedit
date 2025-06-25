/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposeractionstest.h"
using namespace Qt::Literals::StringLiterals;

#include "../richtextcomposer.h"
#include "../richtextcomposeractions.h"
#include "../richtextcomposercontroler.h"
#include <KActionCollection>
#include <QAction>
#include <QStandardPaths>
#include <QTest>

RichTextComposerActionsTest::RichTextComposerActionsTest(QObject *parent)
    : QObject(parent)
{
    QStandardPaths::setTestModeEnabled(true);
}

RichTextComposerActionsTest::~RichTextComposerActionsTest() = default;

void RichTextComposerActionsTest::shouldHaveDefaultValue()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controller);

    auto actionCollection = new KActionCollection(&composerActions);
    QVERIFY(actionCollection->actions().isEmpty());

    composerActions.createActions(actionCollection);

    QVERIFY(!actionCollection->actions().isEmpty());
    QCOMPARE(composerActions.numberOfActions(), actionCollection->actions().count() - 5);
}

void RichTextComposerActionsTest::shouldHaveActions()
{
    KPIMTextEdit::RichTextComposer composer;
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controller);

    auto actionCollection = new KActionCollection(&composerActions);
    composerActions.createActions(actionCollection);

    QStringList lst;
    lst << u"format_align_left"_s << u"format_align_center"_s << u"format_align_right"_s << u"format_align_justify"_s << u"direction_ltr"_s
        << u"direction_rtl"_s << u"format_text_subscript"_s << u"format_text_superscript"_s << u"format_text_bold"_s << u"format_text_italic"_s
        << u"format_text_underline"_s << u"format_text_strikeout"_s << u"format_font_family"_s << u"format_font_size"_s << u"insert_horizontal_rule"_s
        << u"format_text_foreground_color"_s << u"format_text_background_color"_s << u"manage_link"_s << u"format_list_indent_less"_s
        << u"format_list_indent_more"_s << u"format_list_style"_s << u"add_image"_s << u"insert_html"_s << u"insert_table"_s << u"delete_line"_s
        << u"format_reset"_s << u"format_painter"_s << u"format_heading_level"_s << u"format_list_checkbox"_s;

    QStringList actionNoRichText;
    actionNoRichText << u"paste_quoted"_s << u"tools_quote"_s << u"tools_unquote"_s << u"add_emoticon"_s << u"paste_without_formatting"_s;

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
    KPIMTextEdit::RichTextComposerControler controller(&composer);
    KPIMTextEdit::RichTextComposerActions composerActions(&controller);

    auto actionCollection = new KActionCollection(&composerActions);
    composerActions.createActions(actionCollection);

    QStringList actionNoRichText;
    actionNoRichText << u"paste_quoted"_s << u"tools_quote"_s << u"tools_unquote"_s << u"add_emoticon"_s << u"paste_without_formatting"_s;

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

#include "moc_richtextcomposeractionstest.cpp"
