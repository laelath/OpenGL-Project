#pragma once

#include "console.h"
#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "window.h"
#include "image_loader.h"
#include "model_loader.h"

void initGL();
void initEngine(int width, int height, string name, bool fullscreen);

void addShader(Shader* shader);