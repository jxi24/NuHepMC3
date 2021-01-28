add_executable(main
    #    NuHepMCWriter.cc
    main.cc
)
target_include_directories(main PUBLIC SYSTEM ${HEPMC3_INCLUDE_DIR})
target_link_libraries(main PRIVATE project_options
                           PUBLIC ${HEPMC3_LIB})
