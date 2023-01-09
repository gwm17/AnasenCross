add_executable(AnasenCross)

target_include_directories(AnasenCross PRIVATE
    SYSTEM ${ROOT_INCLUDE_DIRS}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(AnasenCross PRIVATE
    CSParams.h
    CrossSection.h
    CrossSection.cpp
    MassLookup.h
    MassLookup.cpp
    Target.h
    Target.cpp
    Precision.h
    main.cpp
)

target_link_libraries(AnasenCross PRIVATE ${ROOT_LIBS})