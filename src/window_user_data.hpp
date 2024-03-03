#pragma once

#include "scene.hpp"
#include "texture.hpp"

struct WindowUserData
{
	Scene* scene{};
	Texture* texture{};
};
