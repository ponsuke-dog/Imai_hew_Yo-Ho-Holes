//	client.h
//	
//
//	2025.02.01			大山凌太朗

#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <chrono>
#include "packet.h"

#pragma comment(lib, "ws2_32.lib")

class Client {
private:
    //サーバーソケット
    SOCKET serverSocket;

    //受信用全プレイヤーデータ
    PlayerData m_AllPlayerData[MAX_PLAYERS];
    //送信用プレイヤーデータ
    PlayerData m_MyPlayerData;

    //自身のプレイヤーID
    int m_MyPlayerId = -1;
    //自身のチームID
    int m_MyTeamId = -1;

    //受信用ゲームシーンデータ
    GameSceneData m_GameSceneData;

    //受信用全マッチングデータ
    MatchingData m_AllMatchingData[MAX_PLAYERS];

    //大砲弾データ
    std::vector<CannonData> m_CannonData;

    //船上アクションオブジェクト
    ActionObjectData m_ActionObjectData[TEAM_COUNT];

    //受信デッキブロックデータ
    std::vector<BlockData> m_BlockData;

    //イベント受信用データ
    EventData m_EventData;

    //受信用リクエストデータ
    UseReqest m_UseReqest;
    //保管リクエストデータ
    UseReqest m_SaveReqest;

    bool m_Action = true;

    //受信用スレッド
    std::thread receiveThread;
    bool isRunning = false;

public:
    Client();
    ~Client();
    bool ConnectToServer(const std::string& serverIp, int port);

    //受信スレッド処理の開始/終了
    void StartReceiveThread();
    void StopReceiveThread();

    //自身プレイヤーデータの送信
    void SendMyPlayerData();

    //自身のマッチングデータの送信
    void SendMyMatchingData();

    //自身のゲームスタートデータの送信
    void SendMyGameStart(GameStart data);

    //大砲弾データの送信
    void SendCannonBallData(CannonData data);

    //Cannon使用データの送信
    void SendCannonObjectData(CannonObject data);

    //Gather使用データの送信
    void SendGatherObjectData(GatherObject data);

    //Enhance使用データ
    void SendEnhanceObjectData(EnhanceObject data);

    //ストック追加データ
    void SendAddStockObjectData(AddStockData data);

    //ストック消費データ
    void SendReduceStockObjectData(ReduceStockData data);

    //ブロックデータの送信
    void SendBlockData(BlockData data);

    //イベントデータ送信
    void SendEventData(EventPlayerData data);

    //使用データの送信
    void SendUseReqest(UseReqest data);

    //データ受信処理
    void ReceiveData();
    void ProcessPacket(const PacketHeader& header);

private:
    bool SafeRecv(SOCKET socket, char* buffer, int size);

public:
    bool SafeSend(SOCKET socket, const void* buffer, size_t dataSize);

    //各種ゲッター
   //プレイヤーID
    const int GetMyPlayerID() const { return m_MyPlayerId; }
    //チームID
    const int GetMyTeamId() const { return m_MyTeamId; }
    //ゲームシーンデータ
    GameSceneData GetGameScene() const { return m_GameSceneData; }
    //自身のプレイヤーデータ
    PlayerData* GetMyPlayerData() { return &m_MyPlayerData; }
    //全プレイヤーデータ
    PlayerData* GetAllPlayerData() { return m_AllPlayerData; }
    //マッチングデータ
    MatchingData* GetAllMatchingData() { return m_AllMatchingData; }
    //大砲弾データ
    std::vector<CannonData>& GetCannnonData() { return m_CannonData; }
    //船上アクションオブジェクト
    ActionObjectData* GetActionObjectData() { return m_ActionObjectData; }
    //受信デッキブロックデータ
    std::vector<BlockData>& GetBlockData() { return m_BlockData; };
    //イベントデータ
    EventData GetEventData() { return m_EventData; }
    //リクエストデータ
    UseReqest GetUseReqest() { return m_UseReqest; }
    UseReqest GetSaveReqest() { return m_SaveReqest; }
    //アクションフラグ
    bool GetActionFlag() { return m_Action; }
    
    //各種セッター
    //送信プレイヤーデータのセット
    void SetMyPlayerData(PlayerData data) { m_MyPlayerData = data; }
    void SetMyPosition(XMFLOAT3 pos) { m_MyPlayerData.position = pos; }
    void SetMyRotation(XMFLOAT3 rot) { m_MyPlayerData.rotation = rot; }
    void SetMyAnimation(AnimationState anim) { m_MyPlayerData.anim = anim; }

    void SetMyMatchingData(MatchingData data) { m_AllMatchingData[m_MyPlayerId] = data; }

    void SetActionFlag(bool action) { m_Action = action; }

    void ResetSaveReqest();
};

//クライアント作成
void CreateClient();
//クライアント削除
void DestroyClient();
//クライアントゲッター
Client* GetClient();

#endif // CLIENT_H
