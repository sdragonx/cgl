
#include <cgl/cmap.hpp>
#include "tilemap.hpp"

namespace boxworld{

//---------------------------------------------------------------------------

cmap<string_t, boxTileSet> tilesetList;

int tileset_LoadAtlas(const char* name, const char* file, const char* loadorder)
{
	boxTileSet t;

	if(tilesetList.empty()){
		tilesetList.insert("null", t);
	}

	t.load(file, loadorder);
	t.name = name;
	tilesetList.insert(name, t);
	return 0;
}

void tileset_dispose()
{
	for(size_t i=0; i<tilesetList.size(); ++i){
		tilesetList[i].dispose();
	}
	tilesetList.clear();
}

int tileset_count()
{
	return tilesetList.size();
}

const graphics::teximage* tileset_getImage(int index, int style)
{
	if(size_t(index) < tilesetList.size()){
		return tilesetList[index].image(style);
	}
	return null;
}

const graphics::teximage* tileset_getImage(const string_t& tileset, int style)
{
	boxTileSet* ts = tilesetList.find(tileset);
	if(ts){
		return ts->image(style);
	}
	return null;
}

int tileset_getIndex(const string_t& name)
{
	return tilesetList.indexof(name);
}

string_t tileset_getName(int index)
{
	if(size_t(index) < tilesetList.size()){
		return tilesetList.keyof(index);
	}
	return "null";
}

vec4ub tileset_getColor(int index)
{
    return vec4ub();
}

//---------------------------------------------------------------------------

void draw_tile(graphics::draw_context& dc, int x, int y, int tile, int style)
{
	const graphics::teximage* image = tileset_getImage(tile, style);
	if(image){
		dc.draw_image(image, x - 2, y - 2, WORLD_TILE_SIZE + 4, WORLD_TILE_SIZE + 4);
	}
}

#if 0
void draw_tileShadow(TCanvas* dc, int x, int y)
{
	int sx = mouse.x - frmMain->ClientWidth/2;
	int sy = mouse.y - frmMain->ClientHeight/2;
	sx = -sx / 4;
	sy = -sy / 4;

	int border = TILE_SIZE * 3 / 20;
	int size = TILE_SIZE - border - border;

//	windows::gdi::alpha_rect(dc->Handle, sx + x + border, sy + y + border, size, size, 0xFF000000, 128);
//	dc->Pen->Color = clBlack;
//	draw_line(dc, x + border, y + border, sx + x + border, sy + y + border);
//	draw_line(dc, x + border, y + border + size, sx + x + border, sy + y + border + size);
//	draw_line(dc, x + border + size, y + border, sx + x + border + size, sy + y + border);
//	draw_line(dc, x + border + size, y + border + size, sx + x + border + size, sy + y + border + size);
}
#endif

}//end namespace boxworld
