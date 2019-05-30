#pragma once

#include "Vei2.h"
#include "Graphics.h"
#include "RectI.h"

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
		void Draw(const Vei2& screenPos, Graphics& gfx)const;
	private:
		State state = State::Hidden;
		bool hasMeme;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx)const;
	//Ϊ�˰ѱ�����ɻ�ɫ����һ����ȡ���������ǵľ��εĺ���
	RectI GetRect()const;
private:
	//��һ���Ӷ�ά��Vei2��ת����һά������Ԫ�����ã�
	Tile& TileAt(const Vei2& gridPos);		//�����ں������const�޶�����Ϊ�㲻����һ��const��������һ��non-const������
	const Tile& TileAt(const Vei2& gridPos)const;		//ר������һ��const�汾�ĺ��������Draw�е�����
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	Tile tiles[width * height];
};