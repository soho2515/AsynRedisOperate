/*
	文件	：DBCommandCallbackFactory.h
	简介	：DB命令工厂类定义，DB命令池管理类
	          通过池，可以尽量保证运行前不断执行命令时不频繁操作内存
			  通过工厂，可以根据实际情况产生不同的命令
			  通过模板实现了工厂类的具体实现，只需在初始化时new出来即可了
	作者	：侯志芳
	创建日期：2013/02/19

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __DBCOMMAND_CALLBACK_FACTORY_H__
#define __DBCOMMAND_CALLBACK_FACTORY_H__

#include "DBCommandCallBack.h"
#include "ServerDefine.h"
#include "lock.h"
#include <stdlib.h>

#define DBCOMMAND_CALLBACK_CACHE_SIZE   20
#define DBCOMMAND_CALLBACK_CACHE_ADD_SIZE    10

//这个DBCommandCallback池应该做成线程安全的，需要检查跨线程处理时的隐患问题
template<class T>
class DBCommandCallbackCache
{
public:
	DBCommandCallbackCache():_DBCommandCache(NULL),/*_Flag(NULL),*/_nextIdle(-1),_nowPoolSize(0),_nowUsedSize(0){};
	~DBCommandCallbackCache(){};

	int InitPool(){
		int nRet = 0;
		if (_DBCommandCache == NULL)
		{
			_DBCommandCache = (T**)malloc(DBCOMMAND_CALLBACK_CACHE_SIZE*sizeof(T*));
			//_Flag = (BOOL*)malloc(DBCOMMAND_CACHE_SIZE*sizeof(BOOL));
			if (_DBCommandCache == NULL/* || _Flag == NULL*/)
			{
				return 0;
			}

			for (int i = 0; i < DBCOMMAND_CALLBACK_CACHE_SIZE; i++)
			{
				_DBCommandCache[i] = new T;
				//_Flag[i] = FALSE;
				_DBCommandCache[i]->SetStatus(DBCOMMAND_CALLBACK_STATUS_IDLE);
			}
			_nowPoolSize = DBCOMMAND_CALLBACK_CACHE_SIZE;
		}
		_nextIdle = 0;
		nRet = _nowPoolSize;
		return nRet;
	}

	void UnInitPool(){
		if (_DBCommandCache != NULL)
		{
			for (INT i = 0; i < _nowPoolSize; i++)
			{
				delete _DBCommandCache[i];
				_DBCommandCache[i] = NULL;
			}
			free(_DBCommandCache);
			_DBCommandCache = NULL;
			/*free(_Flag);
			_Flag = NULL;*/
		}
		_DBCommandCache = NULL;
		//_Flag = NULL;
		_nextIdle = -1;
		_nowPoolSize = 0;
		_nowUsedSize = 0;
	}

	T* GetIdleDBCommandCallback(){
		if (_nowPoolSize <= 0)
		{
			return NULL;
		}
		T* pRet = NULL;
		//预测空间
		//CSmartLock lock(m_lock);              // 分配和销毁都将在主线程中完成，这里去掉锁
		if (_nowUsedSize >= _nowPoolSize-1)
		{
            T** pNewCache = (T**)realloc(_DBCommandCache, (_nowPoolSize+DBCOMMAND_CALLBACK_CACHE_ADD_SIZE)*sizeof(T*));
			
			if (pNewCache == NULL)
			{
				return NULL;
			}

            _DBCommandCache = pNewCache;
			
			for (INT i = _nowPoolSize; i<_nowPoolSize+DBCOMMAND_CALLBACK_CACHE_ADD_SIZE;i++)
			{
				_DBCommandCache[i] = new T;
				//_Flag[i] = FALSE;
			}

			_nowPoolSize += DBCOMMAND_CALLBACK_CACHE_ADD_SIZE;
		}

		//得到今次使用的空间
		if (_nextIdle < 0 || _nextIdle >= _nowPoolSize)
		{
			return NULL;
		}
		pRet = _DBCommandCache[_nextIdle];
		if (pRet == NULL)
		{
			CacheLog_DEBUG(LOG_ERROR, "_DBCommandCache in callback get a NULL:％ｄ", _nextIdle);
		}
		_DBCommandCache[_nextIdle]->SetStatus(DBCOMMAND_CALLBACK_STATUS_INIT);
		//_Flag[_nextIdle] = TRUE;
		++_nowUsedSize;
		//assert(pRet != NULL && _nowUsedSize < _nowPoolSize);

		//计算下次使用位置
		bool bFind = false;
		for (INT i = _nextIdle+1; i< _nowPoolSize; i++)
		{
			if (_DBCommandCache[i]->GetStatus() == DBCOMMAND_CALLBACK_STATUS_IDLE)
			{
				_nextIdle = i;
				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			for (INT i = 0; i < _nextIdle; i++)
			{
				if (_DBCommandCache[i]->GetStatus() == DBCOMMAND_CALLBACK_STATUS_IDLE)
				{
					_nextIdle = i;
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			_nextIdle = -1;
			//非常严重的日志，不可能找不到的。
		}

		return pRet;
	}

	bool RecycleDBCommandCallback(DBCommandCallBack* pCommand){
		if (pCommand == NULL)
		{
			return false;
		}

		//CSmartLock lock(m_lock);
		for (INT i = 0; i < _nowPoolSize; i++)
		{
			if (/*_Flag[i] && */pCommand == _DBCommandCache[i])
			{
				_DBCommandCache[i]->CleanUp();
				_DBCommandCache[i]->SetStatus(DBCOMMAND_CALLBACK_STATUS_IDLE);
				_nowUsedSize--;
				//_Flag[i] = FALSE;
				break;
			}
		}
		return true;
	}

private:
	T** _DBCommandCache;
	//BOOL* _Flag;
	int _nextIdle;
	int _nowPoolSize;
	int _nowUsedSize;
	CLock m_lock;
};

class DBCommandCallBackFactoryBase
{
public:
	DBCommandCallBackFactoryBase(DBCOMMAND_CALLBACK_TYPE type)
		:m_FactoryType(type){};
	virtual ~DBCommandCallBackFactoryBase(){};

	DBCOMMAND_CALLBACK_TYPE GetCallbackType(){
		return m_FactoryType;
	}

	virtual DBCommandCallBack* CreateDBCommandCallback() = 0;

	virtual bool DestoryDBCommandCallBack(DBCommandCallBack* command) = 0;

protected:
	DBCOMMAND_CALLBACK_TYPE m_FactoryType;

	//这俩函数只让友元类DBCommandFactoryManager调用
	virtual bool InitFactory() = 0;
	virtual void UnInitFactory() = 0;

	friend class DBCommandCallbackFactoryManager;
};

template<class DBCommandCallBackClass>
class DBCommandCallBackFactory : public DBCommandCallBackFactoryBase
{
public:
	DBCommandCallBackFactory(DBCOMMAND_CALLBACK_TYPE type)
		:DBCommandCallBackFactoryBase(type){
	};
	virtual ~DBCommandCallBackFactory(){};

	virtual DBCommandCallBack* CreateDBCommandCallback(){
		DBCommandCallBack* pCommand = _CommandCache.GetIdleDBCommandCallback();   //_CommandCache内部保证了支持线程安全性。
		if (pCommand)
		{
			pCommand->CleanUp();
		}
		else
		{
			CacheLog_DEBUG(LOG_ERROR, "DBCommandCallBackFactory _CommandCache.GetIdleDBCommand() error");
			Assert(false);
		}
		//返回出去后，继续构造这个命令
		return pCommand;
	}

	virtual bool DestoryDBCommandCallBack(DBCommandCallBack* command){
		bool bRet = true;
		bRet = _CommandCache.RecycleDBCommandCallback(command);
		return bRet;
	}

protected:
	DBCommandCallbackCache<DBCommandCallBackClass> _CommandCache;

	virtual bool InitFactory(){
		bool bRet = true;
		int nPoolSize = _CommandCache.InitPool();
		if (nPoolSize <= 0)
		{
			bRet = false;
		}

		return bRet;
	}

	virtual void UnInitFactory(){
		_CommandCache.UnInitPool();
	}
};

#endif   // __DBCOMMAND_CALLBACK_FACTORY_H__
