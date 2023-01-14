add_library(waterer)

target_sources(
  waterer
  PUBLIC #
         model/FBSettings.cpp
         #
         presenter/Presenter.cpp
         #
         view/Display.cpp
         view/DisplayKeys.cpp
         view/Key.cpp
         view/TabBase.cpp
         view/CurrentTimeTab.cpp
         view/FBHourTab.cpp
         view/FBHumidityTab.cpp
         view/Application.cpp)

target_link_libraries(waterer GTest::gmock_main)
