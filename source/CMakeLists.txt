# Include OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBS_DIR})

# Specify source files
set(source_files
	command.cpp)

# Create exetutable
add_executable(imageprocessor main.cpp ${source_files})
target_link_libraries(imageprocessor ${OpenCV_LIBS})
