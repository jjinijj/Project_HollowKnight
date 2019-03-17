#pragma once

class terrain;
class npc;
class enemy;
class mapData
{
	typedef vector<terrain*>			vTerrain;
	typedef vector<terrain*>::iterator	iterVTerrain;
	typedef vector<npc*>				vNpc;
	typedef vector<npc*>::iterator		iterVNpc;
	typedef vector<enemy*>				vEnemy;
	typedef vector<enemy*>::iterator	iterEnemy;

	enum
	{
		TRRIGER_MAX_COUNT = 100,
	};

private:
	vTerrain _terrains;		// ��� ������
	vNpc	 _npcs;			// npc
	vEnemy	 _enemys;		// enemy

	vTerrain _colTerrains;	// �浹ü�� �ִ� ������
	vTerrain _terrainsByLayer[MAP_LAYER_COUNT];	// ���̾������ �з�

	UINT _uidCount;

	WORD _triggerPool[TRRIGER_MAX_COUNT];			// terrain uid
	
public:
	mapData();
	~mapData();

	HRESULT init();
	void release();
	void clear();

	void update();
	void render();

	// �ش� ���̾ ����
	void render(UINT layer);

	// ���̾� ���� ����
	void terrainUp(UINT layer, UINT uid);
	// ���̾� ���� �Ʒ���
	void terrainDown(UINT layer, UINT uid);
	// ���̾� ���� toLayer, 
	void changeLayer(UINT sourLayer, UINT destLayer, UID uid);
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
	
	void loadMapDate(string fileName, int* terrainCnt);
	void loadMapInfo(string fileName, int* terrainCnt);

	void saveMapDate(string fileName);
	void saveMapInfo(string fileName);

	WORD getUsableTriggerIndex();


};

