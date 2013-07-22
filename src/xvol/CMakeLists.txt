IF(OPENGL_FOUND AND XMU_FOUND)
    INCLUDE_DIRECTORIES(
        ${ATLAS_SOURCE_DIR}/../include
        ${OPENGL_INCLUDE_DIR}
        )

    SET(xvol_SOURCES
        xvol.cpp
    )

    ADD_EXECUTABLE(xvol
        ${xvol_SOURCES}
        )

    TARGET_LINK_LIBRARIES(xvol
        volren volimg vff tpcimgio tpcmisc
        ${OPENGL_LIBRARIES}
        ${XMU_LIBRARY}
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No OpenGL or Xmu -- 'xvol' will not be built!\n"
        )
ENDIF()

