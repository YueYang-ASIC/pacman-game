#pragma once

#include <list>

#define MAPLENTH 19//逻辑地图大小
#define P_ROW 10
#define P_COLUMS 9
#define E_ROW 8
#define E_COLUMS 9

using std::list;

//抽象类地图，三关类的父类
class GMap
{
	//保护类:子类可以访问到，通用变量
protected:
	static int LD;//障碍物尺寸
	static int PD;//豆子半径
	void InitOp();//敌我双方出现位置没有豆子出现
	bool mapData[MAPLENTH][MAPLENTH];//障碍物逻辑地图点阵
	bool peaMapData[MAPLENTH][MAPLENTH];//逻辑地图点阵
	COLORREF color;//地图颜色

public:
	void DrawMap(HDC &hdc);//绘制地图
	void DrawPeas(HDC &hdc);//绘制豆子

	GMap() {};
	virtual ~GMap();

	//为了让玩家类和敌人类可以访问到地图类，声明友元
	friend class GObject;//允许物体类使用直线的起点和终点的信息做碰撞检测
	friend class PacMan;//允许"大嘴"访问豆子地图
};
//继承
class Stage_1 :public GMap {
private:
	bool static initData[MAPLENTH][MAPLENTH];//初始化地图
public:
	Stage_1();//构造函数
};



class Stage_2 :public GMap {
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_2();
};



class Stage_3 :public GMap {
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_3();
};