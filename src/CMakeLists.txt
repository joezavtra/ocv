set(BOOST_COMPONENTS
    system
)

find_package(Boost 1.58 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)
find_package( OpenCV REQUIRED )

include_directories(${Boost_INCLUDE_DIR})

add_executable(vr_tracker server.cpp tracker.cpp sender.cpp monitor.cpp)
target_link_libraries(vr_tracker ${Boost_SYSTEM_LIBRARIES})
target_link_libraries( vr_tracker ${OpenCV_LIBS} )
