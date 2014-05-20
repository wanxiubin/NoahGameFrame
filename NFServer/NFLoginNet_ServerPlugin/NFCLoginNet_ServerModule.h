// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_LOGINNET_SERVER_MODULE_H_
#define _NFC_LOGINNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIActor.h"

#define NET_MSG_PROCESS(xNFMsg, msg) \
    int32_t nPlayerID = 0; \
    xNFMsg xMsg; \
    if (!Recive(msg, xMsg, nPlayerID)) \
    { \
    return 0; \
    } \
    \
    NFIActorMessage xActorMsg; \
    xActorMsg.eType = NFIActorMessage::EACTOR_NET_MSG; \
    xActorMsg.nSubMsgID = msg.GetMsgHead().unMsgID; \
    xMsg.SerializeToString(&xActorMsg.data); \
    pPluginManager->GetFramework().Send(xActorMsg, pPluginManager->GetAddress(), pPluginManager->GetAddress());

class NFCLoginNet_ServerModule
	: public NFILoginNet_ServerModule

{
public:
	NFCLoginNet_ServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute(const float fLasFrametime, const float fStartedTime);


	virtual bool BeforeShut();
	virtual bool AfterInit();

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

	virtual void SynWorldToClient();


protected:
	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const uint16_t nSockIndex, const NF_NET_EVENT eEvent);

protected:
	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const uint16_t& nAddress);
	//������
	void OnClientConnected(const uint16_t& nAddress);
	//���� 
	int OnLoginProcess(const NFIPacket& msg);
	//�ǳ�
	int OnLogoutProcess(const NFIPacket& msg);
	//ѡ�������
	int OnSelectWorldProcess(const NFIPacket& msg);
	//����鿴�����б�
	int OnViewWorldProcess(const NFIPacket& msg);
	//////////////////////////////////////////////////////////////////////////
	//��¼���
	int OnLoginResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);
	//�ǳ����
	int OnLogOutResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);
	//ѡ���������(����key�ȸ��ͻ���)
	int OnSelectWorldResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

protected:
	//����������¼�
	int OnWorldServerObjectEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var);

protected:
	int GetConnectObjectByAddress(const int nAddress, NFIValueList& varObjectList);
	void SynWorldToClient(const int nFD);


private:
	int mnClientConnectContainer;
	int mnWorldContainerID;

	NFIEventProcessModule* m_pEventProcessModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIElementInfoModule* m_pElementInfoModule;	
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	NFILoginLogicModule* m_pLoginLogicModule;
};

#endif