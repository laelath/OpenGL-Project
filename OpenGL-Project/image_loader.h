#pragma once

#include <string>
using namespace std;

#include <GL/glew.h>

bool loadDefaultTexture(string path);
GLuint loadTexture(string filename);