
#ifndef __HeroDataAndOperate_h__
#define __HeroDataAndOperate_h__
#pragma once

class HeroObj
{
public:
	HeroObj();
	~HeroObj();

	bool InitHero(int nTblIndex, int nStarLevel = 1, int nLevel = 1);


private:
	HeroDynamicData m_HeroDynamicData;
};


#endif // __HeroDataAndOperate_h__
