/*
Week 2 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//TODO: Write templated LERP function.
template<typename T>
T LERP(const T& p0, const T& p1, float t) {
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 900, 800);
	App::SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//TODO: Load in duck model and texture.
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");
	Material duckMat(prog_texLit);
	duckMat.AddTexture("albedo", duckTex);

	//Load in bot model and texture
	Mesh botMesh;
	GLTF::LoadMesh("bot/bot.gltf", botMesh);
	Texture2D botTex = Texture2D("bot/ArgumentsComeAfterNesquik.png");
	Material botMat(prog_texLit);
	botMat.AddTexture("kuromukuro", botTex);

	//TODO: Set up camera entity and transform.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//TODO: Create our duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.0005f, 0.0005f, 0.0005f);
	duckEntity.transform.m_pos = glm::vec3(1.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Create bot entity
	Entity botEntity = Entity::Create();
	botEntity.Add<CMeshRenderer>(botEntity, botMesh, botMat);
	botEntity.transform.m_scale = glm::vec3(0.01f, 0.01f, 0.01f);
	botEntity.transform.m_pos = glm::vec3(-1.0f, -1.0f, 0.0f);
	botEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	//Variables for LERPing position
	glm::vec3 origPos = glm::vec3(1.0f, -1.0f, 0.0f), newPos = glm::vec3(1.0f, 0.0f, 0.0f);
	float posTimer = 0.0f, posTimeLimit = 1.0f;
	bool duckyGoPlaces = true;

	//Variables for LERPing duck colour
	glm::vec3 origCol = glm::vec3(1.0f, 1.0f, 1.0f), newCol = glm::vec3(0.0f, 1.0f, 0.0f);
	float colTimer = 0.0f, colTimeLimit = 0.5f;
	bool duckyGoColour = true;

	//Variables for LERPing scale
	glm::vec3 origScl = glm::vec3(0.0005f, 0.0005f, 0.0005f), newScl = glm::vec3(0.01f, 0.01f, 0.01f);
	float sclTimer = 0.0f, sclTimeLimit = 5.0f;
	bool duckyGoSize = true;

	//Variables for LERPing rotation
	glm::vec3 origRot = glm::vec3(0.0f, -1.0f, 0.0f), newRot = glm::vec3(0.0f, 1.0f, 0.0f);
	float rotTimer = 0.0f, rotTimeLimit = 2.5f;
	bool botGoBrr = true;

	//Variables for LERPing bot colour
	glm::vec3 origBotCol = glm::vec3(1.0f, 1.0f, 1.0f), newBotCol = glm::vec3(1.0f, 0.0f, 1.0f);
	float botColTimer = 0.0f, botColTimeLimit = 5.0f;
	bool botGoColour = true;

	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: LERP stuff!
		posTimer += deltaTime;
		colTimer += deltaTime;
		sclTimer += deltaTime;
		rotTimer += deltaTime;
		botColTimer += deltaTime;

		if (posTimer >= posTimeLimit) {
			posTimer = 0.0f;
			duckyGoPlaces = !duckyGoPlaces;
		}

		if (colTimer >= colTimeLimit) {
			colTimer = 0.0f;
			duckyGoColour = !duckyGoColour;
		}

		if (sclTimer >= sclTimeLimit) {
			sclTimer = 0.0f;
			duckyGoSize = !duckyGoSize;
		}

		if (botColTimer >= botColTimeLimit) {
			botColTimer = 0.0f;
			botGoColour = !botGoColour;
		}

		//Calculate t
		float tPos = posTimer / posTimeLimit;
		float tCol = colTimer / colTimeLimit;
		float tScl = sclTimer / sclTimeLimit;
		float tRot = rotTimer / rotTimeLimit;
		float tBotCol = botColTimer / botColTimeLimit;

		if (duckyGoPlaces == true) {
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else if (duckyGoPlaces == false) {
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}

		if (duckyGoColour == true) {
			duckMat.m_color = LERP(origCol, newCol, tCol);
		}
		else if (duckyGoColour == false) {
			duckMat.m_color = LERP(newCol, origCol, tCol);
		}

		if (duckyGoSize == true) {
			duckEntity.transform.m_scale = LERP(origScl, newScl, tScl);
		}
		else if (duckyGoSize == false) {
			duckEntity.transform.m_scale = LERP(newScl, origScl, tScl);
		}

		if (botGoBrr == true) {
			botEntity.transform.m_rotation = LERP(origRot, newRot, tRot);
		}
		else if (botGoBrr == false) {
			botEntity.transform.m_rotation = LERP(newRot, origRot, tRot);
		}

		if (botGoColour == true) {
			botMat.m_color = LERP(origBotCol, newBotCol, tBotCol);
		}
		else if (botGoColour == false) {
			botMat.m_color = LERP(newBotCol, origBotCol, tBotCol);
		}

		//TODO: Update camera, update duck transform, draw duck...
		camEntity.Get<CCamera>().Update();

		duckEntity.transform.RecomputeGlobal();
		duckEntity.Get<CMeshRenderer>().Draw();

		botEntity.transform.RecomputeGlobal();
		botEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
