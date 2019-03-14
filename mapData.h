#pragma once
#include "terrain.h"

class mapData
{
	typedef vector<terrain*>			vTerrain;
	typedef vector<terrain*>::iterator	iterVTerrain;

	enum
	{
		TRRIGER_MAX_COUNT = 100,
	};

private:
	vTerrain _terrains;		// ��� ������

	vTerrain _colTerrains;	// �浹ü�� �ִ� ������
	vTerrain _terrainsByLayer[MAP_LAYER_COUNT];	// ���̾������ �з�

	UINT _uidCount;

	WORD _triggerPool[TRRIGER_MAX_COUNT];			// terrain uid
	
public:
	mapData();
	~mapData();

	HRESULT init();
	void release();
	void update();
	void render();

	// ���̾� ���� ����
	void terrainUp(UINT layer, UINT uid);
	// ���̾� ���� �Ʒ���
	void terrainDown(UINT layer, UINT uid);
	// ���̾� ����
	void changeLayer(UINT destLayer, UINT sourLayer, int idx);
	// ���� ����
	void changeTerrain(UINT layer, int idx, terrain* ter);

	void useTrigger(WORD terrainUID);
	void deleteTrigger(WORD terrainUID);	

	int getTriggerIndex(WORD terrinUID);

	// ���� �߰�
	terrain* addTerrainDrag(UINT layer, float destX, float destY, float sourX, float sourY, float width, float height, eImageUID imgUid);
	terrain* addTerrainFrame(UINT layer, float destX, float destY, UINT frameX, UINT frameY, eImageUID imgUid);
	terrain* addTerrainClear(UINT layer, float destX, float destY, float width, float height);

	void deleteTerrain(UINT layer, UID uid);
	
	// ������ �Ӽ� �߰�
	void addTerrainAttribute(UINT layer, UID uid, eAttribute attr);
	// ������ �Ӽ� ����
	void removeTerrainAttribute(UINT layer, UID uid, eAttribute attr);

	// ���̾�, uid
	terrain* getTerrain(UINT layer, UINT uid);
	// uid
	terrain* getTerrain(UINT uid);
	// layer, uid
	int getTerrainIndex(UINT layer, UINT uid);
	// uid
	int getTerrainIndex(UINT uid);

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

	WORD getUsableTriggerIndex();
};

