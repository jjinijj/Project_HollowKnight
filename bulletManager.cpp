#include "stdafx.h"
#include "bulletManager.h"
#include "enemyManager.h"
#include "ObjectManager.h"
#include "player.h"

void bulletManager::init()
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
}

void bulletManager::update()
{
	// 이동중인 bullet
	for ( _iter = _bulletList.begin(); _bulletList.end() != _iter;  )
	{
		bullet* bt = (*_iter);
		
		// 터지는 중인 bullet
		if (bt->isPang())
		{
			if (bt->isAppear())
			{
				bt->update();
				++_iter;
			}
			else
			{
				_iter = returnBulletQ(_iter);
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
			_iter = returnBulletQ(_iter);
		}
		else
		{
			bt->update();
			++_iter;
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

void bulletManager::render()
{
	for ( _iter = _bulletList.begin(); _bulletList.end() != _iter; ++_iter )
	{
		(*_iter)->render();
	}

	if(_playerBullet)
		_playerBullet->render();
}

void bulletManager::release()
{
	for ( _iter = _bulletList.begin(); _bulletList.end() != _iter; ++_iter )
	{
		bullet* bul = *_iter;

		SAFE_RELEASE(bul);
		SAFE_DELETE(bul);
	}

	while ( !_arcBulletQ.empty() )
	{
		arcBullet* arc = _arcBulletQ.front();
		_arcBulletQ.pop();

		SAFE_RELEASE(arc);
		SAFE_DELETE(arc);
	}

	while ( !_linearbulletQ.empty() )
	{
		linearBullet* linear = _linearbulletQ.front();
		_linearbulletQ.pop();
		
		SAFE_RELEASE(linear);
		SAFE_DELETE(linear);
	}

	_bulletList.clear();

	SAFE_RELEASE(_playerBullet);
	SAFE_DELETE(_playerBullet);
}

bullet* bulletManager::createBullet(eBULLET_TYPE type)
{
	bullet* newBullet = nullptr;
	switch ( type )
	{
		case eLINEARBULLET:
		{
			if( !_linearbulletQ.empty() )
			{
				newBullet = _linearbulletQ.front();
				_linearbulletQ.pop();
				_bulletList.push_back(newBullet);
			}

			break;
		}
		case eARCBULLET:
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

void bulletManager::firePlayerBullet(POINTF pos, float angle)
{
	POINTF colPos = {};

	_playerBullet = new linearBullet;

	if (angle < PI / 2)
	{
		_playerBullet->init(pos, angle, 20, 40, "player_bullet_fire_R", "player_bullet_pang_R");
		colPos = {pos.x + 270 - 100, pos.y + 100};
	}
	else
	{
		_playerBullet->init(pos, angle, 20, 40, "player_bullet_fire_L", "player_bullet_pang_L");
		colPos = { pos.x + 100, pos.y + 100};
	}

	_playerBullet->setColPos(colPos);
}

bool bulletManager::checkPlayerBullet()
{
	return (!_playerBullet);
}

bool bulletManager::checkHitSomething(bullet* bt)
{
	RECT collision = { (int)(bt->getPosition().x), (int)(bt->getPosition().y)
					  ,(int)(bt->getPosition().x + bt->getRadius()), (int)(bt->getPosition().y + bt->getRadius())};

	if ( nullptr != _objM )
	{
		lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

		// 벽 or 블록 이랑 충돌
		if ( objList->size() != 0 )
		{
			gameObject* obj = nullptr;
			RECT objCol = {};
			ilObject end = objList->end();

			for ( ilObject iter = objList->begin(); end != iter; ++iter )
			{
				obj = ( *iter );
				objCol = ( obj->getCollision() );

				// todo 충돌 수정해야하마마암아마암암아맘
				if (CheckIntersectRect(collision, objCol) )
				{
					return true;
				}
			}
		}
	}

	if ( _player )
	{
		// 플레이어가 맞음
		if ( CheckIntersectRect(collision, _player->getCollisionRECT()) )
		{
			_player->takeDamage();
			return true;
		}
	}
	return false;
}

bool bulletManager::checkPlayerBulletHitSomething()
{
	POINTF pos = _playerBullet->getColPos();

	RECT collision = { (int)(pos.x - _playerBullet->getRadius())
					 , (int)(pos.y - _playerBullet->getRadius())
					 , (int)(pos.x + _playerBullet->getRadius())
					 , (int)(pos.y + _playerBullet->getRadius()) };

	if (nullptr != _objM)
	{
		lObject* objList = _objM->getObjectList(eOBJECT_GROUND);

		// 벽 or 블록 이랑 충돌
		if (objList->size() != 0)
		{
			gameObject* obj = nullptr;
			RECT objCol = {};
			ilObject end = objList->end();

			for (ilObject iter = objList->begin(); end != iter; ++iter)
			{
				obj = (*iter);
				objCol = (obj->getCollision());

				// todo 충돌 수정해야하마마암아마암암아맘
				if (CheckIntersectRect(collision, objCol))
				{
					return true;
				}
			}
		}
	}

	if (nullptr != _enemyM)
	{
		const lEnemy& emList = _enemyM->getEnemyList();

		// 적과 충돌
		if (emList.size() != 0)
		{
			enemy* em = nullptr;
			RECT emCol = {};
			cilEnemy end = emList.end();

			for (cilEnemy iter = emList.begin(); end != iter; ++iter)
			{
				em = (*iter);
				emCol = (em->getCollision());

				// todo 충돌 수정해야하마마암아마암암아맘
				if (CheckIntersectRect(collision, emCol))
				{
					_enemyM->hitEnemy(em->getUid());
					return true;
				}
			}
		}
	}

	return false;
}

bool bulletManager::checkBulletIntheField(bullet* bt)
{
	// 맵 안에 있는지 체크
	POINTF pos = bt->getPosition();
	if( pos.x < 0  || MAPSIZEX < pos.x)
		return false;

	if( pos.y < 0  || MAPSIZEY < pos.y)
		return false;

	return true;
}

list<bullet*>::iterator bulletManager::returnBulletQ(list<bullet*>::iterator it)
{
	bullet* bt = (*it);

	// 돌려준다 bullet을 q에
	eBULLET_TYPE type = bt->getBulletType();
	if ( eLINEARBULLET == type )
	{
		linearBullet* linear = dynamic_cast<linearBullet*>(bt);
		if(linear)
			_linearbulletQ.push(linear);
	}
	else if ( eARCBULLET == type )
	{
		arcBullet* arc = dynamic_cast<arcBullet*>(bt);
		if(arc)
			_arcBulletQ.push(arc);
	}

	it = _bulletList.erase(it);

	return it;
}
