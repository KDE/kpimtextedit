if (USE_NEW_DESIGNERPLUGIN_MACRO)
    include(ECMAddQtDesignerPlugin)
    ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextEditorWidget
        INCLUDE_FILE "kpimtextedit/richtexteditorwidget.h"
        TOOLTIP "A RichTextEditor (KDE-PIM)"
        WHATSTHIS "A RichTextEditor."
        GROUP "Input (KDE-PIM)"
        )

    ecm_qtdesignerplugin_widget(KPIMTextEdit::PlainTextEditorWidget
        INCLUDE_FILE "kpimtextedit/plaintexteditorwidget.h"
        TOOLTIP "A PlainTextEditor (KDE-PIM)"
        WHATSTHIS "A PlainTextEditor."
        GROUP "Input (KDE-PIM)"
        )

    ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposer
        INCLUDE_FILE "kpimtextedit/richtextcomposer.h"
        TOOLTIP "A RichText Composer (KDE-PIM)"
        WHATSTHIS "A RichText Composer."
        GROUP "Input (KDE-PIM)"
        )

    ecm_qtdesignerplugin_widget(KPIMTextEdit::RichTextComposerWidget
        INCLUDE_FILE "kpimtextedit/richtextcomposerwidget.h"
        TOOLTIP "A RichText Composer Widget(KDE-PIM)"
        WHATSTHIS "A RichText Composer Widget."
        GROUP "Input (KDE-PIM)"
        )

    ecm_add_qtdesignerplugin(kpimtexteditwidgets
        NAME KPimTextEditWidgets
        OUTPUT_NAME kpimtexteditwidgets
        WIDGETS
        KPIMTextEdit::RichTextEditorWidget
        KPIMTextEdit::PlainTextEditorWidget
        KPIMTextEdit::RichTextComposer
        KPIMTextEdit::RichTextComposerWidget
        LINK_LIBRARIES
        KF5::PimTextEdit
        Qt5::Widgets
        INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
        COMPONENT Devel
        )

else()	
    if (KF5DesignerPlugin_FOUND)
        kf5designerplugin_add_plugin(kpimtexteditwidgets kpimtexteditwidgets.widgets)
        target_link_libraries(kpimtexteditwidgets
            KF5::PimTextEdit Qt5::Widgets
            )
        set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/kpimtexteditwidgetswidgets.cpp PROPERTY SKIP_AUTOMOC ON)
        if(NOT WIN32)
            # Since there are no libraries provided by this module,
            # there is no point including the build tree in RPath,
            # and then having to edit it at install time.
            set_target_properties(kpimtexteditwidgets PROPERTIES
                SKIP_BUILD_RPATH TRUE
                BUILD_WITH_INSTALL_RPATH TRUE
                )
        endif()

        install(TARGETS
            kpimtexteditwidgets
            DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer
            )
    endif()
endif()
