#include "Core/Application.h"
#include "Core/EntryPoint.h"

class ImGuiTools : public taskhub::Layer {
public:
	void OnUIRender() override {
		ImGui::ShowDemoWindow();
		ImGui::ShowStyleEditor();
	}
};

class TestApp : public taskhub::Layer {
public:
	void OnUIRender() override {
        ImGui::Begin("todo");
        ShowTodoListApp();
        ImGui::End();

	}

    //struct TodoItem {
    //    int id;
    //    std::string text;
    //    bool completed;
    //    std::vector<TodoItem> children;
    //};

    //std::vector<TodoItem> todoItems;

    
    struct TodoItem {
        std::string text;
        bool completed;
    };

    void ShowTodoListApp() {
        static std::vector<TodoItem> todo_items;
        static char new_todo_text[256] = "";

        if (ImGui::TreeNode("Todo List")) {
            ImGui::Text("Manage your tasks:");

            // Input for new todo item
            ImGui::InputText("New Task", new_todo_text, IM_ARRAYSIZE(new_todo_text));
            ImGui::SameLine();
            if (ImGui::Button("Add Task")) {
                if (strlen(new_todo_text) > 0) {
                    todo_items.push_back({ std::string(new_todo_text), false });
                    new_todo_text[0] = '\0'; // Clear input field
                }
            }

            // Display todo items
            for (size_t i = 0; i < todo_items.size(); ++i) {
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                if (todo_items[i].completed) {
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                }

                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, todo_items[i].text.c_str());
                //if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                //    todo_items[i].completed = !todo_items[i].completed; // Toggle completed state
                //}

                // Drag and drop functionality (optional)
                if (ImGui::BeginDragDropSource()) {
                    ImGui::SetDragDropPayload("_TODOITEM", &i, sizeof(size_t));
                    ImGui::Text("Move %s", todo_items[i].text.c_str());
                    ImGui::EndDragDropSource();
                }
                if (ImGui::BeginDragDropTarget()) {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TODOITEM")) {
                        size_t src_idx = *(const size_t*)payload->Data;
                        if (src_idx != i) {
                            std::swap(todo_items[src_idx], todo_items[i]);
                        }
                    }
                    ImGui::EndDragDropTarget();
                }

                // Button to remove todo item
                ImGui::SameLine();
                if (ImGui::SmallButton("X")) {
                    todo_items.erase(todo_items.begin() + i);
                    --i; // Adjust index after deletion
                }
            }

            ImGui::TreePop();
        }
    }


private:

};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;
	provision.Name = "Test";

	taskhub::Application* app = new taskhub::Application(provision);
	app->PushLayer<ImGuiTools>();
    app->PushLayer<TestApp>();

	return app;
}