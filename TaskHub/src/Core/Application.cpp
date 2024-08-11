//#include "glad/glad.h" TODO: Fix
#include "Application.h"
#include "Assert.h"
#include "GraphicsContext.h"
#include "GUI/ImGuiStyle.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "GUI/Embeds/Fonts/Roboto-Regular.embed"
#include "stb_image.h"

static taskhub::Application* s_Instance = nullptr;

static void glfw_error_callback(int error, const char* description) {

	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace taskhub {

	Application::Application(const ApplicationProvision& provision)
		: m_AppProvision(provision) 
	{
		s_Instance = this;
		Init();
	}

	Application::~Application() {

		s_Instance = nullptr;
		Shutdown();
	}

	void Application::Run() {

		ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGuiIO& io = ImGui::GetIO();

		while (!glfwWindowShouldClose(m_Window) && m_Running) {

			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();

			for (auto& layer : m_LayerStack) {
				layer->OnUpdate();
			}

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			
				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
				if (m_MenuBar)
					window_flags |= ImGuiWindowFlags_MenuBar;
			
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			
				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
				// and handle the pass-thru hole, so we ask Begin() to not render a background.
				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;
			
				// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
				// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
				// all active windows docked into it will lose their parent and become undocked.
				// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
				// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("DockSpace", nullptr, window_flags);
				ImGui::PopStyleVar();
			
				ImGui::PopStyleVar(2);
			
				// Submit the DockSpace
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
					ImGuiID dockspace_id = ImGui::GetID("AppDockspace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				// MenuBar Callback
				if (m_MenuBar) {
					if (ImGui::BeginMenuBar()) {
						m_MenuBar();
						ImGui::EndMenuBar();
					}
				}
			
				for (auto& layer : m_LayerStack) {
					layer->OnUIRender();
				}

				ImGui::End();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(5));

			// Rendering
			ImGui::Render();
			//int displayW, displayH;
			//glfwGetFramebufferSize(m_Window, &displayW, &displayH);
			//glViewport(0, 0, displayW, displayH);
			//glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
			//glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}

			m_Context->SwapBuffers();
		}
	}

	void Application::Init() {

		glfwSetErrorCallback(glfw_error_callback);

		if (!glfwInit()) {
			HUB_CORE_ERROR("Failed to initialize glfw");
			return;
		}

		#if defined(HUB_DEBUG)
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

		m_Window = glfwCreateWindow(m_AppProvision.Width, m_AppProvision.Height, m_AppProvision.Name.c_str(), nullptr, nullptr);

		HUB_CORE_ASSERT(m_Window, "Failed to create window");

		m_Context = std::make_unique<GraphicsContext>(m_Window);
		m_Context->Init();

		glfwSwapInterval(1); // Enable vsync

		// Set app icon if defined by user
		if (!m_AppProvision.AppIconPath.empty()) {
			
			GLFWimage icon;
			int channels;
			int desiredChannels = 4;

			HUB_CORE_ASSERT(std::filesystem::exists(m_AppProvision.AppIconPath), "Filepath does not exist");
			std::string pathString = m_AppProvision.AppIconPath.string();
			icon.pixels = stbi_load(pathString.c_str(), &icon.width, &icon.height, &channels, desiredChannels);
			glfwSetWindowIcon(m_Window, 1, &icon);
			stbi_image_free(icon.pixels);
		}

		// ImGui initialization
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.ConfigDockingTransparentPayload = true;
		//io.ConfigViewportsNoAutoMerge = true;

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		// Set default font
		ImFontConfig fontConfig;
		fontConfig.FontDataOwnedByAtlas = false;
		ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
		io.FontDefault = robotoFont;

		// Set default theme
		UI::SetTaskHubStyle();
	}

	void Application::Close() {
		m_Running = false;
	}

	Application& Application::Get() {
		return *s_Instance;
	}

	bool Application::IsMinimized() const {
		return (bool)glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED);
	}

	void Application::Shutdown() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}
}