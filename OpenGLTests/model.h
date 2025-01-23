#pragma once

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>   
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <map>

#include "mesh.h"
#include "node.h"
#include "boneinfo.h"
#include "assimpglm.h"

class Model {
private:
	std::map<std::string, unsigned int> boneInfoMap;
	std::vector<BoneInfo> boneInfoVector;
	int boneCount = 0;

	std::vector<Mesh> meshes;
	Node* baseNode;

	Assimp::Importer importer;
	glm::mat4 globalInverseTransformation;
	const aiScene* scene;
	const aiAnimation* animation_0;

	void drawNodes(Shader* shader_program, Node* node, glm::mat4 transformation);

	unsigned int findPosition(float animTime, const aiNodeAnim* nodeAnim);
	unsigned int findRotation(float animTime, const aiNodeAnim* nodeAnim);
	unsigned int findScale(float animTime, const aiNodeAnim* nodeAnim);

	const aiNodeAnim* findNodeAnim(std::string nodeName, const aiAnimation* animation);

public:
	std::vector<glm::mat4> transforms;

	Model(const char* filepath);
	void transferNodes(aiNode* ainode, Node* node);

	auto& GetBoneInfoMap() { return boneInfoMap; }
	int& GetBoneCount() { return boneCount; }

	void boneTransform(float time);
	void readNodeHierarchy(float animTime, const aiNode* node, glm::mat4 parentTransform);

	void draw(Shader* shader_program);
};