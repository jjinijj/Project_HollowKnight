#pragma once
#include "terrain.h"

class mapData
{
	typedef vector<terrain*>			vTerrain;
	typedef vector<terrain*>::iterator	iterVTerrain;

private:
	vTerrain _terrains;		// 모든 지형들

	vTerrain _colTerrains;	// 충돌체가 있는 지형들
	vTerrain _terrainsByLayer[MAP_LAYER_COUNT];	// 레이어단위로 분류

	UINT _uidCount;
	
public:
	mapData();
	~mapData();

	HRESULT init();
	void release();
	void update();
	void render();

	// 레이어 상의 순서 변경
	void swapTerrain(UINT layer, int dest, int sour);
	// 레이어 변경
	void changeLayer(UINT destLayer, UINT sourLayer, int idx);
	
	// 지형 추가
	terrain* addTerrainDrag(UINT layer, float destX, float destY, float sourX, float sourY, float width, float height, eImageUID imgUid);
	terrain* addTerrainFrame(UINT layer, float destX, float destY, UINT frameX, UINT frameY, eImageUID imgUid);
	terrain* addTerrainClear(UINT layer, float destX, float destY, float width, float height);
	
	// 레이어, 레이어 상의 인덱스
	terrain* getTerrain(UINT layer, int idx);
	// 레이어에 있는 지형들
	vTerrain* getLayerTerrains(UINT layer)	{ return &_terrainsByLayer[layer]; }
	// 충돌체 있는 지형들
	vTerrain* getColliderTerrains()			{ return& _colTerrains; }

	vTerrain* getTerrains() { return &_terrains; }
	
	
	
private:
	void addTerrain(UINT layer, terrain* ter);
};

