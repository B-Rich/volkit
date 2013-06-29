IF(QT4_FOUND AND OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${ATLAS_SOURCE_DIR}/../include/tpc
        ${QT_INCLUDES} ${OPENGL_INCLUDE_DIR}
        )

    SET(atlas_SOURCES
        img.cpp v.cpp colormap.cpp
        glwidget.cpp window.cpp atlas.cpp
    )
    SET(atlas_HEADERS window.h)

    QT4_WRAP_CPP(atlas_HEADERS_MOC ${atlas_HEADERS})
    SET(QT_USE_QTOPENGL TRUE)
    INCLUDE(${QT_USE_FILE})
    ADD_DEFINITIONS(${QT_DEFINITIONS})

    ADD_EXECUTABLE(atlas
        ${atlas_SOURCES}
        ${atlas_HEADERS_MOC}
        )

    TARGET_LINK_LIBRARIES(atlas
        tpcimgio tpcmisc
        ${QT_LIBRARIES} ${OPENGL_LIBRARIES}
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No QT/OpenGL -- 'atlas' will not be built!\n"
        )
ENDIF()
