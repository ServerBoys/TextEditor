# CMakeList.txt : CMake project for TextEditor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TextEditor")
add_subdirectory(Editor)
add_executable (TextEditor "TextEditor.cpp" "TextEditor.h")

target_include_directories(TextEditor PUBLIC include)
target_link_libraries(TextEditor Editor)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TextEditor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
