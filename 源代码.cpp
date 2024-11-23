#include<stdio.h>
#include<easyx.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>

IMAGE bk;//����ͼƬ
IMAGE img_role[2];//�ɻ���ͼƬ
IMAGE image_bull[2];//�ӵ�ͼƬ
IMAGE image_enemy[2][2];//����ͼƬ

enum My
{
	weidth=750,
	height=780,
	bullet_num=1,//�ӵ�����
	enemy_num=10,
	big,
	Small
};

struct Plane
{
	int x;
	int y;
	bool live; //�Ƿ�����
	int width;
	int height;
	int hp;//Ѫ��
	int type; //����
}player,bull[bullet_num],enemy[enemy_num];

//�����ɻ�
void enemyHP(int i)
{
	if (rand() % 8)//С�ɻ� 1-8
	{
		enemy[i].type = Small;
		enemy[i].hp = 1;
		enemy[i].width = 72;
		enemy[i].height = 40;
	}
	else//��ɻ�
	{
		enemy[i].type = big;
		enemy[i].hp = 5;
		enemy[i].width = 105;
		enemy[i].height = 58;
		
	}
}

//�ɻ���ʼ��
void gameInit()
{	
	player.x = weidth / 2;
	player.y = height - 220;
	player.live = true;
	//��ʼ���ӵ�
	for (int i = 0; i < bullet_num; i++)
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].y = false;
	}
	//��ʼ���л�
	for (int i = 0; i < enemy_num; i++)
	{
		//�л�û��
		enemy[i].live = false;
		//�����ɻ�
		enemyHP(i);
	}
	enemy[0].live = true;
	enemy[0].y = 10;
	enemy[0].x = 10;
}

//ͼƬ�ĳ�ʼ��
void loadImg()
{
	//��ʼ��ͼƬ ����ͼƬ
	loadimage(&bk,"./img/����.jpg");
	//�ɻ�ͼƬ
	loadimage(&img_role[0], "./img/����.jpg");
	loadimage(&img_role[1], "./���2���·��");
	//�ӵ�ͼƬ
	loadimage(&image_bull[0], "./img/�ʺ��ӵ�.jpg");
	loadimage(&image_bull[1], "./img/������ӵ�.jpg");

	//�л�ͼƬ �� С
	loadimage(&image_enemy[0][0], "./img/С�м�.jpg");
	loadimage(&image_enemy[1][0], "./img/��м�.jpg");
}

//����һ���ӵ�
void createBullte()
{
	for (int i = 0; i < bullet_num; i++)
	{
		if (!bull[i].live)
		{
			bull[i].x = player.x+25;
			bull[i].y = player.y-30;
			bull[i].live = true;
			//������һ���ӵ� ��ѭ��
			mciSendString("close PUT_NUM", NULL, 0, NULL);
			mciSendString("open ./music/������Ч.mp3 alias PUT_NUM", NULL, 0, NULL);
			mciSendString("play PUT_NUM", NULL, 0, NULL);
			break;
		}
	}
}

//�ӵ��ƶ�
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

//�����л�
void createenemy()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live)
		{
			enemy[i].live = true;
			//x�������
			enemy[i].x = rand() % (weidth - 60);
			enemy[i].y = 0;
			break;
		}
	}
}

//�л����ƶ�
void enemymove(int speed)
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (enemy[i].live)
		{
			enemy[i].y += speed;
			if (enemy[i].y > height)//�ж��Ƿ����
			{
				enemy[i].live = false;
			}
		}
	}
}

void gameDraw()
{
	//��ʼ��
	loadImg();
	//��ͼ
	putimage(0,0, &bk);
	//����ҷɻ�  ȥ��͸������ NOTSRCERASE ������
	putimage(player.x,player.y, &img_role[0]);
	putimage(player.x,player.y, &img_role[1]);
	//�����ӵ�
	for (int i = 0; i < bullet_num; i++)
	{
		if (bull[i].live)
		{
			putimage(bull[i].x, bull[i].y, &image_bull[0]);
			putimage(bull[i].x, bull[i].y, &image_bull[1]);
		}
	}

	//���Ƶл�
	for (int i = 0; i < enemy_num; i++)
	{
		//����ел�
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

//speed �ٶ�
void playerMove(int speed)//��Ҳ���
{
	//�����¼� GetAsyncKeyState ����������
	//�ƶ�
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
	//�����ӵ�
	static DWORD t1 = 0, t2 = 0;//��������
	if (GetAsyncKeyState(VK_SPACE) && t2-t1>50)
	{
		createBullte();
	}
	t2 = GetTickCount();
}

//���ел�
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
		//�����л�
		t[id] = clock();
		return true;
	}
	return false;
}


int main()
{
	//��Ϸ���ڵĺ���
	initgraph(weidth, height);

	gameInit();

	//˫������� ��ֹһ��һ��
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