### 消息队列
每一个窗口程序有一个消息队列,系统把消息放到队列里面,设置一个循环来翻译队列中的消息.然后分发这个消息.

```cpp
 // 主消息循环: 
    //while (GetMessage(&msg, nullptr, 0, 0))//如果没有消息,就阻塞在这里,不会返回.
    while (PeekMessage(&msg, nullptr, 0, 0,PM_REMOVE)){
        //修改后即使没有消息.也不会阻塞.
        if (!TranslateAccelerator(msg.hwnd, 
                                  hAccelTable, &msg)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
```

## GDI
### 画点
```cpp
HWND g_hWnd = nullptr;//定义一个全局变量
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
_In_opt_ HINSTANCE hPrevInstance,
_In_ LPWSTR    lpCmdLine,
_In_ int       nCmdShow){
HDC hdc = ::GetDC(g_hWnd);//传入全局变量的句柄
int x = rand()%WLENGTH ; //[0,WLENGTH)
int y = rand()%HLENGTH ; //[0,HLENGTH)
SetPixel(hdc, x, y, RGB(rand()%255, rand() % 255, rand() % 255));
::DeleteDC(hdc);//释放hdc	}
```

### 画线
```cpp
HDC hdc = ::GetDC(g_hWnd);
HPEN hPen = CreatePen(PS_SOLID, 3, RGB(rand()%255, rand() % 255, rand() % 255));//新画笔
HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);//选中画笔
int x = rand() % WLENGTH;
int y = rand() % HLENGTH;
int xx = rand() % WLENGTH;
int yy = rand() % HLENGTH;
MoveToEx(hdc, x, y, nullptr);
LineTo(hdc, xx, yy);
SelectObject(hdc, hOldPen);//恢复老画笔
DeleteObject(hPen);//删除画笔
ReleaseDC(g_hWnd, hdc);
Sleep(1);//使当前线程暂停1s 
```
![画线](https://github.com/hoshinotsuki/pacman-game/blob/master/res/line.JPG)

### 画矩形

```cpp
HDC hdc = ::GetDC(g_hWnd);
//空心矩形。 
HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
Rectangle(hdc, 100, 200, 300, 500);//左上角和右下角坐标。(100,200),(300,500)
SelectObject(hdc, holdPen);
DeleteObject(hPen);

//实心矩形
HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
RECT rect;
rect.left = 50;
rect.right = 150;
rect.top = 270;
rect.bottom = 370;
FillRect(hdc, &rect, hBrush);
DeleteObject(hBrush); 
ReleaseDC(g_hWnd, hdc);
```
![画矩形](https://github.com/hoshinotsuki/pacman-game/tree/master/Release/res/rect.JPG)

### 画弧
```cpp
HDC hdc = ::GetDC(g_hWnd); 
HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
//圆。左上右下		
Ellipse(hdc, 200, 150, 300, 250);
//椭圆。左上右下
Ellipse(hdc, 200, 170, 300, 250);
//弧形
Arc(hdc,
100, 100, 300, 250,//矩形
200,150,//起始点
200,200//终点
);
SelectObject(hdc, holdPen);
DeleteObject(hPen); 
ReleaseDC(g_hWnd, hdc);
```
![画弧](https://github.com/hoshinotsuki/pacman-game/blob/master/res/arc.JPG)

### 画玩家
```cpp
//闭嘴
void Draw0() {
int x = 300, y = 300;//中心点
int r = 100;//圆的半径
HDC hdc = ::GetDC(g_hWnd);
//人脸
Ellipse(hdc, x - r, y - r, x + r, y + r);
//嘴巴
MoveToEx(hdc, x, y, nullptr);
LineTo(hdc, x - r, y);
ReleaseDC(g_hWnd,hdc);
}
```
![](https://github.com/hoshinotsuki/pacman-game/blob/master/res/play1.JPG)
```cpp
#define PI 3.1415926f
//半张嘴
void Draw1() {
	int x = 300, y = 300;//中心点
	int r = 100;//圆的半径
	HDC hdc = ::GetDC(g_hWnd);
	//脸
	int x0, y0;
	int x1, y1;
	x0 = x + (r*sin(PI*1.25f));
	y0 = y - (r*cos(PI*1.25f)); 
	x1 = x - (r*sin(PI*0.75f));
	y1 = y + (r*cos(PI*0.75f));
	Arc(hdc, 
		x-r, y-r, x+r, y+r,//矩形
		x0, y0,//起始点
		x1, y1);//终点 	
	//嘴巴
	MoveToEx(hdc, x0, y0, nullptr);
	LineTo(hdc, x, y);
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x, y);
	ReleaseDC(g_hWnd, hdc);
}
```
![](https://github.com/hoshinotsuki/pacman-game/blob/master/res/play2.JPG)
```cpp
//全张嘴。半圆+竖线
void Draw2() {
	int x = 300, y = 300;//中心点
	int r = 100;//圆的半径
	HDC hdc = ::GetDC(g_hWnd);
	Arc(hdc,
		x - r, y - r, x + r, y + r,
		x, y + r,
		x, y - r);
	MoveToEx(hdc,x, y - r, nullptr);
	LineTo(hdc, x, y + r);
	ReleaseDC(g_hWnd, hdc);
}
```
![](https://github.com/hoshinotsuki/pacman-game/blob/master/res/play3.JPG)

### 动画
```cpp
//画玩家
//控制时间
static DWORD dwTime = GetTickCount();
if (GetTickCount() - dwTime >= 40) 
    dwTime = GetTickCount();//更新时间 
else continue;//跳出 

//清除上一次画布
HDC hdc = GetDC(g_hWnd);
RECT  rc;
GetClientRect(g_hWnd,&rc);//rc获得窗口大小
HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
FillRect(hdc, &rc, hbr);//窗口刷白
DeleteObject(hbr);
ReleaseDC(g_hWnd, hdc);

//0：闭嘴
//1.半张
//2.全张
//3.半张
//4.闭嘴
static int iFrame = 0;//设置一个静态变量来循环播放
iFrame++;
if (iFrame >= 5)
    iFrame = 0;
switch (iFrame)
{
    case 0: Draw0();break;
    case 1: Draw1();break;
    case 2: Draw2();break;
    case 3: Draw1();break;
    case 4: Draw0();break;
} 
```


## 地图类
抽象类地图的设计
```cpp
//抽象类地图，三关类的父类
class GMap
{
	//保护类:子类可以访问到，通用变量
protected:
	static int LD;//障碍物尺寸
	static int PD;//豆子半径
	void InitOp();//初始化玩家和敌人
	bool mapData[MAPLENTH][MAPLENTH];//地图数据
	bool peaMapData[MAPLENTH][MAPLENTH];//豆子数据
	COLORREF color;//地图颜色
public:
	void DrawMap(HDC &hdc);//绘制地图
	void DrawPeas(HDC &hdc);//绘制豆子
	GMap() {};
	virtual ~GMap();

	//为了让玩家类和敌人类可以访问到地图类，声明友元
	friend class GObject;
	friend class PacMan;
};
//继承
class Stage_1 :public GMap {
private:
	bool static initData[MAPLENTH][MAPLENTH];//初始化地图
public:
	Stage_1();//构造函数
}; 
```
抽象类地图的实现
```cpp
int GMap::LD = 36;//墙宽
int GMap::PD = 3;//豆子的半径
void GMap::InitOp()//初始化敌人和玩家的位置没有豆子
{
	peaMapData[E_ROW][E_COLUMS] = false;
	peaMapData[P_ROW][P_COLUMS] = false;
}
void GMap::DrawMap(HDC &memDC){
HBRUSH hBrush = CreateSolidBrush(color);
for (int i = 0; i < MAPLENTH; i++) {
for (int j = 0; j < MAPLENTH; j++) {
//绘制墙壁,x对应列,y对应行
if (!mapData[i][j]) {
RECT rect;
rect.left = j * LD;
rect.top = i * LD;
rect.right = (j + 1) * LD;
rect.bottom = (i + 1) * LD;
FillRect(memDC, &rect, hBrush);}}}
DeleteObject(hBrush);}
void GMap::DrawPeas(HDC &hdc){ 	
for (int i = 0; i < MAPLENTH; i++){
for (int j = 0; j < MAPLENTH; j++){
if (mapData[i][j]) {
//画豆子,把豆子放在一个矩形里面
Ellipse(hdc,
(LD / 2 - PD) + j*LD,//left
(LD / 2 - PD) + i*LD,//top
(LD / 2 + PD) + j*LD,//right
(LD / 2 + PD) + i*LD);}}}}//bottom
GMap::~GMap(){}
//初始化地图
#define A true
#define B false
bool Stage_1::initData[MAPLENTH][MAPLENTH] = {
.....};
#undef A
#undef B
//取消宏定义，防止歧义
```
![画线](https://github.com/hoshinotsuki/pacman-game/blob/master/res/map.JPG)

## 可移动对象类的设计
- 基类GObject的部分变量
```cpp
public:
static GMap *pStage; 
//指向地图类的指针,设置为静态,使所有自类对象都能够使用相同的地图
void DrawBlank(HDC &hdc);// 画空白
protected://想让玩家类和敌人类访问这些数据
//祯数
int m_nFrame;
//判断物体是否到达逻辑坐标位置
bool Achive();
//逻辑碰撞检测,将物体摆放到合理的位置
bool Collision();
//将实际坐标转换为逻辑坐标
int PtTransform(int k) 
//到达逻辑点后更新数据
virtual void AchiveCtrl();
```
- 敌军对象
```cpp
class Enemy : public GObject
{
protected:
    void Catch();//是否抓住玩家
    void virtual MakeDecision(bool b) = 0;
    //确定方向.AI实现.三种敌人重写
    COLORREF color;
public:
    static std::shared_ptr<PacMan> player;//智能指针，指向玩家，静态变量
    void virtual Draw(HDC &hdc);//绘制自己
    Enemy(int x, int y) : GObject(x, y)//构造函数
    {
        this->m_nSpeed = ENERMYSPEED;//设置速度
        m_dir = LEFT;//设置朝向
        m_cmd = UP;//设置移动方向
    }
    void virtual action();//要重写的动作
};
```
1. class RedOne : public Enemy //随机移动
2. class BlueOne : public RedOne //守卫者
3. class YellowOne : public RedOne //扰乱者

- 玩家对象 
```cpp
class PacMan : public GObject
{
protected:
	virtual void AchiveCtrl();//重写虚函数
public:
	POINT GetPos();//获取位置
	bool IsOver();//游戏是否结束
	bool IsWin();//玩家是否赢得游戏
	void Draw(HDC &hdc);//绘制自己
	void SetTwCommand(TWARDS command);//设置玩家下一步指令
	PacMan(int x, int y) : GObject(x, y)//构造函数
	{
		this->m_nSpeed = PLAYERSPEED;//玩家速度
		m_cmd = m_dir = LEFT;//设置方向
	}
	void action();//动作重写
	void SetOver();//设置游戏结束函数
};
```

## 可移动对象的实现

```cpp
//将实际坐标转换为逻辑坐标
int GObject::PtTransform(int k){
	return (k - (pStage->LD) / 2) / pStage->LD;
}
//判断物体是否到达逻辑坐标位置
bool GObject::Achive(){
	int n = (m_ptCenter.x - pStage->LD / 2) % pStage->LD; 
    // 计算x坐标的余数
	int k = (m_ptCenter.y - pStage->LD / 2) % pStage->LD; 
    // 计算y坐标的余数
	bool l = (n == 0 && k == 0);         
    // 如果两个余数都为0,说明到达中心位置
	return l;
}
//到达逻辑点后更新数据
void GObject::AchiveCtrl(){
	if (Achive()) {
		m_nArray = PtTransform(m_ptCenter.x);  //更新列
		m_nRow = PtTransform(m_ptCenter.y);  //更新行
	}
}
```
将上一帧的图像清除,画这一帧的图像.
```cpp
void GObject::DrawBlank(HDC &hdc)
{
    // 申请资源,并交给智能指针处理
    HBRUSH hbr = ::CreateSolidBrush(RGB(255, 255, 255));
    //创建画刷，绘制矩形函数要求使用
    std::shared_ptr<HBRUSH> phbr(&hbr, [](auto hbr) {
        //把资源交给智能指针处理，自动释放
        // 离开 DrawBlank函数时,会自动调用释放资源
        DeleteObject(*hbr);
    });
    RECT rect;
    rect.top = m_nY - RD;
    rect.left = m_nX - RD;
    rect.right = m_nX + RD;
    rect.bottom = m_nY + RD; 
    FillRect(hdc, &rect, *phbr);//绘制空白矩形
}
```
碰撞检测
```cpp
bool GObject::Collision()
{
	bool b = false;
	//执行重写的AchiveCtrl函数消除豆子
	AchiveCtrl();
	//判断指令的有效性
	if (m_nArray < 0 || m_nRow < 0 || 
        m_nArray > MAPLENTH - 1|| 
        m_nRow > MAPLENTH - 1) b = true;
	else if (Achive()) {
		switch (m_cmd) {//判断行进的方向
		case LEFT://判断下一个格子是否能够通行
			if (m_nArray > 0 &&
				!pStage->mapData[m_nRow][m_nArray - 1]) {
				b = true;//指令无效
			}break;
	    //以下方向的判断原理相同
		case RIGHT: ...
		case UP: ...
		case DOWN: ...}
		if (!b) m_dir = m_cmd; //没撞墙,指令成功
	}
	//依照真实的方向位移
	m_nX = m_ptCenter.x;
	m_nY = m_ptCenter.y;
	int MAX = pStage->LD * MAPLENTH + pStage->LD / 2;
	int MIN = pStage->LD / 2;
	switch (m_dir) {  //判断行进的方向
	case LEFT://判断下一个格子是否能够通行
		if (m_nArray > 0 &&
			!pStage->mapData[m_nRow][m_nArray - 1]) {
			b = true;
			break;}//碰撞了
		m_ptCenter.x -= m_nSpeed;//向左位移
		if (m_ptCenter.x < MIN)
			m_ptCenter.x = MAX;//穿墙
        break;
	//以下方向的判断原理相同
	case RIGHT: ...
	case UP: ...
	case DOWN: ...
	}
	return b;
}
```

敌军对象

```cpp
// 抓住，游戏结束
void Enermy::Catch()
{
	int DX = m_ptCenter.x - player->GetPos().x;
	int DY = m_ptCenter.y - player->GetPos().y;
	if ((-RD < DX && DX < RD) && (-RD < DY && DY < RD)) {
		player->SetOver();
	}
}
```

敌军对象绘制

```cpp
void Enermy::Draw(HDC &hdc)
{
	HPEN pen = ::CreatePen(0, 0, color);
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    //绘制半圆型的头 
	//根据祯数来绘制身体和“腿部”
	if (m_nFrame % 2 == 0) {
		//矩形的身子 
        //从左往右绘制“腿部” 
	}
	else {
        //绘制身体
		//从左往右绘制“腿部”
	}
	//根据方向绘制眼睛 
	switch (m_dir) {
	case UP: 
	case DOWN: 
	case LEFT: 
	case RIGHT: 
	}
	m_nFrame++; //准备绘制下一祯
	SelectObject(hdc, oldPen);//还原画笔
	DeleteObject(pen);//删除画笔对象
	return;
}
```
敌军对象的行为
```cpp
void Enermy::action()
{
	bool b = Collision();//碰撞检测
	MakeDecision(b);//设定方向
	Catch();//开始捕捉
}
```

## 游戏主体逻辑

```cpp
// 玩家对象,智能指针
// 自己 
auto g_me = std::make_shared<PacMan>(P_ROW, P_COLUMS);
// 设定四个敌人对象
auto e1 = std::make_shared<RedOne>(E_ROW, E_COLUMS);
auto e2 = std::make_shared<RedOne>(E_ROW, E_COLUMS);
auto e3 = std::make_shared<BlueOne>(E_ROW, E_COLUMS);
auto e4 = std::make_shared<YellowOne>(E_ROW, E_COLUMS);

//静态变量，初始化为第一关地图
GObject::pStage = MapArray[s_n]; 

//引用静态成员变量
// 设定玩家
Enermy::player = g_me;
 
// 主消息循环:
// 玩家没有被抓,并且关卡<3
while (!g_me->IsOver() && s_n < STAGE_COUNT) {
// 判断是否赢得比赛
if (g_me->IsWin()) {
s_n++;
// 重设自己和敌人位置 
// 判断是否完成了3关,如果完成,退出游戏,否则进入下一关
if (s_n < 3) { 
continue;}// 继续进行循环
else break;}// 跳出循环


// 获取消息
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
TranslateMessage(&msg);
DispatchMessage(&msg);
}
// 判断时间,否则画得太快，敌人一下就把我们抓住了就游戏结束了
if (GetTickCount() - dwLastTime >= 40) {
dwLastTime = GetTickCount();
}
else {
// 时间不到,本次不进行绘画
continue;
}
// 获得设备
HDC hdc = ::GetDC(g_hwnd);
// 不使用时自动释放
std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {
::ReleaseDC(g_hwnd, hdc);
});
// 画豆子
MapArray[s_n]->DrawPeas(hdc);
// 画地图
MapArray[s_n]->DrawMap(hdc);
// 画敌人及自动运动 
// 画自己 
// 自己向前移动 
// 获取按键 : 控制自己的方向
    
// 如果游戏结束
if (g_me->IsOver()) {... }
// 否则,提示赢得游戏
else { ...}

```
### 金手指
```cpp
//快速通关
if (GetAsyncKeyState('B') & 0x8000)
{
    for (int i = 0; i < MAPLENTH; i++)
    {
        for (int j = 0; j < MAPLENTH; j++)
        {
            peaMapData[i][j] = false;
        }
    }
}	
```
![画线](https://github.com/hoshinotsuki/pacman-game/blob/master/res/WIN.JPG)

@Lightmare小蒟蒻

2019.3.17