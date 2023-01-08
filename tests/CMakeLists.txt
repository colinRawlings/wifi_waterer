enable_testing()

add_executable(
  test_runner
  ./view/display_test.cc
  ./view/key_test.cc
  ./view/current_time_tab_test.cc
)
target_link_libraries(
  test_runner
  waterer
  GTest::gtest_main
  GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(test_runner)