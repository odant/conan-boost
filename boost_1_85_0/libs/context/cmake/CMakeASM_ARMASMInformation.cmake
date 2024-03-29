
# for the armasm assembler, armasm or armasm64

set(ASM_DIALECT "_ARMASM")

set(CMAKE_ASM${ASM_DIALECT}_SOURCE_FILE_EXTENSIONS asm)

set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OBJECT "<CMAKE_ASM${ASM_DIALECT}_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")

# The ASM_ARMASM compiler id for this compiler is "MSVC", so fill out the runtime library table.
set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreaded         "")
set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDLL      "")
set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDebug    "")
set(CMAKE_ASM${ASM_DIALECT}_COMPILE_OPTIONS_MSVC_RUNTIME_LIBRARY_MultiThreadedDebugDLL "")

include(CMakeASMInformation)
set(ASM_DIALECT)
