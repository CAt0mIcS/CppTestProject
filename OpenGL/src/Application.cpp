#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Texture.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Timer.h"

#include <GLFW/glfw3.h>
   

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    //window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Activates VSync
    glfwSwapInterval(1);

    if (GLEW_OK != glewInit()) {
        return -1;

    }

    std::cout << glGetString(GL_VERSION) << '\n';

    {

        //For texture rendering
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        //IMGUI
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();


        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

        while (!glfwWindowShouldClose(window))
        {

            renderer.SetClearColor();
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");

                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();

                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        delete currentTest;
        if(currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();


    glfwTerminate();
    return 0;
}
