add_executable(Lab2CPP
    main.cpp
)

target_link_libraries(Lab2CPP
    PRIVATE
        LibraryCPP
)

target_include_directories(Lab2CPP
    PRIVATE
        ${CMAKE_SOURCE_DIR}/LibraryCPP
)

function(add_lab2_test TEST_NAME INPUT_FILE EXPECTED_FILE)
    set(ACTUAL_FILE
        "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}_actual.txt"
    )

    add_test(
        NAME "${TEST_NAME}"
        COMMAND "${CMAKE_COMMAND}"
            "-DPROGRAM=$<TARGET_FILE:Lab2CPP>"
            "-DINPUT=${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}"
            "-DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/${EXPECTED_FILE}"
            "-DACTUAL=${ACTUAL_FILE}"
            "-P"
            "${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake"
    )
endfunction()

add_lab2_test(
    Lab2_example_bad_ret
    input_bad_ret.txt
    output_bad_ret.txt
)

add_lab2_test(
    Lab2_example_success
    input_success.txt
    output_success.txt
)

add_lab2_test(
    Lab2_bad_pop_return_address
    input_bad_pop.txt
    output_bad_pop.txt
)

add_lab2_test(
    Lab2_arithmetic
    input_arithmetic.txt
    output_arithmetic.txt
)