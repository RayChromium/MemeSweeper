#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "RectI.h"
#include "SpriteCodex.h"

class MemeField
{
private:
	//
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
	public:
		void SpawnMeme();
		bool HasMeme()const;
		//���嵽ÿ��Tile�ͱ���涨λ��������
		//�����Vei2��һ���ƶ��ˡ���Ļ�������ꡱ������
		//��MemeField�����õ���gridPos��һ�����Ǹ�ָ������������
		void Draw(const Vei2& screenPos, bool fucked, Graphics& gfx)const;
		void Reveal();
		void ToggleFlag();
		bool IsFlagged()const;
		bool IsRevealed()const;
		void SetNeighborMemeCount(int memeCount);
	private:
		State state = State::Hidden;
		bool hasMeme;
		//��Ҫ�����ÿ��TileΪ���ĵ�9�����Ƿ���Meme�������м���
		int nNeighborMemes = -1;	//-1Ϊδ��ʼ��
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx)const;
	//Ϊ�˰ѱ�����ɻ�ɫ����һ����ȡ���������ǵľ��εĺ���
	RectI GetRect()const;
	//����������¼���
	//������������Reveal
	void OnRevealClick(const Vei2& screenPos);
	//����Ҽ���������״̬ΪHidden��Tileת��ΪFlagged
	void OnFlagClick(const Vei2& screenPos);
	Vei2 ScreenToGrid(const Vei2& screenPos);
private:
	//��һ���Ӷ�ά��Vei2��ת����һά������Ԫ�����ã�
	Tile& TileAt(const Vei2& gridPos);		//�����ں������const�޶�����Ϊ�㲻����һ��const��������һ��non-const������
	const Tile& TileAt(const Vei2& gridPos)const;		//ר������һ��const�汾�ĺ��������Draw�е�����
	//��������ʼ�������󣬱���һ������Ϊ���ĵ�9�񣬼����ܱߵ�����
	int CountNeighborMemes(const Vei2& gridPos);
private:
	//�Ƿ�ը��
	bool isFucked = false;
	static constexpr int width = 20;
	static constexpr int height = 16;
	static constexpr int leftStart = (Graphics::ScreenWidth - SpriteCodex::tileSize * width) / 2;
	static constexpr int topStart = (Graphics::ScreenHeight - SpriteCodex::tileSize * height) / 2;
	Tile tiles[width * height];
};