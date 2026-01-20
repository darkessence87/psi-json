include_directories(
    include
    src
)

set(SOURCES
    src/psi/json/JArray.cpp
    src/psi/json/JObject.cpp
    src/psi/json/JsonParser.cpp
    src/psi/json/JTree.cpp
    src/psi/json/JTypes.cpp
)

add_library(psi-json ${SOURCES})
psi_config_target(psi-json)

set(TEST_SRC
    tests/JsonParserTests.cpp
    tests/JTypesTests.cpp
)
psi_make_tests("Json" "${TEST_SRC}" "psi-json")

if(${PSI_BUILD_TESTS})
    add_custom_command(TARGET PSI_TEST_Json POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/tests/test_data ${BUILD_OUT})
endif()

psi_make_examples("1_JsonExamples" "examples/1_JsonExamples.cpp" "psi-json")