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
		//具体到每个Tile就必须规定位置坐标了
		//这里的Vei2是一个制定了“屏幕像素坐标”的类型
		//和MemeField类中用到的gridPos不一样，那个指的是网格坐标
		void Draw(const Vei2& screenPos, Graphics& gfx)const;
	private:
		State state = State::Hidden;
		bool hasMeme;
	};
public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx)const;
	//为了把背景变成灰色，搞一个获取背景所覆盖的矩形的函数
	RectI GetRect()const;
private:
	//搞一个从二维的Vei2类转换成一维的数组元素引用：
	Tile& TileAt(const Vei2& gridPos);		//不能在后面加上const限定，因为你不能让一个const函数返回一个non-const的引用
	const Tile& TileAt(const Vei2& gridPos)const;		//专门重载一个const版本的函数，解决Draw中的问题
private:
	static constexpr int width = 20;
	static constexpr int height = 16;
	Tile tiles[width * height];
};