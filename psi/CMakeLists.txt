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

set(TEST_SRC
    tests/JsonParserTests.cpp
    tests/JTypesTests.cpp
)
psi_make_tests("Json" "${TEST_SRC}" "psi-json")

if(NOT EXISTS ${PSI_BUILD_TESTS})
    set(BUILD_TESTS true)
else()
    set(BUILD_TESTS ${PSI_BUILD_TESTS})
endif()
if(${BUILD_TESTS})
    add_custom_command(TARGET PSI_TEST_Json POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/tests/test_data ${BUILD_OUT})
endif()

psi_make_examples("1_JsonExamples" "examples/1_JsonExamples.cpp" "psi-json")