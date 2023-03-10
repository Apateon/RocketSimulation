#pragma once
#include <vector>
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"
#include <Keyboard.h>
#include "AntTweakManager.h"

class Game
{
	std::vector<GameObject> g_GameObjects;
	std::vector<Light> g_Lights;
	std::vector<Camera> g_Cameras;
	Camera* mActiveCamera = nullptr;
	std::unique_ptr<DirectX::Keyboard> g_Keyboard;
	DirectX::Keyboard::KeyboardStateTracker g_Tracker;
	AntTweakManager* g_AwManager;
	float g_TimeScale;
	float g_CameraSpeed;
	bool g_Exit;
	bool g_Launch;
	GameObject* g_Environment = nullptr;
	GameObject* g_Launcher = nullptr;
	GameObject* g_Rocket = nullptr;
	GameObject* g_Terrain = nullptr;
	GameObject* g_Sun = nullptr;
	GameObject* g_Moon = nullptr;
	float g_TerrainScale = 1;
	int g_TerrainX = 120;
	int g_TerrainY = 40;
	int g_TerrainZ = 40;
	int g_CubeCount = 0;
	float g_Width;
	float g_Height;
	float g_Time = 0;
	float g_AverageDt = 0;
	std::vector<float> g_DeltaTimeSamples;
	float g_FrameRate = 0;
	float g_RocketSpeed = 2;
	float g_ExplosionRadius = 7.0f;
	float g_ParticleTimer = 0.0f;

	void CreateScene();
	void HandleInput(const double& pDt);
	void CheckCollision(const Shape& pShape);
	void Explosion(const DirectX::XMFLOAT4X4& pTransform);
	void onExplode(DirectX::XMFLOAT4X4 transform);
	void ResetRocket();
	void InitialiseLights();
	void InitialiseCameras();
	void DayNightCycle(const float pDt);
	void ResetGame();
	
public:
	Game(const float pWidth, const float pHeight, AntTweakManager& pAwManager);
	~Game() = default;

	Game& operator=(const Game& pGame) = delete;
	Game(const Game& pGame) = delete;
	const bool& Exit() const;

	void Update(const double& pDt);
	const std::vector<GameObject>& GameObjects() const;
	const Camera * const Cam() const;
	const std::vector<Light> & Lights() const;
	const float ScaledTime() const;
};