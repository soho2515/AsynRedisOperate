#pragma once

#include <vector>
#include "DataDefine.h"
#include "BaseType.h"
#include "ds_bitset.h"
#include "Obj.h"
#include "Player.h"
#include "ClientConnection.h"


class Packet;
enum enum_PBASE_DATA_FLAG
{
	BASE_DATA_FLAG_Gold = 0,
	BASE_DATA_FLAG_Energy,
	BASE_DATA_FLAG_MaxGold,
	BASE_DATA_FLAG_MaxEnergy,
	BASE_DATA_FLAG_WorkingBuilderCount,
	BASE_DATA_FLAG_BuilderCount,
	BASE_DATA_FLAG_Count,
};

template <typename PacketType>
class SendPacketToVisitor
{
private:
	PacketType* _packet;
public:
	SendPacketToVisitor( PacketType* packet ) : _packet ( packet )
	{

	}
	//void operator ( ) ( VisitorOnClient theVisitor )
	void operator ( ) ( const VisitorOnClient& theVisitor )
	{
		__ENTER_FUNCTION__
			if (theVisitor.pClientConn != NULL){
				_packet->visitorGuid(theVisitor.nVisitorGuid);
				theVisitor.pClientConn->SendMsg(_packet);
			}
			__LEAVE_FUNCTION__
	}
};

class PersonalBase : public Obj
{
public:
	PersonalBase(void);
	~PersonalBase(void);

	bool load(const PersonalBaseInfo& personalbaseinfo);
	void BeginNewVistor(ClientConnection* pClientConnectoin, Guid_t nVisitorGuid);
	virtual void HeartBeat(float nowTime, float dt);

	void logicBuild( Building &b, LONGLONG llnow );    

	void OnBuildingBuildCompleted( Building &b );
	void logicUpgrade( Building &b, LONGLONG llnow );
	void logicProduce( Building &b, LONGLONG llnow );
	void logicResourceCollect( Building &b, LONGLONG llnow );

	void OnBuildingUpgradeCompleted( Building &b );

	void OnBuildingEffect( Building &b );

	void onAchievementProgressChanged( int nAchieveId, int nProgress );

	void FillBuildingTableCfg(Building* pData, int nTblId, int nLevel);

	void CreateInitBase();
	int GetBuildingCount();
	Building* GetBuidingByIndex(int idx);
	Building* GetBuildingById(int nObjId);
	Building* CreateNewBuilding(int nTblId, int nTag, Guid_t nHelperGuid);

	std::vector< std::vector<_TreasureData> > GetAllTreasures();
    std::map<ID_t, int> getAllTreasures();
	void AddTreasure(ID_t nTreasureId);
    void RemoveTreasure(ID_t nTreasureId);
    //void AddTreasure(int nBagIdx, ID_t nTreasureId);
    void OnTreasureChanged();
	void HireFriend( Guid_t nHelperGuid );
	bool CanHireFriend( Guid_t nHelperGuid );

	Building* CreateInitBuilding(int nTblId);  
	Building* FindBuidingByTag(int nTag);
	bool DestroyBuildingByID(int nObjId);

	Guid_t GetOwnerGUID() const { return m_nOwnerGUID; }
	void SetOwnerGUID(const Guid_t nID) { m_nOwnerGUID = nID; }

 //   template <typename PacketType>
	//bool	BroadcastMsg(PacketType* pPacket);

	template <typename PacketType>
	bool BroadcastMsg( PacketType* pPacket )
	{
		__ENTER_FUNCTION__
			std::for_each(m_lstVisitors.begin(), m_lstVisitors.end(), SendPacketToVisitor<PacketType>(pPacket));
		return true;
		__LEAVE_FUNCTION__
	}

	void	AddVisitor(Guid_t playerguid, ClientConnection* pConnection);
	void	RemoveVisitor(Guid_t playerguid, ClientConnection* pConnection);
	void	RemoveVisitorFromConnection(ClientConnection * pConnection);
	ClientConnection* GetOwnerClientConnection() const;

	int GetHallLevel() const { return m_PersonalBaseInfo.m_nHallLevel; }

	int GetGemCount() const{ return m_PersonalBaseInfo.m_nGemCount; };
	bool CostGem(int nVal);

	int GetEnergy() const { return m_PersonalBaseInfo.m_nEnergy; }
	void SetEnergy(int val) { m_PersonalBaseInfo.m_nEnergy = val; }
	int GetGoldCoin() const { return m_PersonalBaseInfo.m_nGoldCoin; }
	void SetGoldCoin(int val) { m_PersonalBaseInfo.m_nGoldCoin = val; }
	int GetMaxEnergy() const { return m_PersonalBaseInfo.m_nMaxEnergy; }
	void SetMaxEnergy(int val) { m_PersonalBaseInfo.m_nMaxEnergy = val; }
	int GetMaxGoldCoin() const { return m_PersonalBaseInfo.m_nMaxGoldCoin; }
	void SetMaxGoldCoin(int val) { m_PersonalBaseInfo.m_nMaxGoldCoin = val; }

	bool AddGold(int nVal);
	bool AddEnergy(int nVal);
	bool CostGold(int nVal);
	bool CostEnergy(int nVal);

	void AddGoldToWareHouse(int nVal);
	void CostGoldFromWarehouse(int nVal);
	void AddEnergyToWareHouse(int nVal);
	void CostEnergyFromWarehouse(int nVal);

	int GetBuilderCount() const { return m_PersonalBaseInfo.m_nBuilderCount; }
	void SetBuilderCount(int val);
	int GetWorkingBuilderCount() const { return m_PersonalBaseInfo.m_nWorkingBuilderCount; }
	void SetWorkingBuilderCount(int val);

	void SetDirtyFlag(enum_PBASE_DATA_FLAG flag);

private:
	void ReCalcGoldCapacity();
	void ReCalcEnergyCapacity();

private:
	int						m_nMaxBuildingObjID;	
	int					m_nId;
	Guid_t				m_nOwnerGUID;
	//std::list<Guid_t>	m_lstVisitors; // 当前所有访问此基地的玩家，包括主人
	typedef std::list<VisitorOnClient>	VisitoContainer;
	std::list<VisitorOnClient>	m_lstVisitors; // 当前所有访问此基地的玩家，包括主人

	DsBitSet_T<BASE_DATA_FLAG_Count> m_DirtyFlagsForClient;

	PersonalBaseInfo m_PersonalBaseInfo;
	std::vector<Building *> m_vecBuildings;

	LONGLONG				m_llLatestUpdateTime;
};
