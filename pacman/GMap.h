#pragma once

#include <list>

#define MAPLENTH 19//�߼���ͼ��С
#define P_ROW 10
#define P_COLUMS 9
#define E_ROW 8
#define E_COLUMS 9

using std::list;

//�������ͼ��������ĸ���
class GMap
{
	//������:������Է��ʵ���ͨ�ñ���
protected:
	static int LD;//�ϰ���ߴ�
	static int PD;//���Ӱ뾶
	void InitOp();//����˫������λ��û�ж��ӳ���
	bool mapData[MAPLENTH][MAPLENTH];//�ϰ����߼���ͼ����
	bool peaMapData[MAPLENTH][MAPLENTH];//�߼���ͼ����
	COLORREF color;//��ͼ��ɫ

public:
	void DrawMap(HDC &hdc);//���Ƶ�ͼ
	void DrawPeas(HDC &hdc);//���ƶ���

	GMap() {};
	virtual ~GMap();

	//Ϊ���������͵�������Է��ʵ���ͼ�࣬������Ԫ
	friend class GObject;//����������ʹ��ֱ�ߵ������յ����Ϣ����ײ���
	friend class PacMan;//����"����"���ʶ��ӵ�ͼ
};
//�̳�
class Stage_1 :public GMap {
private:
	bool static initData[MAPLENTH][MAPLENTH];//��ʼ����ͼ
public:
	Stage_1();//���캯��
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