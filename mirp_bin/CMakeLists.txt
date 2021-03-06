add_library(test_common OBJECT cmdline.cpp
                               testfile_io.cpp
                               reffile_io.cpp
                               read_construct_basis.cpp
                               test_boys.cpp
                               test_common.cpp
                               test_integral.cpp
                               test_integral_single.cpp
                               ref_integral.cpp
)

# Add the include directories to the object library
target_include_directories(test_common PRIVATE
                           $<TARGET_PROPERTY:mirp,INTERFACE_INCLUDE_DIRECTORIES>)
target_include_directories(test_common SYSTEM PRIVATE
                           $<TARGET_PROPERTY:mirp,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)

# The executables themselves
add_executable(mirp_verify_test         mirp_verify_test.cpp         $<TARGET_OBJECTS:test_common>) 
add_executable(mirp_create_test      mirp_create_test.cpp      $<TARGET_OBJECTS:test_common>)
add_executable(mirp_create_reference mirp_create_reference.cpp $<TARGET_OBJECTS:test_common>)
add_executable(mirp_verify_reference   mirp_verify_reference.cpp   $<TARGET_OBJECTS:test_common>)

# Link these to mirp. The dependency and include directories
# will be included through here as well (they were added as PUBLIC)
target_link_libraries(mirp_verify_test         PRIVATE mirp)
target_link_libraries(mirp_create_test      PRIVATE mirp)
target_link_libraries(mirp_create_reference PRIVATE mirp)
target_link_libraries(mirp_verify_reference   PRIVATE mirp)

# Occasionally used to play with arb features or something
#add_executable(mirp_play mirp_play.cpp $<TARGET_OBJECTS:test_common>)
#target_link_libraries(mirp_play PRIVATE mirp)

# Installing the executables
install(TARGETS mirp_verify_test
                mirp_create_test
                mirp_create_reference
                mirp_verify_reference
        EXPORT mirpTargets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

