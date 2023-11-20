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

IDirect3DDevice9* Device = NULL;

// window size
const int Width = 1024;
const int Height = 768;

// There are four balls
// initialize the position (coordinate) of each ball (ball0 ~ ball3)

/*
const float spherePos[64][2] = {

};
*/

const float spherePos[64][2] = {
     {-1.75f, 1.75f}, {-1.25f, 1.75f}, {-0.75f, 1.75f}, {-0.25f, 1.75f}, {0.25f, 1.75f}, {0.75f, 1.75f}, {1.25f, 1.75f}, {1.75f, 1.75f},
    {-1.75f, 1.25f}, {-1.25f, 1.25f}, {-0.75f, 1.25f}, {-0.25f, 1.25f}, {0.25f, 1.25f}, {0.75f, 1.25f}, {1.25f, 1.25f}, {1.75f, 1.25f},
    {-1.75f, 0.75f}, {-1.25f, 0.75f}, {-0.75f, 0.75f}, {-0.25f, 0.75f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {1.25f, 0.75f}, {1.75f, 0.75f},
    {-1.75f, 0.25f}, {-1.25f, 0.25f}, {-0.75f, 0.25f}, {-0.25f, 0.25f}, {0.25f, 0.25f}, {0.75f, 0.25f}, {1.25f, 0.25f}, {1.75f, 0.25f},
    {-1.75f, -0.25f}, {-1.25f, -0.25f}, {-0.75f, -0.25f}, {-0.25f, -0.25f}, {0.25f, -0.25f}, {0.75f, -0.25f}, {1.25f, -0.25f}, {1.75f, -0.25f},
    {-1.75f, -0.75f}, {-1.25f, -0.75f}, {-0.75f, -0.75f}, {-0.25f, -0.75f}, {0.25f, -0.75f}, {0.75f, -0.75f}, {1.25f, -0.75f}, {1.75f, -0.75f},
    {-1.75f, -1.25f}, {-1.25f, -1.25f}, {-0.75f, -1.25f}, {-0.25f, -1.25f}, {0.25f, -1.25f}, {0.75f, -1.25f}, {1.25f, -1.25f}, {1.75f, -1.25f},
    {-1.75f, -1.75f}, {-1.25f, -1.75f}, {-0.75f, -1.75f}, {-0.25f, -1.75f}, {0.25f, -1.75f}, {0.75f, -1.75f}, {1.25f, -1.75f}, {1.75f, -1.75f}

};


/* 
//공모양
const float spherePos[64][2] = { {3.2f, 0.0f}, {3.1841f, 0.2788f}, {3.1366f, 0.5548f}, {3.0578f, 0.8253f},
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
;*/

/*
//사각형
const float spherePos[64][2] = {
    {-1.75f, 1.75f}, {-1.25f, 1.75f}, {-0.75f, 1.75f}, {-0.25f, 1.75f}, {0.25f, 1.75f}, {0.75f, 1.75f}, {1.25f, 1.75f}, {1.75f, 1.75f},
    {-1.75f, 1.25f}, {-1.25f, 1.25f}, {-0.75f, 1.25f}, {-0.25f, 1.25f}, {0.25f, 1.25f}, {0.75f, 1.25f}, {1.25f, 1.25f}, {1.75f, 1.25f},
    {-1.75f, 0.75f}, {-1.25f, 0.75f}, {-0.75f, 0.75f}, {-0.25f, 0.75f}, {0.25f, 0.75f}, {0.75f, 0.75f}, {1.25f, 0.75f}, {1.75f, 0.75f},
    {-1.75f, 0.25f}, {-1.25f, 0.25f}, {-0.75f, 0.25f}, {-0.25f, 0.25f}, {0.25f, 0.25f}, {0.75f, 0.25f}, {1.25f, 0.25f}, {1.75f, 0.25f},
    {-1.75f, -0.25f}, {-1.25f, -0.25f}, {-0.75f, -0.25f}, {-0.25f, -0.25f}, {0.25f, -0.25f}, {0.75f, -0.25f}, {1.25f, -0.25f}, {1.75f, -0.25f},
    {-1.75f, -0.75f}, {-1.25f, -0.75f}, {-0.75f, -0.75f}, {-0.25f, -0.75f}, {0.25f, -0.75f}, {0.75f, -0.75f}, {1.25f, -0.75f}, {1.75f, -0.75f},
    {-1.75f, -1.25f}, {-1.25f, -1.25f}, {-0.75f, -1.25f}, {-0.25f, -1.25f}, {0.25f, -1.25f}, {0.75f, -1.25f}, {1.25f, -1.25f}, {1.75f, -1.25f},
    {-1.75f, -1.75f}, {-1.25f, -1.75f}, {-0.75f, -1.75f}, {-0.25f, -1.75f}, {0.25f, -1.75f}, {0.75f, -1.75f}, {1.25f, -1.75f}, {1.75f, -1.75f}
};*/



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
#define DECREASE_RATE 0.9982

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
// Global variables
// -----------------------------------------------------------------------------
CWall   g_legoPlane;
CWall   g_legowall[4];
CSphere   g_sphere[64];
CSphere   g_target_blueball;
CLight   g_light;



CSphere	whiteball; // 발사될 공

double g_camera_pos[3] = { 0.0, 5.0, -8.0 };

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------


void destroyAllLegoBlock(void)
{
}

// initialization
bool Setup()
{
    int i;

    D3DXMatrixIdentity(&g_mWorld);
    D3DXMatrixIdentity(&g_mView);
    D3DXMatrixIdentity(&g_mProj);

    // create plane and set the position
    if (false == g_legoPlane.create(Device, -1, -1, 9, 0.03f, 6, d3d::GREEN)) return false;
    g_legoPlane.setPosition(0.0f, -0.0006f / 5, 0.0f);

    // create walls and set the position. note that there are four walls
    if (false == g_legowall[0].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
    g_legowall[0].setPosition(0.0f, 0.12f, 3.06f);
    if (false == g_legowall[1].create(Device, -1, -1, 9, 0.3f, 0.12f, d3d::DARKRED)) return false;
    g_legowall[1].setPosition(0.0f, 0.12f, -3.06f);
    if (false == g_legowall[2].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
    g_legowall[2].setPosition(4.56f, 0.12f, 0.0f);
    if (false == g_legowall[3].create(Device, -1, -1, 0.12f, 0.3f, 6.24f, d3d::DARKRED)) return false;
    g_legowall[3].setPosition(-4.56f, 0.12f, 0.0f);

    // create four balls and set the position
    for (i = 0; i < 64; i++) {
        if (false == g_sphere[i].create(Device, sphereColor[0])) return false;
        g_sphere[i].setCenter(spherePos[i][0], (float)M_RADIUS, spherePos[i][1]);
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
    lit.Position = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
    lit.Range = 100.0f;
    lit.Attenuation0 = 0.0f;
    lit.Attenuation1 = 0.9f;
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

void Cleanup(void)
{
    g_legoPlane.destroy();
    for (int i = 0; i < 4; i++) {
        g_legowall[i].destroy();
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

        
        if (whiteball.getCenter().x >= g_target_blueball.getCenter().x) {
            ::MessageBox(0, "Game Over!", "Game Over", MB_OK);
            ::PostQuitMessage(0);  // exit of the program -> change to the life reduction
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


            
            whiteball.setPower(-2, 0);

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

    d3d::EnterMsgLoop(Display);

    Cleanup();

    Device->Release();

    return 0;
}
