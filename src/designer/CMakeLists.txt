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
