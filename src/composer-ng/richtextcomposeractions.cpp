/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposeractions.h"
#include "emoticon/emoticontexteditaction.h"
#include "richtextcomposercontroler.h"
#include "tableactionmenu.h"
#include <KActionCollection>
#include <KFontAction>
#include <KFontSizeAction>
#include <KLocalizedString>
#include <KToggleAction>
#include <QTextCharFormat>
#include <QTextList>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposerActions::RichTextComposerActionsPrivate
{
public:
    RichTextComposerActionsPrivate(KPIMTextEdit::RichTextComposerControler *controler)
        : composerControler(controler)
    {
    }

    QVector<QAction *> richTextActionList;

    KPIMTextEdit::RichTextComposerControler *const composerControler;
    KToggleAction *action_align_left = nullptr;
    KToggleAction *action_align_right = nullptr;
    KToggleAction *action_align_center = nullptr;
    KToggleAction *action_align_justify = nullptr;

    KToggleAction *action_direction_ltr = nullptr;
    KToggleAction *action_direction_rtl = nullptr;

    KToggleAction *action_text_superscript = nullptr;
    KToggleAction *action_text_subscript = nullptr;

    KToggleAction *action_text_bold = nullptr;
    KToggleAction *action_text_italic = nullptr;
    KToggleAction *action_text_underline = nullptr;
    KToggleAction *action_text_strikeout = nullptr;

    KFontAction *action_font_family = nullptr;
    KFontSizeAction *action_font_size = nullptr;

    QAction *action_insert_horizontal_rule = nullptr;
    QAction *action_text_foreground_color = nullptr;
    QAction *action_text_background_color = nullptr;
    QAction *action_manage_link = nullptr;

    QAction *action_list_indent = nullptr;
    QAction *action_list_dedent = nullptr;

    KSelectAction *action_list_style = nullptr;

    QAction *action_paste_quotation = nullptr;
    QAction *action_add_quote_chars = nullptr;
    QAction *action_remove_quote_chars = nullptr;
    QAction *action_paste_without_formatting = nullptr;

    QAction *action_add_image = nullptr;
    KPIMTextEdit::EmoticonTextEditAction *action_add_emoticon = nullptr;
    QAction *action_insert_html = nullptr;
    KPIMTextEdit::TableActionMenu *action_add_table = nullptr;
    QAction *action_delete_line = nullptr;
    QAction *action_format_reset = nullptr;

    KToggleAction *action_format_painter = nullptr;
    KSelectAction *action_heading_level = nullptr;
    KToggleAction *action_list_checkbox = nullptr;

    bool richTextEnabled = false;
};

RichTextComposerActions::RichTextComposerActions(KPIMTextEdit::RichTextComposerControler *controler, QObject *parent)
    : QObject(parent)
    , d(new RichTextComposerActions::RichTextComposerActionsPrivate(controler))
{
}

RichTextComposerActions::~RichTextComposerActions()
{
    delete d;
}

QVector<QAction *> RichTextComposerActions::richTextActionList() const
{
    return d->richTextActionList;
}

int RichTextComposerActions::numberOfActions() const
{
    return d->richTextActionList.count();
}

void RichTextComposerActions::createActions(KActionCollection *ac)
{
    // Alignment
    d->action_align_left = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-justify-left")), i18nc("@action", "Align &Left"), this);
    d->action_align_left->setIconText(i18nc("@label left justify", "Left"));
    d->richTextActionList.append((d->action_align_left));
    d->action_align_left->setObjectName(QStringLiteral("format_align_left"));
    connect(d->action_align_left, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::alignLeft);
    if (ac) {
        ac->addAction(QStringLiteral("format_align_left"), d->action_align_left);
    }

    d->action_align_center = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-justify-center")), i18nc("@action", "Align &Center"), this);
    d->action_align_center->setIconText(i18nc("@label center justify", "Center"));
    d->richTextActionList.append((d->action_align_center));
    d->action_align_center->setObjectName(QStringLiteral("format_align_center"));
    connect(d->action_align_center, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::alignCenter);
    if (ac) {
        ac->addAction(QStringLiteral("format_align_center"), d->action_align_center);
    }

    d->action_align_right = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-justify-right")), i18nc("@action", "Align &Right"), this);
    d->action_align_right->setIconText(i18nc("@label right justify", "Right"));
    d->richTextActionList.append((d->action_align_right));
    d->action_align_right->setObjectName(QStringLiteral("format_align_right"));
    connect(d->action_align_right, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::alignRight);
    if (ac) {
        ac->addAction(QStringLiteral("format_align_right"), d->action_align_right);
    }

    d->action_align_justify = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-justify-fill")), i18nc("@action", "&Justify"), this);
    d->action_align_justify->setIconText(i18nc("@label justify fill", "Justify"));
    d->richTextActionList.append((d->action_align_justify));
    d->action_align_justify->setObjectName(QStringLiteral("format_align_justify"));
    connect(d->action_align_justify, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::alignJustify);
    if (ac) {
        ac->addAction(QStringLiteral("format_align_justify"), d->action_align_justify);
    }

    auto alignmentGroup = new QActionGroup(this);
    alignmentGroup->addAction(d->action_align_left);
    alignmentGroup->addAction(d->action_align_center);
    alignmentGroup->addAction(d->action_align_right);
    alignmentGroup->addAction(d->action_align_justify);

    // Align text
    d->action_direction_ltr = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-direction-ltr")), i18nc("@action", "Left-to-Right"), this);
    d->action_direction_ltr->setIconText(i18nc("@label left-to-right", "Left-to-Right"));
    d->richTextActionList.append(d->action_direction_ltr);
    d->action_direction_ltr->setObjectName(QStringLiteral("direction_ltr"));
    connect(d->action_direction_ltr, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::makeLeftToRight);
    if (ac) {
        ac->addAction(QStringLiteral("direction_ltr"), d->action_direction_ltr);
    }

    d->action_direction_rtl = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-direction-rtl")), i18nc("@action", "Right-to-Left"), this);
    d->action_direction_rtl->setIconText(i18nc("@label right-to-left", "Right-to-Left"));
    d->richTextActionList.append(d->action_direction_rtl);
    d->action_direction_rtl->setObjectName(QStringLiteral("direction_rtl"));
    connect(d->action_direction_rtl, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::makeRightToLeft);
    if (ac) {
        ac->addAction(QStringLiteral("direction_rtl"), d->action_direction_rtl);
    }

    auto directionGroup = new QActionGroup(this);
    directionGroup->addAction(d->action_direction_ltr);
    directionGroup->addAction(d->action_direction_rtl);

    // Sub/Super script
    d->action_text_subscript = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-subscript")), i18nc("@action", "Subscript"), this);
    d->richTextActionList.append((d->action_text_subscript));
    d->action_text_subscript->setObjectName(QStringLiteral("format_text_subscript"));
    connect(d->action_text_subscript, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextSubScript);
    if (ac) {
        ac->addAction(QStringLiteral("format_text_subscript"), d->action_text_subscript);
    }

    d->action_text_superscript = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-superscript")), i18nc("@action", "Superscript"), this);
    d->richTextActionList.append((d->action_text_superscript));
    d->action_text_superscript->setObjectName(QStringLiteral("format_text_superscript"));
    connect(d->action_text_superscript, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextSuperScript);
    if (ac) {
        ac->addAction(QStringLiteral("format_text_superscript"), d->action_text_superscript);
    }

    d->action_text_bold = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-bold")), i18nc("@action boldify selected text", "&Bold"), this);
    QFont bold;
    bold.setBold(true);
    d->action_text_bold->setFont(bold);
    d->richTextActionList.append((d->action_text_bold));
    d->action_text_bold->setObjectName(QStringLiteral("format_text_bold"));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_bold"), d->action_text_bold);
        ac->setDefaultShortcut(d->action_text_bold, Qt::CTRL | Qt::Key_B);
    }
    connect(d->action_text_bold, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextBold);

    d->action_text_italic =
        new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-italic")), i18nc("@action italicize selected text", "&Italic"), this);
    QFont italic;
    italic.setItalic(true);
    d->action_text_italic->setFont(italic);
    d->richTextActionList.append((d->action_text_italic));
    d->action_text_italic->setObjectName(QStringLiteral("format_text_italic"));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_italic"), d->action_text_italic);
        ac->setDefaultShortcut(d->action_text_italic, Qt::CTRL | Qt::Key_I);
    }
    connect(d->action_text_italic, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextItalic);

    d->action_text_underline =
        new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-underline")), i18nc("@action underline selected text", "&Underline"), this);
    QFont underline;
    underline.setUnderline(true);
    d->action_text_underline->setFont(underline);
    d->richTextActionList.append((d->action_text_underline));
    d->action_text_underline->setObjectName(QStringLiteral("format_text_underline"));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_underline"), d->action_text_underline);
        ac->setDefaultShortcut(d->action_text_underline, Qt::CTRL | Qt::Key_U);
    }
    connect(d->action_text_underline, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextUnderline);

    d->action_text_strikeout = new KToggleAction(QIcon::fromTheme(QStringLiteral("format-text-strikethrough")), i18nc("@action", "&Strike Out"), this);
    QFont strikeout;
    strikeout.setStrikeOut(true);
    d->action_text_strikeout->setFont(strikeout);
    d->richTextActionList.append((d->action_text_strikeout));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_strikeout"), d->action_text_strikeout);
    }
    d->action_text_strikeout->setObjectName(QStringLiteral("format_text_strikeout"));
    if (ac) {
        ac->setDefaultShortcut(d->action_text_strikeout, Qt::CTRL | Qt::Key_L);
    }
    connect(d->action_text_strikeout, &KToggleAction::triggered, d->composerControler, &RichTextComposerControler::setTextStrikeOut);

    // Font Family
    d->action_font_family = new KFontAction(i18nc("@action", "&Font"), this);
    d->richTextActionList.append((d->action_font_family));
    d->action_font_family->setObjectName(QStringLiteral("format_font_family"));
    if (ac) {
        ac->addAction(QStringLiteral("format_font_family"), d->action_font_family);
    }
    connect(d->action_font_family, &KFontAction::textTriggered, d->composerControler, &RichTextComposerControler::setFontFamily);

    // Font Size
    d->action_font_size = new KFontSizeAction(i18nc("@action", "Font &Size"), this);
    d->richTextActionList.append((d->action_font_size));
    d->action_font_size->setObjectName(QStringLiteral("format_font_size"));
    if (ac) {
        ac->addAction(QStringLiteral("format_font_size"), d->action_font_size);
    }
    connect(d->action_font_size, &KFontSizeAction::fontSizeChanged, d->composerControler, &RichTextComposerControler::setFontSize);

    d->action_insert_horizontal_rule = new QAction(QIcon::fromTheme(QStringLiteral("insert-horizontal-rule")), i18nc("@action", "Insert Rule Line"), this);
    d->richTextActionList.append((d->action_insert_horizontal_rule));
    d->action_insert_horizontal_rule->setObjectName(QStringLiteral("insert_horizontal_rule"));
    if (ac) {
        ac->addAction(QStringLiteral("insert_horizontal_rule"), d->action_insert_horizontal_rule);
    }
    connect(d->action_insert_horizontal_rule, &QAction::triggered, d->composerControler, &RichTextComposerControler::insertHorizontalRule);

    // Foreground Color
    d->action_text_foreground_color = new QAction(QIcon::fromTheme(QStringLiteral("format-stroke-color")), i18nc("@action", "Text &Color..."), this);
    d->action_text_foreground_color->setIconText(i18nc("@label stroke color", "Color"));
    d->richTextActionList.append((d->action_text_foreground_color));
    d->action_text_foreground_color->setObjectName(QStringLiteral("format_text_foreground_color"));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_foreground_color"), d->action_text_foreground_color);
    }
    connect(d->action_text_foreground_color, &QAction::triggered, d->composerControler, &RichTextComposerControler::setChangeTextForegroundColor);
    // Background Color
    d->action_text_background_color = new QAction(QIcon::fromTheme(QStringLiteral("format-fill-color")), i18nc("@action", "Text &Highlight..."), this);
    d->richTextActionList.append((d->action_text_background_color));
    if (ac) {
        ac->addAction(QStringLiteral("format_text_background_color"), d->action_text_background_color);
    }
    d->action_text_background_color->setObjectName(QStringLiteral("format_text_background_color"));
    connect(d->action_text_background_color, &QAction::triggered, d->composerControler, &RichTextComposerControler::setChangeTextBackgroundColor);

    d->action_manage_link = new QAction(QIcon::fromTheme(QStringLiteral("insert-link")), i18nc("@action", "Link"), this);
    d->richTextActionList.append((d->action_manage_link));
    d->action_manage_link->setObjectName(QStringLiteral("manage_link"));
    if (ac) {
        ac->addAction(QStringLiteral("manage_link"), d->action_manage_link);
    }
    connect(d->action_manage_link, &QAction::triggered, d->composerControler, &RichTextComposerControler::manageLink);

    d->action_list_indent = new QAction(QIcon::fromTheme(QStringLiteral("format-indent-more")), i18nc("@action", "Increase List Level"), this);
    d->richTextActionList.append((d->action_list_indent));
    d->action_list_indent->setObjectName(QStringLiteral("format_list_indent_more"));
    if (ac) {
        ac->addAction(QStringLiteral("format_list_indent_more"), d->action_list_indent);
    }
    connect(d->action_list_indent, &QAction::triggered, d->composerControler, &RichTextComposerControler::indentListMore);
    connect(d->action_list_indent, &QAction::triggered, this, &RichTextComposerActions::slotUpdateMiscActions);
    d->action_list_dedent = new QAction(QIcon::fromTheme(QStringLiteral("format-indent-less")), i18nc("@action", "Decrease List Level"), this);
    d->richTextActionList.append((d->action_list_dedent));
    d->action_list_dedent->setObjectName(QStringLiteral("format_list_indent_less"));
    if (ac) {
        ac->addAction(QStringLiteral("format_list_indent_less"), d->action_list_dedent);
    }
    connect(d->action_list_dedent, &QAction::triggered, d->composerControler, &RichTextComposerControler::indentListLess);
    connect(d->action_list_dedent, &QAction::triggered, this, &RichTextComposerActions::slotUpdateMiscActions);

    d->action_list_style = new KSelectAction(QIcon::fromTheme(QStringLiteral("format-list-unordered")), i18nc("@title:menu", "List Style"), this);
    QStringList listStyles;
    listStyles << i18nc("@item:inmenu no list style", "None") << i18nc("@item:inmenu disc list style", "Disc")
               << i18nc("@item:inmenu circle list style", "Circle") << i18nc("@item:inmenu square list style", "Square")
               << i18nc("@item:inmenu numbered lists", "123") << i18nc("@item:inmenu lowercase abc lists", "abc")
               << i18nc("@item:inmenu uppercase abc lists", "ABC") << i18nc("@item:inmenu lower case roman numerals", "i ii iii")
               << i18nc("@item:inmenu upper case roman numerals", "I II III");

    d->action_list_style->setItems(listStyles);
    d->action_list_style->setCurrentItem(0);
    d->richTextActionList.append((d->action_list_style));
    d->action_list_style->setObjectName(QStringLiteral("format_list_style"));
    if (ac) {
        ac->addAction(QStringLiteral("format_list_style"), d->action_list_style);
    }
    connect(d->action_list_style, &KSelectAction::indexTriggered, this, &RichTextComposerActions::setListStyle);
    connect(d->action_list_style, QOverload<bool>::of(&QAction::triggered), this, &RichTextComposerActions::slotUpdateMiscActions);
    d->action_paste_quotation = new QAction(i18n("Pa&ste as Quotation"), this);
    d->action_paste_quotation->setObjectName(QStringLiteral("paste_quoted"));
    if (ac) {
        ac->addAction(QStringLiteral("paste_quoted"), d->action_paste_quotation);
        ac->setDefaultShortcut(d->action_paste_quotation, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_O));
    }
    connect(d->action_paste_quotation, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotPasteAsQuotation);

    d->action_add_quote_chars = new QAction(i18n("Add &Quote Characters"), this);
    d->action_add_quote_chars->setObjectName(QStringLiteral("tools_quote"));
    if (ac) {
        ac->addAction(QStringLiteral("tools_quote"), d->action_add_quote_chars);
    }
    connect(d->action_add_quote_chars, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotAddQuotes);

    d->action_remove_quote_chars = new QAction(i18n("Re&move Quote Characters"), this);
    d->action_remove_quote_chars->setObjectName(QStringLiteral("tools_unquote"));
    connect(d->action_remove_quote_chars, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotRemoveQuotes);
    if (ac) {
        ac->addAction(QStringLiteral("tools_unquote"), d->action_remove_quote_chars);
    }

    d->action_paste_without_formatting = new QAction(i18n("Paste Without Formatting"), this);
    d->action_paste_without_formatting->setObjectName(QStringLiteral("paste_without_formatting"));
    if (ac) {
        ac->addAction(QStringLiteral("paste_without_formatting"), d->action_paste_without_formatting);
        ac->setDefaultShortcut(d->action_paste_without_formatting, QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_V));
    }
    connect(d->action_paste_without_formatting, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotPasteWithoutFormatting);

    d->action_add_image = new QAction(QIcon::fromTheme(QStringLiteral("insert-image")), i18n("Add Image"), this);
    d->action_add_image->setObjectName(QStringLiteral("add_image"));
    if (ac) {
        ac->addAction(QStringLiteral("add_image"), d->action_add_image);
    }
    connect(d->action_add_image, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotAddImage);
    d->richTextActionList.append(d->action_add_image);

    d->action_add_emoticon = new KPIMTextEdit::EmoticonTextEditAction(this);
    d->action_add_emoticon->setObjectName(QStringLiteral("add_emoticon"));
    if (ac) {
        ac->addAction(QStringLiteral("add_emoticon"), d->action_add_emoticon);
    }
    // Don't add to d->richTextActionList as we want to use it when we use plain text email too
    connect(d->action_add_emoticon, &EmoticonTextEditAction::emoticonActivated, d->composerControler->richTextComposer(), &RichTextComposer::insertEmoticon);

    d->action_insert_html = new QAction(i18n("Insert HTML"), this);
    d->action_insert_html->setObjectName(QStringLiteral("insert_html"));
    if (ac) {
        ac->addAction(QStringLiteral("insert_html"), d->action_insert_html);
    }
    connect(d->action_insert_html, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotInsertHtml);
    d->richTextActionList.append(d->action_insert_html);

    d->action_add_table = new KPIMTextEdit::TableActionMenu(d->composerControler->richTextComposer());
    d->action_add_table->setIcon(QIcon::fromTheme(QStringLiteral("insert-table")));
    d->action_add_table->setText(i18n("Table"));
    d->action_add_table->setPopupMode(QToolButton::InstantPopup);

    d->action_add_table->setObjectName(QStringLiteral("insert_table"));
    d->richTextActionList.append(d->action_add_table);
    if (ac) {
        ac->addAction(QStringLiteral("insert_table"), d->action_add_table);
    }

    d->action_delete_line = new QAction(i18n("Delete Line"), this);
    if (ac) {
        ac->setDefaultShortcut(d->action_delete_line, QKeySequence(Qt::CTRL | Qt::Key_K));
    }
    d->action_delete_line->setObjectName(QStringLiteral("delete_line"));
    if (ac) {
        ac->addAction(QStringLiteral("delete_line"), d->action_delete_line);
    }
    connect(d->action_delete_line, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotDeleteLine);
    d->richTextActionList.append(d->action_delete_line);

    d->action_format_reset = new QAction(QIcon::fromTheme(QStringLiteral("draw-eraser")), i18n("Reset Font Settings"), this);
    d->action_format_reset->setIconText(i18n("Reset Font"));
    d->action_format_reset->setObjectName(QStringLiteral("format_reset"));
    connect(d->action_format_reset, &QAction::triggered, d->composerControler, &RichTextComposerControler::slotFormatReset);
    if (ac) {
        ac->addAction(QStringLiteral("format_reset"), d->action_format_reset);
    }
    d->richTextActionList.append(d->action_format_reset);

    d->action_format_painter = new KToggleAction(QIcon::fromTheme(QStringLiteral("draw-brush")), i18nc("@action", "Format Painter"), this);
    d->richTextActionList.append(d->action_format_painter);
    d->action_format_painter->setObjectName(QStringLiteral("format_painter"));
    if (ac) {
        ac->addAction(QStringLiteral("format_painter"), d->action_format_painter);
    }
    connect(d->action_format_painter, &QAction::toggled, d->composerControler, &RichTextComposerControler::slotFormatPainter);

    d->action_heading_level = new KSelectAction(i18nc("@title:menu", "Heading Level"), this);
    const QStringList headingLevels = {i18nc("@item:inmenu no heading", "Basic text"),
                                       i18nc("@item:inmenu heading level 1 (largest)", "Title"),
                                       i18nc("@item:inmenu heading level 2", "Subtitle"),
                                       i18nc("@item:inmenu heading level 3", "Section"),
                                       i18nc("@item:inmenu heading level 4", "Subsection"),
                                       i18nc("@item:inmenu heading level 5", "Paragraph"),
                                       i18nc("@item:inmenu heading level 6 (smallest)", "Subparagraph")};

    d->action_heading_level->setItems(headingLevels);
    d->action_heading_level->setCurrentItem(0);
    d->richTextActionList.append(d->action_heading_level);
    d->action_heading_level->setObjectName(QStringLiteral("format_heading_level"));
    connect(d->action_heading_level, &KSelectAction::indexTriggered, this, &RichTextComposerActions::setHeadingLevel);
    if (ac) {
        ac->addAction(QStringLiteral("format_heading_level"), d->action_heading_level);
    }

    d->action_list_checkbox = new KToggleAction(QIcon::fromTheme(QStringLiteral("checkbox")), i18nc("@action", "Checkbox"), this);
    d->richTextActionList.append(d->action_list_checkbox);
    d->action_list_checkbox->setObjectName(QStringLiteral("format_list_checkbox"));
    connect(d->action_list_checkbox, &KToggleAction::toggled, d->composerControler, &RichTextComposerControler::addCheckbox);
    if (ac) {
        ac->addAction(QStringLiteral("format_list_checkbox"), d->action_list_checkbox);
    }

    disconnect(d->composerControler->richTextComposer(), &QTextEdit::currentCharFormatChanged, this, &RichTextComposerActions::slotUpdateCharFormatActions);
    disconnect(d->composerControler->richTextComposer(), &QTextEdit::cursorPositionChanged, this, &RichTextComposerActions::slotUpdateMiscActions);

    connect(d->composerControler->richTextComposer(), &QTextEdit::currentCharFormatChanged, this, &RichTextComposerActions::slotUpdateCharFormatActions);
    connect(d->composerControler->richTextComposer(), &QTextEdit::cursorPositionChanged, this, &RichTextComposerActions::slotUpdateMiscActions);

    updateActionStates();
}

void RichTextComposerActions::updateActionStates()
{
    slotUpdateMiscActions();
    slotUpdateCharFormatActions(d->composerControler->richTextComposer()->currentCharFormat());
}

void RichTextComposerActions::setHeadingLevel(int level)
{
    d->composerControler->setHeadingLevel(level);
    slotUpdateMiscActions();
}

void RichTextComposerActions::setListStyle(int _styleindex)
{
    d->composerControler->setListStyle(_styleindex);
    slotUpdateMiscActions();
}

void RichTextComposerActions::setActionsEnabled(bool enabled)
{
    for (QAction *action : std::as_const(d->richTextActionList)) {
        action->setEnabled(enabled);
    }
    d->richTextEnabled = enabled;
}

void RichTextComposerActions::slotUpdateCharFormatActions(const QTextCharFormat &format)
{
    QFont f = format.font();

    d->action_font_family->setFont(f.family());
    if (f.pointSize() > 0) {
        d->action_font_size->setFontSize(f.pointSize());
    }
    d->action_text_bold->setChecked(f.bold());
    d->action_text_italic->setChecked(f.italic());
    d->action_text_underline->setChecked(f.underline());
    d->action_text_strikeout->setChecked(f.strikeOut());
    const QTextCharFormat::VerticalAlignment vAlign = format.verticalAlignment();
    d->action_text_superscript->setChecked(vAlign == QTextCharFormat::AlignSuperScript);
    d->action_text_subscript->setChecked(vAlign == QTextCharFormat::AlignSubScript);
}

void RichTextComposerActions::slotUpdateMiscActions()
{
    const RichTextComposer *richTextComposer = d->composerControler->richTextComposer();
    const Qt::Alignment a = richTextComposer->alignment();
    if (a & Qt::AlignLeft) {
        d->action_align_left->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        d->action_align_center->setChecked(true);
    } else if (a & Qt::AlignRight) {
        d->action_align_right->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        d->action_align_justify->setChecked(true);
    }
    if (richTextComposer->textCursor().currentList()) {
        d->action_list_style->setCurrentItem(-richTextComposer->textCursor().currentList()->format().style());
    } else {
        d->action_list_style->setCurrentItem(0);
    }
    if (d->richTextEnabled) {
        d->action_list_indent->setEnabled(d->composerControler->canIndentList());
    } else {
        d->action_list_indent->setEnabled(false);
    }
    if (d->richTextEnabled) {
        d->action_list_dedent->setEnabled(d->composerControler->canDedentList());
    } else {
        d->action_list_dedent->setEnabled(false);
    }

    const Qt::LayoutDirection direction = richTextComposer->textCursor().blockFormat().layoutDirection();
    d->action_direction_ltr->setChecked(direction == Qt::LeftToRight);
    d->action_direction_rtl->setChecked(direction == Qt::RightToLeft);
    d->action_heading_level->setCurrentItem(richTextComposer->textCursor().blockFormat().headingLevel());
    d->action_list_checkbox->setChecked(richTextComposer->textCursor().blockFormat().marker() != QTextBlockFormat::MarkerType::NoMarker);
}

void RichTextComposerActions::uncheckActionFormatPainter()
{
    d->action_format_painter->setChecked(false);
}

void RichTextComposerActions::textModeChanged(KPIMTextEdit::RichTextComposer::Mode mode)
{
    if (d->action_add_table) {
        d->action_add_table->setRichTextMode(mode == KPIMTextEdit::RichTextComposer::Rich);
    }
}
