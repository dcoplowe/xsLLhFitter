add_executable(CCQEFit ccqefit.cc)

set_target_properties(CCQEFit PROPERTIES LINK_FLAGS ${ROOT_LD_FLAGS})
foreach(mod ${MODULES})
 target_include_directories(CCQEFit PUBLIC ${CMAKE_SOURCE_DIR}/${mod}/src)
endforeach()

foreach(mod ${MODULETargets})
  cmessage(STATUS "Linking with: ${mod}")
 target_link_libraries(CCQEFit ${mod})
endforeach()


install(TARGETS CCQEFit DESTINATION bin)
