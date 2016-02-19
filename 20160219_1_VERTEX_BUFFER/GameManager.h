#pragma once

#include <time.h>

#include "Camera.h"


class GameManager
{
private:
	GameManager();
	GameManager(const GameManager& other);
	~GameManager();

public:
	static GameManager& Get()
	{
		static GameManager instance;
		return instance;
	}	
	static inline LPDIRECT3DDEVICE9 GetDevice(){ return GameManager::Get().GetInstanceDevice(); }
	static inline double GetTick(){ return GameManager::Get().GetCurrentTick(); }
	static inline Camera* GetCamera(){ return GameManager::Get().GetInstanceCamera(); }

	void Initialize(HWND handle);
	void Destroy();
	void Loop(double tick);
	LRESULT GameInputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline HWND GetWindowHandle(){ return windowHandle; }	

protected:
	bool hasInitialized = false;

	double currentTickTime = 0.0f;
	unsigned long FPSFrameCount = 0.0f;
	float FPSTimeElapsed = 0.0f;
	int frameRate = 0;
	
	HWND windowHandle = nullptr;
	LPDIRECT3D9 direct3d = nullptr;
	LPDIRECT3DDEVICE9 direct3dDevice = nullptr;
		
	D3DLIGHT9 mainLight;

	Camera* camera = nullptr;
	
	void Update();
	void Render();

	inline LPDIRECT3DDEVICE9 GetInstanceDevice(){ return direct3dDevice; }
	inline double GetCurrentTick(){ return currentTickTime; }
	inline Camera* GetInstanceCamera(){ return camera; }

	LPD3DXFONT font = nullptr;
}; 

