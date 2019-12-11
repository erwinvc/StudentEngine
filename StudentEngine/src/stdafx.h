#pragma once

#define _HAS_STD_BYTE 0
#pragma warning (disable: 4244) /*type_traits conversion warning*/

// Windows Header Files:
#include <windows.h>
#include <time.h>
#include <fileapi.h>

#include <limits>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <thread>
#include <math.h>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <iomanip>

// Datastructures
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <unordered_map>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS 
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_internal.h"
#include "imgui_impl_opengl3.h"

// Assimp
#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/material.h"
#include <assimp/Importer.hpp>

#include "vendor/noise.h"
#include "vendor/json.h"
#include "vendor/stb_image.h"

using namespace std;

#include "util/types.h"
#include "util/memory.h"

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#ifdef _DEBUG
//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
//#endif


#include "util/math/vector2.h"
#include "util/math/vector2I.h"
#include "util/math/vector3.h"
#include "util/math/vector4.h"
#include "util/math/matrix4.h"
#include "util/math/Rectangle.h"
#include "util/math/math.h"
#include "util/math/transform.h"

#include "util/ref.h"
#include "util/singleton.h"
#include "util/fileSystem.h"
#include "util/utils.h"
#include "util/datastructures/asyncQueue.h"
#include "util/datastructures/doublyLinkedList.h"
#include "util/thread/threadManager.h"
#include "util/thread/threadPool.h"
#include "util/logger.h"
#include "util/path.h"
#include "util/thread/glFiberManager.h"
#include "util/color.h"
#include "util/timestep.h"
#include "util/timer.h"
#include "util/glError.h"
#include "util/fonts/IconsFontAwesome.h"

#include "util/tween/tween.h"

#include "editor/inspector/inspectorDrawable.h"
#include "editor/inspector/inspector.h"

#include "assets/assetBase.h"
#include "assets/icon.h"

#include "graphics/glCallbackManager.h"
#include "graphics/window.h"

#include "assets/texture/textureParameters.h"

#include "graphics/buffers/bufferLayout.h"
#include "graphics/buffers/frameBuffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexBuffer.h"
#include "graphics/buffers/vertexArray.h"

#include "assets/assetWatcher.h"

#include "graphics/shaders/shaderUniformBuffer.h"
#include "graphics/shaders/shader.h"
#include "graphics/shaders/shaderManager.h"

#include "assets/texture/texture.h"
#include "assets/texture/streamedTexture.h"
#include "assets/sprite.h"
#include "assets/audio/audio.h"

#include "input/keyboard.h"
#include "input/mouse.h"

#include "graphics/mesh.h"
#include "graphics/instancedRenderer.h"
#include "graphics/spriteRenderer.h"
#include "graphics/camera/camera.h"
#include "graphics/camera/orthographicCamera.h"
#include "graphics/renderingPipeline.h"

#include "imgui/imGuiManager.h"

#include "util/math/groundRaycast.h"

#include "assets/loadjobs/assetLoadJob.h"
#include "assets/loadjobs/shaderLoadJob.h"
#include "assets/loadjobs/stateLoadJob.h"
#include "assets/loadjobs/textureLoadJob.h"
#include "assets/loadjobs/customLoadJob.h"
#include "assets/loadjobs/audioLoadJob.h"

#include "assets/assetManager.h"

#include "application.h"
#include "gameObject/gameObject.h"
#include "gameObject/editorGameObject.h"
#include "gameObject/playerObject.h"
#include "editor/hierarchyObject.h"
#include "editor/hierarchy.h"
#include "editor/editorGrid.h"
#include "editor/editorWindow.h"
#include "editor/editorAssetManager.h"
#include "util/undo.h"
#include "util/fonts/IconsFontAwesome.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "audio/audioManager.h"

#include "states/baseState.h"
#include "states/loadingState.h"
#include "states/editState.h"
#include "states/playState.h"
#include "states/stateManager.h"
