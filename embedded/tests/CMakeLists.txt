enable_testing()

add_executable(
  test_runner
  ./model/feedback_runner_test.cpp ./view/display_test.cpp ./view/key_test.cpp
  ./view/current_time_tab_test.cpp ./view/formatting_test.cpp)
target_link_libraries(test_runner waterer GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(test_runner)
