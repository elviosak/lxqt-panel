set(PLUGIN "wingmenu")

set(HEADERS
  wingmenuplugin.h
  wingmenuconfiguration.h
  wingmenuwidget.h
  itemdescriptiondelegate.h
  applicationsview.h
  common.h
)

set(SOURCES
  wingmenuplugin.cpp
  wingmenuconfiguration.cpp
  wingmenuwidget.cpp
  itemdescriptiondelegate.cpp
  applicationsview.cpp
)

set(UIS
  wingmenuconfiguration.ui
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