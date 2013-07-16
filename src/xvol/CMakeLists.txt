IF(OPENGL_FOUND)
    INCLUDE_DIRECTORIES(${ATLAS_SOURCE_DIR}/../include/tpc
        ${OPENGL_INCLUDE_DIR}
        )

    SET(xvol_SOURCES
        window.cpp
    )

    ADD_EXECUTABLE(xvol
        ${xvol_SOURCES}
        )

    TARGET_LINK_LIBRARIES(xvol
        img vff tpcimgio tpcmisc
        ${OPENGL_LIBRARIES} Xmu
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No OpenGL -- 'xvol' will not be built!\n"
        )
ENDIF()

