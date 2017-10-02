project(ember_slim)
set(CMAKE_MACOSX_RPATH 1) # See CMake policy CMP0042

#include_directories(../Headers)
#set(${PROJECT_NAME}_INCLUDE_DIRS
#        ${PROJECT_SOURCE_DIR}/Headers)
set(HEADER_FILES
    Source/api.h
    Source/ber.h
    Source/berio.h
    Source/berreader.h
    Source/bertag.h
    Source/bertypes.h
    Source/bytebuffer.h
    Source/ember.h
    Source/emberasyncreader.h
    Source/emberframing.h
    Source/emberinternal.h
    Source/emberplus.h
    Source/glow.h
    Source/glowrx.h
    Source/glowtx.h
    )

set(SOURCE_FILES
    Source/ber.c
    Source/berio.c
    Source/berreader.c
    Source/bertag.c
    Source/bytebuffer.c
    Source/ember.c
    Source/emberasyncreader.c
    Source/emberframing.c
    Source/emberinternal.c
    Source/glow.c
    Source/glowrx.c
    Source/glowtx.c
    )

add_library(${PROJECT_NAME}_static STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_library(${PROJECT_NAME}_shared SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME}_shared PUBLIC LIBEMBER_DLL=1)
target_compile_definitions(${PROJECT_NAME}_shared PRIVATE LIBEMBER_DLL_EXPORTS=1)
