cmake_minimum_required(VERSION 3.24)
project(ClamAV-qt)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Network Gui Core5Compat)

set(FILE_TS
    translations/clamav-gui-da_DK.ts
    translations/clamav-gui-de_DE.ts
    translations/clamav-gui-en_GB.ts
    translations/clamav-gui-es_ES.ts
    )

set(FILE_UI
    ui/clamav_ctrl.ui
    ui/clamav_gui.ui
    ui/freshclamsetter.ui
    ui/logviewerobject.ui
    ui/partiallogobject.ui
    ui/scantab.ui
    ui/schedulescanobject.ui
    ui/includeexcludeoptions.ui
    ui/logviewobject.ui
    ui/profilemanager.ui
    ui/progressdialog.ui
    ui/scheduler.ui
    ui/dragablepushbutton.ui
    ui/infodialog.ui
    ui/optionsdialog.ui
    ui/profilewizarddialog.ui
    ui/scanlimitstab.ui
    )

if(QT_VERSION_MAJOR EQUAL 5)
    qt5_add_translation(QM_FILES ${FILE_TS})
    qt5_wrap_ui(UI_FILES_H ${FILE_UI})

    set(TRANSLATION_QRC_FILE "${CMAKE_CURRENT_BINARY_DIR}/translations.qrc")
    set(TRANSLATION_FILE_LIST "")
    foreach(QM_FILE ${QM_FILES})
        get_filename_component(QM_FILE_BASENAME "${QM_FILE}" NAME)
        string(APPEND TRANSLATION_FILE_LIST "        <file>${QM_FILE_BASENAME}</file>\n")
    endforeach()
    configure_file("translations.qrc.in" "${TRANSLATION_QRC_FILE}")

    # Target to update translation (.ts) files
    file(READ GDAVDialog.qrc QRC_FILE)
    string(REPLACE "<file>" . QRC_FILE ${QRC_FILE})
    string(REPLACE "\n" "" QRC_FILE ${QRC_FILE})
    string(REPLACE " " "" QRC_FILE ${QRC_FILE})
    string(REPLACE "</file>" ";" QRC_FILE ${QRC_FILE})

    add_custom_target(BusinessUI_UpdateTranslations
        COMMAND Qt5::lupdate -locations relative ${FILE_CPP} ${FILE_H} ${FILE_UI} -ts ${FILE_TS}
        BYPRODUCTS ${FILE_TS}
        )
else()
    set(TRANSLATION_QRC_FILE "")
    qt6_wrap_ui(UI_FILES_H ${FILE_UI})
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(ClamAV-qt
    ${FILE_TS}
    ${TRANSLATION_QRC_FILE}
    ${UI_FILES_H}
    resources.qrc
    src/main.cpp
    src/clamav_gui.cpp
    src/clamav_gui.h
    src/cfilesystemmodel.cpp
    src/cfilesystemmodel.h
    src/optionsdialog.cpp
    src/optionsdialog.h
    src/scantab.cpp
    src/scantab.h
    src/setupfilehandler.cpp
    src/setupfilehandler.h
    src/clamav_ctrl.cpp
    src/clamav_ctrl.h
    src/scanlimitstab.cpp
    src/scanlimitstab.h
    src/includeexcludeoptions.cpp
    src/includeexcludeoptions.h
    src/freshclamsetter.cpp
    src/freshclamsetter.h
    src/dragablepushbutton.cpp
    src/dragablepushbutton.h
    src/scheduler.cpp
    src/scheduler.h
    src/highlighter.cpp
    src/highlighter.h
    src/infodialog.cpp
    src/infodialog.h
    src/schedulescanobject.cpp
    src/schedulescanobject.h
    src/logviewobject.cpp
    src/logviewerobject.h
    src/partiallogobject.cpp
    src/partiallogobject.h
    src/logviewerobject.cpp
    src/logviewerobject.h
    src/qroundprogressbar.cpp
    src/qroundprogressbar.h
    src/progressdialog.cpp
    src/progressdialog.h
    src/profilewizarddialog.cpp
    src/profilewizarddialog.h
    src/profilemanager.cpp
    src/profilemanager.h
    )
target_include_directories(ClamAV-qt
    PRIVATE
    ${CMAKE_BINARY_DIR}
    src
    )

target_link_libraries(ClamAV-qt
    PRIVATE
    Qt6::Gui
    Qt6::Widgets
    Qt6::Core
    Qt6::Network
    Qt6::Core5Compat
    )
