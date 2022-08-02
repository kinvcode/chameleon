#pragma once

////////////////////////[ 七 度 基 址 区 域  ]///////////////////////////////////

#define C_USER_ADDRESS 0x149806960 // 人物基址
#define C_EMPTY_ADDRESS 0x13FCA04C0 // 全局空白
#define C_USER_CALL 0x143E45830 // 人物CALL
#define C_SCORE_ADDRESS 0x1495579E8 // 评分基址
#define C_GAME_STATUS 0x1492365E0 // 游戏状态
#define C_ASM_CALL 0x13FDC0000 // 汇编CALL
#define C_SKILL_CALL 0x143980820 // 技能CALL
#define C_AUTO_PICKUP 0x143D51BEA // 自动捡物
#define C_TIME_ADDRESS 0x20A050 // 时间基址
#define C_ROOM_NUMBER 0x149558E48 // 房间编号
#define C_COORDINATE_PASS_ROOM 0x142DC7F30 // 坐标顺图CALL
#define C_SUMMON_MONSTER 0x143A44950 // 召唤怪物CALL
#define C_SUMMON_FIGURE 0x143A53720 // 召唤人偶CALL
#define C_HIDDEN_CALL 0x144DAA020 // 透明CALL
#define C_GLOBAL_ADDRESS 0x143D077FE // 全局基址
#define C_CASTING_SPEED 0xC90 // 释放速度
#define C_ATTACK_SPEED 0xC88 // 攻击速度
#define C_MOVE_SPEED 0xC80 // 移动速度
#define C_FLOAT_COOL_DOWN2 0x2290 // 浮点冷却2
#define C_MOVEMENT_ID 0x5450 // 动作ID
#define C_MAP_HEAD 0x148 // 地图开始2
#define C_MAP_END 0x150 // 地图结束2
#define C_MAP_OFFSET 0x180 // 地图偏移
#define C_TYPE_OFFSET 0x154 // 类型偏移
#define C_CE_SCORE 0xC98 // C_E_评分
#define C_MONSTER_BLOOD 0x6040 // 怪物血量
#define C_CAMP_OFFSET 0xE68 // 阵营偏移
#define C_CODE_OFFSET 0x838 // 代码偏移
#define C_NAME_OFFSET 0x840 // 名称偏移
#define C_PENETRATE_MAP 0x848 // 地图穿透
#define C_PENETRATE_BUILDING 0x84C // 建筑穿透
#define C_READ_COORDINATE 0x338 // 读取坐标
#define C_OBJECT_COORDINATE 0x168 // 对象坐标
#define C_DOOR_OFFSET 0x1E4 // 是否开门
#define C_CURRENT_ROOM_X 0x1BD8 // 当前房间X
#define C_CURRENT_ROOM_Y 0x1BDC // 当前房间Y
#define C_BOSS_ROOM_X 0x1CE4 // BOSS房间X
#define C_BOSS_ROOM_Y 0x1CEC // BOSS房间Y
#define C_BONFIRE_JUDGE 0x1DA8 // 篝火判断
#define C_MAP_CODE 0x1CCC // 索引偏移
#define C_DOOR_TYPE_OFFSET 0x170 // 门型偏移
#define C_WH_OFFSET 0x7A0 // 宽高偏移
#define C_AISLE_OFFSET 0x7C0 // 数组偏移
#define C_BEGIN_ROOM_X 0x178 // 起始坐标X
#define C_BEGIN_ROOM_Y 0x17C // 起始坐标Y
#define C_PASS_ROOM_OFFSET 0xF0 // 顺图偏移
#define C_SHOE_OFFSET 0x5360 // 鞋子偏移
// 新加的
#define C_COOL_DOWN_JUDGE_CALL 0x143E31BC0 // 冷却判断CALL
#define C_SKILL_LIST 0xACA8 // 技能栏
#define C_SKILL_LIST_OFFSET 0x90 // 技能栏偏移
#define C_COOR_CALL_OFFSET 0x1C8 // 坐标CALL偏移

////////////////////////[ 心 悦 基 址 区 域  ]///////////////////////////////////

