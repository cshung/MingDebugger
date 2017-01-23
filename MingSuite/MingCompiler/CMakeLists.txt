project(MingDebugger CXX)
add_definitions(-std=c++11)
set (SRCS argument_parser.cpp code_generator.cpp compiler.cpp debugger.cpp instructions.cpp nodes.cpp parser.cpp program.cpp scanner.cpp source_location.cpp utilities.cpp virtual_machine.cpp)
add_executable (MingDebugger ${SRCS})
