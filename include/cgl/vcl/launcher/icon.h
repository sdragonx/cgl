//---------------------------------------------------------------------------

#ifndef iconH
#define iconH

const int ICON_SIZE		= 64;	//图标大小
const int ICON_RES_SIZE = 64;	//载入png大小
const int ICON_MAX_SIZE = 68;	//图标缩放大小
const int TITLE_WIDTH	= 80;	//标题显示宽度

const int ICON_RANDER_MODE = 0;	//0无特效 1 缩放 2 偏移
const int ICON_RANDER_ZOOM = 1;
const int ICON_RANDER_OFFSET = 2;

//2015-03-25 18:49:14
//---------------------------------------------------------------------------

#include <vcl.h>
#include <Controls.hpp>
#include "public.h"
#include <stdlib.h>

class icon_image
{
private:
	HBITMAP m_bitmap;	//绘制时载入，退出时要清理
	HICON	m_icon;		//图标句柄
	String	m_fimage;	//图像、图标文件
	int		m_index;	//图标索引（系统索引或者文件索引）
	bool	m_sheard;	//是否共享资源
	int		m_size;		//加载图像大小
public:
	int 	alpha;
public:
	icon_image();
	~icon_image();

	int open_image(String fname, int id = -1, int sz=48);

	String get_image()
	{
        return m_fimage;
	}

	int get_index()
	{
        return m_index;
	}

	/*
	bool is_open()const
	{
		return m_bitmap || m_icon;
	}
	//*/

	void close();
	int paint(HDC handle, int x, int y, int w, int h);
	int fastpaint(HDC handle, int x, int y, int w, int h);
};

struct icon_t
{
	std::wstring title;	//标题
	std::wstring link;	//运行的程序、网页连接
	std::wstring url;	//下载地址
	std::wstring hint;	//提示信息
	icon_image image;
	//std::vector<icon_info*> subicons;//子图标
};

//
class TWindow:public TCustomLabel
{
private:
	//bool bHotspot;//焦点
	String Fshell;	//运行文件、连接
	int    FSize;
	TPoint FPosition;	//window的绝对位置
	bool   selected;    //选中
	bool   hotspot;		//是否热点
	idlist_t idlist;
public://自定义变量
	String group;	//config保存名字
	int id;			//config保存ID
	icon_image image;
	//String title;	//Caption
	//String shell;	//运行文件、连接
	String url;		//网址
	//String comment;	//Hint
	bool   lock;		//锁定，不可编辑
	bool   show_title;	//是否显示标题
	int    mode;		//绘制效果
	__property String shell = {read=Fshell, write=SetShell};
public:
	__property int Size = {read=GetSize, write=SetSize};
	__property TPoint Position = {read=FPosition, write=SetPosition};
	__property int Alpha = {read=GetAlpha, write=SetAlpha};
	__property PopupMenu;
	__property OnMouseDown;
	__property OnMouseEnter;
	__property OnMouseLeave;
	__property OnMouseMove;
	__property OnMouseUp;
	Classes::TNotifyEvent OnPaint;//重绘事件
public:
	//void __fastcall CreateParams(TCreateParams Params)
	__fastcall TWindow(TComponent* AOwner);
	__fastcall virtual ~TWindow();
	int config_save(TIniFile* ini);
	int shellexec();
	//config_load
	int PaintTo(HDC dc, int left=0, int top=0);
	void ReDraw();
	int GetTextOffsetY();
	int SetDisplayRect();
__published:
protected:
	void __fastcall SetShell(String path);
	int __fastcall GetAlpha();
	void __fastcall SetAlpha(int alpha);
	void __fastcall SetPosition(TPoint pt);
	int __fastcall GetSize();
	void __fastcall SetSize(int ASize);
	void __fastcall WndProc(Messages::TMessage &Message);
	DYNAMIC void __fastcall TWindow::DoDrawText(Types::TRect &Rect, int Flags);
private://消息处理
	void __fastcall Icon_MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
			  int X, int Y);
	void __fastcall Icon_MouseEnter(TObject *Sender);
	void __fastcall Icon_MouseLeave(TObject *Sender);
	void __fastcall Icon_MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Icon_MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
};

//TWindow* NewIcon(TWinControl* parent, int id, String title, String shell, String icon, int index)
//列举文件
void __fastcall ShellView(vector<TWindow*>& List, LPITEMIDLIST lpPIDL, TWinControl* owner,
	int iconsize, bool filter = false);

void __fastcall ViewFolder(vector<TWindow*>& List, TWinControl* owner, String Path);
void InsertLink(vector<TWindow*>& list, int clsid, TWinControl* owner);

//清空图标列表
void IconList_Clear(vector<TWindow*>& List);

//弹出菜单
int __fastcall InvokeContextMenu(HWND owner, const idlist_t& id, int X, int Y);
int __fastcall InvokeContextMenu(HWND owner, String File, int X, int Y);

#endif
