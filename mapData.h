#pragma once

class terrain;
class npc;
class enemy;
class actorBase;
class mapData
{
	typedef vector<terrain*>				vTerrain;
	typedef vector<terrain*>::iterator		iterVTerrain;
	typedef map<UINT, npc*>					mNpc;
	typedef map<UINT, npc*>::iterator		iterMNpc;
	typedef vector<enemy*>					vEnemy;
	typedef vector<enemy*>::iterator		iterEnemy;

	enum
	{
		TRRIGER_MAX_COUNT = 100,
	};

private:
	vTerrain _terrains;		// 모든 지형들
	mNpc	 _npcs;			// npc
	vEnemy	 _enemys;		// enemy

	vTerrain _colTerrains;	// 충돌체가 있는 지형들
	vTerrain _terrainsByLayer[eLayer_Count];	// 레이어단위로 분류

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
	void renderBack();
	void renderFront();
	void renderActors();

	// 해당 레이어만 렌더
	void render(UINT layer);

	// 레이어 순서 위로
	void terrainUp(UINT layer, UINT uid);
	// 레이어 순서 아래로
	void terrainDown(UINT layer, UINT uid);
	// 레이어 변경 toLayer, 
	void changeLayer(UINT sourLayer, UINT destLayer, UID uid);
	// 지형 변경
	void changeTerrain(UINT layer, int idx, terrain* ter);

	void useTrigger(WORD terrainUID);
	void deleteTrigger(WORD terrainUID);	

	int getTriggerIndex(WORD terrinUID);

	// 지형 추가
	terrain* addTerrainDrag(UINT layer, float destX, float destY, float sourX, float sourY, float width, float height, eImageUID imgUid);
	terrain* addTerrainFrame(UINT layer, float destX, float destY, UINT frameX, UINT frameY, eImageUID imgUid);
	terrain* addTerrainClear(UINT layer, float destX, float destY, float width, float height);

	// npc 추가
	npc* addNpc(float destX, float destY, eImageUID imgUid);

	void deleteTerrain(UINT layer, UID uid);
	
	// 지형에 속성 추가
	void addTerrainAttribute(UINT layer, UID uid, eAttribute attr);
	// 지형에 속성 제거
	void removeTerrainAttribute(UINT layer, UID uid, eAttribute attr);

	// 레이어, uid
	terrain* getTerrain(UINT layer, UINT uid);
	// uid
	terrain* getTerrain(UINT uid);
	// layer, uid
	int getTerrainIndex(UINT layer, UINT uid);
	// uid
	int getTerrainIndex(UINT uid);

	// 레이어에 있는 지형들
	vTerrain* getLayerTerrains(UINT layer)	{ return &_terrainsByLayer[layer]; }
	// 충돌체 있는 지형들
	vTerrain* getColliderTerrains()			{ return &_colTerrains; }
	vTerrain* getCollisionTerains()			{ return &_colTerrains; }

	vTerrain* getTerrains() { return &_terrains; }

	actorBase* getActor(UINT uid);
	
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

