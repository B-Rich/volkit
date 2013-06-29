IF(QT4_FOUND AND OPENGL_FOUND)
    SET(QT_USE_QTOPENGL TRUE)
    INCLUDE(${QT_USE_FILE})

    INCLUDE_DIRECTORIES(${ATLAS_SOURCE_DIR}/../include/tpc
        ${QT_INCLUDES} ${OPENGL_INCLUDE_DIR}
        )

    ADD_EXECUTABLE(atlas
        img.cpp v.cpp colormap.cpp
        glwidget.cpp window.cpp atlas.cpp
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
