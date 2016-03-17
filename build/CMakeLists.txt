cmake_minimum_required(VERSION 2.8.9)

# Use this plugin for precompiled headers.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

project (inline4)

include_directories(../src)
include_directories(../third_party/duktape/src-separate)
include_directories(../third_party/v8/)
include_directories(../third_party/v8/include)

# V8 requires C++11
if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
endif()

set ( I4_SOURCES
  ../src/inline4.cc
  ../src/inline4.h
)

set ( DUKTAPE_SOURCES 
../third_party/duktape/src-separate/duk_alloc_default.c   ../third_party/duktape/src-separate/duk_hbuffer_alloc.c
../third_party/duktape/src-separate/duk_api_buffer.c    ../third_party/duktape/src-separate/duk_hbuffer_ops.c
../third_party/duktape/src-separate/duk_api_bytecode.c    ../third_party/duktape/src-separate/duk_hbufferobject_misc.c
../third_party/duktape/src-separate/duk_api_call.c    ../third_party/duktape/src-separate/duk_heap_alloc.c
../third_party/duktape/src-separate/duk_api_codec.c   ../third_party/duktape/src-separate/duk_heap_hashstring.c
../third_party/duktape/src-separate/duk_api_compile.c   ../third_party/duktape/src-separate/duk_heap_markandsweep.c
../third_party/duktape/src-separate/duk_api_debug.c   ../third_party/duktape/src-separate/duk_heap_memory.c
../third_party/duktape/src-separate/duk_api_heap.c    ../third_party/duktape/src-separate/duk_heap_misc.c
../third_party/duktape/src-separate/duk_api_logging.c   ../third_party/duktape/src-separate/duk_heap_refcount.c
../third_party/duktape/src-separate/duk_api_memory.c    ../third_party/duktape/src-separate/duk_heap_stringcache.c
../third_party/duktape/src-separate/duk_api_object.c    ../third_party/duktape/src-separate/duk_heap_stringtable.c
../third_party/duktape/src-separate/duk_api_stack.c   ../third_party/duktape/src-separate/duk_hobject_alloc.c
../third_party/duktape/src-separate/duk_api_string.c    ../third_party/duktape/src-separate/duk_hobject_class.c
../third_party/duktape/src-separate/duk_api_var.c   ../third_party/duktape/src-separate/duk_hobject_enum.c
../third_party/duktape/src-separate/duk_bi_array.c    ../third_party/duktape/src-separate/duk_hobject_finalizer.c
../third_party/duktape/src-separate/duk_bi_boolean.c    ../third_party/duktape/src-separate/duk_hobject_misc.c
../third_party/duktape/src-separate/duk_bi_buffer.c   ../third_party/duktape/src-separate/duk_hobject_pc2line.c
../third_party/duktape/src-separate/duk_bi_date.c   ../third_party/duktape/src-separate/duk_hobject_props.c
../third_party/duktape/src-separate/duk_bi_date_unix.c    ../third_party/duktape/src-separate/duk_hstring_misc.c
../third_party/duktape/src-separate/duk_bi_date_windows.c ../third_party/duktape/src-separate/duk_hthread_alloc.c
../third_party/duktape/src-separate/duk_bi_duktape.c    ../third_party/duktape/src-separate/duk_hthread_builtins.c
../third_party/duktape/src-separate/duk_bi_error.c    ../third_party/duktape/src-separate/duk_hthread_misc.c
../third_party/duktape/src-separate/duk_bi_function.c   ../third_party/duktape/src-separate/duk_hthread_stacks.c
../third_party/duktape/src-separate/duk_bi_global.c   ../third_party/duktape/src-separate/duk_js_call.c
../third_party/duktape/src-separate/duk_bi_json.c   ../third_party/duktape/src-separate/duk_js_compiler.c
../third_party/duktape/src-separate/duk_bi_logger.c   ../third_party/duktape/src-separate/duk_js_executor.c
../third_party/duktape/src-separate/duk_bi_math.c   ../third_party/duktape/src-separate/duk_js_ops.c
../third_party/duktape/src-separate/duk_bi_number.c   ../third_party/duktape/src-separate/duk_js_var.c
../third_party/duktape/src-separate/duk_bi_object.c   ../third_party/duktape/src-separate/duk_lexer.c
../third_party/duktape/src-separate/duk_bi_pointer.c    ../third_party/duktape/src-separate/duk_numconv.c
../third_party/duktape/src-separate/duk_bi_proxy.c    ../third_party/duktape/src-separate/duk_regexp_compiler.c
../third_party/duktape/src-separate/duk_bi_regexp.c   ../third_party/duktape/src-separate/duk_regexp_executor.c
../third_party/duktape/src-separate/duk_bi_string.c   ../third_party/duktape/src-separate/duk_replacements.c
../third_party/duktape/src-separate/duk_bi_thread.c   ../third_party/duktape/src-separate/duk_selftest.c
../third_party/duktape/src-separate/duk_bi_thrower.c    ../third_party/duktape/src-separate/duk_strings.c
../third_party/duktape/src-separate/duk_builtins.c    ../third_party/duktape/src-separate/duk_tval.c
../third_party/duktape/src-separate/duk_debug_fixedbuffer.c ../third_party/duktape/src-separate/duk_unicode_support.c
../third_party/duktape/src-separate/duk_debug_heap.c    ../third_party/duktape/src-separate/duk_unicode_tables.c
../third_party/duktape/src-separate/duk_debug_macros.c    ../third_party/duktape/src-separate/duk_util_bitdecoder.c
../third_party/duktape/src-separate/duk_debug_vsnprintf.c ../third_party/duktape/src-separate/duk_util_bitencoder.c
../third_party/duktape/src-separate/duk_debugger.c    ../third_party/duktape/src-separate/duk_util_bufwriter.c
../third_party/duktape/src-separate/duk_error_augment.c   ../third_party/duktape/src-separate/duk_util_hashbytes.c
../third_party/duktape/src-separate/duk_error_longjmp.c   ../third_party/duktape/src-separate/duk_util_hashprime.c
../third_party/duktape/src-separate/duk_error_macros.c    ../third_party/duktape/src-separate/duk_util_misc.c
../third_party/duktape/src-separate/duk_error_misc.c    ../third_party/duktape/src-separate/duk_util_tinyrandom.c
../third_party/duktape/src-separate/duk_error_throw.c 
)


add_executable(shell ${I4_SOURCES} ${DUKTAPE_SOURCES} ../third_party/v8/samples/shell.cc)
add_executable(simple ${I4_SOURCES} ${DUKTAPE_SOURCES} ../src/test.cc)
add_executable(jsplayground ../jsplayground.cpp ${DUKTAPE_SOURCES})

set ( PROJECT_LINK_LIBS 
)

target_link_libraries(shell ${PROJECT_LINK_LIBS} )
target_link_libraries(simple ${PROJECT_LINK_LIBS} )
target_link_libraries(jsplayground ${PROJECT_LINK_LIBS} )

