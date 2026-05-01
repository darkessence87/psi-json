set(SOURCES
    src/psi/json/JArray.cpp
    src/psi/json/JObject.cpp
    src/psi/json/JsonParser.cpp
    src/psi/json/JTree.cpp
    src/psi/json/JTypes.cpp
)

set(target_lib "psi-json")

add_library(${target_lib} STATIC ${SOURCES})
add_library(psi::json ALIAS ${target_lib})

target_include_directories(${target_lib}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

psi_config_target(${target_lib})

set(TEST_SRC
    tests/JsonParserTests.cpp
    tests/JTypesTests.cpp
)
psi_make_tests("Json" "${TEST_SRC}" "${target_lib}")

if(${PSI_BUILD_TESTS})
    add_custom_command(TARGET PSI_TEST_Json POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/tests/test_data ${BUILD_OUT})
endif()

psi_make_examples("1_JsonExamples" "examples/1_JsonExamples.cpp" "${target_lib}")