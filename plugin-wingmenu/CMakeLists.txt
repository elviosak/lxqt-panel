set(PLUGIN "wingmenu")

set(HEADERS
  "${PLUGIN}plugin.h"
  "${PLUGIN}configuration.h"
  "${PLUGIN}widget.h"
  itemdescriptiondelegate.h
  applicationsview.h
  common.h
)

set(SOURCES
  "${PLUGIN}plugin.cpp"
  "${PLUGIN}configuration.cpp"
  "${PLUGIN}widget.cpp"
  itemdescriptiondelegate.cpp
  applicationsview.cpp
)

set(UIS
  "${PLUGIN}configuration.ui"
)

set(LIBRARIES
  ${LIBRARIES}
  lxqt
  lxqt-globalkeys-ui
  Qt5Xdg
  Qt5Xml
)
set(ICON_SHARE_DIR ${CMAKE_INSTALL_FULL_DATAROOTDIR}/icons/hicolor/scalable/apps)
set(ICON_FILE "resources/wing-lxqt.svg")
install(FILES "${ICON_FILE}" DESTINATION "${ICON_SHARE_DIR}")

BUILD_LXQT_PLUGIN(${PLUGIN})