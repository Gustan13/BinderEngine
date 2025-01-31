#include "modeltable.h"

Model* ModelTable::findModel(std::string name) {
	if (table.find(name) != table.end())
		return table[name];
	return nullptr;
}

bool ModelTable::loadModel(std::string path, std::string name) {
	Model* newModel;

	newModel = new Model(path.c_str());

	table[name.c_str()] = newModel;

	return true;
}

void ModelTable::destroyTable() {
	//for (std::string model = table.begin(); ) {

	//}
}