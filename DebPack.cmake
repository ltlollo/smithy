include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${smithy_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${smithy_VERSION_MINOR}")
SET(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "ltlollo")
set(CPACK_PACKAGE_CONTACT "example@example.example")
set(CPACK_SET_DESTDIR on)
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libsfml-graphics2 (>= 2.1+dfsg2-1),
    libsfml-system2 (>= 2.1+dfsg2-1),
    libsfml-window2 (>= 2.1+dfsg2-1),
    fonts-dejavu (>= 2.34-1)")

include(CPack)
