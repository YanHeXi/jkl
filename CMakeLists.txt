cmake_minimum_required(VERSION 3.25)
project(yhx)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

include (cmake/utils.cmake)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations -lssl -lcrypto")


include_directories(.)
include_directories(/home/yhx/project/jkl/include)

link_directories(/home/yhx/vscode_project/jkl/lib)


# message("***",${YAMLCPP})

set(LIB_SRC 
    yhx/log.cc 
	#yhx/util.cc
    #yhx/config.cc
    #yhx/thread.cc
    #yhx/mutex.cc
    #yhx/util.cc
    #yhx/fiber.cc
    #yhx/scheduler.cc
    #yhx/iomanager.cc
    #yhx/timer.cc
    #yhx/hook.cc
    #yhx/fd_manager.cc
    #yhx/address.cc
    #yhx/socket.cc
    #yhx/bytearray.cc
    #yhx/http/http.cc
    #yhx/http/httpclient_parser.rl.cc
    #yhx/http/http11_parser.rl.cc
    #yhx/http/http_parser.cc
    #yhx/tcp_server.cc
    #yhx/stream.cc
    #yhx/streams/socket_stream.cc
    #yhx/http/http_session.cc
    #yhx/http/http_server.cc
    #yhx/http/servlet.cc
    #yhx/http/http_connection.cc
    #yhx/uri.rl.cc
    )

add_library(yhx SHARED ${LIB_SRC} )

force_redefine_file_macro_for_sources(yhx)
find_library(YAMLCPP yaml-cpp)
find_library(PTHREAD pthread)

#ragelmaker(yhx/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/yhx/http)
#ragelmaker(yhx/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/yhx/http)
#ragelmaker(yhx/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/yhx)

set(LIBS
    yhx
	# dl
   	pthread
   	yaml-cpp
   	jsoncpp
    ${OPENSSL_LIBRARIES}
    )


	#    yhx_add_executable(test_util "tests/test_util.cc" yhx "${LIBS}")
	#    yhx_add_executable(test "tests/test.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_config "tests/test_config.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_thread "tests/test_thread.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_fiber "tests/test_fiber.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_scheduler "tests/test_scheduler.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_address "tests/test_address.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_socket "tests/test_socket.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_bytearray "tests/test_bytearray.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_http "tests/test_http.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_http_parser "tests/test_http_parser.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_tcp_server "tests/test_tcp_server.cc" yhx "${LIBS}")
	#    yhx_add_executable(echo_server "examples/echo_server.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_http_server "tests/test_http_server.cc" yhx "${LIBS}")
	#    yhx_add_executable(test_http_connection "tests/test_http_connection.cc" yhx "${LIBS}")
	#
	#    yhx_add_executable(test_uri "tests/test_uri.cc" yhx "${LIBS}")
	#    yhx_add_executable(my_http_server "samples/my_http_server.cc" yhx "${LIBS}")
    


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
