
# Add source to this project's executable.
add_executable(VKGuide
    main.cpp
    vk_engine.cpp
    vk_engine.h
    vk_types.h
    vk_initializers.cpp
    vk_initializers.h
    vk_mesh.h
    vk_mesh.cpp)


set_property(TARGET VKGuide PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:VKGuide>")

target_include_directories(VKGuide PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(VKGuide vkbootstrap vma glm tinyobjloader imgui stb_image)

target_link_libraries(VKGuide Vulkan::Vulkan sdl2)

add_dependencies(VKGuide Shaders)
