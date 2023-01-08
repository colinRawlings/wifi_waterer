add_library(waterer)

target_sources(waterer PUBLIC
#
presenter/Presenter.cc
#
view/Display.cc view/Keys.cc view/Key.cc view/TabBase.cc view/CurrentTimeTab.cc )

target_link_libraries(
  waterer
  GTest::gmock_main
)