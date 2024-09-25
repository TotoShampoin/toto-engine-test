#include <format>
#include <iostream>

#include <toto-engine/window.hpp>

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

using namespace toto;

int main(int argc, const char* argv[]) {
    std::print(std::cout, "Helo\n");

    Window::initGLFW();
    {
        auto window = Window(800, 600, "Hello, World!");
        window.makeContextCurrent();

        Window::initGL();

        while (!window.shouldClose()) {
            glClear(GL_COLOR_BUFFER_BIT);
            window.swapBuffers();
            window.pollEvents();
        }
    }
    Window::terminateGLFW();
    return 0;
}
