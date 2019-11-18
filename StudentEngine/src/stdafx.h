#pragma once

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

#include "util/types.h"