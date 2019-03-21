#include "stdafx.h"
#include "tiktik.h"
#include "terrain.h"

HRESULT tiktik::init(UINT uid, float x, float y)
{
	enemy::init(uid, x, y);
	{
		image* img = IMAGEMANAGER->findImage("tiktik_move");
		ANIMANAGER->addArrayFrameAnimation( _uid, eMOVE_ON, "tiktik_move"
										   ,0, img->GetMaxFrameX()
										   ,TIKTIK_ANI_SPEED, true);
	}
	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_under");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_UNDER, "tiktik_move_under"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_sideup");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_SIDE_UP, "tiktik_move_sideup"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_move_sidedown");
		ANIMANAGER->addArrayFrameAnimation(_uid, eMOVE_SIDE_DOWN, "tiktik_move_sidedown"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_stu");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIMB_SIDE_TO_ON, "tiktik_climbup_stu"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbup_uts");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIME_UNDER_TO_SIDE, "tiktik_climbup_uts"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_stu");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIMB_SIDE_TO_UNDER, "tiktik_climbup_uts"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_climbdown_ots");
		ANIMANAGER->addArrayFrameAnimation(_uid, eCLIME_ON_TO_SIDE, "tiktik_climbdown_ots"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	{
		image* img = IMAGEMANAGER->findImage("tiktik_dead");
		ANIMANAGER->addArrayFrameAnimation(_uid, eDEAD, "tiktik_dead"
										   , 0, img->GetMaxFrameX()
										   , TIKTIK_ANI_SPEED, true);
	}

	_hp = TIKTIK_MAX_HP;
	_speed = TIKTIK_MOVE_SPEED;

	_subType = eEnemy_Tiktik;
	_collision = {   _x - TIKTIK_WIDTH / 2, _y - TIKTIK_HIEGHT
					,_x + TIKTIK_WIDTH / 2, _y};
	setActiveArea();

	return S_OK;
}

void tiktik::update()
{
	enemy::update();
}

void tiktik::move()
{
	if(nullptr == _area)
		return;

	switch ( _state->getState() )
	{
		case eMOVE_ON:
		{
			bool isChange = false;
			if ( eRIGHT == _dir )
			{
				_x += _speed;
				isChange =  ( _area->getCollision().right < _x );
			}
			else
			{
				_x -= _speed;
				isChange =  ( _x < _area->getCollision().left );
			}

			if(isChange)
			{
				//changeState(eCLIME_ON_TO_SIDE);
				_dirUD = eDOWN;
			}

			break;
		}

		case eMOVE_UNDER:
		{
			bool isChange = false;
			if ( eRIGHT == _dir )
			{
				_x += _speed;
				isChange = _area->getCollision().right < _x;
			}
			else
			{
				_x -= _speed;
				isChange = _x < _area->getCollision().left;
			}

			if ( isChange )
			{
				//changeState(eCLIME_UNDER_TO_SIDE);
				_dirUD = eUP;
			}

			break;
		}


		case eMOVE_SIDE_UP:
		{
			bool isChange = false;

			_y -= _speed;
			isChange =  _y < _area->getCollision().top + (_colSize.y / 2);

			if ( isChange )
			{
				if ( eRIGHT == _dir )
					_dir = eLEFT;
				else
					_dir = eRIGHT;

				//changeState(eCLIMB_SIDE_TO_ON);
			}

			break;
		}

		case eMOVE_SIDE_DOWN:
		{
			bool isChange = false;
			_y += _speed;
			isChange = _area->getCollision().bottom + (_colSize.y / 2) < _y;

			if ( isChange )
			{
				if ( eRIGHT == _dir )
					_dir = eLEFT;
				else
					_dir = eRIGHT;

				//changeState(eCLIMB_SIDE_TO_UNDER);
			}

			break;
		}

		case eCLIMB_SIDE_TO_ON:
		{
			if(eRIGHT == _dir)
				_x += _speed;
			else
				_x -= _speed;

			_y -= _speed;


			//if ( _anim )
				//if ( !_anim->IsPlayingAnimation() )
					//changeState(eMOVE_ON);
			break;
		}

		case eCLIMB_SIDE_TO_UNDER:
		{
			if(eRIGHT == _dir)
				_x += _speed;
			else
				_x -= _speed;

			_y += _speed;

			//if( _anim )
			//	if(!_anim->IsPlayingAnimation())
			//		changeState(eMOVE_UNDER);
			break;
		}

		case eCLIME_ON_TO_SIDE:
		{
			if(eRIGHT == _dir)
				_x += _speed;
			else
				_x -= _speed;

			_y += _speed;

			//if ( _anim )
			//	if ( !_anim->IsPlayingAnimation() )
			//		changeState(eMOVE_SIDE_DOWN);

			break;
		}

		case eCLIME_UNDER_TO_SIDE:
		{
			if(eRIGHT == _dir)
				_x += _speed;
			else
				_x -= _speed;

			_y -= _speed;

			//if( _anim )
			//	if(!_anim->IsPlayingAnimation())
			//		changeState(eMOVE_SIDE_UP);
			break;
		}
	}
}

//void tiktik::dead()
//{
//	enemy::dead();
//	changeState(eDEAD);
//}

void tiktik::setActiveArea()
{
	//if(nullptr == _objM )
	//	return;
	//
	//lObject* objList = _objM->getObjectList(eOBJECT_GROUND);
	//
	//if(objList->size() == 0)
	//	return;
	//
	//bool isFloating = true;
	//int offsetX = 0;
	//int offsetY = 0;
	//
	//gameObject* obj = nullptr;
	//RECT objCol = {};
	//ilObject end = objList->end();
	//
	//for ( ilObject iter = objList->begin(); end != iter; ++iter )
	//{
	//	obj		= (*iter);
	//	objCol	= ( obj->getCollision() );
	//
	//	if( !CheckIntersectRect(_collision, objCol) )
	//		continue;
	//
	//	int offsetY = _collision.bottom - objCol.top;
	//	_y -= offsetY;
	//
	//	_area = obj;
	//
	//	break;
	//}
}