aux_source_directory(. UT_SRC_FILES)

add_executable(UT_LD19 ${UT_SRC_FILES})
target_link_libraries(UT_LD19 PRIVATE LD19_LIB gtest gtest_main)