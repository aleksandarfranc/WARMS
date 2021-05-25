#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

//lambda, polimorfizam, dot product (linearna), 

// static vector <pair <float,float>> vecModel; Ovo je koristio u asteroid klipu

#include "olcConsoleGameEngine.h"

class cPhysicsObject {
public:
	float px = 0.0f;
	float py = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float ax = 0.0f;
	float ay = 0.0f;

	float radius = 4.0f;
	bool bStable = false;
	float fFriction = 0.8f;

	int nBounceBeforeDeath = -1;
	bool bDead = false;


	cPhysicsObject(float x = 0.0f, float y = 0.0f)
	{
		px = x;
		py = y;
	}

	virtual void Draw(olcConsoleGameEngine* engine, float fOffsetX, float fOffsetY) = 0; // pokazivac je tu na olc da bi koristili olc klasu
	virtual int BounceDeathAction() = 0;
};

class cWorm : public cPhysicsObject // A unit, or worm
{
public:
	cWorm(float x = 0.0f, float y = 0.0f) : cPhysicsObject(x, y)
	{
		radius = 3.5f;
		fFriction = 0.2f;
		bDead = false;
		nBounceBeforeDeath = -1;

		// load sprite data from sprite file
		if (sprWorm == nullptr)
			sprWorm = new olcSprite(L"worms.spr");
	}

	virtual void Draw(olcConsoleGameEngine* engine, float fOffsetX, float fOffsetY)
	{
		engine->DrawPartialSprite(px - fOffsetX - radius, py - fOffsetY - radius, sprWorm, 0, 0, 8, 8);
	}

	virtual int BounceDeathAction()
	{
		return 0; // Nothing
	}

private:
	static olcSprite* sprWorm;
};

olcSprite* cWorm::sprWorm = nullptr;

class cMissile : public cPhysicsObject // A projectile weapon
{
public:
	cMissile(float x = 0.0f, float y = 0.0f, float _vx = 0.0f, float _vy = 0.0f) : cPhysicsObject(x, y)
	{
		radius = 2.5f;
		fFriction = 0.5f;
		vx = _vx;
		vy = _vy;
		bDead = false;
		nBounceBeforeDeath = 1;
	}

	virtual void Draw(olcConsoleGameEngine* engine, float fOffsetX, float fOffsetY)
	{
		engine->DrawWireFrameModel(vecModel, px - fOffsetX, py - fOffsetY, atan2f(vy, vx), radius, FG_YELLOW);
	}

	virtual int BounceDeathAction()
	{
		return 20; // Explode Big
	}

private:
	static vector<pair<float, float>> vecModel;
};

class cDummy : public cPhysicsObject {
public:
	cDummy(float x = 0.0f, float y = 0.0f) : cPhysicsObject(x,y){}

	virtual void Draw(olcConsoleGameEngine* engine, float fOffsetX, float fOffsetY) {
		engine->DrawWireFrameModel(vecModel, px - fOffsetX, py - fOffsetY, atan2f(vy, 0.866f), radius, FG_WHITE); // px i py su koo na mapi ali nama treba da nacrtamo te moodele na konzoli

	}

	virtual int BounceDeathAction() {
		return 0;
	}

private:
	static vector<pair<float, float>> vecModel;
};

vector<pair<float, float>> DefineDummy() {
	vector<pair<float, float>> vecModel;
	vecModel.push_back({ 0.0f,0.0f });
	for (int i = 0; i < 10; i++) {
		vecModel.push_back({ cosf(i / 9.0f * 2.0f * 3.14159f), sinf(i / 9.0f * 2.0f * 3.14159f) });
	}
	return vecModel;
}

vector<pair<float, float>> cDummy::vecModel = DefineDummy();

class cDebris : public cPhysicsObject {
public:
	cDebris(float x = 0.0f, float y = 0.0f) :cPhysicsObject(x, y) {
		vx = 10.0f * cosf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);
		vy = 10.0f * sinf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);
		radius = 1.0f;
		fFriction - 0.0f;
		nBounceBeforeDeath = 5;
	}

	virtual void Draw(olcConsoleGameEngine* engine, float fOffsetX, float fOffsetY) {
		engine->DrawWireFrameModel(vecModel, px - fOffsetX, py - fOffsetY, atan2f(vy, vx), radius, FG_DARK_GREEN);
	}

	virtual int BounceDeathAction() {
		return 0;
	}

private:
	static vector<pair<float, float>> vecModel;
};

vector<pair<float, float>> DefineDebris() {
	vector<pair<float, float>> vecModel;
	vecModel.push_back({ 0.0f,0.0f });
	vecModel.push_back({ 0.0f,0.1f });
	vecModel.push_back({ 0.1f,0.1f });
	vecModel.push_back({ 0.1f,0.0f });
	return vecModel;
}

vector<pair<float, float>> cDebris::vecModel = DefineDebris();

vector<pair<float, float>> DefineMissile()
{
	// Defines a rocket like shape
	vector<pair<float, float>> vecModel;
	vecModel.push_back({ 0.0f, 0.0f });
	vecModel.push_back({ 1.0f, 1.0f });
	vecModel.push_back({ 2.0f, 1.0f });
	vecModel.push_back({ 2.5f, 0.0f });
	vecModel.push_back({ 2.0f, -1.0f });
	vecModel.push_back({ 1.0f, -1.0f });
	vecModel.push_back({ 0.0f, 0.0f });
	vecModel.push_back({ -1.0f, -1.0f });
	vecModel.push_back({ -2.5f, -1.0f });
	vecModel.push_back({ -2.0f, 0.0f });
	vecModel.push_back({ -2.5f, 1.0f });
	vecModel.push_back({ -1.0f, 1.0f });

	// Scale points to make shape unit sized
	for (auto& v : vecModel)
	{
		v.first /= 2.5f; v.second /= 2.5f;
	}
	return vecModel;
}
vector<pair<float, float>> cMissile::vecModel = DefineMissile();

class OneLoneCoder_Worms : public olcConsoleGameEngine {
public:
	OneLoneCoder_Worms() {
		m_sAppName = L"Worms";

	}

private:

	int nMapWidth = 1024;
	int nMapHeight = 512;
	unsigned char* map = nullptr;

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

	list <unique_ptr<cPhysicsObject>> listObjects; // kada god izleti iz obima(scope) gde se nalazi poziva se destruktor

	virtual bool OnUserUpdate(float fElapsedTime) {

		if (m_keys[L'M'].bReleased)
			CreateMap();

		// Right click to drop missile
		if (m_mouse[1].bReleased)
			listObjects.push_back(unique_ptr<cMissile>(new cMissile(m_mousePosX + fCameraPosX, m_mousePosY + fCameraPosY)));

		if (m_mouse[0].bReleased) {
			Boom(m_mousePosX + fCameraPosX, m_mousePosY + fCameraPosY, 10.0f);
		}

		// Middle click to spawn worm/unit
		if (m_mouse[2].bReleased)
			listObjects.push_back(unique_ptr<cWorm>(new cWorm(m_mousePosX + fCameraPosX, m_mousePosY + fCameraPosY)));

		// mouse Edge Map Scroll // ovo je za konzolu/kameru ne celu mapu
		float fMapScrollSpeed = 400.0f;
		if (m_mousePosX < 5) fCameraPosX -= fMapScrollSpeed * fElapsedTime;
		if (m_mousePosX > ScreenWidth() - 5) fCameraPosX += fMapScrollSpeed * fElapsedTime;
		if (m_mousePosY < 5) fCameraPosY -= fMapScrollSpeed * fElapsedTime;
		if (m_mousePosY > ScreenHeight() - 5) fCameraPosY += fMapScrollSpeed * fElapsedTime;

		//clamp map boundaries
		if (fCameraPosX < 0) fCameraPosX = 0;
		if (fCameraPosX >= nMapWidth - ScreenWidth()) fCameraPosX = nMapWidth - ScreenWidth();
		if (fCameraPosY < 0) fCameraPosY = 0;
		if (fCameraPosY >= nMapHeight - ScreenHeight()) fCameraPosY = nMapHeight - ScreenHeight();

		// 10 physuscs iterations per frame
		for (int z = 0; z < 10; z++) 
		{
			for (auto& p : listObjects)
			{
				// Apply gravity
				p->ay += 2.0f;

				// Update velocity
				p->vx += p->ax * fElapsedTime;
				p->vy += p->ay * fElapsedTime;

				// Update Position // potential jer ako dodje do kolizije ne zelimo da objekat ostane preko objekta
				float fPotentialX = p->px + p->vx * fElapsedTime;
				float fPotentialY = p->py + p->vy * fElapsedTime;

				// Reset Acceleration
				p->ax = 0.0f;
				p->ay = 0.0f;
				p->bStable = false;

				//collision check with map
				float fAngle = atan2f(p->vy, p->vx); // vraca radius
				float fResponseX = 0;
				float fResponseY = 0;
				bool bCollision = false;

				for (float r = fAngle - 3.14159f / 2.0f; r < fAngle + 3.14159f / 2.0f; r += 3.14159 / 8.0f) {
					float fTestPosX = (p->radius) * cosf(r) + fPotentialX;
					float fTestPosY = (p->radius) * sinf(r) + fPotentialY;

					if (fTestPosX >= nMapWidth) fTestPosX = nMapWidth - 1;
					if (fTestPosY >= nMapHeight) fTestPosY = nMapHeight - 1;
					if (fTestPosX < 0) fTestPosX = 0;
					if (fTestPosY < 0) fTestPosY = 0;

					//test if any points on semicircle intersect with terrain
					if (map[(int)fTestPosY * nMapWidth + (int)fTestPosX] != 0) 
					{
						//accumulate collision point to give and excape respone vector
						//Effectively, normal to the areas od contact
						fResponseX += fPotentialX - fTestPosX;
						fResponseY += fPotentialY - fTestPosY;
						bCollision = true;
					}
				}
				float fMagVelocity = sqrtf(p->vx * p->vx + p->vy * p->vy);
				float fMagResponse = sqrtf(fResponseX * fResponseX + fResponseY * fResponseY);

				if (bCollision) {
					p->bStable = true;
					
					float dot = p->vx * (fResponseX / fMagResponse) + p->vy * (fResponseY / fMagResponse);
					p->vx = p-> fFriction * (-2.0f * dot * (fResponseX / fMagResponse) + p->vx);
					p->vy = p-> fFriction * (-2.0f * dot * (fResponseY / fMagResponse) + p->vy);
					
					if (p->nBounceBeforeDeath > 0) {
						p->nBounceBeforeDeath--;
						p->bDead = p->nBounceBeforeDeath == 0;

						if (p->bDead) {
							// Action upon object death
							// = 0 Nothing
							// > 0 Explosion 
							int nResponse = p->BounceDeathAction();
							if (nResponse > 0)
								Boom(p->px, p->py, nResponse);
						}
					}

				}
				else {
					p->px = fPotentialX;
					p->py = fPotentialY;
				}

				if (fMagVelocity < 1.0f) p->bStable = true;

				p->px = fPotentialX;
				p->py = fPotentialY;

			}

			listObjects.remove_if([](unique_ptr<cPhysicsObject> &o) {return o->bDead; }); // this is removing objecct from list not dealocating

		}
		

		//Draw Landscape
		for(int x = 0; x <  ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++) {
				switch (map[(y + (int)fCameraPosY) * nMapWidth + (x + (int)fCameraPosX)]) { 
				case 0:
					Draw(x, y, PIXEL_SOLID, FG_CYAN);
					break;
				case 1:
					Draw(x, y, PIXEL_SOLID, FG_DARK_GREEN);
					break;
				}
			}

		for (auto& p : listObjects)
			p->Draw(this, fCameraPosX, fCameraPosY);

		return true;
	}
	
	virtual bool OnUserCreate() {
		map = new unsigned char[nMapWidth * nMapHeight];
		memset(map, 0, nMapWidth * nMapHeight * sizeof(unsigned char));

		CreateMap();

		return true;
	}

	void Boom(float fWorldX, float fWorldY, float fRadius)
	{

		auto CircleBresenham = [&](int xc, int yc, int r) {
			int x = 0;
			int y = r;
			int p = 3 - 2 * r;
			if (!r) return;

			auto drawline = [&](int sx, int ex, int ny) {
				for (int i = sx; i < ex; i++)
					if (ny >= 0 && ny < nMapHeight && i >= 0 && i < nMapWidth)
						map[ny * nMapWidth + i] = 0;
			};
			while (y >= x)  // only formulate 1/8 od circle
			{
				drawline(xc - x, xc + x, yc - y);
				drawline(xc - y, xc + y, yc - x);
				drawline(xc - x, xc + x, yc + y);
				drawline(xc - y, xc + y, yc + x);
				if (p < 0) p += 4 * x++ + 6;
				else p += 4 * (x++ - y--) + 10;
			}
		};

		//create creater
		CircleBresenham(fWorldX, fWorldY, fRadius);

		// shockwave other entities in range
		for (auto& p : listObjects) {
			float dx = p->px - fWorldX;
			float dy = p->py - fWorldY;
			float fDist = sqrt(dx * dx + dy * dy);

			if ( fDist < 0.0001f ) fDist = 0.0001f;

			if (fDist < fRadius) {
				p->vx = (dx / fDist) * fRadius;
				p->vy = (dy / fDist) * fRadius;
				p->bStable = false;
			}
		}

		// launch debris
		for (int i = 0; i < (int)fRadius; i++) 
			listObjects.push_back(unique_ptr<cDebris>(new cDebris(fWorldX, fWorldY)));
		
	}

	void CreateMap() {
		// used 1D Perlin Noise
		float* fSurface = new float[nMapWidth];
		float* fNoiseSeed = new float[nMapWidth];

		for (int i = 0; i < nMapWidth; i++)
			fNoiseSeed[i] = (float)rand() / (float)RAND_MAX;

		fNoiseSeed[0] = 0.5f; // prvi i poslednji element ce biti 0.5 u 1D PN
		PerlinNoise1D(nMapWidth, fNoiseSeed, 8, 2.0f, fSurface);

		for(int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++) {
				if (y >= fSurface[x] * nMapHeight) // if (y == 150){...}
					map[y * nMapWidth + x] = 1; 
				else
					map[y * nMapWidth + x] = 0;
			}

		delete[] fSurface;
		delete[] fNoiseSeed;

	}

		void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutout) {
		//fSeed[0] = 0.5;
		for (int x = 0; x < nCount; x++) // why nCount
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nCount >> o; // 256>>1=128,64,32,16,8,4,2;1
				// x = 63, nPitch = 32,16,8,4,2,1 => nSample1 = 32,48,56,60,62,63
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % nCount;
								
				float fBlend = (float)(x - nSample1) / (float)nPitch; //offset izmedju 0 i 1. 
				float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
				fNoise += fSample * fScale;

				fScaleAcc += fScale;
				fScale = fScale / fBias;
			}

			//
			fOutout[x] = fNoise/fScaleAcc;
		}
		
	}


};

int main() {
	OneLoneCoder_Worms game;
	game.ConstructConsole(450, 250, 3, 3);
	game.Start();
	return 0;
}
//18:00