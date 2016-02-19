#pragma once
#include "GameState.h"
#include "Grid.h"
#include "Cube_VertexBuffer.h"


class GameState_VertexBuffer :
	public GameState
{
public:
	GameState_VertexBuffer();
	virtual ~GameState_VertexBuffer();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
	enum RenderMode
	{
		NORMAL,
		VB,
		VB_SHARED,
		NUM_OF_RENDER_MODE
	};

	RenderMode currentRenderMode = NORMAL;

	enum KeyState
	{
		UP,
		DOWN,
		NUM_OF_KEY_TYPE
	};

	bool keyState[NUM_OF_KEY_TYPE];

	Grid* grid = nullptr;
	Cube_VertexBuffer* cube = nullptr;

	static const int CUBE_COUNT_SQRT = 200;
	Cube_VertexBuffer* cubeArray = nullptr;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;

	LPD3DXFONT font = nullptr;
};

