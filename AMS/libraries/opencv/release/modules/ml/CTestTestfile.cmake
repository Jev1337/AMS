# CMake generated Testfile for 
# Source directory: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/ml
# Build directory: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/modules/ml
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_ml "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/bin/opencv_test_ml.exe" "--gtest_output=xml:opencv_test_ml.xml")
set_tests_properties(opencv_test_ml PROPERTIES  LABELS "Main;opencv_ml;Accuracy" WORKING_DIRECTORY "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/test-reports/accuracy")
