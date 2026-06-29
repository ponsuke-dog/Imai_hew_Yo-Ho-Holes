#ifndef PACKET_H
#define PACKET_H

#define NOMINMAX
#include <DirectXMath.h>
using namespace DirectX;

//最大プレイヤー
#define MAX_PLAYERS (4)
//チーム数
#define TEAM_COUNT    (2)

//甲板のMAXHP
#define DECK_HP_WIDTH   (11)
#define DECK_HP_HEIGHT   (21)
#define DECK_HP_MAX     (DECK_HP_WIDTH * DECK_HP_HEIGHT * 100)

//接続ポート番号
#define PORT (12345)

//最大パケットサイズ
#define MAX_PACKET_SIZE (4096)

//パケットタイプ
enum PacketType {
    PACKET_GAME_SCENE_STATE,        //ゲームシーンデータパケット
    PACKET_PLAYERDATA,              //各プレイヤーデータパケット
    PACKET_ALLPLAYER_DATA,          //全プレイヤーデータパケット
    PACKET_MATCHING_DATA,           //全プレイヤーマッチングデータパケット
    PACKET_GAMESTART_DATA,           //ゲームスタートデータパケット
    PACKET_CANNON_DATA,             //生成砲弾パケット
    PACKET_ACTIONOBJECT_DATA,       //全アクションオブジェクトパケット
    PACKET_USE_CANNON_DATA,         //Cannon使用パケット
    PACKET_USE_GATHER_DATA,         //Gather使用パケット
    PACKET_USE_ENHANCE_DATA,        //Enhance使用パケット
    PACKET_ADD_STOCK_DATA,          //ストック増加パケット
    PACKET_REDUCE_STOCK_DATA,       //ストック消費パケット
    PACKET_BLOCK_DATA,              //ブロックデータパケット
    PACKET_EVENT_DATA,              //イベントデータパケット
    PACKET_USE_REQUEST              //使用リクエスト
};

//パケットヘッダー
struct PacketHeader {
    PacketType type;
    int dataSize;
};

//アニメーションステート
typedef enum
{
    ANIMATIONSTATE_NONE = -1,
    ANIMATIONSTATE_IDLE,
    ANIMATIONSTATE_RUN,
    ANIMATIONSTATE_CARRY_WOOD_IDLE,
    ANIMATIONSTATE_CARRY_WOOD,
    ANIMATIONSTATE_CARRY_CANNON_IDLE,
    ANIMATIONSTATE_CARRY_CANNON,
    ANIMATIONSTATE_HOLD,
    ANIMATIONSTATE_SHOT,
    ANIMATIONSTATE_REPAIR,
    ANIMATIONSTATE_BLOWOFF,

    ANIMATIONSTATE_MAX,
}AnimationState;


//サーバーステート
typedef enum {
    SERVERSTATE_NONE = -1,
    SERVERSTATE_CONNECT,
    SERVERSTATE_MATCHING,
    SERVERSTATE_GAME,
    SERVERSTATE_END,
}ServerState;

//サーバーゲームステート
typedef enum {
    SERVERGAME_NONE = -1,
    SERVERGAME_READY,
    SERVERGAME_PLAY,
    SERVERGAME_EVENT,
    SERVERGAME_END,
}ServerGame;

//プレイヤー準備状況
typedef enum {
    PLAYERREADY_NONE = -1,
    PLAYERREADY_OK,
    PLAYERREADY_NO,

}PlayerReadyFlag;

//ゲームシーンデータ情報
struct GameSceneData {
    ServerState serverstate = SERVERSTATE_NONE;
    ServerGame servergame = SERVERGAME_NONE;
    int frame = 0;
    int time = 0;
    float ship_hp[TEAM_COUNT] = { DECK_HP_MAX };
};

//マッチングデータ
struct MatchingData
{
    int playerID = -1;
    int teamID = -1;
    PlayerReadyFlag flag = PLAYERREADY_NO;
};

//ゲームスタートデータ
struct GameStart
{
    PlayerReadyFlag flag = PLAYERREADY_NO;
};

//プレイヤーデータ
struct PlayerData
{
    int playerid = -1;
    int teamid = -1;
    XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
    AnimationState anim = ANIMATIONSTATE_IDLE;
};

//大砲データ
struct CannonData
{
    int teamid = -1;
    int playerid = -1;
    int cannonid = -1;
    int endpoint = -1;
};

typedef enum
{
    USEFLAG_NONE = -1,
    USEFLAG_TRUE,
    USEFLAG_FALSE,

    USEFLAG_END,
}UseFlag;

enum StorageType
{
    STORAGETYPE_NONE = -1,

    STORAGETYPE_REPAIR,
    STORAGETYPE_CANNON,

    STORAGETYPE_MAX,
};

//Cannonオブジェクト
struct CannonObject
{
    int teamid = -1;
    int cannonid = -1;
    UseFlag use = USEFLAG_TRUE;
};

//Gatherオブジェクト
struct GatherObject
{
    int teamid = -1;
    int gatherid = -1;
    UseFlag use = USEFLAG_TRUE;
};

//Enhanceオブジェクト
struct EnhanceObject
{
    int teamid = -1;
    int stock = -1;
    int cannonlevel = -1;
    int gatherlevel = -1;
    int repairlevel = -1;
    UseFlag use = USEFLAG_TRUE;
};

//Storageオブジェクト
struct StorageObject
{
    int teamid = -1;
    StorageType type = STORAGETYPE_NONE;
    int stock = -1;
};

struct ActionObjectData
{
    CannonObject cannon[3];
    GatherObject gather[3];
    EnhanceObject enhance;
    StorageObject storage[2];
};

enum StockType
{
    STOCKTYPE_NONE = -1,
    STOCKTYPE_WOOD,
    STOCKTYPE_CANNON,
    STOCKTYPE_ENHANCE,

    STOCKTYPE_MAX,
};

struct AddStockData
{
    int teamid = -1;
    int add_stock[STOCKTYPE_MAX] = { -1 };
};

struct ReduceStockData
{
    int teamid = -1;
    StockType type = STOCKTYPE_NONE;
    int reduce_stock = -1;
};

typedef enum
{
    BLOCKSTATE_TRUE,
    BLOCKSTATE_BREAK,
    BLOCKSTATE_REPAIR,
}BlockState;

//甲板ブロックデータ
struct BlockData
{
    int teamid = -1;
    int blocknumber = -1;
    float hp = 0.0f;
    BlockState state = BLOCKSTATE_TRUE;
};

//イベント用データ

//イベントタイプ
typedef enum {
    EVENTTYPE_NONE = -1,
    EVENTTYPE_MASH,
    EVENTTYPE_ROLING,
    EVENTTYPE_COMMAND,

    EVENTTYPE_MAX,
}EventType;

//イベントシーン
typedef enum {
    EVENTSCENE_INPUT,
    EVENTSCENE_ANIM_WAIT,
    EVENTSCENE_DAMAGE,
    EVENTSCENE_ENDWAIT,
    EVENTSCENE_END,
}EventScene;

typedef enum {
    EVENTPLAYERREADY_NONE = -1,
    EVENTPLAYERREADY_PLAY,
    EVENTPLAYERREADY_INPUT_END,
    EVENTPLAYERREADY_ANIM_END,
    EVENTPLAYERREADY_END,
}EventPlayerReady;

//イベントデータ
struct EventData {
    EventType type = EVENTTYPE_NONE;
    EventScene scene = EVENTSCENE_INPUT;
    int count[TEAM_COUNT] = { 0 };
    EventPlayerReady readyflag[MAX_PLAYERS] = { EVENTPLAYERREADY_NONE };
};

//イベントプレイヤー送信データ
struct EventPlayerData
{
    int teamid = -1;
    int eventcount = 0;
    EventPlayerReady readyflag = EVENTPLAYERREADY_NONE;
};

//使用オブジェクト
typedef enum {
    USEOBJECT_NONE = -1,
    USEOBJECT_CANNON,
    USEOBJECT_GATHER,
    USEOBJECT_ENHANCE,
    USEOBJECT_WOOD_STORAGE,
    USEOBJECT_CANNON_STORAGE,
    USEOBJECT_REPAIR_DECKBLOCK,
}UseObject;

typedef enum {
    REQUEST_NONE = -1,
    REQUEST_OK,
    REQUEST_NO,
    REQUEST_ASK,
}RequestType;

//使用リクエストデータ
struct UseReqest
{
    UseObject object = USEOBJECT_NONE;
    int useid = -1;             //使用する各オブジェクトのIDを選択、ストレージの場合消費量を送る
    int teamid = -1;
    RequestType type = REQUEST_NONE;
};

#endif // PACKET_H
