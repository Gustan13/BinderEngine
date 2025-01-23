#include "model.h"

Model::Model(const char* filepath) {
	//Assimp::Importer importer;

    scene = importer.ReadFile(filepath,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (scene == nullptr) {
        std::cout << "Error loading 3d model: " << filepath << "\n";
        exit(0);
    }

    animation_0 = scene->mAnimations[0];
    globalInverseTransformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation);
    globalInverseTransformation = glm::inverse(globalInverseTransformation);

    if (scene->HasMeshes()) {
        // transfering meshes
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            Mesh newMesh;

            // transfering mesh vertices, normals and texture coordinates
            for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; ++j) {
                Vertex newVertex;

                //SetVertexBoneDataToDefault(newVertex);
                newVertex.weights[0] = 0.0;
                newVertex.weights[1] = 0.0;
                newVertex.weights[2] = 0.0;
                newVertex.weights[3] = 0.0;

                aiVector3D currentVertex = scene->mMeshes[i]->mVertices[j];

                newVertex.position.x = currentVertex.x;
                newVertex.position.y = currentVertex.y;
                newVertex.position.z = currentVertex.z;

                aiVector3D currentNormal = scene->mMeshes[i]->mNormals[j];

                newVertex.normal.x = currentNormal.x;
                newVertex.normal.y = currentNormal.y;
                newVertex.normal.z = currentNormal.z;

                if (scene->mMeshes[i]->HasTextureCoords(0)) {
                    aiVector3D currentTexCoord = scene->mMeshes[i]->mTextureCoords[0][j];

                    newVertex.texCoord.x = currentTexCoord.x;
                    newVertex.texCoord.y = currentTexCoord.y;
                }
                else {
                    newVertex.texCoord.x = 0;
                    newVertex.texCoord.y = 0;
                }
                newMesh.data.push_back(newVertex);
            }

            aiMesh* mesh = scene->mMeshes[i];

            for (int i = 0; i < mesh->mNumBones; ++i) {
                unsigned int boneIndex = 0;
                std::string boneName(mesh->mBones[i]->mName.data);

                if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
                    boneIndex = boneCount;
                    ++boneCount;
                    BoneInfo bInfo;
                    boneInfoVector.push_back(bInfo);
                }
                else {
                    boneIndex = boneInfoMap[boneName];
                }

                boneInfoMap[boneName] = boneIndex;
                boneInfoVector[boneIndex].offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[i]->mOffsetMatrix);

                for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
                    unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
                    float weight = mesh->mBones[i]->mWeights[j].mWeight;

                    for (unsigned int k = 0; k < 4; ++k) {
                        if (newMesh.data[vertexID].weights[k] != 0.0)
                            continue;

                        newMesh.data[vertexID].boneIDs[k] = boneIndex;
                        newMesh.data[vertexID].weights[k] = weight;
                        break;
                    }
                }
            }

            // transfering mesh faces
            for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
                aiFace currentFace = scene->mMeshes[i]->mFaces[j];

                newMesh.indices.push_back(currentFace.mIndices[0]);
                newMesh.indices.push_back(currentFace.mIndices[1]);
                newMesh.indices.push_back(currentFace.mIndices[2]);
            }

            meshes.push_back(newMesh);
            meshes[i].setupMesh();
        }
    }
    
    std::cout << "bones transfered\n";

    Node* newBaseNode = new Node();
    baseNode = newBaseNode;
    transferNodes(scene->mRootNode, baseNode);
}

void Model::boneTransform(float time) {
    glm::mat4 identity = glm::mat4(1.f);

    float tps = (float)scene->mAnimations[0]->mTicksPerSecond;
    float timeInTicks = time * tps;
    float animTime = (float)fmod(timeInTicks, scene->mAnimations[0]->mDuration);

    readNodeHierarchy(animTime, scene->mRootNode, identity);

    transforms.resize(boneCount);

    for (unsigned int i = 0; i < boneCount; ++i)
        transforms[i] = boneInfoVector[i].finalTransform;
}

void Model::readNodeHierarchy(float animTime, const aiNode * node, glm::mat4 parentTransform) {

    std::string nodeName(node->mName.data);

    const aiAnimation* animation = scene->mAnimations[0];

    //assert(animation);

    glm::mat4 nodeTransformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation);

    const aiNodeAnim* nodeAnim = findNodeAnim(nodeName, animation);

    if (nodeAnim) {

        aiVectorKey Scaling;
        Scaling = nodeAnim->mScalingKeys[findScale(animTime, nodeAnim)];
        glm::mat4 ScalingM = glm::mat4(1.f);
        ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.mValue.x, Scaling.mValue.y, Scaling.mValue.z));

        aiVectorKey Position;
        Position = nodeAnim->mPositionKeys[findPosition(animTime, nodeAnim)];
        glm::mat4 PositionM = glm::mat4(1.f);
        PositionM = glm::translate(PositionM, glm::vec3(Position.mValue.x, Position.mValue.y, Position.mValue.z));

        aiQuatKey Rotation;
        Rotation = nodeAnim->mRotationKeys[findRotation(animTime, nodeAnim)];
        glm::mat4 RotationM = glm::mat4(1.f);
        RotationM = AssimpGLMHelpers::ConvertMatrix3ToGLMFormat4(Rotation.mValue.GetMatrix());

        nodeTransformation = PositionM * RotationM * ScalingM;
        //nodeTransformation = ScalingM * PositionM * RotationM;
        //nodeTransformation = glm::mat4(1.f);
    }

    if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
        unsigned int boneIndex = boneInfoMap[nodeName];
        boneInfoVector[boneIndex].finalTransform = globalInverseTransformation * parentTransform * nodeTransformation * boneInfoVector[boneIndex].offset;
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        readNodeHierarchy(animTime, node->mChildren[i], parentTransform * nodeTransformation);
    }
}

const aiNodeAnim* Model::findNodeAnim(std::string nodeName, const aiAnimation* animation) {

    for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
        std::string n(animation->mChannels[i]->mNodeName.data);
        if (std::strcmp(n.data(), nodeName.data()) == 0)
            return animation->mChannels[i];
    }

    return nullptr;
}

unsigned int Model::findPosition(float animTime, const aiNodeAnim* nodeAnim) {
    //assert(nodeAnim->mNumPositionKeys < 0);

    for (unsigned int i = 0; i < nodeAnim->mNumPositionKeys; ++i) {
        if (animTime < (float)nodeAnim->mPositionKeys[i].mTime) {
            return i;
        }
    }
    assert(0);
}

unsigned int Model::findRotation(float animTime, const aiNodeAnim* nodeAnim) {
    //assert(nodeAnim->mNumRotationKeys < 0);

    for (unsigned int i = 0; i < nodeAnim->mNumRotationKeys; ++i) {
        if (animTime < (float)nodeAnim->mRotationKeys[i].mTime) {
            return i;
        }
    }
    assert(0);
}

unsigned int Model::findScale(float animTime, const aiNodeAnim* nodeAnim) {
    //assert(nodeAnim->mNumScalingKeys < 0);

    for (unsigned int i = 0; i < nodeAnim->mNumScalingKeys; ++i) {
        if (animTime < (float)nodeAnim->mScalingKeys[i].mTime) {
            return i;
        }
    }
    assert(0);
}

void Model::transferNodes(aiNode* ainode, Node* node) {
    for (unsigned int i = 0; i < ainode->mNumMeshes; ++i)
        node->meshes.push_back(ainode->mMeshes[i]);

    aiMatrix4x4 t = ainode->mTransformation;

    float stuff[] = {
        t.a1, t.b1, t.c1, t.d1,
        t.a2, t.b2, t.c2, t.d2,
        t.a3, t.b3, t.c3, t.d3,
        t.a4, t.b4, t.c4, t.d4,
    };

    node->transformation = glm::make_mat4x4(stuff);

    for (unsigned int i = 0; i < ainode->mNumChildren; ++i) {
        node->subnodes.push_back(new Node());
        transferNodes(ainode->mChildren[i], node->subnodes[i]);
    }
}

void Model::draw(Shader* shader_program) {
    glm::mat4 model = glm::mat4(1.f);
    //model = glm::translate(model, glm::vec3(0.f,-1.f,0.f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    //model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));

    for (Mesh mesh : meshes)
        mesh.drawMesh(shader_program);

    //drawNodes(shader_program, baseNode, model);
}

void Model::drawNodes(Shader* shader_program, Node* node, glm::mat4 transformation) {
    glm::mat4 currentTransformation = transformation * node->transformation;

    shader_program->setMatrix("model", glm::value_ptr(currentTransformation));

    for (unsigned int i = 0; i < node->meshes.size(); ++i)
        meshes[node->meshes[i]].drawMesh(shader_program);

    for (int i = 0; i < node->subnodes.size(); ++i) {
        drawNodes(shader_program, node->subnodes[i], currentTransformation);
    }
}