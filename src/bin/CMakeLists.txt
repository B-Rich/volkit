IF(OPENGL_FOUND AND XMU_FOUND)
    INCLUDE_DIRECTORIES(
        ${ATLAS_SOURCE_DIR}/../include
        ${OPENGL_INCLUDE_DIR}
        )

    ADD_EXECUTABLE(xvolren xvolren.cpp)
    TARGET_LINK_LIBRARIES(xvolren
        volren volgeo volimg vff tpcimgio tpcmisc
        ${OPENGL_LIBRARIES}
        ${XMU_LIBRARY}
        )
ELSE()
    MESSAGE(STATUS
        "\n\n   WARNING: No OpenGL or Xmu -- 'xvolren' will not be built!\n"
        )
ENDIF()

