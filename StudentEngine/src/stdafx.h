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

#include "util/math/vector2.h"
#include "util/math/vector2I.h"
#include "util/math/vector3.h"
#include "util/math/vector4.h"
#include "util/math/matrix4.h"
#include "util/math/rect.h"
#include "util/math/math.h"
#include "util/math/transform.h"

#include "util/types.h"
#include "util/ref.h"
#include "util/singleton.h"
#include "util/fileSystem.h"
#include "util/utils.h"
#include "util/asyncQueue.h"
#include "util/thread/threadManager.h"
#include "util/thread/threadPool.h"
#include "util/logger.h"
#include "util/thread/glFiberManager.h"
#include "util/color.h"
#include "util/timestep.h"
#include "util/timer.h"
#include "util/glError.h"

#include "imgui/imGuiManager.h"

#include "util/math/groundRaycast.h"

#include "graphics/glCallbackManager.h"
#include "graphics/window.h"
#include "application.h"