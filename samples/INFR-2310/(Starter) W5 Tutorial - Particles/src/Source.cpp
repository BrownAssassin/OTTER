/*
Week 5 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "CParticleSystem.h"
#include "NOU/GLTFLoader.h"

#include "imgui.h"

#include <memory>
#include <ctime>

using namespace nou;

//Forward declaring our global resources for this demo.
std::unique_ptr<ShaderProgram> prog_texLit, prog_particles;
std::unique_ptr<Mesh> duckMesh;
std::unique_ptr<Texture2D> duckTex, particleTex, particleTwoTex;
std::unique_ptr<Material> duckMat, particleMat, particleTwoMat;

//This function will load in our global resources.
//(It's only been separated to make main() a bit cleaner to look at.)
void LoadDefaultResources();

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	App::Init("Week 5 Tutorial - Particles", 800, 800);
	App::SetClearColor(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));

	App::InitImgui();

	LoadDefaultResources();

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, *duckMesh, *duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//TODO: Set up our particle system.
	ParticleParam particleData;
	particleData.lifetime = 1.5f;
	particleData.startColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	particleData.endColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	particleData.startSize = 0.1f;
	particleData.maxParticles = 200;
	particleData.emissionRate = 50.0f;
	particleData.tanTheta = glm::tan(glm::radians(30.0f));

	Entity particleEntity = Entity::Create();
	particleEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	particleEntity.Add<CParticleSystem>(particleEntity, *particleMat, particleData);

	//Second particle system
	ParticleParam particleTwoData;
	particleTwoData.lifetime = 1.0f;
	particleTwoData.startColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	particleTwoData.endColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	particleTwoData.startSize = 0.1f;
	particleTwoData.maxParticles = 200;
	particleTwoData.emissionRate = 50.0f;
	particleTwoData.tanTheta = glm::tan(glm::radians(30.0f));

	Entity particleTwoEntity = Entity::Create();
	particleTwoEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	particleTwoEntity.Add<CParticleSystem>(particleTwoEntity, *particleTwoMat, particleTwoData);

	App::Tick();

	std::string space = "";
	std::string pos = "Position";
	std::string lif = "Lifetime";
	std::string scol = "Start Colour";
	std::string ecol = "End Colour";
	std::string ssiz = "Start Size";
	std::string emrt = "Emission Rate";
	std::string tta = "Theta";

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		
		//Updates the camera.
		camEntity.Get<CCamera>().Update();
		 
		particleEntity.Get<CParticleSystem>().Update(deltaTime);
		particleTwoEntity.Get<CParticleSystem>().Update(deltaTime);

		duckEntity.transform.RecomputeGlobal();
		particleEntity.transform.RecomputeGlobal();
		particleTwoEntity.transform.RecomputeGlobal();

		//We draw particles with the depth buffer disabled
		//to prevent z-fighting.
		//Custom depth sorting is handled by our particle system.
		glDisable(GL_DEPTH_TEST);
		particleEntity.Get<CParticleSystem>().Draw();
		particleTwoEntity.Get<CParticleSystem>().Draw();
		glEnable(GL_DEPTH_TEST);
		
		duckEntity.Get<CMeshRenderer>().Draw();

		//For Imgui...
		App::StartImgui(); 

		// Particle One Controls
		static bool particlePanel = true;
		ImGui::Begin("Particle Two", &particlePanel, ImVec2(125, 555));

		auto& transform = particleTwoEntity.transform.m_pos;
		auto& lifetime = particleTwoEntity.Get<CParticleSystem>().GetData()->param.lifetime;
		auto& startColor = particleTwoEntity.Get<CParticleSystem>().GetData()->param.startColor;
		auto& endColor = particleTwoEntity.Get<CParticleSystem>().GetData()->param.endColor;
		auto& startSize = particleTwoEntity.Get<CParticleSystem>().GetData()->param.startSize;
		auto& emissionRate = particleTwoEntity.Get<CParticleSystem>().GetData()->param.emissionRate;
		auto& tanTheta = particleTwoEntity.Get<CParticleSystem>().GetData()->param.tanTheta;
		float theta = std::round(glm::degrees(glm::tanh(particleTwoEntity.Get<CParticleSystem>().GetData()->param.tanTheta)));

		ImGui::Text(pos.c_str());
		ImGui::SliderFloat("X", &(transform.x), -2.0f, 2.0f);
		ImGui::SliderFloat("Y", &(transform.y), -2.0f, 2.0f);
		ImGui::SliderFloat("Z", &(transform.z), -2.0f, 2.0f);

		ImGui::Text(space.c_str());

		ImGui::Text(lif.c_str());
		ImGui::InputFloat("", &lifetime);
			
		ImGui::Text(space.c_str());

		ImGui::Text(scol.c_str());
		ImGui::SliderFloat("R", &(startColor.x), 0.0f, 1.0f);
		ImGui::SliderFloat("G", &(startColor.y), 0.0f, 1.0f);
		ImGui::SliderFloat("B", &(startColor.z), 0.0f, 1.0f);

		ImGui::Text(space.c_str());

		ImGui::Text(ecol.c_str());
		ImGui::SliderFloat("R ", &(endColor.x), 0.0f, 1.0f);
		ImGui::SliderFloat("G ", &(endColor.y), 0.0f, 1.0f);
		ImGui::SliderFloat("B ", &(endColor.z), 0.0f, 1.0f);

		ImGui::Text(space.c_str());

		ImGui::Text(ssiz.c_str());
		ImGui::InputFloat(" ", &startSize);

		ImGui::Text(space.c_str());

		ImGui::Text(emrt.c_str());
		ImGui::InputFloat("  ", &emissionRate);

		ImGui::Text(space.c_str());

		ImGui::Text(tta.c_str());
		ImGui::InputFloat("   ", &theta);
		tanTheta = glm::tan(glm::radians(theta));		

		ImGui::End();

		App::EndImgui();  

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
}

void LoadDefaultResources()
{
	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	//Lit and textured shader program.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> texLit = { v_texLit.get(), f_texLit.get() };
	prog_texLit = std::make_unique<ShaderProgram>(texLit);

	//Billboarded particles shader program.
	auto v_particles = std::make_unique<Shader>("shaders/particles.vert", GL_VERTEX_SHADER);
	auto g_particles = std::make_unique<Shader>("shaders/particles.geom", GL_GEOMETRY_SHADER);
	auto f_particles = std::make_unique<Shader>("shaders/particles.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> particles = { v_particles.get(), g_particles.get(), f_particles.get() };
	prog_particles = std::make_unique<ShaderProgram>(particles);

	//Load in the ducky model.
	duckMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("duck/Duck.gltf", *duckMesh);

	//Load in textures.
	duckTex = std::make_unique<Texture2D>("duck/DuckCM.png");
	particleTex = std::make_unique<Texture2D>("particle.png");
	particleTwoTex = std::make_unique<Texture2D>("particleTwo.png");
	 
	//Make materials. 
	duckMat = std::make_unique<Material>(*prog_texLit);
	duckMat->AddTexture("albedo", *duckTex);

	particleMat = std::make_unique<Material>(*prog_particles);
	particleMat->AddTexture("albedo", *particleTex);

	particleTwoMat = std::make_unique<Material>(*prog_particles);
	particleTwoMat->AddTexture("albedo", *particleTwoTex);
}