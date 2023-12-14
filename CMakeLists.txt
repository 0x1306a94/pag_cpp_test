cmake_minimum_required(VERSION 3.20)
project(pag_cpp_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

message(STATUS "CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Installation directory" FORCE)
message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

add_library(libpag SHARED IMPORTED)
add_library(libEGL SHARED IMPORTED)
add_library(libGLESv2 SHARED IMPORTED)

if(APPLE)
  if(NOT DEFINED CMAKE_OSX_ARCHITECTURES OR "${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
    set(CMAKE_OSX_ARCHITECTURES ${CMAKE_HOST_SYSTEM_PROCESSOR})
  endif()

  message(STATUS "CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")

  if(${CMAKE_OSX_ARCHITECTURES} MATCHES "x86_64")
    set_target_properties(libpag PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/macos/x86_64/libpag.dylib"
    )
  else()
    set_target_properties(libpag PROPERTIES
        IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/macos/arm64/libpag.dylib"
    )
  endif()

elseif(WIN32)
  set_target_properties(libpag PROPERTIES
      IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/pag.lib"
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/pag.dll"
  )
  set_target_properties(libEGL PROPERTIES
      IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/libEGL.lib"
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/libEGL.dll"
  )
  set_target_properties(libGLESv2 PROPERTIES
      IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/libGLESv2.lib"
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/win/libGLESv2.dll"
  )
else()

  set_target_properties(libpag PROPERTIES
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/linux/libpag.so"
  )

  set_target_properties(libEGL PROPERTIES
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/linux/libEGL.so"
  )

  set_target_properties(libGLESv2 PROPERTIES
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/lib/linux/libGLESv2.so"
  )
  
endif()


file(GLOB ALL_SRC_FILES src/*.h src/*.hpp src/*.cc src/*.cpp)

add_executable(${PROJECT_NAME} ${ALL_SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/third_party/libpag/include)

if(APPLE)
else()
target_link_libraries(${PROJECT_NAME} PUBLIC libEGL libGLESv2)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC libpag)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/pag_samples
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/pag_samples
)

install(TARGETS ${PROJECT_NAME} 
    RUNTIME DESTINATION bin
)


if(APPLE)

  if(${CMAKE_OSX_ARCHITECTURES} MATCHES "x86_64")
    install(
      FILES ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/macos/x86_64/libpag.dylib
      DESTINATION lib
    )
  else()
    install(
      FILES ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/macos/arm64/libpag.dylib
      DESTINATION lib
    )
  endif()
elseif(WIN32)
  install(
    FILES ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/win/pag.lib
          ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/win/pag.dll
    DESTINATION lib
  )
else()
  install(
    FILES ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/linux/libpag.so
          ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/linux/lib/libEGL.so
          ${CMAKE_CURRENT_LIST_DIR}/../third_party/libpag/lib/linux/libGLESv2.so
    DESTINATION lib
  )
endif()
