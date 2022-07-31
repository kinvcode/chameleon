#pragma once

////////////////////////[ 七 度 基 址 区 域  ]///////////////////////////////////

#define C_USER_ADDRESS 0x1493C88A8 // 人物基址
#define C_USER_CALL 0x0 // 人物CALL
#define C_NAME_OFFSET 0x0 // 名称偏移
#define C_SHOE_OFFSET 0x5358 // 鞋子偏移
#define C_ATTACK_SPEED 0xC88 // 攻击速度
#define C_MOVE_SPEED 0xC80 // 移动速度
#define C_CASTING_SPEED 0xC90 // 释放速度
#define C_EMPTY_ADDRESS 0x13FCCA9B0 // 空白地址
#define C_ASM_CALL 0x13FDC0000 // 汇编CALL
#define C_SUMMON_FIGURE 0x1439C82C0 // 召唤人偶CALL
#define C_SUMMON_MONSTER 0x1439B9560// 召唤怪物CALL
#define C_HIDDEN_CALL 0x144CFA7F0 // 透明CALL
#define C_SKILL_CALL 0x1438F55F0// 技能CALL
#define C_GAME_STATUS 0x14912D540 // 游戏状态
#define C_ROOM_NUMBER 0x14944ED18 // 房间编号
#define C_TIME_ADDRESS 0x20A050 // 时间基址
#define C_DOOR_TYPE_OFFSET 0x168 // 门型偏移
#define C_BONFIRE_JUDGE 0x1DA8 // 篝火判断
#define C_SCORE_ADDRESS 0x14944D8B8 // 评分基址
#define C_CE_SCORE 0xC98 // C_E_评分
#define C_DOOR_OFFSET 0x1E4 // 是否开门
#define C_MAP_OFFSET 0x180 // 地图偏移
#define C_BOSS_ROOM_X 0x1CF4 // BOSS房间X
#define C_BOSS_ROOM_Y 0x1CFC // BOSS房间Y
#define C_CURRENT_ROOM_X 0x1BD8 // 当前房间X
#define C_CURRENT_ROOM_Y 0x1BDC // 当前房间Y
#define C_BEGIN_ROOM_X 0x178 // 起始坐标X
#define C_BEGIN_ROOM_Y 0x17C // 起始坐标Y
#define C_MAP_HEAD 0x148 // 地图开始2
#define C_MAP_END 0x150 // 地图结束2
#define C_TYPE_OFFSET 0x154 // 类型偏移
#define C_CAMP_OFFSET 0xE60 // 阵营偏移
#define C_CODE_OFFSET 0x830 // 代码偏移
#define C_MONSTER_BLOOD 0x6038 // 怪物血量
#define C_READ_COORDINATE 0x330 // 读取坐标
#define C_OBJECT_COORDINATE 0x168 // 对象坐标
#define C_GLOBAL_ADDRESS 0x168 // 全局基址
#define C_PASS_ROOM_OFFSET 0x168 // 顺图偏移
#define C_COORDINATE_PASS_ROOM 0x168 // 坐标顺图CALL
#define C_AUTO_PICKUP 0x143CB9F73 // 自动捡物
#define C_MOVEMENT_ID 0x00 // 动作ID
#define C_PENETRATE_MAP 0x00 // 地图穿透
#define C_PENETRATE_BUILDING 0x00 // 建筑穿透
#define C_WH_OFFSET 0x00 // 宽高偏移
#define C_AISLE_OFFSET 0x7C0 // 数组偏移
#define C_FLOAT_COOL_DOWN2 0x2288 // 浮点冷却2

////////////////////////[ 心 悦 基 址 区 域  ]///////////////////////////////////

#define C_MAP_CODE 0x2290 // 索引偏移