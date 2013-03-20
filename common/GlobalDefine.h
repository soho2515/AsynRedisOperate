//----------------------------------------------------------------------------------------------------------------
//	π´”√∂®“ÂŒƒº�?
//
//----------------------------------------------------------------------------------------------------------------

#ifndef __GLOBALDEFINE_H__
#define __GLOBALDEFINE_H__

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <map>
#include <list>
#include <algorithm>
#include <set>

#define  DEFAULT_HEARTBEAT_TIME   1000*30    //ms-->30s

#define	PI					3.141592654f
#define	PI2					(PI*2)
#define	PI_HALF				(PI*0.5f)
#define	RAD					(PI/180.f)
#define	ANG_TO_RAD(x)		((x)*RAD)
#define	RAD_TO_ANG(x)		((x)*(1/RAD))

#define MAX_IP_STR_LEN      16
#define MAX_NAME_LEN		256

typedef int Guid_t;
typedef int ID_t;
#define invalid_guid  (-1)

struct PlayerInfo{
	void clear(){
		nGuid = -1;
		nParam = 0;
	}
	Guid_t nGuid;
	int nParam;
};

#endif // __GLOBALDEFINE_H__

