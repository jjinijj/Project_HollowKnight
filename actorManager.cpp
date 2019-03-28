#include "stdafx.h"
#include "actorManager.h"
#include "enemyHeaders.h"
#include "npcHeaders.h"
#include "bullet.h"
#include "mapData.h"
#include "terrain.h"


actorManager::actorManager()
: _uidCount(10000)
{
}


actorManager::~actorManager()
{
}

HRESULT actorManager::init()
{
	for ( int ii = 0; ii < BULLET_COUNT; ++ii )
	{
		arcBullet* arc = new arcBullet;
		_arcBulletQ.push(arc);
	}

	for ( int ii = 0; ii < BULLET_COUNT * 3; ++ii )
	{
		linearBullet* linear = new linearBullet;
		_linearbulletQ.push(linear);
	}


	return S_OK;
}

void actorManager::release()
{
	{
		iterN iter = _npcMap.begin();
		for (iter; _npcMap.end() != iter; )
			iter = _npcMap.erase(iter);
	}
	{
		iterE iter = _enemyMap.begin();
		for (iter; _enemyMap.end() != iter; )
			iter = _enemyMap.erase(iter);
	}
	{
		iterA iter = _actorMap.begin();
		for (iter; _actorMap.end() != iter; )
		{
			actorBase* actor = iter->second;
			iter = _actorMap.erase(iter);
		}
		_actorMap.clear();
	}

	{
		iterB iter = _bulletList.begin();
		for (iter ; _bulletList.end() != iter;)
		{
			bullet* bt = *iter;

			iter = _bulletList.erase(iter);

			SAFE_RELEASE(bt);
			SAFE_DELETE(bt);
		}
		_bulletList.clear();

		while (!_arcBulletQ.empty())
		{
			arcBullet* arc = _arcBulletQ.front();
			_arcBulletQ.pop();

			SAFE_RELEASE(arc);
			SAFE_DELETE(arc);
		}

		while (!_linearbulletQ.empty())
		{
			linearBullet* linear = _linearbulletQ.front();
			_linearbulletQ.pop();

			SAFE_RELEASE(linear);
			SAFE_DELETE(linear);
		}

	}
	SAFE_RELEASE(_playerBullet);
	SAFE_DELETE(_playerBullet);
}

void actorManager::render()
{
	{
		iterA iter = _actorMap.begin();
		iterA end = _actorMap.end();
		for (iter; iter != end; ++iter)
			iter->second->render();
	}

	{
		iterB iter = _bulletList.begin();
		iterB end  = _bulletList.end();
		for (iter; end != iter; ++iter)
			(*iter)->render();
	}

	if(_playerBullet)
		_playerBullet->render();
}

void actorManager::update()
{
	{
		iterA iter = _actorMap.begin();
		iterA end = _actorMap.end();
		for (iter; iter != end; ++iter)
		{
			actorBase* actor = iter->second;
			if (CAMERA->inRangeInActiveArea(  actor->getRect().left
											, actor->getRect().top
											, static_cast<int>(actor->getWidth())
											, static_cast<int>(actor->getHeight())))
				actor->update();

		}
	}

	{
		iterE iter = _enemyMap.begin();
		iterE end = _enemyMap.end();
		for (iter; end != iter; ++iter)
		{
			enemy* em = iter->second;

			if(em->isFireState())
			{
				fireEnemyBullet(em);
				em->bulletFire();
			}

			if(!em->isAlive() && !em->isAppear())
			{
				destroyActor(em->getUID());
				break;
			}
		}
	}

	{
		iterB iter = _bulletList.begin();
		for (iter; _bulletList.end() != iter; )
		{
			bullet* bt = (*iter);
			
			// 터지는 중인 bullet
			if (bt->isPang())
			{
				if (bt->isAppear())
				{
					bt->update();
					++iter;
				}
				else
				{
					iter = returnBulletQ(iter);
					bt->clear();
				}
			}
			// 충돌하면 bulletPangList로
			else if ( checkHitSomething(bt) )
			{
				bt->hitSomething();
			}
			else if ( !checkBulletIntheField(bt))
			{
				iter = returnBulletQ(iter);
			}
			else
			{
				bt->update();
				++iter;
			}
		}
	}

	if (_playerBullet)
	{
		if (_playerBullet->isPang())
		{
			if(_playerBullet->isAppear())
				_playerBullet->update();
			else
			{
				SAFE_RELEASE(_playerBullet);
				SAFE_DELETE(_playerBullet);
			}
		}
		else
		{
			if(checkPlayerBulletHitSomething())
				_playerBullet->hitSomething();
			else
				_playerBullet->update();
		}
	}
}

npc* actorManager::createNPC(float x, float y, WORD type)
{
	npc* n = nullptr;

	switch(type)
	{
		case eNpc_Elderbug:	  {n = new elderbug;  break; }
		case eNpc_Sly:		  { break; }
		case eNpc_Quirrel:	  { break; }
		case eNpc_Iselda:	  { break; }
		case eNpc_Cornifer:	  { break; }
		case eNpc_TheLastStag:{ break; }

		default:
			break;
	}

	if(n)
	{
		++_uidCount;
		n->init(_uidCount, x, y);

		_actorMap.insert(make_pair(n->getUID(), n));
		_npcMap.insert(make_pair(n->getUID(),n));
	}

	return n;
}

enemy* actorManager::createEnemy(float x, float y, WORD type)
{
	enemy* em = nullptr;

	switch (type)
	{
		case eEnemy_Gruzzer:	{ em = new gruzzer; break;}
		case eEnemy_Tiktik:		{ em = new tiktik; break;}
		//case eEnemy_Primalaspid:{ em = new gruzzer; break;}
		//case eEnemy_Mawlek:		{ em = new gruzzer; break;}

		default:
			break;
	}

	if (em)
	{
		++_uidCount;
		em->init(_uidCount, x, y);
		em->mapDataLink(_mapData);

		_actorMap.insert(make_pair(em->getUID(), em));
		_enemyMap.insert(make_pair(em->getUID(), em));
	}

	switch (type)
	{
		case eEnemy_Gruzzer:	{ break; }
		case eEnemy_Tiktik:		
		{



			break;
		}
		//case eEnemy_Primalaspid:{ em = new gruzzer; break;}
		//case eEnemy_Mawlek:		{ em = new gruzzer; break;}

		default:
			break;
	}

	return em;
}

bullet* actorManager::createBullet(WORD type)
{
	bullet* newBullet = nullptr;
	switch ( type )
	{
		case bullet::eLINEARBULLET:
		{
			if( !_linearbulletQ.empty() )
			{
				newBullet = _linearbulletQ.front();
				_linearbulletQ.pop();
				_bulletList.push_back(newBullet);
			}

			break;
		}
		case bullet::eARCBULLET:
		{
			if( !_arcBulletQ.empty() )
			{
				newBullet = _arcBulletQ.front();
				_arcBulletQ.pop();
				_bulletList.push_back(newBullet);
			}

			break;
		}
	}

	return newBullet;
}

npc* actorManager::addNpc(npc* npc)
{
	if (npc)
	{
		_actorMap.insert(make_pair(npc->getUID(), npc));
		_npcMap.insert(make_pair(npc->getUID(), npc));
	}

	return npc;
}

enemy* actorManager::addEnemy(enemy* enemy)
{
	if (enemy)
	{
		enemy->mapDataLink(_mapData);
		_actorMap.insert(make_pair(enemy->getUID(), enemy));
		_enemyMap.insert(make_pair(enemy->getUID(), enemy));
	}

	return enemy;
}

void actorManager::destroyActor(UINT uid)
{
	if(_actorMap.find(uid) == _actorMap.end())
		return;

	actorBase* actor = _actorMap[uid];
	
	_actorMap.erase(uid);
	if (actor->getType() == eActor_Enemy)
		_enemyMap.erase(uid);
	else if (actor->getType() == eActor_Npc)
		_actorMap.erase(uid);

	_mapData->deleteActor(uid);

	//SAFE_RELEASE(actor);
	//SAFE_DELETE(actor);
}

actorBase* actorManager::getActor(UINT uid)
{
	actorBase* actor = nullptr;
	if(_actorMap.find(uid) != _actorMap.end())
		actor = _actorMap[uid];

	return actor;
}

npc* actorManager::getNPC(UINT uid)
{
	npc* n = nullptr;
	if(_npcMap.find(uid) != _npcMap.end())
		n = _npcMap[uid];

	return n;
}

enemy* actorManager::getEnemy(UINT uid)
{
	enemy* em = nullptr;
	if(_enemyMap.find(uid) != _enemyMap.end())
		em = _enemyMap[uid];

	return em;
}



void actorManager::firePlayerBullet(POINTF pos, float angle)
{
	POINTF colPos = {};

	_playerBullet = new linearBullet;

	if (angle < PI / 2)
	{
		_playerBullet->init( PLAYER_UID, pos.x, pos.y
							,angle, 20, 40, "player_bullet_fire_R", "player_bullet_pang_R");
		colPos = {pos.x + 270 - 100, pos.y + 100};
	}
	else
	{
		_playerBullet->init( PLAYER_UID, pos.x, pos.y
							,angle, 20, 40, "player_bullet_fire_L", "player_bullet_pang_L");
		colPos = { pos.x + 100, pos.y + 100};
	}

	_playerBullet->setColPos(colPos);
}

void actorManager::fireEnemyBullet(enemy* em)
{
	switch (em->getSubType())
	{
		case eEnemy_Tiktik:
		case eEnemy_Gruzzer:
		{
			break;
		}
		case eEnemy_Primalaspid:
		{
			primalAspid* pa = dynamic_cast<primalAspid*>(em);

			if(pa)
			{
				pa->bulletFire();
				POINTF startPos = pa->getBulletFirePoint();
				float baseAngle = atan2f(PLAYER->getPosY() - startPos.y, PLAYER->getPosX() - startPos.x);
				float angle = 0.f;
				for (int ii = 0; ii < 3; ++ii)
				{
					bullet* bt = createBullet(bullet::eLINEARBULLET);
					if (bt)
					{
						++_uidCount;
						angle = baseAngle + (PI / 4) * (1 - ii);
						bt->init( _uidCount, startPos.x, startPos.y
								 ,angle, 10, 20, "bullet_fire", "bullet_pang");
					}
				}
			}

			break;
		}
		/*
		case eENEMY_MAWLEK:
		{
			mawlek* mk = dynamic_cast<mawlek*>(em);
			if (mk)
			{
				RECT rc = mk->getCollision();
				POINTF pos = mk->getBulletFirePoint();

				mk->bulletFire();

				if (!mk->isAlive())
				{
					bullet* bt = _bulletM->createBullet(eARCBULLET);
					if (bt)
					{
						arcBullet* arc = dynamic_cast<arcBullet*>(bt);

						int value = RND->getFromIntTo(1, 179);
						float angle = (PI / 180) * value;
						value = RND->getFromIntTo(10, 30);

						bt->init(pos, angle, value, 20, "bullet_fire", "bullet_pang");
						arc->setGravity(0.7f);
					}
				}
				else if (mk->isOnBulletFire())
				{
					bullet* bt = _bulletM->createBullet(eARCBULLET);
					if (bt)
					{
						arcBullet* arc = dynamic_cast<arcBullet*>(bt);
						float x = (float)((mk->getPosition().x - (float)_target->getPositionX()) / 2 + _target->getPositionX());
						float y = _target->getPositionY() - 1000;
						float angle = atan2f(pos.y - y, pos.x - x);

						int value = RND->getFromIntTo(10, 15);
						bt->init(pos, angle, value, 20, "bullet_fire", "bullet_pang");
						arc->setGravity(0.5f);
					}
				}
				else
				{
					for (int ii = 0; ii < 50; ++ii)
					{
						bullet* bt = _bulletM->createBullet(eARCBULLET);
						if (bt)
						{
							arcBullet* arc = dynamic_cast<arcBullet*>(bt);
							//float x = (float)abs(mk->getPosition().x - (float)(_target->getPositionX()) * 0.5);
							//float y = _target->getPositionY() - 500;
							//float angle = atan2f(y - pos.y, x - pos.x);
							int value = RND->getFromIntTo(1, 90);
							float angle = 0;
							if (mk->getPosition().x < _target->getPositionX())
								angle = (PI / 180 * (value + 90));
							else
								angle = (PI / 180 * value);

							value = RND->getFromIntTo(10, 30);

							bt->init(pos, angle, value, 20, "bullet_fire", "bullet_pang");
							arc->setGravity(0.7f);
						}
					}
				}

				break;
			}
		}
		*/
	}
}

bool actorManager::checkPlayerBullet()
{
	return (!_playerBullet);
}

void actorManager::hitEnemy(enemy* em, UINT damage)
{
}

void actorManager::hitEnemy(UINT uid, UINT damage)
{
	enemy* em = getEnemy(uid);
	if(em)
	{
		em->takeDamage(damage);
	}
}

bool actorManager::checkHitSomething(bullet* bt)
{
	//RECT collision = { (int)(bt->getPosition().x), (int)(bt->getPosition().y)
	//				  ,(int)(bt->getPosition().x + bt->getRadius()), (int)(bt->getPosition().y + bt->getRadius())};

	RECTD2D rc = bt->getCollision();

	vector<terrain*>* vCol = _mapData->getColliderTerrains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();

	// 지형이랑 충돌
	terrain* ter = nullptr;
	RECTD2D col = {};

	for ( iter; end != iter; ++iter )
	{
		ter = (*iter);
		col = (ter->getCollision());

		if (CheckIntersectRect(rc, col) )
			return true;
	}

		// 플레이어가 맞음
	if ( CheckIntersectRect(rc, PLAYER->getCollision()) )
	{
		PLAYER->takeDamage();
		return true;
	}

	return false;
}

bool actorManager::checkPlayerBulletHitSomething()
{
	POINTF pos = _playerBullet->getColPos();

	RECTD2D collision = _playerBullet->getCollision();

	vector<terrain*>* vCol = _mapData->getColliderTerrains();
	vector<terrain*>::iterator iter = vCol->begin();
	vector<terrain*>::iterator end = vCol->end();

	// 지형이랑 충돌
	terrain* ter = nullptr;
	RECTD2D col = {};

	for (iter; end != iter; ++iter)
	{
		ter = (*iter);
		col = (ter->getCollision());

		// todo 충돌 수정해야하마마암아마암암아맘
		if (CheckIntersectRect(collision, col))
			return true;
	}

	// 적과 충돌
	if (_enemyMap.size() != 0)
	{
		enemy*  em = nullptr;
		RECTD2D emCol = {};
		
		iterE iter = _enemyMap.begin();
		iterE end = _enemyMap.end();

		for (iter; end != iter; ++iter)
		{
			em = iter->second;
			emCol = em->getCollision();

			// todo 충돌 수정해야하마마암아마암암아맘
			if (CheckIntersectRect(collision, emCol))
			{
				hitEnemy(em, PLAYER->getPower());
				return true;
			}
		}
	}

	return false;
}

bool actorManager::checkBulletIntheField(bullet* bt)
{
	// 맵 안에 있는지 체크
	POINTF pos = {bt->getPosX(), bt->getPosY()};
	if( pos.x < 0  || MAPSIZEX < pos.x)
		return false;

	if( pos.y < 0  || MAPSIZEY < pos.y)
		return false;

	return true;
}

list<bullet*>::iterator actorManager::returnBulletQ(iterB it)
{
	bullet* bt = (*it);

	// 돌려준다 bullet을 q에
	bullet::eBULLET_TYPE type = (bullet::eBULLET_TYPE)bt->getSubType();
	if ( bullet::eLINEARBULLET == type )
	{
		linearBullet* linear = dynamic_cast<linearBullet*>(bt);
		if(linear)
			_linearbulletQ.push(linear);
	}
	else if (  bullet::eARCBULLET == type )
	{
		arcBullet* arc = dynamic_cast<arcBullet*>(bt);
		if(arc)
			_arcBulletQ.push(arc);
	}

	it = _bulletList.erase(it);

	return it;
}
