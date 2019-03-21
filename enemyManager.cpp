#include "stdafx.h"
#include "enemyManager.h"
#include "ObjectManager.h"
#include "bulletManager.h"
#include "tiktik.h"
#include "gruzzer.h"
#include "primalAspid.h"
#include "mawlek.h"
#include "player.h"

HRESULT enemyManager::init()
{
	_enemyCnt		= 0;
	_isExistBoss	= false;

	return S_OK;
}

void enemyManager::update()
{
	if(!checkAllLinkedFinish())
		return;


	for ( _iter = _enemyList.begin(); _enemyList.end() != _iter; ++_iter )
	{
		enemy* em = (*_iter);
		em->update();
		// ÃÑ¾Ë¹ß½Î
		if (em->isFire())
			bulletFire(em);
	}

	for ( _iter = _deadEnemyList.begin(); _deadEnemyList.end() != _iter;)
	{
		enemy* em = (*_iter);
		if ( !em->isAppear() )
		{
			em->release();
			_iter = _deadEnemyList.erase(_iter);
		}
		else
		{
			em->update();
			if (eENEMY_MAWLEK == em->getEnemyType())
			{
				if (em->isFire())
					bulletFire(em);
			}
			
			++_iter;
		}
	}

	if ( _enemyList.size() < ENEMY_COUNT_IN_STAGE )
	{
		if ( _enemyCnt < 10 )
		{
			// ¹°·º(º¸½º)¸¦ Á¦¿ÜÇÑ
			eENEMY_TYPE type = (eENEMY_TYPE)RND->getInt((int)eENEMY_MAWLEK);
			//eENEMY_TYPE type = eENEMY_PRIMALASPID;
			createEnemy(type);
		}
		else if( !_isExistBoss )
			createEnemy(eENEMY_MAWLEK);
	}
}

void enemyManager::render()
{
	if (!checkAllLinkedFinish())
		return;

	if ( _isDebugMode )
	{
		WCHAR str[128];
		swprintf_s(str, L"[enemyCnt : %d / %d] [curEnemy : %zd] ", _enemyCnt, ENEMY_COUNT_NEED_EXIST_BOSS, _enemyList.size());
		D2DMANAGER->drawTextD2D(D2DMANAGER->_defaultBrush, L"³ª´®°íµñ", 15.0f
								, str
								, CAMERA->getPosX() + 10
								, CAMERA->getPosY() + 170
								, CAMERA->getPosX() + 500
								, CAMERA->getPosY() + 1000);
	}


	for ( _iter = _enemyList.begin(); _enemyList.end() != _iter; ++_iter )
	{
		(*_iter)->render();
	}

	for ( _iter = _deadEnemyList.begin(); _deadEnemyList.end() != _iter; ++_iter)
	{
		(*_iter)->render();
	}
}

void enemyManager::release()
{
	for ( _iter = _enemyList.begin(); _enemyList.end() != _iter; ++_iter )
	{
		(*_iter)->release();
	}

	for ( _iter = _deadEnemyList.begin(); _deadEnemyList.end() != _iter; ++_iter )
	{
		(*_iter)->release();
	}

	_enemyList.clear();
	_deadEnemyList.clear();
	_objM		= nullptr;
	_bulletM	= nullptr;
	_target		= nullptr;
}

void enemyManager::createEnemy(eENEMY_TYPE type)
{
	bool isCreate = false;
	switch ( type )
	{
		case eENEMY_TIKTIK:
		{	
			if(nullptr != _objM)
			{
				lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

				if ( objList->size() != 0 )
				{
					// ¸Ê Å×µÎ¸®¸¦ Á¦¿ÜÇÑ ¶¥¿¡¼­¸¸ »ý¼º
					int idx = RND->getInt(objList->size() - 8);

					gameObject* obj = nullptr;
					RECT objCol = {};
					ilObject end = objList->end();

					for ( ilObject iter = objList->begin(); end != iter, 0 <= idx; ++iter, --idx )
					{
						obj = ( *iter );
						objCol = ( obj->getCollision() );
					}

					if ( obj )
					{
						enemy* em = new tiktik;
						POINTF pos;

						pos.x = (float)RND->getFromIntTo(objCol.left, objCol.right);
						pos.y = (float)objCol.top;

						em->setManagerLink(_objM);
						em->init(pos, _enemyCnt);

						_enemyList.push_back(em);
					}

					isCreate = true;
				}
			}

			break;
		}
		case eENEMY_GRUZZER:
		{
			POINTF position;
			position.x = (float)RND->getFromIntTo(100, MAPSIZEX - 100);
			position.y = (float)RND->getFromIntTo(100, MAPSIZEY - 100);

			enemy* em = new gruzzer;
			em->init(position, _enemyCnt);

			em->setManagerLink(_objM);
			_enemyList.push_back(em);

			break;
		}
		case eENEMY_PRIMALASPID:
		{
			POINTF position;
			position.x = (float)RND->getFromIntTo(100, MAPSIZEX - 100);
			position.y = (float)RND->getFromIntTo(100, MAPSIZEY - 100);

			primalAspid* em = new primalAspid;
			em->init(position, _enemyCnt);

			em->setManagerLink(_objM);
			em->setPlayerLink(_target);
			em->setBulletMLink(_bulletM);
			em->setBulletImgNames("bullet_fire","bullet_pang");

			_enemyList.push_back(em);

			break;
		}
		case eENEMY_MAWLEK:
		{
			if( _isExistBoss )
				return;

			POINTF position;
			position.x = 1000;
			position.y = 684;

			mawlek* mk = new mawlek;
			mk->init(position, _enemyCnt);

			mk->setManagerLink(_objM);
			mk->setPlayerLink(_target);

			_enemyList.push_back(mk);

			_isExistBoss = true;
			break;
		}
	}

	++_enemyCnt;
}

void enemyManager::bulletFire(enemy* em)
{
	switch (em->getEnemyType())
	{
		case eENEMY_TIKTIK:
		case eENEMY_GRUZZER:
		{
			break;
		}
		case eENEMY_PRIMALASPID:
		{
			primalAspid* pa = dynamic_cast<primalAspid*>(em);
			
			if(pa)
			{
				pa->bulletFire();
				POINTF startPos = pa->getBulletFirePoint();
				float baseAngle = atan2f(_target->getPositionY() - startPos.y, _target->getPositionX() - startPos.x);
				float angle = 0.f;
				for (int ii = 0; ii < 3; ++ii)
				{
					bullet* bt = _bulletM->createBullet(eLINEARBULLET);
					if (bt)
					{
						angle = baseAngle + (PI / 4) * (1 - ii);
						bt->init(startPos, angle, 10, 20, "bullet_fire", "bullet_pang");
					}
				}
			}

			break;
		}
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
	}
}

void enemyManager::removeEnemy(int uid)
{
	for ( _iter = _enemyList.begin(); _enemyList.end() != _iter; ++_iter )
	{
		enemy* em = *_iter;
		if( em->getUid() != uid )
			continue;

		em->release();
		_enemyList.remove(em);

		break;
	}
}

void enemyManager::hitEnemy(int uid)
{
	enemy* em = findEnemy(uid);
	if(nullptr == em )
		return;

	em->takeDamage();

	if ( !em->isAlive() )
	{
		em->dead();
		_deadEnemyList.push_back(em);
		_enemyList.remove(em);

		int value = RND->getFromIntTo(1, 5);
		POINTF pos = em->getPosition();
		for (int ii = 0; ii < value; ++ii)
		{
			_objM->createGameObject(pos.x, pos.y, eOBJECT_COIN);
		}
	}
}

enemy* enemyManager::findEnemy(int uid)
{
	for ( _iter = _enemyList.begin(); _enemyList.end() != _iter; ++_iter )
	{
		if( (*_iter)->getUid() == uid )
			return *_iter;
	}

	return nullptr;
}
