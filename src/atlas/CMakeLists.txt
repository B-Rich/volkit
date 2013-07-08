IF(QT4_FOUND AND OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${ATLAS_SOURCE_DIR}/../include/tpc
        ${QT_INCLUDES} ${OPENGL_INCLUDE_DIR}
        )

    SET(atlas_SOURCES
        sampletool.cpp polytool.cpp imgparams.cpp
        imgwidget.cpp imgwindow.cpp atlas.cpp
    )
    SET(atlas_HEADERS imgparams.h imgwindow.h)
    SET(atlas_RESOURCES application.qrc)

    QT4_WRAP_CPP(atlas_HEADERS_MOC ${atlas_HEADERS})
    QT4_ADD_RESOURCES(atlas_RESOURCES_RCC ${atlas_RESOURCES})
    SET(QT_USE_QTOPENGL TRUE)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})

    ADD_EXECUTABLE(atlas
        ${atlas_SOURCES}
        ${atlas_HEADERS_MOC}
        ${atlas_RESOURCES_RCC}
        )

    TARGET_LINK_LIBRARIES(atlas
        img vff tpcimgio tpcmisc
        ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No QT/OpenGL -- 'atlas' will not be built!\n"
        )
ENDIF()
