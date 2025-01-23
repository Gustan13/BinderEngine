#pragma once

#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

class Texture {
private:
	unsigned int ID;
	unsigned char* data;
	int height, width, nrChannels;

public:
	Texture(const char* filename);
	~Texture();

	void bind();
	void activate(unsigned int type);
};