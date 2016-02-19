#include "stdafx.h"
#include "GameState_VertexBuffer.h"


GameState_VertexBuffer::GameState_VertexBuffer()
{
}


GameState_VertexBuffer::~GameState_VertexBuffer()
{
}

void GameState_VertexBuffer::Initialize()
{
	if (grid == nullptr)
	{
		grid = new Grid;
		grid->Initialize();
	}

	if (cube == nullptr)
	{
		//cube = new Cube_VertexBuffer;
		//cube->Initialize(D3DXVECTOR3(1,1,1), D3DXVECTOR3(0,0,0));
	}

	if (cubeArray == nullptr)
	{
		cubeArray = new Cube_VertexBuffer[CUBE_COUNT_SQRT * CUBE_COUNT_SQRT];

		for (int i = 0; i < CUBE_COUNT_SQRT; ++i)
		{
			for (int j = 0; j < CUBE_COUNT_SQRT; ++j)
			{
				cubeArray[i * CUBE_COUNT_SQRT + j].Initialize(
					D3DXVECTOR3(0.9f, 0.9f, 0.9f),
					D3DXVECTOR3((float)i, 0.0f, (float)j)
					);
			}
		}
	}

	const int VERTEX_COUNT = 8;
	D3DXVECTOR3 vertex[VERTEX_COUNT];
	vertex[0] = D3DXVECTOR3(-0.5f, 0.0f, -0.5f);
	vertex[1] = D3DXVECTOR3(-0.5f, 1.0f, -0.5f);
	vertex[2] = D3DXVECTOR3(0.5f, 1.0f, -0.5f);
	vertex[3] = D3DXVECTOR3(0.5f, 0.0f, -0.5f);
	vertex[4] = D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
	vertex[5] = D3DXVECTOR3(-0.5f, 1.0f, 0.5f);
	vertex[6] = D3DXVECTOR3(0.5f, 1.0f, 0.5f);
	vertex[7] = D3DXVECTOR3(0.5f, 0.0f, 0.5f);

	FVF_PositionNormal fvfVertex[VERTEX_COUNT];
	fvfVertex[0] = FVF_PositionNormal(vertex[0], D3DXVECTOR3(-1.0f, -1.0f, -1.0f));
	fvfVertex[1] = FVF_PositionNormal(vertex[1], D3DXVECTOR3(-1.0f, 1.0f, -1.0f));
	fvfVertex[2] = FVF_PositionNormal(vertex[2], D3DXVECTOR3(1.0f, 1.0f, -1.0f));
	fvfVertex[3] = FVF_PositionNormal(vertex[3], D3DXVECTOR3(1.0f, -1.0f, -1.0f));
	fvfVertex[4] = FVF_PositionNormal(vertex[4], D3DXVECTOR3(-1.0f, -1.0f, 1.0f));
	fvfVertex[5] = FVF_PositionNormal(vertex[5], D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	fvfVertex[6] = FVF_PositionNormal(vertex[6], D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	fvfVertex[7] = FVF_PositionNormal(vertex[7], D3DXVECTOR3(1.0f, -1.0f, 1.0f));

	GameManager::GetDevice()->CreateVertexBuffer(
		sizeof(FVF_PositionNormal) * VERTEX_COUNT,
		0,
		FVF_PositionNormal::FVF,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		nullptr);

	FVF_PositionNormal* pV;
	vertexBuffer->Lock(0, 0, (void**)&pV, 0);
	memcpy_s(pV, VERTEX_COUNT * sizeof(FVF_PositionNormal),
		fvfVertex, VERTEX_COUNT * sizeof(FVF_PositionNormal));
	vertexBuffer->Unlock();

	GameManager::GetDevice()->CreateIndexBuffer(
		36 * sizeof(DWORD),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&indexBuffer,
		nullptr
		);

	DWORD index[36];
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 0; index[4] = 2; index[5] = 3;

	index[6] = 4; index[7] = 5; index[8] = 1;
	index[9] = 4; index[10] = 1; index[11] = 0;

	index[12] = 7; index[13] = 6; index[14] = 5;
	index[15] = 7; index[16] = 5; index[17] = 4;

	index[18] = 3; index[19] = 2; index[20] = 6;
	index[21] = 3; index[22] = 6; index[23] = 7;

	index[24] = 1; index[25] = 5; index[26] = 6;
	index[27] = 1; index[28] = 6; index[29] = 2;

	index[30] = 4; index[31] = 0; index[32] = 3;
	index[33] = 4; index[34] = 3; index[35] = 7;

	LPVOID pI;
	indexBuffer->Lock(0, 0, &pI, 0);
	memcpy_s(pI, 36 * sizeof(DWORD), index, 36 * sizeof(DWORD));
	indexBuffer->Unlock();

	D3DXFONT_DESC font_desc;
	ZeroMemory(&font_desc, sizeof(D3DXFONT_DESC));
	font_desc.Height = 50;
	font_desc.Width = 25;
	font_desc.Weight = FW_NORMAL;
	font_desc.MipLevels = D3DX_DEFAULT;
	font_desc.Italic = false;
	font_desc.CharSet = DEFAULT_CHARSET;
	font_desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	font_desc.Quality = DEFAULT_QUALITY;
	font_desc.PitchAndFamily = FF_DONTCARE;
	wcscpy_s(font_desc.FaceName, L"휴먼편지체");	//글꼴 스타일
	//AddFontResource(L"./Data/Font/umberto.ttf");
	//wcscpy_s(font_desc.FaceName, L"umberto");

	D3DXCreateFontIndirect(
		GameManager::GetDevice(),
		&font_desc,
		&font);

	printf_s("Current Render Mode: %d\n", currentRenderMode);

	Reset();
}

void GameState_VertexBuffer::Destroy()
{
	SAFE_RELEASE(font);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	if (cubeArray)
	{
		int count = CUBE_COUNT_SQRT*CUBE_COUNT_SQRT;
		for (int i = 0; i < count; ++i)
		{
			cubeArray[i].Destroy();
		}
		delete[] cubeArray;
		cubeArray = nullptr;
	}


	SAFE_DELETE(cube);
	SAFE_DELETE(grid);
}

void GameState_VertexBuffer::Reset()
{
}

void GameState_VertexBuffer::Update()
{
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
	{
		bool wasPressed = keyState[UP];
		if (wasPressed == false)
		{
			int modeCount = static_cast<int>(currentRenderMode);
			modeCount++;
			currentRenderMode = static_cast<RenderMode>(modeCount % NUM_OF_RENDER_MODE);
			printf_s("Current Render Mode: %d\n", currentRenderMode);
		}
		keyState[UP] = true;
	}

	else
	{
		keyState[UP] = false;
	}
}

void GameState_VertexBuffer::Render()
{
	if (grid)
	{
		grid->Render();
	}

	/*if (cube)
	{
		cube->Render();
	}*/

	GameManager::GetDevice()->SetStreamSource(
		0, vertexBuffer, 0, sizeof(FVF_PositionNormal));
	GameManager::GetDevice()->SetIndices(indexBuffer);
	int count = CUBE_COUNT_SQRT*CUBE_COUNT_SQRT;

	if (cubeArray && (currentRenderMode == NORMAL))
	{
		for (int i = 0; i < count; ++i)
		{
			cubeArray[i].Render();
		}
	}

	if (cubeArray && (currentRenderMode == VB))
	{
		for (int i = 0; i < count; ++i)
		{
			cubeArray[i].RenderVB();
		}
	}

	if (cubeArray && (currentRenderMode == VB_SHARED))
	{
		for (int i = 0; i < count; ++i)
		{
			cubeArray[i].RenderShared();
		}
	}

	if (font)
	{
		RECT rc = { 450, 10, 11, 11 };
		char buff[256] = "\0";

		switch (currentRenderMode)
		{
		case NORMAL:
			sprintf_s(buff, "Render Mode : Normal");
			break;

		case VB:
			sprintf_s(buff, "Render Mode : Vertex Buffer");
			break;

		case VB_SHARED:
			sprintf_s(buff, "Render Mode : Vertex Buffer Shared");
			break;

		default:
			break;
		}

		font->DrawTextA(
			NULL,
			LPCSTR(buff),
			strlen(buff),
			&rc,
			DT_TOP | DT_LEFT | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 255)
			);
	}
}

void GameState_VertexBuffer::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(20.0f);

	Initialize();
}

void GameState_VertexBuffer::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_VertexBuffer::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	return result;
}
