set(PROJECT_NAME Case-Engine)

# Source groups
set(no_group_source_files
    "../README.md"
    "main.cpp"
    "pch.cpp"
    "pch.h"
    "resource.h"
    "Saved/Ini/scene.json"
)
source_group("" FILES ${no_group_source_files})

set(Core
    "Core/Defines.h"
    "Core/Engine.cpp"
    "Core/Engine.h"
    "Core/Input.cpp"
    "Core/Input.h"
    "Core/Logger.cpp"
    "Core/Logger.h"
    "Core/Paths.cpp"
    "Core/Paths.h"
    "Core/Window.cpp"
    "Core/Window.h"
)
source_group("Core" FILES ${Core})

set(Editor
    "Editors/Editor.cpp"
    "Editors/Editor.h"
    "Editors/EditorLogger.cpp"
    "Editors/EditorLogger.h"
    "Editors/GUI.cpp"
    "Editors/GUI.h"
)
source_group("Editors" FILES ${Editor})

set(TextEditor
    "Editors/TextEditor/TextEditor.cpp"
    "Editors/TextEditor/TextEditor.h"
)
source_group("TextEditor" FILES ${TextEditor})

set(Graphics
    "Graphics/GfxBuffer.h"
    "Graphics/GfxCommandContext.cpp"
    "Graphics/GfxCommandContext.h"
    "Graphics/GfxConstantBuffer.h"
    "Graphics/GfxDefines.h"
    "Graphics/GfxDevice.cpp"
    "Graphics/GfxDevice.h"
    "Graphics/GfxFormat.h"
    "Graphics/GfxInputLayout.cpp"
    "Graphics/GfxInputLayout.h"
    "Graphics/GfxProfiler.cpp"
    "Graphics/GfxProfiler.h"
    "Graphics/GfxQuery.cpp"
    "Graphics/GfxQuery.h"
    "Graphics/GfxRenderPass.h"
    "Graphics/GfxResourceCommon.h"
    "Graphics/GfxScopedAnnotation.cpp"
    "Graphics/GfxScopedAnnotation.h"
    "Graphics/GfxShader.h"
    "Graphics/GfxShaderCompiler.cpp"
    "Graphics/GfxShaderCompiler.h"
    "Graphics/GfxShaderProgram.cpp"
    "Graphics/GfxShaderProgram.h"
    "Graphics/GfxStates.cpp"
    "Graphics/GfxStates.h"
    "Graphics/GfxTexture.h"
    "Graphics/GfxVertexFormat.h"
    "Graphics/GfxView.h"
)
source_group("Graphics" FILES ${Graphics})

set(Graphics__ImGui
    "../Includes/FontAwesome/IconsFontAwesome4.h"
    "../Includes/ImGui/ImGui/imconfig.h"
    "../Includes/ImGui/ImGui/imgui.cpp"
    "../Includes/ImGui/ImGui/imgui.h"
    "../Includes/ImGui/ImGui/imgui_demo.cpp"
    "../Includes/ImGui/ImGui/imgui_draw.cpp"
    "../Includes/ImGui/ImGui/imgui_impl_dx11.cpp"
    "../Includes/ImGui/ImGui/imgui_impl_dx11.h"
    "../Includes/ImGui/ImGui/imgui_impl_sdl.cpp"
    "../Includes/ImGui/ImGui/imgui_impl_sdl.h"
    "../Includes/ImGui/ImGui/imgui_internal.h"
    "../Includes/ImGui/ImGui/imgui_tables.cpp"
    "../Includes/ImGui/ImGui/imgui_widgets.cpp"
    "../Includes/ImGui/ImGui/ImGuizmo.cpp"
    "../Includes/ImGui/ImGui/ImGuizmo.h"
    "../Includes/ImGui/ImGui/imstb_rectpack.h"
    "../Includes/ImGui/ImGui/imstb_textedit.h"
    "../Includes/ImGui/ImGui/imstb_truetype.h"
)
source_group("Graphics\\ImGui" FILES ${Graphics__ImGui})

set(Math
    "Math/BoundingVolumeHelpers.h"
    "Math/ComputeNormals.h"
    "Math/ComputeTangentFrame.h"
    "Math/Constants.h"
    "Math/Halton.h"
    "Math/MathTypes.h"
)
source_group("Math" FILES ${Math})

set(Rendering
    "Rendering/Camera.cpp"
    "Rendering/Camera.h"
    "Rendering/Components.cpp"
    "Rendering/Components.h"
    "Rendering/ConstantBuffers.h"
    "Rendering/Enums.h"
    "Rendering/ModelImporter.cpp"
    "Rendering/ModelImporter.h"
    "Rendering/ParticleRenderer.cpp"
    "Rendering/ParticleRenderer.h"
    "Rendering/Picker.h"
    "Rendering/Renderer.cpp"
    "Rendering/Renderer.h"
    "Rendering/RendererSettings.h"
    "Rendering/SceneViewport.h"
    "Rendering/ShaderManager.cpp"
    "Rendering/ShaderManager.h"
    "Rendering/SkyModel.cpp"
    "Rendering/SkyModel.h"
    "Rendering/Terrain.cpp"
    "Rendering/Terrain.h"
    "Rendering/TextureManager.cpp"
    "Rendering/TextureManager.h"
)
source_group("Rendering" FILES ${Rendering})

set(Shaders
    "Shaders/Common.hlsli"
    "Shaders/CommonData.hlsli"
    "Shaders/Constants.hlsli"
#    "Resources/Shaders/Particles/ParticleUtil.hlsli"
    "Shaders/Util/DitherUtil.hlsli"
    "Shaders/Util/LightUtil.hlsli"
    "Shaders/Util/ShadowUtil.hlsli"
    "Shaders/Util/ToneMapUtil.hlsli"
    "Shaders/Util/VoxelUtil.hlsli"
)
source_group("Shaders" FILES ${Shaders})

set(Utillites
    "../Includes/gfx-external/DDSTextureLoader.cpp"
    "../Includes/gfx-external/DDSTextureLoader.h"
    "../Includes/gfx-external/WICTextureLoader.cpp"
    "../Includes/gfx-external/WICTextureLoader.h"
    "../Includes/ImGui/ImGui/ImGuiUtil.h"
    "../Includes/SimpleMath/SimpleMath.cpp"
    "../Includes/SimpleMath/SimpleMath.h"
    "../Includes/SimpleMath/SimpleMath.inl"
    "tecs/registry.h"
    "Utilities/AllocatorUtil.h"
    "Utilities/AutoRefCountPtr.h"
    "Utilities/CLIParser.h"
    "Utilities/ConcurrentQueue.h"
    "Utilities/Delegate.h"
    "Utilities/EnumUtil.h"
    "Utilities/FilesUtil.h"
    "Utilities/FileWatcher.h"
    "Utilities/HashMap.h"
    "Utilities/HashSet.h"
    "Utilities/HashUtil.h"
    "Utilities/Heightmap.cpp"
    "Utilities/Heightmap.h"
    "Utilities/HosekDataRGB.h"
    "Utilities/Image.cpp"
    "Utilities/Image.h"
    "Utilities/JsonUtil.h"
    "Utilities/LinearAllocator.h"
    "Utilities/MemoryDebugger.h"
    "Utilities/Random.h"
    "Utilities/RingAllocator.h"
    "Utilities/RingBuffer.h"
    "Utilities/Singleton.h"
    "Utilities/StringUtil.cpp"
    "Utilities/StringUtil.h"
    "Utilities/TemplatesUtil.h"
    "Utilities/ThreadPool.h"
    "Utilities/Timer.h"
)
source_group("Utillites" FILES ${Utillites})

set(tecs
    "tecs/component_pool.h"
    "tecs/entity.h"
    "tecs/entity_view.h"
    "tecs/sparse_set.h"
)
source_group("tecs" FILES ${tecs})

set(ALL_FILES
    ${no_group_source_files}
    ${Core}
    ${Editor}
    ${TextEditor}
    ${Graphics}
    ${Graphics__ImGui}
    ${Math}
    ${Rendering}
    ${Shaders}
    ${Utillites}
    ${tecs}
)

# Target
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Adria)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

# Include directories
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/SDL2/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/stb;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/tinygltf;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/tinyobjloader;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/FastNoiseLite;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/json;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/nfd/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/FontAwesome;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/cereal;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/ImGui;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/gfx-external;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/SimpleMath;"
			"${CMAKE_CURRENT_SOURCE_DIR}/../Includes/OpenGL;"
            "${CMAKE_CURRENT_SOURCE_DIR}/."
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/SDL2/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/stb;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/tinygltf;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/tinyobjloader;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/FastNoiseLite;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/json;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/nfd/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/FontAwesome;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/cereal;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/ImGui;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/gfx-external;"
            "${CMAKE_CURRENT_SOURCE_DIR}/../Includes/SimpleMath;"
			"${CMAKE_CURRENT_SOURCE_DIR}/../Includes/OpenGL;"
            "${CMAKE_CURRENT_SOURCE_DIR}/."
        ">"
    )
endif()

# Compile definitions
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "_CRT_SECURE_NO_WARNINGS;"
        "NOMINMAX;"
        "_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS;"
        "_SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

# Compile and link options
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /std:c++latest;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /Ot;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /SUBSYSTEM:CONSOLE;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

# Dependencies
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "nfd_d;"
            "SDL2;"
            "SDL2main;"
            "dxgi;"
            "dxguid;"
			"opengl32"
        ">"
        "$<$<CONFIG:Release>:"
            "nfd"
        ">"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_SOURCE_DIR}/Includes/SDL2/lib/x64;"
            "${CMAKE_SOURCE_DIR}/Includes/dxc/lib;"
            "${CMAKE_SOURCE_DIR}/Includes/FMOD/lib/x64;"
            "${CMAKE_SOURCE_DIR}/Includes/nfd/lib"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_SOURCE_DIR}/ThirdParty/dxc/lib;"
            "${CMAKE_SOURCE_DIR}/ThirdParty/FMOD/lib/x64;"
            "${CMAKE_SOURCE_DIR}/ThirdParty/nfd/lib"
        ">"
    )
endif()

