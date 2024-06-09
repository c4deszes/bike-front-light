cmake_minimum_required(VERSION 3.20)

project(
    front-light
    DESCRIPTION "Front Light"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

message("-- Front Light profile: ${PROJECT_PROFILE}")
include(profiles/${PROJECT_PROFILE}.cmake)
message("\tBoard LED driver: ${BOARD_LED_DRIVER}")

find_package(Python REQUIRED)

include(tools/cmake/CPM.cmake)

CPMAddPackage("gh:c4deszes/samd21-hal#feature/atsamc21-support")
CPMAddPackage("gh:c4deszes/bike-line-protocol#feature/python-tool-improvements")
CPMAddPackage("gh:c4deszes/bike-flash-tool#master")
# CPMAddPackage("gh:c4deszes/samd21-line-bootloader#master")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

include(CTest)
include(GoogleTest)

line_codegen(
    TARGET protocol-stack-api
    NETWORK ../customer/network.json
    NODE FrontLight
    ADAPTER
)

samx21_hal(samc21-hal ATSAMC21E18A)

add_executable(
    application
    EXCLUDE_FROM_ALL

    src/app/brightness.c
    src/app/comm.c
    src/app/init.c
    src/app/main.c
    src/app/strobe.c
    src/app/sys_state.c
    src/app/tasks.c

    src/bsp/board.c
    $<$<STREQUAL:${BOARD_LED_DRIVER},ILD8150>:src/bsp/led_driver_ild8150.c>
    $<$<STREQUAL:${BOARD_LED_DRIVER},TLD5099>:src/bsp/led_driver_tld509x.c>
    src/bsp/usart.c
)
# TODO: enable, but needs header as well
#target_compile_definitions(application PRIVATE BOARD_LED_DRIVER=${BOARD_LED_DRIVER})
target_include_directories(application PRIVATE include)
target_link_libraries(application PRIVATE   cmsis
                                            samc21-hal
                                            flash-line-api flash-line-sources
                                            protocol-stack-api
                                            # bootloader-api)
)
set_target_properties(application PROPERTIES SUFFIX ".elf")
target_link_directories(application PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ld)
target_link_options(application PRIVATE -T$<TARGET_PROPERTY:samc21-hal,DEFAULT_LINKERSCRIPT>)
target_link_options(application PRIVATE -Wl,-Map=application.map -Wl,--gc-sections)

hexify(TARGET application HEX ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex)
dump_symbols(application ${CMAKE_CURRENT_BINARY_DIR}/application.lss)
dump_size(application)

# bootloader_header(
#     TARGET factory_header
#     CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tools/bootloader/boot_config.json
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tools/openocd/factory_header.hex
# )

# bootloader_header(
#     TARGET app_header
#     CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tools/bootloader/boot_config.json
#     APPLICATION ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex
# )

# add_custom_target(application-merge
#     DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex
#     BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/application.hex
#     COMMAND srec_cat
#             ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex -intel
#             ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex -intel
#             -o ${CMAKE_CURRENT_BINARY_DIR}/application.hex -intel
# )

add_custom_target(application-full
    DEPENDS application-hex application-lss application-memory
)
