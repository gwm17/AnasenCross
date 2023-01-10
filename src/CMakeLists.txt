add_executable(AnasenCross)

target_include_directories(AnasenCross PRIVATE
    SYSTEM ${ROOT_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/catima/
)

target_sources(AnasenCross PRIVATE
    Application.h
    Application.cpp
    CSParams.h
    CrossSection.h
    CrossSection.cpp
    MassLookup.h
    MassLookup.cpp
    Target.h
    Target.cpp
    Precision.h
    Assert.h
    main.cpp
)

target_link_libraries(AnasenCross PRIVATE ${ROOT_LIBRARIES} catima fmt::fmt nlohmann_json::nlohmann_json)

set_target_properties(AnasenCross PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ACROSS_BINARY_DIR})