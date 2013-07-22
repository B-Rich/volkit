IF(QT4_FOUND AND OPENGL_FOUND AND GLEW_FOUND)
    INCLUDE_DIRECTORIES(
        ${ATLAS_SOURCE_DIR}/../include/tpc
        ${QT_INCLUDES} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDES}
        )

    SET(qvol_SOURCES
        sampletool.cpp polytool.cpp imgparams.cpp
        imgwidget.cpp imgwindow.cpp
        volwidget.cpp volwindow.cpp
        main.cpp
    )
    SET(qvol_HEADERS imgparams.h imgwindow.h volwindow.h)
    SET(qvol_RESOURCES application.qrc)

    QT4_WRAP_CPP(qvol_HEADERS_MOC ${qvol_HEADERS})
    QT4_ADD_RESOURCES(qvol_RESOURCES_RCC ${qvol_RESOURCES})
    SET(QT_USE_QTOPENGL TRUE)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})

    ADD_EXECUTABLE(qvol
        ${qvol_SOURCES}
        ${qvol_HEADERS_MOC}
        ${qvol_RESOURCES_RCC}
        )

    TARGET_LINK_LIBRARIES(qvol
        volimg vff tpcimgio tpcmisc
        ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No QT/OpenGL -- 'qvol' will not be built!\n"
        )
ENDIF()
