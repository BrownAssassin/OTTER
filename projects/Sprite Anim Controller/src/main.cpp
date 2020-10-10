#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "AdventurerFSM.h"

#include "imgui.h"

#include <memory>

using namespace nou;

int main() {
	App::Init("", 1536, 864);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	App::InitImgui();

	// Shaders
	auto vs = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto fs = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);
	auto sp = ShaderProgram({ vs.get(), fs.get() });

	// Textures & Materials
	Texture2D adventurerTexture = Texture2D("adventurer.png", true);
	Material adventurerMaterial(sp);
	adventurerMaterial.AddTexture("albedo", adventurerTexture);

	Texture2D bgTexture = Texture2D("SunsetField.png", true);
	Material bgMaterial(sp);
	bgMaterial.AddTexture("albedo", bgTexture);

	// Spritesheet
	auto adventurerSP = std::make_unique<Spritesheet>(adventurerTexture, glm::vec2(50.0f, 37.0f));
	adventurerSP->AddAnimation(AdventurerFSM::idleClip, 0, 3, 6.0f);
	adventurerSP->AddAnimation(AdventurerFSM::crouchClip, 4, 7, 6.0f);
	adventurerSP->AddAnimation(AdventurerFSM::runClip, 8, 13, 12.0f);
	adventurerSP->AddAnimation(AdventurerFSM::flipClip, 14, 23, 12.0f);
	adventurerSP->AddAnimation(AdventurerFSM::attackClip, 41, 47, 12.0f);

	auto backgroundSP = std::make_unique<Spritesheet>(bgTexture, glm::vec2(1920.0f, 1080.0f));

	// Entities
	Entity cameraEntt = Entity::Create();
	auto& camera = cameraEntt.Add<CCamera>(cameraEntt);
	camera.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	cameraEntt.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	Entity backgroundEntt = Entity::Create();
	backgroundEntt.transform.m_scale = glm::vec3(0.42f, 0.75f, 0.0f);
	backgroundEntt.Add<CSpriteRenderer>(backgroundEntt, *backgroundSP, bgMaterial);

	Entity AdventurerEntity = Entity::Create();
	AdventurerEntity.transform.m_scale = glm::vec3(1.5f, 2.5f, 0.0f);
	AdventurerEntity.transform.m_pos = glm::vec3(0.0f, -225.0f, 0.0f);
	AdventurerEntity.Add<CSpriteRenderer>(AdventurerEntity, *adventurerSP, adventurerMaterial);
	AdventurerEntity.Add<CSpriteAnimator>(AdventurerEntity, *adventurerSP);
	AdventurerEntity.Add<AdventurerFSM>(AdventurerEntity);

	App::Tick();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE)) {
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		bool crouching = Input::GetKey(GLFW_KEY_S);
		AdventurerEntity.Get<AdventurerFSM>().SetVar("crouching", crouching);

		bool running = Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_D);
		AdventurerEntity.Get<AdventurerFSM>().SetVar("running", running);

		if (Input::GetKey(GLFW_KEY_W))
			AdventurerEntity.Get<AdventurerFSM>().SetTrig("flipping");

		if (Input::GetKey(GLFW_KEY_ENTER))
			AdventurerEntity.Get<AdventurerFSM>().SetTrig("attacking");

		if (running && !(AdventurerEntity.Get<AdventurerFSM>().GetVar("crouching")) && !(AdventurerEntity.Get<AdventurerFSM>().GetTrig("attacking"))) {
			bool flip = Input::GetKey(GLFW_KEY_A);

			AdventurerEntity.transform.m_scale.x = (flip) ? -1.5f : 1.5f;
			AdventurerEntity.transform.m_pos.x += (flip) ? -100.0f * deltaTime : 100.0f * deltaTime;
		}

		// Updates
		cameraEntt.Get<CCamera>().Update();
		AdventurerEntity.Get<AdventurerFSM>().Update();
		AdventurerEntity.Get<CSpriteAnimator>().Update(deltaTime);

		// Recompute Global Matricies
		backgroundEntt.transform.RecomputeGlobal();
		AdventurerEntity.transform.RecomputeGlobal();

		// Draw Sprite
		backgroundEntt.Get<CSpriteRenderer>().Draw();
		AdventurerEntity.Get<CSpriteRenderer>().Draw();

		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
} 
