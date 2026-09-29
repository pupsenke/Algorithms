if(NOT DEFINED PROGRAM)
    message(FATAL_ERROR "PROGRAM is not defined")
endif()

if(NOT DEFINED INPUT)
    message(FATAL_ERROR "INPUT is not defined")
endif()

if(NOT DEFINED EXPECTED)
    message(FATAL_ERROR "EXPECTED is not defined")
endif()

if(NOT DEFINED ACTUAL)
    message(FATAL_ERROR "ACTUAL is not defined")
endif()

execute_process(
    COMMAND "${PROGRAM}" "${INPUT}"
    OUTPUT_FILE "${ACTUAL}"
    ERROR_FILE "${ACTUAL}.error.txt"
    RESULT_VARIABLE PROGRAM_RESULT
)

if(NOT PROGRAM_RESULT STREQUAL "0")
    file(READ "${ACTUAL}.error.txt" PROGRAM_ERROR)

    message(
        FATAL_ERROR
        "Program returned error code: ${PROGRAM_RESULT}\n"
        "Program error output:\n${PROGRAM_ERROR}"
    )
endif()

file(READ "${EXPECTED}" EXPECTED_TEXT)
file(READ "${ACTUAL}" ACTUAL_TEXT)

# Нормализация переводов строк и удаление завершающих пробелов/переводов строк
string(REPLACE "\r\n" "\n" EXPECTED_TEXT "${EXPECTED_TEXT}")
string(REPLACE "\r\n" "\n" ACTUAL_TEXT "${ACTUAL_TEXT}")
string(STRIP "${EXPECTED_TEXT}" EXPECTED_TEXT)
string(STRIP "${ACTUAL_TEXT}" ACTUAL_TEXT)

if(NOT EXPECTED_TEXT STREQUAL ACTUAL_TEXT)
    message(
        FATAL_ERROR
        "Output differs from expected output.\n\n"
        "Expected output:\n${EXPECTED_TEXT}\n"
        "Actual output:\n${ACTUAL_TEXT}\n"
    )
endif()