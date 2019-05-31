#include "MemeField.h"
//最好做防御性编程……
#include <assert.h>
#include <random>
#include "SpriteCodex.h"
#include <algorithm>

MemeField::MemeField(int nMemes)
{
	//我们想要多于0个Meme：
	assert(nMemes > 0 && nMemes < width*height);
	//随机生成Meme的位置：
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);
	

	for (int i = 0; i < nMemes; ++i)
	{
		Vei2 spawnPosition;
		//现在内层循环找一个没有Meme的坐标位置：
		do {
			spawnPosition = { xDist(rng),yDist(rng) };
		} while(TileAt(spawnPosition).HasMeme());		//通过Vei2类型的坐标找到Tile对象，看看是否有Meme
		TileAt(spawnPosition).SpawnMeme();
	}
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			TileAt(gridPos).SetNeighborMemeCount( CountNeighborMemes(gridPos) );
		}
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	//要循环完所有的网格坐标，对所有坐标上的Tile调用Draw函数：
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			//调用对应位置的Tile对象Draw函数：
			//要写出一个能将gridPos转换为scrrenPos的操作
			//而且这里的TileAt函数应该需要有一个const限定，不改变调用者
			//不然就没法在这个Draw函数中调用（编译器认为它可能进行改动）
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(0,width*SpriteCodex::tileSize,0,height * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2& screenPos)
{
	//将screenPos转换成gridPos：
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed() && !tile.IsFlagged())
	{
		tile.Reveal();
	}
}

void MemeField::OnFlagClick(const Vei2& screenPos)
{
	//将screenPos转换成gridPos：
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
	Tile & tile = TileAt(gridPos);
	if (!tile.IsRevealed())
	{
		tile.ToggleFlag();
	}
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos)
{
	return screenPos/SpriteCodex::tileSize;
}

//返回类型实际上是一个私有内部类的成员引用
//自动生成的Tile& MemeField::TileAt(const Vei2& gridPos)会报错
//加上MemeField::
MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return tiles[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
	return tiles[gridPos.y * width + gridPos.x];
}

int MemeField::CountNeighborMemes(const Vei2& gridPos)
{
	//防止越界：
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width, gridPos.x + 1);
	const int yEnd = std::min(height, gridPos.y + 1);
	
	int count = 0;
	for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; ++gridPos.y)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; ++gridPos.x)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}
	return count;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme()const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2& screenPos, Graphics& gfx) const
{
	//这里要取决于几个不同的条件来绘制不同图形：
	//被点击没？有没有Meme？
	switch (state)
	{
	case State::Hidden:				//没有点击
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:			//没有点击并标记旗子
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!HasMeme())
		{
			SpriteCodex::DrawTileNumber(screenPos,nNeighborMemes,gfx);
		}
		else
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}
		break;
	default:
		break;
	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state==State::Flagged;
}

bool MemeField::Tile::IsRevealed()const
{
	return state==State::Revealed;
}

void MemeField::Tile::SetNeighborMemeCount(int memeCount)
{
	//检查，只有未初始化才进行赋值
	assert(nNeighborMemes == -1);
	nNeighborMemes = memeCount;
}
