#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Camera.h" 
#include "Renderer.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Walnut;

class ExampleLayer : public Walnut::Layer{

public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f)
	{
		Material& pinkSphere = m_Scene.Materials.emplace_back();
		pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
		pinkSphere.Roughness = 0.0f;

		Material& blueSphere = m_Scene.Materials.emplace_back();
		blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
		blueSphere.Roughness = 0.1f;

		Material& orangeSphere = m_Scene.Materials.emplace_back();
		orangeSphere.Albedo = { 0.8f, 0.5f, 0.2f };
		orangeSphere.Roughness = 0.1f;
		orangeSphere.EmissionColor = orangeSphere.Albedo;
		orangeSphere.EmissionPower = 2.0f;

		

		{
			Sphere sphere;
			sphere.Position = { 0.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 0;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 2.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 2;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 0.0f, -101.0f, 0.0f };
			sphere.Radius = 100.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}

		Material additionalSphere;
		/*Material& additionalSphere = m_Scene.Materials.emplace_back();
		additionalSphere.Albedo = { 1.0f, 0.5f, 0.6f };
		additionalSphere.Roughness = 0.1f;
		additionalSphere.EmissionColor = additionalSphere.Albedo;
		additionalSphere.EmissionPower = 0.2f; */

		{
			Sphere sphere;
			sphere.Position = { 0.0f, 2.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}
	}

	virtual void OnUpdate(float ts) override{

		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnUIRender() override{

		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")){

			Render();
		}

		ImGui::Checkbox("Accumulate", &m_Renderer.GetSettings().Accumulate);

		if (ImGui::Button("Reset"))
			m_Renderer.ResetFrameIndex();

		ImGui::End();

		ImGui::Begin("Scene");

		//changed m_Scene.Spheres.sixe() to 3 in attempt to fix the overload issues
		for (size_t i = 0; i < 3; i++){

			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

			ImGui::Separator();

			ImGui::PopID();
		}
		//changed  m_Scene.Materials.size() to 3
		for (size_t i = 0; i < 3; i++){

			ImGui::PushID(i);

			Material& material = m_Scene.Materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
			ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
			ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

			ImGui::Separator();

			ImGui::PopID();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render(){

		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Scene, m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}


private:
	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv){

	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
		{
			if (ImGui::BeginMenu("File")){

				if (ImGui::MenuItem("Exit")){

					app->Close();
				}

				//ADD SPHERE BUTTON HERE
				if (ImGui::MenuItem("Add Sphere")) {
					//app->AddSphere();
					
					Scene m_Scene;
					Sphere sphere;

					//create the sphere instance
					Material additionalSphere = m_Scene.Materials.emplace_back();
					additionalSphere.Albedo = { 0.8f, 0.5f, 0.2f };
					additionalSphere.Roughness = 0.1f;

					
					sphere.Position = { 1.5f, 2.0f, 0.0f };
					sphere.Radius = 1.5f;
					sphere.MaterialIndex = 1;
					m_Scene.Spheres.push_back(sphere);
					
					
					//add this to the ExampleLayer
					int i = m_Scene.Spheres.size();
					ExampleLayer::ExampleLayer[i].append(additionalSphere, sphere);
					

					ImGui::Begin("Additional Spheres");
						//put the sliders on the sidebar

					
						ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.0f);
						ImGui::DragFloat("Radius", &sphere.Radius, 0.0f);
						ImGui::DragInt("Material", &sphere.MaterialIndex, 0.0f, 0, (int)m_Scene.Materials.size() - 1);

						ImGui::Separator();

						//vector subscript error happens on the line right below this... no clue why cause this line runs fine on the first boot up...
						ImGui::ColorEdit3("Albedo", glm::value_ptr(additionalSphere.Albedo));
						ImGui::DragFloat("Roughness", &additionalSphere.Roughness, 0.0f, 0.0f, 0.0f);
						ImGui::DragFloat("Metallic", &additionalSphere.Metallic, 0.0f, 0.0f, 0.0f);
						ImGui::ColorEdit3("Emission Color", glm::value_ptr(additionalSphere.EmissionColor));
						ImGui::DragFloat("Emission Power", &additionalSphere.EmissionPower, 0.0f, 0.0f, FLT_MAX);

						ImGui::Separator();

					ImGui::End();
				}

				bool orbitStatus = false;
				if (ImGui::MenuItem("Sphere Orbit")) {
					//change the orbitStatus variable to make it a toggle button
					if (orbitStatus == false) {
						orbitStatus = true;
					}
					//if spheres are orbitting when button is clicked again this will stop them.
					else {
						orbitStatus = false;
					}
					//get each spheres x,y,z positions
					//start the loop for incrementing x,z and maybe y by using radian functions to make the orbit shape
					while (orbitStatus == true) {
						
					}
					ImGui::End();
				}

				ImGui::EndMenu();
			}

		});
	return app;
}
