
set_property(GLOBAL PROPERTY PROPERTY_OUTPUT_FORMAT elf32-littlearm)


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(mcu_arch cortex-m33+nodsp)

if (DEFINED ENV{TOOLCHAIN_PREFIX})
    set(TOOLCHAIN_PREFIX $ENV{TOOLCHAIN_PREFIX})
    message("-- TOOLCHAIN_PREFIX: " ${TOOLCHAIN_PREFIX})
elseif (NOT DEFINED TOOLCHAIN_PREFIX)
    set(TOOLCHAIN_PREFIX arm-none-eabi-)
endif()

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)

set(CMAKE_READELF ${TOOLCHAIN_PREFIX}readelf)

# set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
# set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)


set(CMAKE_C_FLAGS "-mthumb -mcpu=${mcu_arch} -fno-builtin -fdata-sections -ffunction-sections -fno-short-enums -mfloat-abi=soft -fno-short-wchar -fno-exceptions -fno-unwind-tables -Xlinker -no-enum-size-warning")
set(CMAKE_CXX_FLAGS "-mthumb -mcpu=${mcu_arch} -fno-rtti -fno-builtin -fdata-sections -ffunction-sections -fstrict-enums -fno-short-enums -mfloat-abi=soft -fno-short-wchar -fno-exceptions")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=${mcu_arch} -mfloat-abi=soft -fno-exceptions -fno-builtin -fno-unwind-tables")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -g3 -ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -g3 -ggdb")
set(CMAKE_ASM_FLAGS_DEBUG "-g -g3 -ggdb")

set(CMAKE_C_FLAGS_RELEASE "-O0 -flto")
set(CMAKE_CXX_FLAGS_RELEASE "-O0 -flto")
set(CMAKE_ASM_FLAGS_RELEASE "")

set(CMAKE_C_LINKER_WRAPPER_FLAG "-Wl,")
set(CMAKE_CXX_LINKER_WRAPPER_FLAG "-Wl,")
set(CMAKE_ASM_LINKER_WRAPPER_FLAG "-Wl,")

set(CMAKE_C_LINKER_WRAPPER_FLAG_SEP ",")
set(CMAKE_CXX_LINKER_WRAPPER_FLAG_SEP ",")
set(CMAKE_ASM_LINKER_WRAPPER_FLAG_SEP ",")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-M  -Wl,--gc-sections -mthumb -mcpu=${mcu_arch} -fno-short-enums -fno-short-wchar -fno-exceptions -Wl,--print-memory-usage")

set(CMAKE_C_COMPILER_WORKS ON)
set(CMAKE_CXX_COMPILER_WORKS ON)
set(CMAKE_ASM_COMPILER_WORKS ON)


add_executable(app)

target_include_directories(app
    PUBLIC
)


set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/user-application.ld)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

target_sources(app 
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/phy-api.c
    ${CMAKE_CURRENT_LIST_DIR}/phy-api-instance.c
    ${CMAKE_CURRENT_LIST_DIR}/user-application-instance.c
)

target_compile_options(app
    PUBLIC
    -DUSER_FLASH_APP=1
    -nostdlib
    -mno-unaligned-access
    -fno-short-enums
)

target_link_options(app
   PUBLIC
    ${CPU_OPTIONS}
    -T${LINKER_SCRIPT}
    -L${CMAKE_CURRENT_LIST_DIR}
    -nostartfiles
    -fno-short-enums
    -Xlinker -Map=output.map
)

target_link_options(app
    PRIVATE
        LINKER:-Map=output.map
        -T${LINKER_SCRIPT}       
        -L${CMAKE_CURRENT_LIST_DIR} 
)

target_link_options(app
    PRIVATE
        --specs=nano.specs
        --specs=nosys.specs
)

target_link_libraries(app
    PRIVATE
    m
)

set_target_properties(app
    PROPERTIES
        SUFFIX ".elf"
)

set_property(TARGET app PROPERTY LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex app.elf app.hex
        COMMAND ${CMAKE_OBJCOPY} -Obinary app.elf app.bin
        COMMENT "Building HEX and BIN files")


add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_SIZE} app.elf 
        COMMENT "size of application")
