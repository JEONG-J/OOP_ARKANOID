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
#include <windows.h> // 딜레이위해 추가

IDirect3DDevice9* Device = NULL;



// -----------------------------------------------------------------------------
// 추가추가추가추가
// -----------------------------------------------------------------------------

ID3DXFont* hud_Font = NULL;        
// 시작화면, 종료화면
ID3DXSprite* start_Sprite = NULL;
IDirect3DTexture9* start_Texture = NULL;
D3DXVECTOR3 spritePos_start;
ID3DXSprite* rank_Sprite = NULL;
IDirect3DTexture9* rank_Texture = NULL;
D3DXVECTOR3 spritePos_rank;

// 푸앙이 색깔
D3DXCOLOR color_puang1(107 / 255.0f, 190 / 255.0f, 226 / 255.0f, 1.0f);
D3DXCOLOR color_puang2(20 / 255.0f, 51 / 255.0f, 101 / 255.0f, 1.0f);
// -----------------------------------------------------------------------------
// 추가추가추가추가
// -----------------------------------------------------------------------------





// window size
const int Width = 1024;
const int Height = 1024;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)

/*
const float spherePos[64][2] = {

};
*/

const float spherePos1[64][2] = {
     {-1.75f, 1.75f}, {-1.25f, 1.75f}, {-0.75f, 1.75f}, {-0.25f, 1.75f}, {0.25f, 1.75f}, {0.75f, 1.75f}, {1.25f, 1.75f}, {1.75f, 1.75f},
    {-1.75f, 1.25f}, {-1.25f, 1.25f}, {-0.75f, 1.25f}, {-0.25f, 1.25f}, {0.25f, 1.25f}, {0.75f, 1.25f}, {1.25f, 1.25f}, {1.75f, 1.25f},
    {-1.75f, 0.75f}, {-1.25f, 0.75f}, {-0.75f, 0.75f}, {-0.25f, 0.75f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {1.25f, 0.75f}, {1.75f, 0.75f},
    {-1.75f, 0.25f}, {-1.25f, 0.25f}, {-0.75f, 0.25f}, {-0.25f, 0.25f}, {0.25f, 0.25f}, {0.75f, 0.25f}, {1.25f, 0.25f}, {1.75f, 0.25f},
    {-1.75f, -0.25f}, {-1.25f, -0.25f}, {-0.75f, -0.25f}, {-0.25f, -0.25f}, {0.25f, -0.25f}, {0.75f, -0.25f}, {1.25f, -0.25f}, {1.75f, -0.25f},
    {-1.75f, -0.75f}, {-1.25f, -0.75f}, {-0.75f, -0.75f}, {-0.25f, -0.75f}, {0.25f, -0.75f}, {0.75f, -0.75f}, {1.25f, -0.75f}, {1.75f, -0.75f},
    {-1.75f, -1.25f}, {-1.25f, -1.25f}, {-0.75f, -1.25f}, {-0.25f, -1.25f}, {0.25f, -1.25f}, {0.75f, -1.25f}, {1.25f, -1.25f}, {1.75f, -1.25f},
    {-1.75f, -1.75f}, {-1.25f, -1.75f}, {-0.75f, -1.75f}, {-0.25f, -1.75f}, {0.25f, -1.75f}, {0.75f, -1.75f}, {1.25f, -1.75f}, {1.75f, -1.75f}

};



//공모양
const float spherePos2[64][2] = { {3.2f, 0.0f}, {3.1841f, 0.2788f}, {3.1366f, 0.5548f}, {3.0578f, 0.8253f},
{2.9487f, 1.0876f}, {2.8103f, 1.3391f}, {2.6440f, 1.5773f}, {2.4513f, 1.7998f},
{2.2344f, 2.0044f}, {1.9952f, 2.1891f}, {1.7361f, 2.3521f}, {1.4599f, 2.4916f},
{1.1691f, 2.6064f}, {0.8667f, 2.6953f}, {0.5557f, 2.7575f}, {0.2391f, 2.7922f},
{-0.0798f, 2.7991f}, {-0.3979f, 2.7783f}, {-0.7121f, 2.7298f}, {-1.0192f, 2.6542f},
{-1.3161f, 2.5522f}, {-1.6f, 2.4249f}, {-1.8680f, 2.2734f}, {-2.1174f, 2.0994f},
{-2.3458f, 1.9045f}, {-2.5508f, 1.6907f}, {-2.7305f, 1.46f}, {-2.8831f, 1.2149f},
{-3.0070f, 0.9577f}, {-3.1010f, 0.6909f}, {-3.1643f, 0.4173f}, {-3.1960f, 0.1396f},
{-3.1960f, -0.1396f}, {-3.1643f, -0.4173f}, {-3.1010f, -0.6909f}, {-3.0070f, -0.9577f},
{-2.8831f, -1.2149f}, {-2.7305f, -1.46f}, {-2.5508f, -1.6907f}, {-2.3458f, -1.9045f},
{-2.1174f, -2.0994f}, {-1.8680f, -2.2734f}, {-1.6f, -2.4249f}, {-1.3161f, -2.5522f},
{-1.0192f, -2.6542f}, {-0.7121f, -2.7298f}, {-0.3979f, -2.7783f}, {-0.0798f, -2.7991f},
{0.2391f, -2.7922f}, {0.5557f, -2.7575f}, {0.8667f, -2.6953f}, {1.1691f, -2.6064f},
{1.4599f, -2.4916f}, {1.7361f, -2.3521f}, {1.9952f, -2.1891f}, {2.2344f, -2.0044f},
{2.4513f, -1.7998f}, {2.6440f, -1.5773f}, {2.8103f, -1.3391f}, {2.9487f, -1.0876f},
{3.0578f, -0.8253f}, {3.1366f, -0.5548f}, {3.1841f, -0.2788f}, {3.2f, -0.0f} }
;


//사각형
const float spherePos3[64][2] = {
    {-1.75f, 1.75f}, {-1.25f, 1.75f}, {-0.75f, 1.75f}, {-0.25f, 1.75f}, {0.25f, 1.75f}, {0.75f, 1.75f}, {1.25f, 1.75f}, {1.75f, 1.75f},
    {-1.75f, 1.25f}, {-1.25f, 1.25f}, {-0.75f, 1.25f}, {-0.25f, 1.25f}, {0.25f, 1.25f}, {0.75f, 1.25f}, {1.25f, 1.25f}, {1.75f, 1.25f},
    {-1.75f, 0.75f}, {-1.25f, 0.75f}, {-0.75f, 0.75f}, {-0.25f, 0.75f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {1.25f, 0.75f}, {1.75f, 0.75f},
    {-1.75f, 0.25f}, {-1.25f, 0.25f}, {-0.75f, 0.25f}, {-0.25f, 0.25f}, {0.25f, 0.25f}, {0.75f, 0.25f}, {1.25f, 0.25f}, {1.75f, 0.25f},
    {-1.75f, -0.25f}, {-1.25f, -0.25f}, {-0.75f, -0.25f}, {-0.25f, -0.25f}, {0.25f, -0.25f}, {0.75f, -0.25f}, {1.25f, -0.25f}, {1.75f, -0.25f},
    {-1.75f, -0.75f}, {-1.25f, -0.75f}, {-0.75f, -0.75f}, {-0.25f, -0.75f}, {0.25f, -0.75f}, {0.75f, -0.75f}, {1.25f, -0.75f}, {1.75f, -0.75f},
    {-1.75f, -1.25f}, {-1.25f, -1.25f}, {-0.75f, -1.25f}, {-0.25f, -1.25f}, {0.25f, -1.25f}, {0.75f, -1.25f}, {1.25f, -1.25f}, {1.75f, -1.25f},
    {-1.75f, -1.75f}, {-1.25f, -1.75f}, {-0.75f, -1.75f}, {-0.25f, -1.75f}, {0.25f, -1.75f}, {0.75f, -1.75f}, {1.25f, -1.75f}, {1.75f, -1.75f}
};



// initialize the color of each ball (ball0 ~ ball3)
const D3DXCOLOR sphereColor[2] = {d3d::YELLOW, d3d::WHITE};

// -----------------------------------------------------------------------------
// Transform matrices
// -----------------------------------------------------------------------------
D3DXMATRIX g_mWorld;
D3DXMATRIX g_mView;
D3DXMATRIX g_mProj;

#define M_RADIUS 0.21   // ball radius
#define PI 3.14159265
#define M_HEIGHT 0.01
#define DECREASE_RATE 1.0

// -----------------------------------------------------------------------------
// CSphere class definition
// -----------------------------------------------------------------------------

class CSphere {
private:
    float               center_x, center_y, center_z;
    float                   m_radius;
    float               m_velocity_x;
    float               m_velocity_z;

public:
    CSphere(void) //공 클래스
    {
        D3DXMatrixIdentity(&m_mLocal);
        ZeroMemory(&m_mtrl, sizeof(m_mtrl));
        m_radius = 0;
        m_velocity_x = 0;
        m_velocity_z = 0;
        m_pSphereMesh = NULL;
    }
    ~CSphere(void) {}

public:



    bool create(IDirect3DDevice9* pDevice, D3DXCOLOR color = d3d::WHITE) // 공 생성 메소드
    {
        if (NULL == pDevice)
            return false;

        m_mtrl.Ambient = color;
        m_mtrl.Diffuse = color;
        m_mtrl.Specular = color;
        m_mtrl.Emissive = d3d::BLACK;
        m_mtrl.Power = 5.0f;

        if (FAILED(D3DXCreateSphere(pDevice, getRadius(), 50, 50, &m_pSphereMesh, NULL)))
            return false;
        return true;
    }

    void destroy(void) // 공 맞추면 없애는 메소드
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

    bool hasIntersected(CSphere& ball) // 두 공이 충돌 했는지 확인
    {
        D3DXVECTOR3 cord = this->getCenter();
        D3DXVECTOR3 ball_cord = ball.getCenter();
        double xDistance = abs((cord.x - ball_cord.x) * (cord.x - ball_cord.x));
        double zDistance = abs((cord.z - ball_cord.z) * (cord.z - ball_cord.z));  //y는 보지 않는 이유 -> 게임이 조작되는 게임판 평면 상에서 움직이기 때문에 x축이랑 z축만 고려하면 된다
        double totalDistance = sqrt(xDistance + zDistance);

        if (totalDistance < (this->getRadius() + ball.getRadius()))
        {
            return true;
        }
        return false;
    }

    void hitBy(CSphere& whiteBall) {
        if (this->hasIntersected(whiteBall)) {
            // 기존의 속도 크기를 유지하기 위해 현재 속도의 크기를 계산
            double currentSpeed = sqrt(whiteBall.m_velocity_x * whiteBall.m_velocity_x + whiteBall.m_velocity_z * whiteBall.m_velocity_z);

            // 충돌 각도 계산
            double diff_x = whiteBall.center_x - this->center_x;
            double diff_z = whiteBall.center_z - this->center_z;
            double angle = atan2(diff_z, diff_x);

            // 새로운 속도 벡터 계산 (방향은 충돌 각도에 따라, 크기는 원래 속도와 동일)
            whiteBall.m_velocity_x = currentSpeed * cos(angle);
            whiteBall.m_velocity_z = currentSpeed * sin(angle);
        }
    }

    

    void handleCollision(CSphere& whiteBall, CSphere& blueBall)
    {
        // Check if the white ball and blue ball have collided
        if (whiteBall.hasIntersected(blueBall)) {
            // Calculate the collision direction
            D3DXVECTOR3 collisionDirection = blueBall.getCenter() - whiteBall.getCenter();
            D3DXVec3Normalize(&collisionDirection, &collisionDirection);

            // Calculate the reflection direction
            D3DXVECTOR3 incident = whiteBall.getCenter() - blueBall.getCenter();
            D3DXVECTOR3 reflection = incident - 2.0f * D3DXVec3Dot(&incident, &collisionDirection) * collisionDirection;
            D3DXVec3Normalize(&reflection, &reflection);

            // Set the new velocity of the white ball based on the reflection direction
            whiteBall.setPower(reflection.x, reflection.z);
        }
    }


    void ballUpdate(float timeDiff)
    {
        const float TIME_SCALE = 3.3;
        D3DXVECTOR3 cord = this->getCenter();
        double vx = abs(this->getVelocity_X());
        double vz = abs(this->getVelocity_Z());

        if (vx > 0.01 || vz > 0.01)
        {
            float tX = cord.x + TIME_SCALE * timeDiff * m_velocity_x;
            float tZ = cord.z + TIME_SCALE * timeDiff * m_velocity_z;

            if (tX >= (4.5 - M_RADIUS))
                tX = 4.5 - M_RADIUS;
            else if (tX <= (-4.5 + M_RADIUS))
                tX = -4.5 + M_RADIUS;
            else if (tZ <= (-3 + M_RADIUS))
                tZ = -3 + M_RADIUS;
            else if (tZ >= (3 - M_RADIUS))
                tZ = 3 - M_RADIUS;

            this->setCenter(tX, cord.y, tZ);
        }
        else { this->setPower(0, 0); }
        //this->setPower(this->getVelocity_X() * DECREASE_RATE, this->getVelocity_Z() * DECREASE_RATE);
        double rate = 1 - (1 - DECREASE_RATE) * timeDiff * 400;
        if (rate < 0)
            rate = 0;
        this->setPower(getVelocity_X() * rate, getVelocity_Z() * rate);
    }

    double getVelocity_X() { return this->m_velocity_x; }
    double getVelocity_Z() { return this->m_velocity_z; }

    void setPower(double vx, double vz)
    {
        this->m_velocity_x = vx;
        this->m_velocity_z = vz;
    }

    void setCenter(float x, float y, float z)
    {
        D3DXMATRIX m;
        center_x = x;   center_y = y;   center_z = z;
        D3DXMatrixTranslation(&m, x, y, z);
        setLocalTransform(m);
    }

    float getRadius(void)  const { return (float)(M_RADIUS); }
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
    ID3DXMesh* m_pSphereMesh;

};



// -----------------------------------------------------------------------------
// CWall class definition
// -----------------------------------------------------------------------------
class CWall {
private:
    float                   m_x;
    float                   m_z;
    float                   m_width;
    float                   m_depth;
    float                   m_height;
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
    int sign(double value) {
        if (value > 0) return 1;
        if (value < 0) return -1;
        return 0;
    }
    D3DXVECTOR3 getCenter(void) const
    {
        D3DXVECTOR3 center(m_x, m_height / 2.0f, m_z);
        return center;
    }
    bool create(IDirect3DDevice9* pDevice, float ix, float iz, float iwidth, float iheight, float idepth, D3DXCOLOR color = d3d::WHITE)
    {
        if (NULL == pDevice)
            return false;
        m_mtrl.Ambient = color;
        m_mtrl.Diffuse = color;
        m_mtrl.Specular = color;
        m_mtrl.Emissive = d3d::BLACK;
        m_mtrl.Power = 5.0f;
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
    CSphere getCollisionPoint(CSphere& ball) {
        CSphere collisionPoint = ball;
        int count = 0;
        while (this->hasIntersectedx(collisionPoint) || this->hasIntersectedz(collisionPoint)) {
            float backtrack_rate = 0.0000001;
            collisionPoint.setCenter(collisionPoint.getCenter().x - backtrack_rate * ball.getVelocity_X(), collisionPoint.getCenter().y, collisionPoint.getCenter().z - backtrack_rate * ball.getVelocity_Z());
            if (count++ > 100)
                break;
        }
        return collisionPoint;
    }
    bool hasIntersectedx(CSphere& ball)
    {
        float sphereCenterX = ball.getCenter().x;
        float wallX = this->m_x;
        float distance = abs(sphereCenterX - wallX) - (ball.getRadius() + 0.1);
        return distance <= 0;
    }
    bool hasIntersectedz(CSphere& ball)
    {
        float sphereCenterZ = ball.getCenter().z;
        float wallZ = this->m_z;
        float distance = abs(sphereCenterZ - wallZ) - (ball.getRadius() + 0.1);
        return distance <= 0;
    }

    void hitBy(CSphere& ball) {
        if (this->hasIntersectedx(ball)) {
            hitByx(ball);
        }
        if (this->hasIntersectedz(ball)) {
            hitByz(ball);
        }
        return;
    }


    void hitByx(CSphere& ball)
    {
        const float someSmallDistance = 0.000001;
        if (!this->hasIntersectedx(ball))
            return;

        CSphere cp = getCollisionPoint(ball);
        D3DXVECTOR3 normal(1.0f, 0.0f, 0.0f);

        D3DXVECTOR3 incident(cp.getVelocity_X(), 0.0f, cp.getVelocity_Z());
        D3DXVECTOR3 reflection = incident - 2.0f * D3DXVec3Dot(&incident, &normal) * normal;

        ball.setPower(reflection.x, reflection.z);
        ball.setCenter(cp.getCenter().x, cp.getCenter().y, cp.getCenter().z);
    }


    void hitByz(CSphere& ball)
    {
        const float someSmallDistance = 0.00005;
        if (!this->hasIntersectedz(ball))
            return;

        CSphere cp = getCollisionPoint(ball);
        D3DXVECTOR3 normal(0.0f, 0.0f, 1.0f);

        D3DXVECTOR3 incident(cp.getVelocity_X(), 0.0f, cp.getVelocity_Z());
        D3DXVECTOR3 reflection = incident - 2.0f * D3DXVec3Dot(&incident, &normal) * normal;

        ball.setPower(reflection.x, reflection.z);
        ball.setCenter(cp.getCenter().x, cp.getCenter().y, cp.getCenter().z);
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
private:
    void setLocalTransform(const D3DXMATRIX& mLocal) { m_mLocal = mLocal; }
    D3DXMATRIX              m_mLocal;
    D3DMATERIAL9            m_mtrl;
    ID3DXMesh* m_pBoundMesh;
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
        if (FAILED(D3DXCreateSphere(pDevice, radius, 10, 10, &m_pMesh, NULL)))
            return false;

        m_bound._center = lit.Position;
        m_bound._radius = radius;

        m_lit.Type = lit.Type;
        m_lit.Diffuse = lit.Diffuse;
        m_lit.Specular = lit.Specular;
        m_lit.Ambient = lit.Ambient;
        m_lit.Position = lit.Position;
        m_lit.Direction = lit.Direction;
        m_lit.Range = lit.Range;
        m_lit.Falloff = lit.Falloff;
        m_lit.Attenuation0 = lit.Attenuation0;
        m_lit.Attenuation1 = lit.Attenuation1;
        m_lit.Attenuation2 = lit.Attenuation2;
        m_lit.Theta = lit.Theta;
        m_lit.Phi = lit.Phi;
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
    ID3DXMesh* m_pMesh;
    d3d::BoundingSphere m_bound;
};





// -----------------------------------------------------------------------------
// 랭킹을 위한 구조체 함수 등등듣읃
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
Ranking new_ranking = { 0, 0 };
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
void eff_out(D3DXVECTOR3& pos) {
    static int frame = 0;
    const int totalFrames = 115; // Total duration of the animation
    const int peakFrame = 30; // Frame at which the slide reaches its lowest point

    if (frame < totalFrames) {
        float progress = static_cast<float>(frame) / totalFrames;
        float phaseProgress;

        if (frame < peakFrame) {
            // 하강
            phaseProgress = progress / (peakFrame / static_cast<float>(totalFrames));
            pos.y += 20 * phaseProgress * phaseProgress;
        }
        else {
            // 상승
            phaseProgress = (progress - 0.5f) * 2;
            pos.y -= 20 * (1 - phaseProgress) * (1 - phaseProgress);
        }

        frame++;
        Sleep(5);
    }
    else {
        frame = 0;
    }
}



void eff_in(D3DXVECTOR3& pos) {
    static int frame = 0;
    const int totalFrames = 115; // Total duration of the animation

    const int startY = -1024;    // Starting position (top)
    const int endY = 0;          // Ending position

    if (frame < totalFrames) {
        float progress = static_cast<float>(frame) / totalFrames;

        progress--;
        float cubicProgress = progress * progress * progress + 1;

        pos.y = startY + (endY - startY) * cubicProgress;

        frame++;
        Sleep(5);
    }
    else {
        frame = 0; // Reset for the next call
    }
}






// -----------------------------------------------------------------------------
// Global variables
// -----------------------------------------------------------------------------
CWall   g_legoPlane;
CWall   g_legowall[4];
CSphere   g_sphere[64];
CSphere   g_target_blueball;
CLight   g_light;



CSphere	whiteball; // 발사될 공

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

int g_stage = 1;
int g_life = 5;
int g_score = 0;
int g_combo = 0;
int g_phase = -1; // -1: 처음실행 0: 시작화면, 1: 게임화면, 2: 랭킹화면

bool g_ready = false;
int frame_1 = 0;
int frame_2 = 0;
int frame_3 = 0;
int frame_4 = 0;



// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
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

    //사진 불러오기
    if (FAILED(D3DXCreateTextureFromFile(Device, "start_puang.png", &start_Texture))) {
        return "Error: Failed to load puang.";
    }
    if (FAILED(D3DXCreateTextureFromFile(Device, "rank_puang.png", &rank_Texture))) {
        return "Error: Failed to load rank.";
    }
    D3DXCreateSprite(Device, &rank_Sprite);
    D3DXCreateSprite(Device, &start_Sprite); // 스프라이트 생성*****************************************************************************************************	
    spritePos_start.x = 0.0f;
    spritePos_start.y = 0.0f;
    spritePos_rank.x = 0.0f;
    spritePos_rank.y = 0.0f;

    g_rankings = ReadRankings("rank.txt");
    // ************************************************************************************************************


    int i;

    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);

    // create plane and set the position
    if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, color_puang1)) return false;
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
    for (i = 0; i < 64; i++) {
        if (false == g_sphere[i].create(Device, sphereColor[0])) return false;
        g_sphere[i].setCenter(spherePos1[i][0], (float)M_RADIUS, spherePos1[i][1]);
        g_sphere[i].setPower(0, 0);
    }

    // create blue ball for set direction
    if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
    g_target_blueball.setCenter(4.2f, (float)M_RADIUS, 0.0f);


    if (false == whiteball.create(Device, d3d::WHITE)) return false;
    whiteball.setCenter(3.7f, (float)M_RADIUS, 0.f);

  


    // light setting 
    D3DLIGHT9 lit;
    ::ZeroMemory(&lit, sizeof(lit));
    lit.Type = D3DLIGHT_POINT;
    lit.Diffuse = d3d::WHITE;
    lit.Specular = d3d::WHITE * 0.9f;
    lit.Ambient = d3d::WHITE * 0.9f;
    lit.Position = D3DXVECTOR3(0.0f, 11.0f, 0.0f);
    lit.Range = 100.0f;
    lit.Attenuation0 = 0.0f;
    lit.Attenuation1 = 0.2f;
    lit.Attenuation2 = 0.0f;
    if (false == g_light.create(Device, lit))
        return false;

    // Position and aim the camera.
    D3DXVECTOR3 pos(7.f, 10.0f, 0.0f);
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

bool Setup_stage()
{


    int i;

    // 스테이지 별 공모양
    switch (g_stage) {
    case 1:
        for (i = 0; i < 64; i++) {
            if (false == g_sphere[i].create(Device, sphereColor[0])) return false;
            g_sphere[i].setCenter(spherePos1[i][0], (float)M_RADIUS, spherePos1[i][1]);
            g_sphere[i].setPower(0, 0);
        }
        break;

    case 2:
        for (i = 0; i < 64; i++) {
            if (false == g_sphere[i].create(Device, sphereColor[0])) return false;
            g_sphere[i].setCenter(spherePos2[i][0], (float)M_RADIUS, spherePos2[i][1]);
            g_sphere[i].setPower(0, 0);
        }
        break;

    case 3:
        for (i = 0; i < 64; i++) {
            if (false == g_sphere[i].create(Device, sphereColor[0])) return false;
            g_sphere[i].setCenter(spherePos3[i][0], (float)M_RADIUS, spherePos3[i][1]);
            g_sphere[i].setPower(0, 0);
        }
        break;

    case 4:
        
        break;

    case 5:

        break;

    }


    // create blue ball for set direction
    if (false == g_target_blueball.create(Device, d3d::BLUE)) return false;
    g_target_blueball.setCenter(4.2f, (float)M_RADIUS, 0.0f);


    if (false == whiteball.create(Device, d3d::WHITE)) return false;
    whiteball.setCenter(3.7f, (float)M_RADIUS, 0.f);

    return true;
}



// 여러개 추가됨
void Cleanup(void)
{
    g_legoPlane.destroy();
    for (int i = 0; i < 4; i++) {
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
    int i = 0;
    int j = 0;


    if (Device)
    {
        Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00afafaf, 1.0f, 0);
        Device->BeginScene();



        // update the position of each ball. during update, check whether each ball hit by walls.
    
        whiteball.ballUpdate(timeDelta);
     
        
        for (j = 0; j < 4; j++)
        {
            g_legowall[j].hitBy(whiteball);
        }
        

        g_target_blueball.handleCollision(whiteball, g_target_blueball);

        // check whether any two balls hit together and update the direction of balls
        for (i = 0; i < 64; i++) {
            if (g_sphere[i].hasIntersected(whiteball)) {
                g_sphere[i].hitBy(whiteball);
                g_sphere[i].setCenter(-15.0f, M_RADIUS, -15.0f);
                g_sphere[i].setPower(.0f, .0f);
            }
        }

        g_target_blueball.hitBy(whiteball);


        // draw plane, walls, and spheres
        g_legoPlane.draw(Device, g_mWorld);
        for (i = 0; i < 4; i++) {
            g_legowall[i].draw(Device, g_mWorld);
        }
        for (i = 0; i < 64; i++) {
            g_sphere[i].draw(Device, g_mWorld);
        }
        
        whiteball.draw(Device, g_mWorld);
        g_target_blueball.draw(Device, g_mWorld);
        g_light.draw(Device);



        
        switch (g_phase)
        {
        case -1:     // 처음 실행시 시작화면
            if (start_Sprite && start_Texture) {
                start_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                start_Sprite->Draw(start_Texture, NULL, NULL, &spritePos_start, D3DCOLOR_XRGB(255, 255, 255));
                start_Sprite->End();
            }

            // press space to continue 
            // g_phase == -1 일때 누르면 g_phase +=2

            break;

        case 0:     // 시작화면, 내려옴

            if (frame_1 < 115) {
                if (start_Sprite && start_Texture) {
                    start_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                    start_Sprite->Draw(start_Texture, NULL, NULL, &spritePos_start, D3DCOLOR_XRGB(255, 255, 255));
                    start_Sprite->End();
                    eff_in(spritePos_start);
                    frame_1++;
                }
            }
            else {
                start_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                start_Sprite->Draw(start_Texture, NULL, NULL, &spritePos_start, D3DCOLOR_XRGB(255, 255, 255));
                start_Sprite->End();
            }
            break;

        case 1:  // 시작화면, 올라감
            if (frame_2 < 115) {
                if (start_Sprite && start_Texture) {
                    start_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                    start_Sprite->Draw(start_Texture, NULL, NULL, &spritePos_start, D3DCOLOR_XRGB(255, 255, 255));
                    start_Sprite->End();
                    eff_out(spritePos_start);
                    frame_2++;
                }
            }
            else if(frame_2 == 115) {
                spritePos_start.y = -1024.0f;
                frame_2++;
            }
            else {

                // 정보창
                TCHAR str[100];
                sprintf_s(str, "Stage: %d     Score: %03d     Combo: %02d\nLife:    %d\n\n\n\n\n\n\n\n                          Press Space!!!", g_stage, g_score, g_combo, g_life);

                // 글자크기
                RECT rc;
                SetRect(&rc, 50, 50, 0, 0); // x, y, width, height

                // 정보창 생성
                hud_Font->DrawText(NULL, str, -1, &rc, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

                g_ready = true;
                break;
            }
            break;


        case 2:  // 게임 진행 화면, 스테이지

            // 정보창
            TCHAR str[100];
            sprintf_s(str, "Stage: %d     Score: %03d     Combo: %02d\nLife:    %d", g_stage, g_score, g_combo, g_life);

            // 글자크기
            RECT rc;
            SetRect(&rc, 50, 50, 0, 0); // x, y, width, height

            // 정보창 생성
            hud_Font->DrawText(NULL, str, -1, &rc, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
            break;

            // 라이프가 0이 되었을때도 구현하기*****************************

            // 스테이지 별로 다른 출력
            switch (g_stage) {
            case 1:
                Setup_stage();
                break;

            case 2:
                Setup_stage();
                break;

            case 3:
                Setup_stage();
                break;

            case 4:
                Setup_stage();
                break;

            case 5:
                Setup_stage();
                break;
            }

        case 3:   // 랭킹화면 내려옴

            if (rank_Sprite && rank_Texture) {
                rank_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                rank_Sprite->Draw(rank_Texture, NULL, NULL, &spritePos_rank, D3DCOLOR_XRGB(255, 255, 255));
                rank_Sprite->End();
            }

            // 랭킹 업데이트
            UpdateRankings(new_ranking);
            DisplayRankings(hud_Font, g_rankings);
            break;

        case 4:  // 랭킹화면 올라감

            if (frame_4 < 115) {
                if (rank_Sprite && rank_Texture) {
                    rank_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
                    rank_Sprite->Draw(rank_Texture, NULL, NULL, &spritePos_rank, D3DCOLOR_XRGB(255, 255, 255));
                    rank_Sprite->End();
                    eff_out(spritePos_rank);
                    frame_4++;
                }

                UpdateRankings(new_ranking);
                DisplayRankings(hud_Font, g_rankings);// update the rankings 아직 추가안함!!!!!!!!!!!!!!!!!!!!!!!!

                

            }
            break;

        case 5:  // 변수들 초기화하고 g_phase = 0으로 돌아감
			g_stage = 1;
			g_life = 5;
			g_score = 0;
			g_combo = 0;
			g_phase = 0;
			break;

        }




        
        if (whiteball.getCenter().x >= g_target_blueball.getCenter().x) {
            //::MessageBox(0, "Game Over!", "Game Over", MB_OK); //  아직 필요없음
            //::PostQuitMessage(0);  // exit of the program -> change to the life reduction
            g_life -= 1;
            // 공 제자리에 옮기기

        }
        

        Device->EndScene();
        Device->Present(0, 0, 0, 0);
        Device->SetTexture(0, NULL);
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

    switch (msg) {
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
            if (g_phase == 1 && g_ready == true) {
                whiteball.setPower(-2, 0);
                g_phase += 1;
            }
            else if (g_phase == 2) {
                whiteball.setPower(-2, 0);
            }
            break;

        // 개발자 옵션
        case VK_F1:
            g_phase += 1;
			break;

        case VK_F2:
			g_phase -= 1;
			break;
        case VK_F3:
            g_stage += 1;
            break;
        case VK_F4:
            g_stage -= 1;
            break;
        case VK_F5:
            frame_1 = 0;
            frame_2 = 0;
            frame_3 = 0;
            frame_4 = 0;
            break;
      
    }
    break;
	}

    case WM_MOUSEMOVE:
    {
        int new_x = LOWORD(lParam);
        int new_y = HIWORD(lParam);
        float dx;
        float dy;
        
        if (LOWORD(wParam) & MK_LBUTTON) {

            if (isReset) {
                isReset = false;
            }
            else {
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
        }
        else {
            isReset = true;

            if (LOWORD(wParam) & MK_RBUTTON) {
                dx = (old_x - new_x);// * 0.01f;
                dy = (old_y - new_y);// * 0.01f;

                D3DXVECTOR3 coord3d = g_target_blueball.getCenter();
                g_target_blueball.setCenter(coord3d.x, coord3d.y, coord3d.z + dy * 0.007f);  // allows blue ball to move in the linear line of z axis
            }
            old_x = new_x;
            old_y = new_y;

            move = WORLD_MOVE;
        }
        break;

    }
    }

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
    HINSTANCE prevInstance,
    PSTR cmdLine,
    int showCmd)
{
    srand(static_cast<unsigned int>(time(NULL)));

    if (!d3d::InitD3D(hinstance,
        Width, Height, true, D3DDEVTYPE_HAL, &Device))
    {
        ::MessageBox(0, "InitD3D() - FAILED", 0, 0);
        return 0;
    }

    if (!Setup())
    {
        ::MessageBox(0, "Setup() - FAILED", 0, 0);
        return 0;
    }

    d3d::EnterMsgLoop( Display );

    Cleanup();

    Device->Release();

    return 0;
}
