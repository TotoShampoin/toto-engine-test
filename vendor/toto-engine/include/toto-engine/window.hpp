#include "import-gl.hpp"

namespace toto {

class Window {
public:
    Window(int width, int height, const char* title);
    ~Window();

    static void setHint(int hint, int value);

    static void initGLFW();
    static void initGL();
    static void terminateGLFW();

    void makeContextCurrent();

    static void pollEvents();
    void swapBuffers();
    bool shouldClose();

    GLFWwindow* handle();

private:
    GLFWwindow* _handle;
};

} // namespace toto
