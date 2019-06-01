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
		//具体到每个Tile就必须规定位置坐标了
		//这里的Vei2是一个制定了“屏幕像素坐标”的类型
		//和MemeField类中用到的gridPos不一样，那个指的是网格坐标
		void Draw(const Vei2& screenPos, bool fucked, Graphics& gfx)const;
		void Reveal();
		void ToggleFlag();
		bool IsFlagged()const;
		bool IsRevealed()const;
		void SetNeighborMemeCount(int memeCount);
	private:
		State state = State::Hidden;
		bool hasMeme;
		//需要计算出每个Tile为中心的9格内是否有Meme，具体有几个
		int nNeighborMemes = -1;	//-1为未初始化
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx)const;
	//为了把背景变成灰色，搞一个获取背景所覆盖的矩形的函数
	RectI GetRect()const;
	//处理鼠标点击事件：
	//鼠标左键单击：Reveal
	void OnRevealClick(const Vei2& screenPos);
	//鼠标右键单击：将状态为Hidden的Tile转变为Flagged
	void OnFlagClick(const Vei2& screenPos);
	Vei2 ScreenToGrid(const Vei2& screenPos);
private:
	//搞一个从二维的Vei2类转换成一维的数组元素引用：
	Tile& TileAt(const Vei2& gridPos);		//不能在后面加上const限定，因为你不能让一个const函数返回一个non-const的引用
	const Tile& TileAt(const Vei2& gridPos)const;		//专门重载一个const版本的函数，解决Draw中的问题
	//构造器初始化雷区后，遍历一个方块为中心的9格，计算周边的雷数
	int CountNeighborMemes(const Vei2& gridPos);
private:
	//是否炸死
	bool isFucked = false;
	static constexpr int width = 20;
	static constexpr int height = 16;
	static constexpr int leftStart = (Graphics::ScreenWidth - SpriteCodex::tileSize * width) / 2;
	static constexpr int topStart = (Graphics::ScreenHeight - SpriteCodex::tileSize * height) / 2;
	Tile tiles[width * height];
};