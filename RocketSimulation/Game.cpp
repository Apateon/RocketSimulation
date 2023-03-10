#include "Game.h"

using namespace DirectX;
using namespace std;

//constructor
Game::Game(const float pWidth, const float pHeight, AntTweakManager& pAwManager) : g_AwManager(&pAwManager), g_TimeScale(5.0f), g_CameraSpeed(8.0f), g_Exit(false), g_Launch(false), g_Width(pWidth), g_Height(pHeight)
{
	const vector<float> vec(50, 0);
	g_DeltaTimeSamples = vec;

	//set up ekyboard for input
	g_Keyboard = std::make_unique<Keyboard>();

	CreateScene();

	//create gui
	g_AwManager->AddBar("WorldSettings");

	//terrain settings
	g_AwManager->AddVariable("WorldSettings", "Terrain Scale", g_TerrainScale, "group = Terrain");
	g_AwManager->AddVariable("WorldSettings", "Cubes in X", g_TerrainX, "group = Terrain");
	g_AwManager->AddVariable("WorldSettings", "Cubes in Y", g_TerrainY, "group = Terrain");
	g_AwManager->AddVariable("WorldSettings", "Cubes in Z", g_TerrainZ, "group = Terrain");
	g_AwManager->AddVariable("WorldSettings", "Cube Count", g_CubeCount, "group = Terrain");

	//rocket settings
	g_AwManager->AddEditVariable("WorldSettings", "Rocket Thrust", g_RocketSpeed, "group = Rocket step=0.1 min=0 max = 3");
	g_AwManager->AddVariable("WorldSettings", "X Pos", const_cast<float&>(g_Rocket->Position().x), "group = Rocket");
	g_AwManager->AddVariable("WorldSettings", "Y Pos", const_cast<float&>(g_Rocket->Position().y), "group = Rocket");
	g_AwManager->AddVariable("WorldSettings", "Z Pos", const_cast<float&>(g_Rocket->Position().z), "group = Rocket");

	//game time settings
	g_AwManager->AddBar("GameSettings");
	g_AwManager->AddEditVariable("GameSettings", "Time Scale", g_TimeScale, "step=0.1");
	g_AwManager->AddVariable("GameSettings", "Time", g_Time, "");
	g_AwManager->AddVariable("GameSettings", "FPS", g_FrameRate, "");

	//camera settings
	g_AwManager->AddVariable("GameSettings", "Screen Width", g_Width, "group = Camera");
	g_AwManager->AddVariable("GameSettings", "Screen Height", g_Height, "group = Camera");
	g_AwManager->AddVariable("GameSettings", "X Pos", const_cast<float&>(mActiveCamera->Eye().x), "group = Camera");
	g_AwManager->AddVariable("GameSettings", "Y Pos", const_cast<float&>(mActiveCamera->Eye().y), "group = Camera");
	g_AwManager->AddVariable("GameSettings", "Z Pos", const_cast<float&>(mActiveCamera->Eye().z), "group = Camera");

	//sun settings
	g_AwManager->AddVariable("GameSettings", "SunX", const_cast<float&>(g_Lights[0].Position().x), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "SunY", const_cast<float&>(g_Lights[0].Position().y), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "SunZ", const_cast<float&>(g_Lights[0].Position().z), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "SunOrbit", const_cast<float&>(g_Lights[0].GetOrbit().z), "group = Lights");
	g_AwManager->AddEditVariable("GameSettings", "SunColour", const_cast<XMFLOAT4&>(g_Lights[0].Colour()), "group = Lights");

	//moon settings
	g_AwManager->AddVariable("GameSettings", "MoonX", const_cast<float&>(g_Lights[1].Position().x), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "MoonY", const_cast<float&>(g_Lights[1].Position().y), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "MoonZ", const_cast<float&>(g_Lights[1].Position().z), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "MoonOrbit", const_cast<float&>(g_Lights[1].GetOrbit().z), "group = Lights");
	g_AwManager->AddEditVariable("GameSettings", "MoonColour", const_cast<XMFLOAT4&>(g_Lights[1].Colour()), "group = Lights");

	//additional light settings
	g_AwManager->AddVariable("GameSettings", "EngineX", const_cast<float&>(g_Lights[2].Position().x), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "EngineY", const_cast<float&>(g_Lights[2].Position().y), "group = Lights");
	g_AwManager->AddVariable("GameSettings", "EngineZ", const_cast<float&>(g_Lights[2].Position().z), "group = Lights");
	g_AwManager->AddEditVariable("GameSettings", "EngineColour", const_cast<XMFLOAT4&>(g_Lights[2].Colour()), "group = Lights");
}

//function to initialise the scene
void Game::CreateScene()
{
	InitialiseLights();

#ifdef _DEBUG
	g_TerrainX = 100;
	g_TerrainY = 20;
	g_TerrainZ = 20;
	g_TerrainScale = 1.5f;
	g_RocketSpeed = 1.0f;
	g_ExplosionRadius = 5.0f;
#endif

	g_GameObjects.reserve(10);
	g_Lights.reserve(5);

	//creating the skysphere
	GameObject atmos(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1));
	atmos.AddShape(nullptr, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), wstring(L"Atmosphere.dds"), wstring(L""), wstring(L""), wstring(L"environmentShader.fx"), "EnvironmentMap", true, false, GeometryType::CUBE);
	g_GameObjects.emplace_back(atmos);

	//creating the launcher base
	GameObject launchbase(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(-(g_TerrainScale*g_TerrainX) * 4 / 10, 0, 0, 1));
	launchbase.AddShape(nullptr, XMFLOAT4(4, 2, 4, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, -0.5f, 0, 1), wstring(L"PoleMetal.dds"), wstring(L""), wstring(L""), wstring(L"defaultShader.fx"), "LauncherBase", false, false, GeometryType::CUBE);
	launchbase.AddShape(nullptr, XMFLOAT4(0.2f, 4, 0.2f, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 2.5f, 0, 1), wstring(L"PoleMetal.dds"), wstring(L""), wstring(L""), wstring(L"defaultShader.fx"), "LauncherPole", false, false, GeometryType::CUBE);
	g_GameObjects.emplace_back(launchbase);

	//creating the ground
	GameObject ground(XMFLOAT4(g_TerrainScale, g_TerrainScale, g_TerrainScale, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(-(g_TerrainScale*g_TerrainX) / 2, -(g_TerrainScale*g_TerrainY), -(g_TerrainScale*g_TerrainZ) / 2, 1));

	vector<Instance> instances;

	for (auto x = 0; x < g_TerrainX; ++x)
	{
		for (auto y = 0; y < g_TerrainY; ++y)
		{
			for (auto z = 0; z < g_TerrainZ; ++z)
			{
				instances.emplace_back(Instance{ XMFLOAT3(x,y,z) });
			}
		}
	}

	//adding texture to ground	
	ground.AddShape(&instances, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), wstring(L"StoneGround.dds"), wstring(L"StoneNormal.dds"), wstring(L""), wstring(L"instanceParallaxShader.fx"), "TerrainCube", false, false, GeometryType::CUBE);
	g_GameObjects.emplace_back(ground);

	GameObject rocket(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(-(g_TerrainScale*g_TerrainX) * 4 / 10, 3, 0, 1));

	rocket.AddShape(nullptr, XMFLOAT4(0.5f, 5, 0.5f, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), wstring(L"MarbleShine.dds"), wstring(L"MarbleNormal.dds"), wstring(L""), wstring(L"parallaxShader.fx"), "RocketBody", false, false, GeometryType::CYLINDER);

	rocket.AddShape(nullptr, XMFLOAT4(0.75f, 2, 0.75f, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 3, 0, 1), wstring(L"Atmosphere.dds"), wstring(L""), wstring(L""), wstring(L"chromeShader.fx"), "RocketCone", false, false, GeometryType::CONE);
	
	vector<Instance> engineParticles;
	for (auto i = 0; i < 2000; ++i)
	{
		engineParticles.emplace_back(Instance{ XMFLOAT3(0,0,i) });	
	}

	rocket.AddShape(&engineParticles, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), wstring(L"ParticleRock.dds"), wstring(L""), wstring(L""), wstring(L"engineParticleShader.fx"), "Particles", false, true, GeometryType::QUAD);
	g_GameObjects.emplace_back(rocket);

	g_Environment = &g_GameObjects[0];
	g_Launcher = &g_GameObjects[1];
	g_Terrain = &g_GameObjects[2];
	g_Rocket = &g_GameObjects[3];

	InitialiseCameras();
}

void Game::HandleInput(const double& pDt)
{
	const auto state = g_Keyboard->GetState();
	g_Tracker.Update(state);

	if (state.Escape)
	{
		g_Exit = true;
	}

	if (state.R)
	{
		ResetGame();
	}
	if (mActiveCamera->Controllable())
	{
		if (state.LeftControl || state.RightControl)
		{
			if (state.Up)
			{
				auto forward = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&forward, XMLoadFloat4(&mActiveCamera->Forward()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(forward);
			}
			if (state.Down)
			{
				auto forward = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&forward, XMLoadFloat4(&mActiveCamera->Forward()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(XMFLOAT4(-forward.x, -forward.y, -forward.z, forward.w));
			}
			if (state.Right)
			{
				auto right = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&right, XMLoadFloat4(&mActiveCamera->Right()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(right);
			}
			if (state.Left)
			{
				auto right = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&right, XMLoadFloat4(&mActiveCamera->Right()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(XMFLOAT4(-right.x, -right.y, -right.z, right.w));
			}

			if (state.PageUp)
			{
				auto up = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&up, XMLoadFloat4(&mActiveCamera->Up()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(up);
			}
			if (state.PageDown)
			{
				auto up = XMFLOAT4(0, 0, 0, 1);
				XMStoreFloat4(&up, XMLoadFloat4(&mActiveCamera->Up()) * g_CameraSpeed * pDt);
				mActiveCamera->TranslateCam(XMFLOAT4(-up.x, -up.y, -up.z, up.w));
			}
		}
		else
		{
			if (state.Up)
			{
				auto rotation = XMFLOAT3(XMConvertToRadians(-10), 0.0f, 0.0f);
				XMStoreFloat3(&rotation, XMLoadFloat3(&rotation) * g_CameraSpeed * pDt);
				mActiveCamera->RotateCam(rotation);
			}
			if (state.Down)
			{
				auto rotation = XMFLOAT3(XMConvertToRadians(10), 0.0f, 0.0f);
				XMStoreFloat3(&rotation, XMLoadFloat3(&rotation) * g_CameraSpeed * pDt);
				mActiveCamera->RotateCam(rotation);
			}
			if (state.Left)
			{
				auto rotation = XMFLOAT3(0.0f, XMConvertToRadians(-10), 0.0f);
				XMStoreFloat3(&rotation, XMLoadFloat3(&rotation) * g_CameraSpeed * pDt);
				mActiveCamera->RotateCam(rotation);
			}
			if (state.Right)
			{
				auto rotation = XMFLOAT3(0.0f, XMConvertToRadians(10), 0.0f);
				XMStoreFloat3(&rotation, XMLoadFloat3(&rotation) * g_CameraSpeed * pDt);
				mActiveCamera->RotateCam(rotation);
			}
		}
	}
	if (g_Tracker.pressed.S)
	{
		g_AwManager->ToggleVisible();
	} 
	if (state.F1)
	{
		mActiveCamera = &g_Cameras[0];
	}
	else if (state.F2)
	{
		mActiveCamera = &g_Cameras[1];
	}
	else if (state.F3)
	{
		mActiveCamera = &g_Cameras[2];
	}
	else if (state.F4)
	{
		mActiveCamera = &g_Cameras[3];
	}
	else if (state.F5)
	{
		mActiveCamera = &g_Cameras[4];
	}
	if (!g_Launch)
	{
		if (state.LeftShift || state.RightShift)
		{
			if (state.OemComma)
			{
				auto rotation = XMFLOAT4(0, 0, XMConvertToRadians(5), 1);
				XMStoreFloat4(&rotation, XMLoadFloat4(&rotation) * g_CameraSpeed * pDt);
				g_Rocket->Rotate(rotation);
				g_Launcher->RotateShape(1, rotation);
			}
			if (state.OemPeriod)
			{
				auto rotation = XMFLOAT4(0, 0, XMConvertToRadians(-5), 1);
				XMStoreFloat4(&rotation, XMLoadFloat4(&rotation) * g_CameraSpeed * pDt);
				g_Rocket->Rotate(rotation);
				g_Launcher->RotateShape(1, rotation);
			}
		}
	}
	if (state.F11)
	{
		g_Launch = true;
	}
	if (state.T)
	{
		if (state.LeftShift || state.RightShift)
		{
			g_TimeScale += 0.1f;
		}
		else
		{
			g_TimeScale -= 0.1f;
		}
	}
}

//collision checker to explode rocket
void Game::CheckCollision(const Shape & pShape)
{
	const auto coneRadius = 0.5f;
	const auto cubeRadius = g_TerrainScale / 2;

	XMFLOAT4X4 transform{};
	XMStoreFloat4x4(&transform, XMLoadFloat4x4(pShape.Transform()) * XMLoadFloat4x4(g_Rocket->Transform()));
	const auto conePosition = XMFLOAT4(transform._41, transform._42, transform._43, transform._44);
	const auto terrain = g_Terrain->Shapes()[0].Instances();

	for (const auto& instance : terrain)
	{
		const auto cubePosition = XMVector3Transform(XMLoadFloat3(&instance.mPosition), XMLoadFloat4x4(g_Terrain->Transform()));
		XMFLOAT4 distance{};
		XMStoreFloat4(&distance, XMVector4Length(XMLoadFloat4(&conePosition) - cubePosition));
		//rocket collided
		if (distance.x < coneRadius + cubeRadius)
		{
			//perform explosion routine
			onExplode(transform);
			return;
		}
	}
}

void Game::onExplode(XMFLOAT4X4 transform)
{
	Explosion(transform);
	ResetRocket();
	return;
}

void Game::Explosion(const XMFLOAT4X4& pTransform)
{
	auto conePosition = XMFLOAT4(pTransform._41, pTransform._42, pTransform._43, pTransform._44);

	if (g_Lights.size() > 3)
	{
		g_Lights[3].SetTranslation(conePosition);
	}
	else
	{
		g_Lights.emplace_back(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), conePosition, XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0.6f, 0.2f, 0.1f, 1));
		g_AwManager->AddVariable("GameSettings", "ExplosionX", const_cast<float&>(g_Lights[3].Position().x), "group = Lights");
		g_AwManager->AddVariable("GameSettings", "ExplosionY", const_cast<float&>(g_Lights[3].Position().y), "group = Lights");
		g_AwManager->AddVariable("GameSettings", "ExplosionZ", const_cast<float&>(g_Lights[3].Position().z), "group = Lights");
		g_AwManager->AddEditVariable("GameSettings", "ExplosionColour", const_cast<XMFLOAT4&>(g_Lights[3].Colour()), "group = Lights");
	}

	vector<Instance> instances;
	for (auto i = 0; i < 2000; ++i)
	{
		instances.emplace_back(Instance{ XMFLOAT3(0,0,i) });
	}

	GameObject particles(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(conePosition.x, conePosition.y-3, conePosition.z, 1));
	particles.AddShape(&instances, XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), wstring(L"ExplosionParticle.dds"), wstring(L""), wstring(L""), wstring(L"explosionParticleShader.fx"), "Particles", false, true, GeometryType::QUAD);
	g_GameObjects.emplace_back(particles);
	g_ParticleTimer = 10.0f;

	vector<Instance> indexToRemove;
	const auto terrain = g_Terrain->Shapes()[0].Instances();
	for (const auto& instance : terrain)
	{
		const auto cubePosition = XMVector3Transform(XMLoadFloat3(&instance.mPosition), XMLoadFloat4x4(g_Terrain->Transform()));
		XMFLOAT4 distance{};
		XMStoreFloat4(&distance, XMVector4Length(XMLoadFloat4(&conePosition) - cubePosition));
		if (distance.x < g_ExplosionRadius)
		{
			indexToRemove.push_back(instance);
		}
	}
	g_Terrain->RemoveInstancesFromShape(0, indexToRemove);
}

void Game::Update(const double& pDt)
{
	g_CubeCount = g_Terrain->Shapes()[0].Instances().size();
	g_Time += pDt;

	g_AverageDt = 0;
	for (auto i = 0; i < 49; ++i)
	{
		g_DeltaTimeSamples[i] = g_DeltaTimeSamples[i + 1];
	}
	g_DeltaTimeSamples[49] = pDt;

	for (const auto& deltaTimeSample : g_DeltaTimeSamples)
	{
		g_AverageDt += deltaTimeSample;
	}
	g_AverageDt /= 50;
	g_FrameRate = 1 / g_AverageDt;

	if (g_Launch)
	{
		XMFLOAT4 translation{};
		XMStoreFloat4(&translation, XMLoadFloat4(&g_Rocket->Up()) * g_RocketSpeed * g_TimeScale * pDt);
		g_Rocket->Translate(translation);

		if (g_Rocket->Rotation().z > -(XM_PI * 8 / 10))
		{
			g_Rocket->Rotate(XMFLOAT4(0, 0, XMConvertToRadians(-2.5)*g_TimeScale*pDt, 1));
		}
		else if (g_Rocket->Rotation().z > -XM_PI)
		{
			g_Rocket->Rotate(XMFLOAT4(0, 0, XMConvertToRadians(-1)*g_TimeScale*pDt, 1));
		}
	}
	XMFLOAT4 enginePos{};
	const auto rocketPos = g_Rocket->Position();
	XMStoreFloat4(&enginePos, XMLoadFloat4(&rocketPos) - (XMLoadFloat4(&g_Rocket->Up()) * 5));
	g_Lights[2].SetTranslation(enginePos);

	HandleInput(pDt);
	if (mActiveCamera->Name() == "RocketConeCam")
	{
		XMFLOAT4X4 transform{};
		XMStoreFloat4x4(&transform, XMLoadFloat4x4(g_Rocket->Shapes()[1].Transform()) * XMLoadFloat4x4(g_Rocket->Transform()));
		const auto conePos = XMFLOAT4(transform._41, transform._42, transform._43, 1);
		mActiveCamera->LookAt(conePos);
		mActiveCamera->SetEye(XMFLOAT4(conePos.x + 1, conePos.y, -1.0f, 1.0f));
	}
	else if (mActiveCamera->Name() == "RocketBodyCam")
	{
		mActiveCamera->LookAt(g_Rocket->Position());
		mActiveCamera->SetEye(XMFLOAT4(g_Rocket->Position().x, g_Rocket->Position().y, -2.0f, 1.0f));
	}
	else if (mActiveCamera->Name() == "WideCam")
	{
		mActiveCamera->LookAt(g_Rocket->Position());
	}

	const auto rocket = g_Rocket->Shapes();
	for (const auto& shape : rocket)
	{
		if (shape.Name() == "RocketCone")
		{
			CheckCollision(shape);
		}
	}

	if (g_ParticleTimer > 0)
	{
		g_ParticleTimer -= pDt * g_TimeScale;
	}
	if (g_ParticleTimer < 0 && g_GameObjects.size() > 4)
	{
		g_GameObjects.pop_back();
	}

	DayNightCycle(pDt);
}

const std::vector<GameObject>& Game::GameObjects() const
{
	return g_GameObjects;
}

const Camera * const Game::Cam() const
{
	return mActiveCamera;
}

const vector<Light> & Game::Lights() const
{
	return g_Lights;
}

const float Game::ScaledTime() const
{
	return g_Time * g_TimeScale;
}

void Game::ResetRocket()
{
	g_Launch = false;
	g_Rocket->ResetObject();
	g_Rocket->Translate(XMFLOAT4(-(g_TerrainScale*g_TerrainX) * 4 / 10, 3, 0, 1));
	g_Launcher->SetShapeRotation(1, XMFLOAT4(0, 0, 0, 1));
}

//initialing the light values
void Game::InitialiseLights()
{
	g_Lights.emplace_back(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, (g_TerrainScale*g_TerrainX / 2) + 10, 0, 1), XMFLOAT4(0.9f, 0.9f, 0.9f, 1));

	g_Lights.emplace_back(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, -((g_TerrainScale*g_TerrainX / 2) + 10), 0, 1), XMFLOAT4(0.5f, 0.5f, 0.9f, 1));

	g_Lights.emplace_back(XMFLOAT4(1, 1, 1, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0, 0, 0, 1), XMFLOAT4(0.5f, 0.5f, 0.5f, 1));
}

//initialising the camera values
void Game::InitialiseCameras()
{
	g_Cameras.emplace_back(XMFLOAT4(g_Launcher->Position().x, g_Launcher->Position().y+3, -7.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		g_Width,
		g_Height,
		true,
		"LauncherCam");

	g_Cameras.emplace_back(XMFLOAT4(-42, 25, 0.0f, 1.0f),
		XMFLOAT4(XM_PIDIV2, 0, 0, 1),
		g_Width,
		g_Height,
		true,
		"TerrainCam");

	g_Cameras.emplace_back(XMFLOAT4(-30, 1.0f, -20.0f, 1.0f),
		XMFLOAT4(0, 0, 0, 1),
		g_Width,
		g_Height,
		false,
		"WideCam");

	XMFLOAT4X4 transform{};
	XMStoreFloat4x4(&transform, XMLoadFloat4x4(g_Rocket->Shapes()[1].Transform()) * XMLoadFloat4x4(g_Rocket->Transform()));
	const auto conePos = XMFLOAT4(transform._41, transform._42, transform._43, 1);
	g_Cameras.emplace_back(XMFLOAT4(conePos.x + 6, conePos.y, -1.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		g_Width,
		g_Height,
		false,
		"RocketConeCam");

	g_Cameras.emplace_back(XMFLOAT4(g_Rocket->Position().x, g_Rocket->Position().y, -2.0f, 1.0f),
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		g_Width,
		g_Height,
		false,
		"RocketBodyCam");

	mActiveCamera = &g_Cameras[0];
}

void Game::DayNightCycle(const float pDt)
{
	g_Lights[0].Orbit(XMFLOAT4(0, 0, -0.05f * g_TimeScale * pDt, 1));

	g_Lights[1].Orbit(XMFLOAT4(0, 0, -0.05f * g_TimeScale * pDt, 1));
}

void Game::ResetGame()
{
	ResetRocket();

	vector<Instance> instances;
	for (auto x = 0; x < g_TerrainX; ++x)
	{
		for (auto y = 0; y < g_TerrainY; ++y)
		{
			for (auto z = 0; z < g_TerrainZ; ++z)
			{
				instances.emplace_back(Instance{ XMFLOAT3(x,y,z) });
			}
		}
	}
	g_Terrain->SetShapeInstances(0, instances);
	g_Lights.clear();
	InitialiseLights();
	g_Cameras.clear();
	InitialiseCameras();
	g_TimeScale = 5.0f;
}

const bool & Game::Exit() const
{
	return g_Exit;
}