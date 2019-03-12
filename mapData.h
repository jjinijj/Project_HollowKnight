#pragma once
#include "terrain.h"

class mapData
{
	typedef vector<terrain*>			vTerrain;
	typedef vector<terrain*>::iterator	iterVTerrain;

private:
	vTerrain _terrains;		// ��� ������

	vTerrain _colTerrains;	// �浹ü�� �ִ� ������
	vTerrain _terrainsByLayer[MAP_LAYER_COUNT];	// ���̾������ �з�

	UINT _uidCount;

	map<WORD, map<UINT, UINT>> _attrMap;
	
public:
	mapData();
	~mapData();

	HRESULT init();
	void release();
	void update();
	void render();

	// ���̾� ���� ���� ����
	void swapTerrain(UINT layer, int dest, int sour);
	// ���̾� ����
	void changeLayer(UINT destLayer, UINT sourLayer, int idx);
	// ���� ����
	void changeTerrain(UINT layer, int idx, terrain* ter);

	void addAttribute(UINT layer, int idx, WORD attr, int value);
	void removeAttribute(UINT layer, int idx, WORD attr);

	
	// ���� �߰�
	terrain* addTerrainDrag(UINT layer, float destX, float destY, float sourX, float sourY, float width, float height, eImageUID imgUid);
	terrain* addTerrainFrame(UINT layer, float destX, float destY, UINT frameX, UINT frameY, eImageUID imgUid);
	terrain* addTerrainClear(UINT layer, float destX, float destY, float width, float height);
	
	// ���̾�, ���̾� ���� �ε���
	terrain* getTerrain(UINT layer, int idx);
	// ���̾ �ִ� ������
	vTerrain* getLayerTerrains(UINT layer)	{ return &_terrainsByLayer[layer]; }
	// �浹ü �ִ� ������
	vTerrain* getColliderTerrains()			{ return& _colTerrains; }

	vTerrain* getTerrains() { return &_terrains; }
	
	HRESULT save(string fileName);
	HRESULT load(string fileName);


private:
	void addTerrain(UINT layer, terrain* ter);
	
	void loadMapDate(string fileName);
	void loadMapInfo(string fileName);

	void saveMapDate(string fileName);
	void saveMapInfo(string fileName);
};

