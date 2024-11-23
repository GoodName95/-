#include<stdio.h>
#include<easyx.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>

IMAGE bk;//背景图片
IMAGE img_role[2];//飞机的图片
IMAGE image_bull[2];//子弹图片
IMAGE image_enemy[2][2];//敌人图片

enum My
{
	weidth=750,
	height=780,
	bullet_num=1,//子弹数量
	enemy_num=10,
	big,
	Small
};

struct Plane
{
	int x;
	int y;
	bool live; //是否死亡
	int width;
	int height;
	int hp;//血量
	int type; //类型
}player,bull[bullet_num],enemy[enemy_num];

//生产飞机
void enemyHP(int i)
{
	if (rand() % 8)//小飞机 1-8
	{
		enemy[i].type = Small;
		enemy[i].hp = 1;
		enemy[i].width = 72;
		enemy[i].height = 40;
	}
	else//大飞机
	{
		enemy[i].type = big;
		enemy[i].hp = 5;
		enemy[i].width = 105;
		enemy[i].height = 58;
		
	}
}

//飞机初始化
void gameInit()
{	
	player.x = weidth / 2;
	player.y = height - 220;
	player.live = true;
	//初始化子弹
	for (int i = 0; i < bullet_num; i++)
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].y = false;
	}
	//初始化敌机
	for (int i = 0; i < enemy_num; i++)
	{
		//敌机没有
		enemy[i].live = false;
		//生产飞机
		enemyHP(i);
	}
	enemy[0].live = true;
	enemy[0].y = 10;
	enemy[0].x = 10;
}

//图片的初始化
void loadImg()
{
	//初始化图片 背景图片
	loadimage(&bk,"./img/背景.jpg");
	//飞机图片
	loadimage(&img_role[0], "./img/康博.jpg");
	loadimage(&img_role[1], "./玩家2相对路径");
	//子弹图片
	loadimage(&image_bull[0], "./img/问号子弹.jpg");
	loadimage(&image_bull[1], "./img/表情包子弹.jpg");

	//敌机图片 大 小
	loadimage(&image_enemy[0][0], "./img/小敌鸡.jpg");
	loadimage(&image_enemy[1][0], "./img/大敌鸡.jpg");
}

//创建一个子弹
void createBullte()
{
	for (int i = 0; i < bullet_num; i++)
	{
		if (!bull[i].live)
		{
			bull[i].x = player.x+25;
			bull[i].y = player.y-30;
			bull[i].live = true;
			//产生了一个子弹 出循环
			mciSendString("close PUT_NUM", NULL, 0, NULL);
			mciSendString("open ./music/攻击音效.mp3 alias PUT_NUM", NULL, 0, NULL);
			mciSendString("play PUT_NUM", NULL, 0, NULL);
			break;
		}
	}
}

//子弹移动
void bulletmove()
{
	for (int i = 0; i < bullet_num; i++)
	{
		if (bull[i].live)
		{
			bull[i].y -= 40;
			if (bull[i].y < 0)
			{
				bull[i].live = false;
			}
		}
	}
}

//生产敌机
void createenemy()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			//x是随机的
			enemy[i].x = rand() % (weidth - 60);
			enemy[i].y = 0;
			break;
		}
	}
}

//敌机的移动
void enemymove(int speed)
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > height)//判断是否出界
			{
				enemy[i].live = false;
			}
		}
	}
}

void gameDraw()
{
	//初始化
	loadImg();
	//贴图
	putimage(0,0, &bk);
	//贴玩家飞机  去掉透明背景 NOTSRCERASE 二进制
	putimage(player.x,player.y, &img_role[0]);
	putimage(player.x,player.y, &img_role[1]);
	//绘制子弹
	for (int i = 0; i < bullet_num; i++)
	{
		if (bull[i].live)
		{
			putimage(bull[i].x, bull[i].y, &image_bull[0]);
			putimage(bull[i].x, bull[i].y, &image_bull[1]);
		}
	}

	//绘制敌机
	for (int i = 0; i < enemy_num; i++)
	{
		//如果有敌机
		if (enemy[i].live)
		{
			if (enemy[i].type == big)
			{
				putimage(enemy[i].x, enemy[i].y, &image_enemy[1][0]);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &image_enemy[0][0]);
			}

		}
	}
}

//speed 速度
void playerMove(int speed)//玩家操作
{
	//键盘事件 GetAsyncKeyState 非阻塞函数
	//移动
	if (GetAsyncKeyState(VK_UP)|| GetAsyncKeyState('W'))
	{
		if(player.y>0) player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
	{
		if(player.y+160<height) player.y += speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
	{
		if(player.x+42<weidth) player.x += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
	{
		if(player.x+30>0) player.x -= speed;
	}
	//生产子弹
	static DWORD t1 = 0, t2 = 0;//射速限制
	if (GetAsyncKeyState(VK_SPACE) && t2-t1>50)
	{
		createBullte();
	}
	t2 = GetTickCount();
}

//击中敌机
void playPlane()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live)
		{
			continue;
		}
		for (int k = 0; k < enemy_num; k++)
		{
			if (bull[k].x >enemy[i].x-enemy[i].width / 2 && bull[k].x<enemy[i].x + enemy[i].width / 2 &&
				bull[k].y>enemy[i].y && bull[k].y < enemy[i].y + enemy[i].height)
			{
				bull[i].live = false;
				enemy[i].hp--;
			}
		}
		if (enemy[i].hp <= 0)
		{
			if (enemy[i].type == 'big') enemy[i].hp = 5;
			else enemy[i].hp = 3;
			enemy[i].live = false;
		}
	}
}

bool Time(int ms,int id)
{
	static DWORD t[10];
	if (clock()-t[10]> 500)
	{
		//创建敌机
		t[id] = clock();
		return true;
	}
	return false;
}


int main()
{
	//游戏窗口的函数
	initgraph(weidth, height);

	gameInit();

	//双缓冲机制 防止一闪一闪
	BeginBatchDraw();
	while (1)
	{
		gameDraw();
		FlushBatchDraw();
		playerMove(5);
		bulletmove();
		if (Time(500, 0))
		{
			createenemy();
		}
		enemymove(1);
		playPlane();
		Sleep(1);
	}
	return 0;
}