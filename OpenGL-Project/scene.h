#pragma once

#include <string>
#include <vector>
using namespace std;

#include "model_loader.h"

class Scene
{
public:
	Scene();
	~Scene();

	void addModel(string path);
	void addModel(model* model);

	void addLight(Shadow_Light* light);

private:
	vector<model*> models;
	vector<Shadow_Light*> lights;
};

