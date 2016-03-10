set(BOOST_COMPONENTS
    system
)

find_package(Boost 1.58 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
find_package( OpenCV REQUIRED )

include_directories( ${Boost_INCLUDE_DIR} )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBRARIES OFF)

add_executable(vr_tracker server.cpp tracker.cpp monitor.cpp)
target_link_libraries(vr_tracker ${Boost_SYSTEM_LIBRARIES} ${OpenCV_LIBS} )


