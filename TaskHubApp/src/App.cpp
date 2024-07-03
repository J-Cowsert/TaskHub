#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "GUI/UIUtils.h"
#include "Input/Input.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioFile.h"
#include "Core/Image.h"

class ImGuiTools : public taskhub::Layer {
public:
	void OnUIRender() override {
		ImGui::ShowDemoWindow();
        //ImGui::ShowMetricsWindow();
        //ImGui::ShowIDStackToolWindow();
		//ImGui::ShowStyleEditor();
	}
};

class AudioTest : public taskhub::Layer {
public:
    void OnAttach() override {

        // ("C:/Dev/Music/Shift (Instrumental).mp3");
        // ("C:/Dev/Music/[dream].mp3");
        // ("C:/Dev/Music/1 HOUR OF CHILL RAP BEATS.mp3");
        // ("C:/Dev/Music/Lofi Jazz Instrumental.mp3");
        // ("C:/Dev/Music/Nocturne.mp3");
        // ("C:/Dev/Music/Mo Beats - Righteous (pepe lore song).mp3");
        // ("C:/Dev/Music/Hope.mp3");
        // ("C:/Dev/Music/DVRST - Close Eyes.mp3");
        // ("C:/Dev/Music/Cmaj7 Backing Track Jam!!.mp3");

        m_AudioPlayer = std::make_unique<taskhub::AudioSource>();
        m_TestFile = std::make_unique<taskhub::AudioFile>("C:/Dev/Music/Mo Beats - Righteous (pepe lore song).mp3");

        HUB_INFO(m_TestFile->GetName());

        m_AudioPlayer->Load(*m_TestFile);
        m_AudioPlayer->Play();
    }

    void OnUIRender() override {
        
    }

private:
    std::unique_ptr<taskhub::AudioSource> m_AudioPlayer;
    std::unique_ptr<taskhub::AudioFile> m_TestFile;
   
};

class ImageTest : public taskhub::Layer {
public:
    void OnAttach() override {
        m_TestImage = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/TestImage.jpg");
        m_HDRTestImage = std::make_shared<taskhub::Image>("C:/Dev/Resources/Images/HDRTestImage.jpg");
    }

    void OnUIRender() override {
        ImGui::Begin("ImageTest");
        ImGui::Text("pointer = %x", m_TestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_TestImage->GetWidth(), m_TestImage->GetHeight());
        ImGui::Image((void*)(intptr_t)m_TestImage->GetTextureID(), ImVec2(m_TestImage->GetWidth(), m_TestImage->GetHeight()));
        ImGui::End();

        ImGui::Begin("HDRImageTest");
        ImGui::Text("pointer = %x", m_HDRTestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_HDRTestImage->GetWidth(), m_HDRTestImage->GetHeight());
        ImGui::Image((void*)(intptr_t)m_HDRTestImage->GetTextureID(), ImVec2(m_HDRTestImage->GetWidth() * 0.3, m_HDRTestImage->GetHeight() * 0.3));
        ImGui::End();

        ImGui::Begin("ImageButtonTest");
        ImGui::Text("pointer = %x", m_TestImage->GetTextureID());
        ImGui::Text("size = %d x %d", m_TestImage->GetWidth(), m_TestImage->GetHeight());

        if (ImGui::InvisibleButton("testbtn", ImVec2(100, 100))) {
            HUB_INFO("Image Button Pressed");
        }
        taskhub::UI::RenderImageButton(m_TestImage, IM_COL32(200, 200, 200, 255), IM_COL32(230, 230, 230, 255), IM_COL32(150, 150, 150, 255));
        
        ImGui::End();
    }

private:
    std::shared_ptr<taskhub::Image> m_TestImage;
    std::shared_ptr<taskhub::Image> m_HDRTestImage;
};

taskhub::Application* taskhub::CreateApplication() {

	taskhub::ApplicationProvision provision;

    provision.AppIconPath = "C:/Dev/Resources/Images/TaskHubIcon3.png";

	taskhub::Application* app = new taskhub::Application(provision);

	app->PushLayer<ImGuiTools>();
    app->PushLayer<AudioTest>();
    app->PushLayer<ImageTest>();

    app->SetMenubarCallback([app]() {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
	});

	return app;
}