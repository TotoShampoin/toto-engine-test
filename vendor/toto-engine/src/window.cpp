#include <expected>
#include <format>
#include <stdexcept>

#include "toto-engine/window.hpp"

namespace toto {

Window::Window(int width, int height, const char* title) {
    _handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!_handle)
        throw std::runtime_error("Failed to create GLFW window\n");
}
Window::~Window() {
    glfwDestroyWindow(_handle);
}

void Window::setHint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void Window::initGLFW() {
    if (glfwInit())
        return;
    char error_cstr[1024];
    glfwGetError(reinterpret_cast<const char**>(&error_cstr));
    throw std::runtime_error(std::format("Failed to initialize GLFW: {}\n", error_cstr));
}
void Window::initGL() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;
    throw std::runtime_error("Failed to initialize GLAD\n");
}
void Window::terminateGLFW() {
    glfwTerminate();
}

void Window::makeContextCurrent() {
    glfwMakeContextCurrent(_handle);
}

void Window::pollEvents() {
    glfwPollEvents();
}
void Window::swapBuffers() {
    glfwSwapBuffers(_handle);
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(_handle);
}

GLFWwindow* Window::handle() {
    return _handle;
}

} // namespace toto