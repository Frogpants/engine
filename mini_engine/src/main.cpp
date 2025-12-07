#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "editor/EditorState.hpp"
#include "editor/Block.hpp"
#include "editor/BlockRenderer.hpp"
#include "editor/BlockConnections.hpp"
#include "editor/BlockArgumentSlots.hpp"
#include "editor/EditorUI.hpp"

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Atom Engine", NULL, NULL);
    glfwMakeContextCurrent(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // --- Sample Blocks ---
    {
        Block* b = new Block();
        b->id = 1;
        b->shape = BlockShape::Stack;
        b->position = ImVec2(400,100);
        b->text = "move ( 10 ) steps";
        b->args.push_back({ArgType::Number, "10"});
        g_editor.blocks.push_back(b);
    }

    {
        Block* b = new Block();
        b->id = 2;
        b->shape = BlockShape::Boolean;
        b->position = ImVec2(600,200);
        b->text = "touching edge?";
        g_editor.blocks.push_back(b);
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // --- Draw full editor UI ---
        DrawEditorUI();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
