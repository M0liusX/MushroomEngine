if not exist "bin" mkdir bin

%VULKAN_SDK%/bin/glslc.exe simple_shader.vert -o bin/simple_shader.vert.spv
%VULKAN_SDK%/bin/glslc.exe simple_shader.frag -o bin/simple_shader.frag.spv

%VULKAN_SDK%/bin/glslc.exe billboard_shader.vert -o bin/billboard_shader.vert.spv
%VULKAN_SDK%/bin/glslc.exe billboard_shader.frag -o bin/billboard_shader.frag.spv

%VULKAN_SDK%/bin/glslc.exe physics.vert -o bin/physics.vert.spv
%VULKAN_SDK%/bin/glslc.exe physics.frag -o bin/physics.frag.spv

if not exist "../Export/shaders/bin/" mkdir "../Export/shaders/bin/"

xcopy bin "../Export/shaders/bin/*" /s /e /h /y