////////////////////////////////////////////////////////////////////////////////
//
// File: virtualLego.cpp
//
// Original Author: 박창현 Chang-hyeon Park, 
// Modified by Bong-Soo Sohn and Dong-Jun Kim
// 
// Originally programmed for Virtual LEGO. 
// Modified later to program for Virtual Billiard.
//        
////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <d3dx9.h> // hud 를 위해 추가
#include <fstream> // 파일 입출력을 위해 추가
#include <algorithm> // std::sort를 위해 추가
#include <windows.h>

IDirect3DDevice9* Device = NULL;
ID3DXFont* hud_Font = NULL;



// 시작화면, 종료화면
ID3DXSprite* start_Sprite = NULL;
IDirect3DTexture9* start_Texture = NULL;
ID3DXSprite* rank_Sprite = NULL;
IDirect3DTexture9* rank_Texture = NULL;
D3DXVECTOR3 spritePosition;

// 푸앙이 색깔
D3DXCOLOR color_puang1(107 / 255.0f, 190 / 255.0f, 226 / 255.0f, 1.0f);
D3DXCOLOR color_puang2(20 / 255.0f, 51 / 255.0f, 101 / 255.0f, 1.0f);







// window size
const int Width  = 1024; 
const int Height = 1024;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)
const float spherePos[4][2] = { {-2.7f,0} , {+2.4f,0} , {3.3f,2.0f} , {-2.7f,-0.9f}};     //(짧은쪽, 긴쪽)
// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[4] = {d3d::RED, d3d::RED, d3d::YELLOW, d3d::WHITE};

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 0.9982










// -----------------------------------------------------------------------------
// 랭킹을 위한 구조체 함수 등등듣읃읃으
// -----------------------------------------------------------------------------
struct Ranking {
	int stage;
	int score;

	// 정렬
	bool operator < (const Ranking& r) const {
		if (stage == r.stage) return score > r.score;
		return stage > r.stage;
	}
};



std::vector<Ranking> g_rankings;
Ranking new_ranking = {0, 0};
int ranked = 10;


std::vector<Ranking> ReadRankings(const std::string& filename) {
	std::vector<Ranking> rankings;
	std::ifstream file(filename);

	if (!file.is_open()) return rankings;

	Ranking r;
	while (file >> r.stage >> r.score) {
		rankings.push_back(r);
	}

	file.close();
	return rankings;
}

// 파일에 저장
void WriteRankings(const std::string& filename, const std::vector<Ranking>& rankings) {
	std::ofstream file(filename);

	if (!file.is_open()) return;

	for (const auto& r : rankings) {
		file << r.stage << " " << r.score << std::endl;
	}

	file.close();
}


void UpdateRankings(const Ranking& new_rank) {
	g_rankings.push_back(new_rank);
	std::sort(g_rankings.begin(), g_rankings.end());

	if (g_rankings.size() > 10) {
		g_rankings.resize(10);
	}

	for (int i = 0; i < g_rankings.size(); i++) {
		if (g_rankings[i].stage == new_rank.stage && g_rankings[i].score == new_rank.score) {
			ranked = i;
			break;
		}
	}

	WriteRankings("rank.txt", g_rankings);
}

void DisplayRankings(ID3DXFont* font, const std::vector<Ranking>& rankings) {
	RECT rc;
	SetRect(&rc, 270, 250, 0, 0); // 위치

	

	for (size_t i = 0; i < 10; ++i) {
		char str[50];
		if (i < rankings.size()) {
			sprintf_s(str, "%02d               %d              %03d", i + 1, rankings[i].stage, rankings[i].score);
		}
		else {
			sprintf_s(str, "%02d               0              000", i + 1);
		}

		D3DCOLOR color;
		if (ranked != 10 && i == ranked) {
			color = d3d::YELLOW;
		}
		else {
			color = D3DCOLOR_XRGB(255, 255, 255);
		}

		font->DrawText(NULL, str, -1, &rc, DT_NOCLIP, color);
		rc.top += 50; // 줄바꿈 너비
	}
}


// -----------------------------------------------------------------------------
// 화면 전환효과
// -----------------------------------------------------------------------------
void eff_out() {
	static int frame = 0;
	const int totalFrames = 115; // Total duration of the animation
	const int peakFrame = 30; // Frame at which the slide reaches its lowest point

	if (frame < totalFrames) {
		float progress = static_cast<float>(frame) / totalFrames;
		float phaseProgress;

		if (frame < peakFrame) {
			// 하강
			phaseProgress = progress / (peakFrame / static_cast<float>(totalFrames));
			spritePosition.y += 20 * phaseProgress * phaseProgress;
		}
		else {
			// 상승
			phaseProgress = (progress - 0.5f) * 2; 
			spritePosition.y -= 20 * (1 - phaseProgress) * (1 - phaseProgress);
		}

		frame++;
		Sleep(5); 
	}
	else {
		frame = 0; 
	}
}



void eff_in() {
	static int frame = 0;
	const int totalFrames = 115; // Total duration of the animation

	const int startY = -1024;    // Starting position (top)
	const int endY = 0;          // Ending position

	if (frame < totalFrames) {
		float progress = static_cast<float>(frame) / totalFrames;

		progress--;
		float cubicProgress = progress * progress * progress + 1;

		spritePosition.y = startY + (endY - startY) * cubicProgress;

		frame++;
		Sleep(5);
	}
	else {
		frame = 0; // Reset for the next call
	}
}




// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private :
	float					center_x, center_y, center_z;   // w중심좌표
    float                   m_radius;
	float					m_velocity_x;                  // x방향 속도
	float					m_velocity_z;		           // z방향 속도

public:
    CSphere(void)
    {
        D3DXMatrixIdentity(&m_mLocal); // 단위행렬
        ZeroMemory(&m_mtrl, sizeof(m_mtrl)); // 0으로 초기화
        m_radius = 0;
		m_velocity_x = 0;
		m_velocity_z = 0;
        m_pSphereMesh = NULL; 
    }
    ~CSphere(void) {}

public:
    bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE)
    {
        if (NULL == pDevice)
            return false;
		
        m_mtrl.Ambient  = color;      // 주변광
        m_mtrl.Diffuse  = color;      // 확산광	
        m_mtrl.Specular = color;      // 반사광
        m_mtrl.Emissive = d3d::BLACK; // 자체광
        m_mtrl.Power    = 5.0f;	      // 광택 
		
        if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL))) // 구형 메쉬 생성(3차원면)
            return false; 
        return true;
    }
	
    void destroy(void)
    {
        if (m_pSphereMesh != NULL) {
            m_pSphereMesh->Release();
            m_pSphereMesh = NULL;
        }
    }

    void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return;
        pDevice->SetTransform(D3DTS_WORLD, &mWorld); 
        pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal); 
        pDevice->SetMaterial(&m_mtrl);
		m_pSphereMesh->DrawSubset(0);
    }
	
    bool hasIntersected(CSphere& ball) 
	{
		// Insert your code here.

		return false;
	}
	
	void hitBy(CSphere& ball) 
	{ 
		// Insert your code here.
	}

	void ballUpdate(float timeDiff) 
	{
		const float TIME_SCALE = 3.3;
		D3DXVECTOR3 cord = this->getCenter();
		double vx = abs(this->getVelocity_X());
		double vz = abs(this->getVelocity_Z());

		if(vx > 0.01 || vz > 0.01)
		{
			float tX = cord.x + TIME_SCALE*timeDiff*m_velocity_x;
			float tZ = cord.z + TIME_SCALE*timeDiff*m_velocity_z;

			//correction of position of ball
			// Please uncomment this part because this correction of ball position is necessary when a ball collides with a wall
			/*if(tX >= (4.5 - M_RADIUS))
				tX = 4.5 - M_RADIUS;
			else if(tX <=(-4.5 + M_RADIUS))
				tX = -4.5 + M_RADIUS;
			else if(tZ <= (-3 + M_RADIUS))
				tZ = -3 + M_RADIUS;
			else if(tZ >= (3 - M_RADIUS))
				tZ = 3 - M_RADIUS;*/
			
			this->setCenter(tX, cord.y, tZ);
		}
		else { this->setPower(0,0);}
		//this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
		double rate = 1 -  (1 - DECREASE_RATE)*timeDiff * 400;
		if(rate < 0 )
			rate = 0;
		this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
	}

	double getVelocity_X() { return this->m_velocity_x;	}
	double getVelocity_Z() { return this->m_velocity_z; }

	void setPower(double vx, double vz)
	{
		this->m_velocity_x = vx;
		this->m_velocity_z = vz;
	}

	void setCenter(float x, float y, float z)
	{
		D3DXMATRIX m;
		center_x=x;	center_y=y;	center_z=z;
		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
	float getRadius(void)  const { return (float)(M_RADIUS);  }
    const D3DXMATRIX& getLocalTransform(void) const { return m_mLocal; }
    void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
    D3DXVECTOR3 getCenter(void) const
    {
        D3DXVECTOR3 org(center_x, center_y, center_z);
        return org;
    }
	
private:
    D3DXMATRIX              m_mLocal;
    D3DMATERIAL9            m_mtrl;
    ID3DXMesh*              m_pSphereMesh;
	
};








// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------

class CWall {

private:
	
    float					m_x;
	float					m_z;
	float                   m_width;
    float                   m_depth;
	float					m_height;

	
public:
    CWall(void)
    {
        D3DXMatrixIdentity(&m_mLocal);
        ZeroMemory(&m_mtrl, sizeof(m_mtrl));
        m_width = 0;
        m_depth = 0;
        m_pBoundMesh = NULL;
    }
    ~CWall(void) {}
public:
    bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
    {
        if (NULL == pDevice)
            return false;
		
        m_mtrl.Ambient  = color;
        m_mtrl.Diffuse  = color;
        m_mtrl.Specular = color;
        m_mtrl.Emissive = d3d::BLACK;
        m_mtrl.Power    = 5.0f;
		
        m_width = iwidth;
        m_depth = idepth;
		
        if (FAILED(D3DXCreateBox(pDevice, iwidth, iheight, idepth, &m_pBoundMesh, NULL)))
            return false;
        return true;
    }
    void destroy(void)
    {
        if (m_pBoundMesh != NULL) {
            m_pBoundMesh->Release();
            m_pBoundMesh = NULL;
        }

    }
    void draw(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return;
        pDevice->SetTransform(D3DTS_WORLD, &mWorld);
        pDevice->MultiplyTransform(D3DTS_WORLD, &m_mLocal);
		pDevice->SetMaterial(&m_mtrl);
		m_pBoundMesh->DrawSubset(0);
    }
	
	bool hasIntersected(CSphere& ball) 
	{
		// Insert your code here.
		return false;
	}

	void hitBy(CSphere& ball) 
	{
		// Insert your code here.
	}    
	
	void setPosition(float x, float y, float z)
	{
		D3DXMATRIX m;
		this->m_x = x;
		this->m_z = z;

		D3DXMatrixTranslation(&m, x, y, z);
		setLocalTransform(m);
	}
	
    float getHeight(void) const { return M_HEIGHT; }


	
	
private :
    void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
	
	D3DXMATRIX              m_mLocal;
    D3DMATERIAL9            m_mtrl;
    ID3DXMesh*              m_pBoundMesh;
};










// -----------------------------------------------------------------------------
// CLight class definition
// -----------------------------------------------------------------------------

class CLight {
public:
    CLight(void)
    {
        static DWORD i = 0;
        m_index = i++;
        D3DXMatrixIdentity(&m_mLocal);
        ::ZeroMemory(&m_lit, sizeof(m_lit));
        m_pMesh = NULL;
        m_bound._center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_bound._radius = 0.0f;
    }
    ~CLight(void) {}
public:
    bool create(IDirect3DDevice9* pDevice, const D3DLIGHT9& lit, float radius = 0.1f)
    {
        if (NULL == pDevice)
            return false;
        if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))  // (1번, 반지름, 경도, 위도, 메쉬, NULL)
            return false;
		
        m_bound._center = lit.Position;
        m_bound._radius = radius;
		
        m_lit.Type          = lit.Type;
        m_lit.Diffuse       = lit.Diffuse;
        m_lit.Specular      = lit.Specular;
        m_lit.Ambient       = lit.Ambient;
        m_lit.Position      = lit.Position;
        m_lit.Direction     = lit.Direction;
        m_lit.Range         = lit.Range;
        m_lit.Falloff       = lit.Falloff;
        m_lit.Attenuation0  = lit.Attenuation0;
        m_lit.Attenuation1  = lit.Attenuation1;
        m_lit.Attenuation2  = lit.Attenuation2;
        m_lit.Theta         = lit.Theta;
        m_lit.Phi           = lit.Phi;
        return true;
    }
    void destroy(void)
    {
        if (m_pMesh != NULL) {
            m_pMesh->Release();
            m_pMesh = NULL;
        }
    }
    bool setLight(IDirect3DDevice9* pDevice, const D3DXMATRIX& mWorld)
    {
        if (NULL == pDevice)
            return false;
		
        D3DXVECTOR3 pos(m_bound._center);
        D3DXVec3TransformCoord(&pos, &pos, &m_mLocal);
        D3DXVec3TransformCoord(&pos, &pos, &mWorld);
        m_lit.Position = pos;
		
        pDevice->SetLight(m_index, &m_lit);
        pDevice->LightEnable(m_index, TRUE);
        return true;
    }

    void draw(IDirect3DDevice9* pDevice)
    {
        if (NULL == pDevice)
            return;
        D3DXMATRIX m;
        D3DXMatrixTranslation(&m, m_lit.Position.x, m_lit.Position.y, m_lit.Position.z);
        pDevice->SetTransform(D3DTS_WORLD, &m);
        pDevice->SetMaterial(&d3d::WHITE_MTRL);
        m_pMesh->DrawSubset(0);
    }

    D3DXVECTOR3 getPosition(void) const { return D3DXVECTOR3(m_lit.Position); }

private:
    DWORD               m_index;
    D3DXMATRIX          m_mLocal;
    D3DLIGHT9           m_lit;
    ID3DXMesh*          m_pMesh;
    d3d::BoundingSphere m_bound;
};


// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall	g_legoPlane;  
CWall	g_legowall[4];
CSphere	g_sphere[4];
CSphere	g_target_blueball;
CLight	g_light;

double g_camera_pos[3] = {0.0, 5.0, -8.0};


int g_stage = 1;
int g_life = 5;
int g_score = 0;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
	// Insert your code here.

}

// initialization
bool Setup()
{
	// HUD를 위한 폰트 생성 ***************************************************************************************
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = 50;    // in logical units
	fontDesc.Width = 0;     // default width
	fontDesc.Weight = FW_BOLD;
	fontDesc.MipLevels = 1;     // no mip-mapping
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(fontDesc.FaceName, "Arial");

	D3DXCreateFontIndirect(Device, &fontDesc, &hud_Font);


	if (FAILED(D3DXCreateTextureFromFile(Device, "start_puang.png", &start_Texture))) {
		return "Error: Failed to load puang.";
	}
	if (FAILED(D3DXCreateTextureFromFile(Device, "rank_puang.png", &rank_Texture))) {
		return "Error: Failed to load rank.";
	}
	D3DXCreateSprite(Device, &rank_Sprite);
	D3DXCreateSprite(Device, &start_Sprite); // 스프라이트 생성*****************************************************************************************************	
	spritePosition.x = 0.0f;
	spritePosition.y = 0.0f;

	g_rankings = ReadRankings("rank.txt");

	int i;
	
    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);
		
	// create plane and set the position
    if (false == g_legoPlane.create(Device, -1, -1, 9, 0.00f, 6, color_puang1)) return false;
    g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);
	
	// create walls and set the position. note that there are four walls
	if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, color_puang2)) return false;
	g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
	if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, color_puang2)) return false;
	g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
	if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, color_puang2)) return false;
	g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
	if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, color_puang2)) return false;
	g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

	// create four balls and set the position
	for (i=0;i<4;i++) {
		if (false == g_sphere[i].create(Device, sphereColor[i])) return false;
		g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS , spherePos[i][1]);
		g_sphere[i].setPower(0,0);
	}
	
	// create blue ball for set direction
    if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
	g_target_blueball.setCenter(.0f, (float)M_RADIUS , .0f);
	
	// light setting 
    D3DLIGHT9 lit;
    ::ZeroMemory(&lit, sizeof(lit));
    lit.Type         = D3DLIGHT_POINT;
    lit.Diffuse      = d3d::WHITE; 
	lit.Specular     = d3d::WHITE * 0.9f;
    lit.Ambient      = d3d::WHITE * 0.9f;
    lit.Position     = D3DXVECTOR3(0.0f, 11.0f, 0.0f);  // 빛의 중심 하얀점 사라지게****************************
    lit.Range        = 100.0f;
    lit.Attenuation0 = 0.0f;
    lit.Attenuation1 = 0.2f;  // 밝기 조절**************************************************************************
    lit.Attenuation2 = 0.0f;
    if (false == g_light.create(Device, lit))
        return false;
	
	// Position and aim the camera.**************************************************************
	D3DXVECTOR3 pos(7.0f, 10.0f, 0.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 2.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_mView, &pos, &target, &up);
	Device->SetTransform(D3DTS_VIEW, &g_mView);
	
	// Set the projection matrix.
	D3DXMatrixPerspectiveFovLH(&g_mProj, D3DX_PI / 4,
        (float)Width / (float)Height, 1.0f, 100.0f);
	Device->SetTransform(D3DTS_PROJECTION, &g_mProj);
	
    // Set render states.
    Device->SetRenderState(D3DRS_LIGHTING, TRUE);
    Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	
	g_light.setLight(Device, g_mWorld);
	return true;
}

void Cleanup(void)
{
    g_legoPlane.destroy();
	for(int i = 0 ; i < 4; i++) {
		g_legowall[i].destroy();
	}

	if (hud_Font) {
		hud_Font->Release();
		hud_Font = NULL;
	}
	if (start_Texture) {
		start_Texture->Release();
		start_Texture = NULL;
	}
	if (rank_Sprite) {
		rank_Sprite->Release();
		rank_Sprite = NULL;
	}

    destroyAllLegoBlock();
    g_light.destroy();
}


// timeDelta represents the time between the current image frame and the last image frame.
// the distance of moving balls should be "velocity * timeDelta"
bool Display(float timeDelta)
{
	int i=0;
	int j = 0;


	if( Device )
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();
		
		// update the position of each ball. during update, check whether each ball hit by walls.
		for( i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for(j = 0; j < 4; j++){ g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for(i = 0 ;i < 4; i++){
			for(j = 0 ; j < 4; j++) {
				if(i >= j) {continue;}
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i=0;i<4;i++) 	{
			g_legowall[i].draw(Device, g_mWorld);
			g_sphere[i].draw(Device, g_mWorld);
		}
		g_target_blueball.draw(Device, g_mWorld);
        g_light.draw(Device);


		// HUD를 위한 텍스트 출력 ************************************************************************************
		
		

		// Convert score to a string
		TCHAR str[50];
		sprintf_s(str, "Stage: %d     Score: %03d\nLife:    %d", g_stage, g_score, g_life);

		// Define the rectangle where the text will be displayed
		RECT rc;
		SetRect(&rc, 50, 50, 0, 0); // x, y, width, height

		// Draw the text
		hud_Font->DrawText(NULL, str, -1, &rc, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));


		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture( 0, NULL );
	}
	return true;
}


bool Display_rank(float timeDelta)
{
	int i = 0;
	int j = 0;


	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();

		// update the position of each ball. during update, check whether each ball hit by walls.
		for (i = 0; i < 4; i++) {
			g_sphere[i].ballUpdate(timeDelta);
			for (j = 0; j < 4; j++) { g_legowall[i].hitBy(g_sphere[j]); }
		}

		// check whether any two balls hit together and update the direction of balls
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (i >= j) { continue; }
				g_sphere[i].hitBy(g_sphere[j]);
			}
		}

		// draw plane, walls, and spheres
		g_legoPlane.draw(Device, g_mWorld);
		for (i = 0; i < 4; i++) {
			g_legowall[i].draw(Device, g_mWorld);
			g_sphere[i].draw(Device, g_mWorld);
		}
		g_target_blueball.draw(Device, g_mWorld);
		g_light.draw(Device);




		//**********************************************************************************************************
		if (rank_Sprite && rank_Texture) {
			rank_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			rank_Sprite->Draw(rank_Texture, NULL, NULL, &spritePosition, D3DCOLOR_XRGB(255, 255, 255));
			rank_Sprite->End();
		}

		// 랭킹 업데이트
		UpdateRankings(new_ranking);
		DisplayRankings(hud_Font, g_rankings);
		


		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
	}
	return true;
}


int loopCounter = 0;
const int maxLoops = 115;

bool Display_eff(float timeDelta)
{
	if (loopCounter >= maxLoops) {
		// Implement a way to exit the loop or close the application
		// For example, PostQuitMessage(0);
		return true;
	}
	loopCounter++;

	int i = 0;
	int j = 0;
	static int k = 0;
	

	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();


		//**********************************************************************************************************
		if (rank_Sprite && rank_Texture) {
			rank_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			rank_Sprite->Draw(rank_Texture, NULL, NULL, &spritePosition, D3DCOLOR_XRGB(255, 255, 255));
			rank_Sprite->End();
		}
		UpdateRankings(new_ranking);
		DisplayRankings(hud_Font, g_rankings);// update the rankings 아직 추가안함!!!!!!!!!!!!!!!!!!!!!!!!

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);
		
		eff_out();
	}
	return true;
}

int loopCounter00 = 0;
const int maxLoops00 = 115;

bool Display_start(float timeDelta)
{
	if (loopCounter00 >= maxLoops00) {
		// Implement a way to exit the loop or close the application
		// For example, PostQuitMessage(0);
		return true;
	}
	loopCounter00++;


	int i = 0;
	int j = 0;
	static int k = 0;


	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
		Device->BeginScene();


		//**********************************************************************************************************
		if (start_Sprite && start_Texture) {
			start_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			start_Sprite->Draw(start_Texture, NULL, NULL, &spritePosition, D3DCOLOR_XRGB(255, 255, 255));
			start_Sprite->End();
		}
		

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		Device->SetTexture(0, NULL);

		eff_in();
	}
	return true;
}



LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool wire = false;
	static bool isReset = true;
    static int old_x = 0;
    static int old_y = 0;
    static enum { WORLD_MOVE, LIGHT_MOVE, BLOCK_MOVE } move = WORLD_MOVE;
	
	switch( msg ) {
	case WM_DESTROY:
        {
			::PostQuitMessage(0);
			break;
        }
	case WM_KEYDOWN:
        {
            switch (wParam) {
            case VK_ESCAPE:
				::DestroyWindow(hwnd);
                break;
            case VK_RETURN:
                if (NULL != Device) {
                    wire = !wire;
                    Device->SetRenderState(D3DRS_FILLMODE,
                        (wire ? D3DFILL_WIREFRAME : D3DFILL_SOLID));
                }
                break;
            case VK_SPACE:
				
				D3DXVECTOR3 targetpos = g_target_blueball.getCenter();
				D3DXVECTOR3	whitepos = g_sphere[3].getCenter();
				double theta = acos(sqrt(pow(targetpos.x - whitepos.x, 2)) / sqrt(pow(targetpos.x - whitepos.x, 2) +
					pow(targetpos.z - whitepos.z, 2)));		// 기본 1 사분면
				if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x >= 0) { theta = -theta; }	//4 사분면
				if (targetpos.z - whitepos.z >= 0 && targetpos.x - whitepos.x <= 0) { theta = PI - theta; } //2 사분면
				if (targetpos.z - whitepos.z <= 0 && targetpos.x - whitepos.x <= 0){ theta = PI + theta; } // 3 사분면
				double distance = sqrt(pow(targetpos.x - whitepos.x, 2) + pow(targetpos.z - whitepos.z, 2));
				g_sphere[3].setPower(distance * cos(theta), distance * sin(theta));

				break;

			}
			break;
        }
		
	/********************* 마우스로 시점 변경 못하게 시점 고정****************************************************
	case WM_MOUSEMOVE:
        {
            int new_x = LOWORD(lParam);
            int new_y = HIWORD(lParam);
			float dx;
			float dy;
			
            if (LOWORD(wParam) & MK_LBUTTON) {
				
                if (isReset) {
                    isReset = false;
                } else {
                    D3DXVECTOR3 vDist;
                    D3DXVECTOR3 vTrans;
                    D3DXMATRIX mTrans;
                    D3DXMATRIX mX;
                    D3DXMATRIX mY;
					
                    switch (move) {
                    case WORLD_MOVE:
                        dx = (old_x - new_x) * 0.01f;
                        dy = (old_y - new_y) * 0.01f;
                        D3DXMatrixRotationY(&mX, dx);
                        D3DXMatrixRotationX(&mY, dy);
                        g_mWorld = g_mWorld * mX * mY;
						
                        break;
                    }
                }
				
                old_x = new_x;
                old_y = new_y;

            } else {
                isReset = true;
				
				if (LOWORD(wParam) & MK_RBUTTON) {
					dx = (old_x - new_x);// * 0.01f;
					dy = (old_y - new_y);// * 0.01f;
		
					D3DXVECTOR3 coord3d=g_target_blueball.getCenter();
					g_target_blueball.setCenter(coord3d.x+dx*(-0.007f),coord3d.y,coord3d.z+dy*0.007f );
				}
				old_x = new_x;
				old_y = new_y;
				
                move = WORLD_MOVE;
            }
            break;
        }*/
	}
	
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
    srand(static_cast<unsigned int>(time(NULL)));
	
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
	
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}
	

	d3d::EnterMsgLoop( Display );

	
	Cleanup();
	
	Device->Release();
	
	return 0;
}