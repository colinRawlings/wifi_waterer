add_library(waterer)

target_sources(
  waterer
  PUBLIC Updateable.cpp
         Types.cpp
         #
         model/FBSettings.cpp
         #
         presenter/Presenter.cpp
         #
         view/Display.cpp
         view/DisplayKeys.cpp
         view/Key.cpp
         view/TabBase.cpp
         view/CurrentTimeTab.cpp
         view/HomeTab.cpp
         view/ManualPumpTab.cpp
         view/FBDurationTab.cpp
         view/SaveTab.cpp
         view/WifiTab.cpp
         view/FBHourTab.cpp
         view/FBHumidityTab.cpp
         view/SoftkeyDisplay.cpp
         view/Helpers.cpp)

target_link_libraries(waterer GTest::gmock_main)
