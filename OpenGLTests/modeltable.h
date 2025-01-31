#pragma once

#include <stdio.h>
#include <map>

#include "model.h"

class ModelTable {
private:
	std::map<std::string, Model*> table;
public:
	Model* findModel(std::string name);
	bool loadModel(std::string path, std::string name);

	void destroyTable();
};