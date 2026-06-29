//	client.h
//	
//
//	2025.02.01			大山凌太朗

#include "deck.h"
#include "client.h"

static Client* g_Client = nullptr;

Client::Client() : serverSocket(INVALID_SOCKET){
    m_MyPlayerId = -1;
    m_MyTeamId = -1;

    m_GameSceneData.serverstate = SERVERSTATE_CONNECT;
    m_GameSceneData.servergame = SERVERGAME_READY;
    m_GameSceneData.frame = 0;
    m_GameSceneData.time = 0;

    m_CannonData.reserve(6);
    m_BlockData.reserve(MAP_MAX_NUM * TEAM_COUNT);
}

Client::~Client() {

    StopReceiveThread();
    closesocket(serverSocket);
    WSACleanup();
}

bool Client::ConnectToServer(const std::string& serverIp, int port) {
    //Winsock軌道
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        //軌道失敗
        return false;
    }

    //ソケット作成
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        //ソケット作成失敗
        return false;
    }

    //サーバー接続
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    

    if (connect(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        //サーバー接続失敗
        return false;
    }

    //ノンブロッキングモード設定
    u_long mode = 1;
    ioctlsocket(serverSocket, FIONBIO, &mode);

    return true;
}

void Client::StartReceiveThread()
{
    //受信スレッド処理の開始
    isRunning = true; //スレッド開始フラグ
    receiveThread = std::thread(&Client::ReceiveData, this);
}

void Client::StopReceiveThread()
{
    isRunning = false; // スレッド終了フラグ
    if (receiveThread.joinable()) {
        receiveThread.join(); // スレッド終了待機
    }
}


void Client::SendMyPlayerData()
{
    PacketHeader header;
    header.type = PACKET_PLAYERDATA;
    header.dataSize = sizeof(PlayerData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &m_MyPlayerData, sizeof(PlayerData))) return;

}

void Client::SendMyMatchingData()
{
    PacketHeader header;
    header.type = PACKET_MATCHING_DATA;
    header.dataSize = sizeof(MatchingData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &m_AllMatchingData[m_MyPlayerId], sizeof(MatchingData))) return;
}

void Client::SendMyGameStart(GameStart data)
{
    PacketHeader header;
    header.type = PACKET_GAMESTART_DATA;;
    header.dataSize = sizeof(GameStart);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(GameStart))) return;
}

void Client::SendCannonBallData(CannonData data)
{
    PacketHeader header;
    header.type = PACKET_CANNON_DATA;
    header.dataSize = sizeof(CannonData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(CannonData))) return;
}

void Client::SendCannonObjectData(CannonObject data)
{
    PacketHeader header;
    header.type = PACKET_USE_CANNON_DATA;
    header.dataSize = sizeof(CannonObject);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(CannonObject))) return;
}

void Client::SendGatherObjectData(GatherObject data)
{
    PacketHeader header;
    header.type = PACKET_USE_GATHER_DATA;
    header.dataSize = sizeof(GatherObject);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(GatherObject))) return;
}

void Client::SendEnhanceObjectData(EnhanceObject data)
{
    PacketHeader header;
    header.type = PACKET_USE_ENHANCE_DATA;
    header.dataSize = sizeof(EnhanceObject);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(EnhanceObject))) return;
}

void Client::SendAddStockObjectData(AddStockData data)
{
    PacketHeader header;
    header.type = PACKET_ADD_STOCK_DATA;
    header.dataSize = sizeof(AddStockData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(AddStockData))) return;
}

void Client::SendReduceStockObjectData(ReduceStockData data)
{
    PacketHeader header;
    header.type = PACKET_REDUCE_STOCK_DATA;
    header.dataSize = sizeof(ReduceStockData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(ReduceStockData))) return;
}

void Client::SendBlockData(BlockData data)
{
    PacketHeader header;
    header.type = PACKET_BLOCK_DATA;
    header.dataSize = sizeof(BlockData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(BlockData))) return;
}

void Client::SendEventData(EventPlayerData data)
{
    PacketHeader header;
    header.type = PACKET_EVENT_DATA;
    header.dataSize = sizeof(EventPlayerData);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(EventPlayerData))) return;
}

void Client::SendUseReqest(UseReqest data)
{
    PacketHeader header;
    header.type = PACKET_USE_REQUEST;
    header.dataSize = sizeof(UseReqest);

    if (!SafeSend(serverSocket, &header, sizeof(PacketHeader))) return;
    if (!SafeSend(serverSocket, &data, sizeof(UseReqest))) return;

    m_Action = false;
    m_SaveReqest = data;
}

void Client::ReceiveData()
{
    while (isRunning) {
        PacketHeader header;

        if (!SafeRecv(serverSocket, (char*)&header, sizeof(PacketHeader))) {
            isRunning = false;
            break;
        }

        // `header.dataSize` のチェック（負の値や極端に大きい値を防ぐ）
        if (header.dataSize < 0 || header.dataSize > MAX_PACKET_SIZE) {
            /*std::cerr << "ReceiveData: 不正な dataSize を受信: " << header.dataSize << std::endl;*/
            isRunning = false;
            break;
        }

        ProcessPacket(header);
    }
}

void Client::ProcessPacket(const PacketHeader& header)
{

    switch (header.type) {
     case PACKET_GAME_SCENE_STATE: {
         GameSceneData gamedata;
         if (!SafeRecv(serverSocket, (char*)&gamedata, header.dataSize)) return;

         m_GameSceneData = gamedata;
         break;
     }

     case PACKET_PLAYERDATA: {
         PlayerData data;
         if (!SafeRecv(serverSocket, (char*)&data, header.dataSize)) return;

         if (m_MyPlayerId == -1 || m_MyPlayerId >= MAX_PLAYERS) {
             m_MyPlayerId = data.playerid;
         }
         if (m_MyTeamId == -1 || m_MyTeamId >= 2) {
             m_MyTeamId = data.teamid;
         }
         break;
     }

     case PACKET_ALLPLAYER_DATA: {
         if (!SafeRecv(serverSocket, (char*)m_AllPlayerData, header.dataSize)) return;
         break;
     }

     case PACKET_MATCHING_DATA: {
         if (!SafeRecv(serverSocket, (char*)m_AllMatchingData, header.dataSize)) return;
         break;
     }

     case PACKET_CANNON_DATA: {
         int elementCount = header.dataSize / sizeof(CannonData);
         if (elementCount <= 0) return;

         std::vector<CannonData> receivedData(elementCount);
         if (!SafeRecv(serverSocket, (char*)receivedData.data(), header.dataSize)) return;

         m_CannonData.insert(m_CannonData.end(), receivedData.begin(), receivedData.end());
         break;
     }
     case PACKET_ACTIONOBJECT_DATA: {
         if (!SafeRecv(serverSocket, (char*)m_ActionObjectData, header.dataSize)) return;
         break;
     }
     case PACKET_BLOCK_DATA: {
         int elementCount = header.dataSize / sizeof(BlockData);
         if (elementCount <= 0) return;

         std::vector<BlockData> receivedData(elementCount);
         if (!SafeRecv(serverSocket, (char*)receivedData.data(), header.dataSize)) return;

         m_BlockData.insert(m_BlockData.end(), receivedData.begin(), receivedData.end());
         break;
     }
     case PACKET_EVENT_DATA: {
         if (!SafeRecv(serverSocket, (char*)&m_EventData, header.dataSize)) return;
         break;
     case PACKET_USE_REQUEST: {
         if (!SafeRecv(serverSocket, (char*)&m_UseReqest, header.dataSize)) return;
         break;
     }

     default:
         /*std::cerr << "ProcessPacket: 不明なパケットタイプ: " << header.type << std::endl;*/
         break;
     }
    }
}

bool Client::SafeRecv(SOCKET socket, char* buffer, int size)
{
    int totalReceived = 0;
    while (totalReceived < size) {
        int received = recv(socket, buffer + totalReceived, size - totalReceived, 0);

        if (received == 0) {
            /*std::cerr << "SafeRecv: サーバーが切断されました。" << std::endl;*/
            return false;
        }
        else if (received < 0) {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            /*std::cerr << "SafeRecv: 受信エラー, error code: " << error << std::endl;*/
            return false;
        }

        totalReceived += received;
    }
    return true;
}

bool Client::SafeSend(SOCKET socket, const void* buffer, size_t dataSize)
{
    const char* sendBuffer = static_cast<const char*>(buffer);
    size_t totalSent = 0;

    while (totalSent < dataSize) {
        int bytesSent = send(socket, sendBuffer + totalSent, dataSize - totalSent, 0);

        if (bytesSent == 0) {
            /*std::cerr << "SafeSend: サーバーが切断されました。" << std::endl;*/
            return false;  // サーバーが切断
        }
        else if (bytesSent < 0) {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
            /*std::cerr << "SafeSend でエラー発生: " << error << std::endl;*/
            return false;  // 送信エラー
        }

        totalSent += bytesSent;
    }

    return true;  // 送信成功
}

void Client::ResetSaveReqest()
{
    m_SaveReqest.object = USEOBJECT_NONE;
    m_SaveReqest.type = REQUEST_NONE;
    m_SaveReqest.useid = -1;

}


void CreateClient()
{

    g_Client = new Client;

}

void DestroyClient()
{
    if (g_Client != nullptr) {
        delete g_Client;
        g_Client = nullptr;
    }
}

Client* GetClient()
{
    return g_Client;
}
