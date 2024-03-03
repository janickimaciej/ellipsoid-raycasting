#include "gui.hpp"
#include "scene.hpp"
#include "shader_program.hpp"
#include "texture.hpp"
#include "window.hpp"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <chrono>
#include <string>
#include <thread>

int main()
{
	using namespace std::chrono_literals;

	int initialWindowWidth = 1200;
	int initialWindowHeight = 700;
	Scene scene{initialWindowWidth, initialWindowHeight};
	Texture texture{initialWindowWidth, initialWindowHeight};
	Window window{initialWindowWidth, initialWindowHeight, scene, texture};
	GUI gui{window.getPtr(), &scene};
	texture.initialize();
	ShaderProgram shaderProgram{"src/vertex_shader.glsl", "src/fragment_shader.glsl"};
	Canvas canvas{};

	while (!window.shouldClose())
	{
		//std::this_thread::sleep_for(500ms);
		gui.update();
		scene.renderToTexture(texture);
		shaderProgram.use();
		texture.use();
		canvas.render();
		gui.render();
		window.swapBuffers();
		window.pollEvents();
	}

	return 0;
}
