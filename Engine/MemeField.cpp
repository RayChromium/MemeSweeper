#include "MemeField.h"
//����������Ա�̡���
#include <assert.h>
#include <random>
#include "SpriteCodex.h"

MemeField::MemeField(int nMemes)
{
	//������Ҫ����0��Meme��
	assert(nMemes > 0 && nMemes < width*height);
	//�������Meme��λ�ã�
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);
	

	for (int i = 0; i < nMemes; ++i)
	{
		Vei2 spawnPosition;
		//�����ڲ�ѭ����һ��û��Meme������λ�ã�
		do {
			spawnPosition = { xDist(rng),yDist(rng) };
		} while(TileAt(spawnPosition).HasMeme());		//ͨ��Vei2���͵������ҵ�Tile���󣬿����Ƿ���Meme
		TileAt(spawnPosition).SpawnMeme();
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	//Ҫѭ�������е��������꣬�����������ϵ�Tile����Draw������
	for (Vei2 gridPos{ 0,0 }; gridPos.y < height; ++gridPos.y)
	{
		for (gridPos.x = 0; gridPos.x < width; ++gridPos.x)
		{
			//���ö�Ӧλ�õ�Tile����Draw������
			//Ҫд��һ���ܽ�gridPosת��ΪscrrenPos�Ĳ���
			//���������TileAt����Ӧ����Ҫ��һ��const�޶������ı������
			//��Ȼ��û�������Draw�����е��ã���������Ϊ�����ܽ��иĶ���
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
	//��screenPosת����gridPos��
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
	//��screenPosת����gridPos��
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

//��������ʵ������һ��˽���ڲ���ĳ�Ա����
//�Զ����ɵ�Tile& MemeField::TileAt(const Vei2& gridPos)�ᱨ��
//����MemeField::
MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return tiles[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
	return tiles[gridPos.y * width + gridPos.x];
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
	//����Ҫȡ���ڼ�����ͬ�����������Ʋ�ͬͼ�Σ�
	//�����û����û��Meme��
	switch (state)
	{
	case State::Hidden:				//û�е��
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Flagged:			//û�е�����������
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case State::Revealed:
		if (!HasMeme())
		{
			SpriteCodex::DrawTile0(screenPos,gfx);
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
