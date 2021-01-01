//---------------------------------------------------------------------------

#ifndef iconH
#define iconH

const int ICON_SIZE		= 64;	//ͼ���С
const int ICON_RES_SIZE = 64;	//����png��С
const int ICON_MAX_SIZE = 68;	//ͼ�����Ŵ�С
const int TITLE_WIDTH	= 80;	//������ʾ���

const int ICON_RANDER_MODE = 0;	//0����Ч 1 ���� 2 ƫ��
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
	HBITMAP m_bitmap;	//����ʱ���룬�˳�ʱҪ����
	HICON	m_icon;		//ͼ����
	String	m_fimage;	//ͼ��ͼ���ļ�
	int		m_index;	//ͼ��������ϵͳ���������ļ�������
	bool	m_sheard;	//�Ƿ�����Դ
	int		m_size;		//����ͼ���С
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
	std::wstring title;	//����
	std::wstring link;	//���еĳ�����ҳ����
	std::wstring url;	//���ص�ַ
	std::wstring hint;	//��ʾ��Ϣ
	icon_image image;
	//std::vector<icon_info*> subicons;//��ͼ��
};

//
class TWindow:public TCustomLabel
{
private:
	//bool bHotspot;//����
	String Fshell;	//�����ļ�������
	int    FSize;
	TPoint FPosition;	//window�ľ���λ��
	bool   selected;    //ѡ��
	bool   hotspot;		//�Ƿ��ȵ�
	idlist_t idlist;
public://�Զ������
	String group;	//config��������
	int id;			//config����ID
	icon_image image;
	//String title;	//Caption
	//String shell;	//�����ļ�������
	String url;		//��ַ
	//String comment;	//Hint
	bool   lock;		//���������ɱ༭
	bool   show_title;	//�Ƿ���ʾ����
	int    mode;		//����Ч��
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
	Classes::TNotifyEvent OnPaint;//�ػ��¼�
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
private://��Ϣ����
	void __fastcall Icon_MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
			  int X, int Y);
	void __fastcall Icon_MouseEnter(TObject *Sender);
	void __fastcall Icon_MouseLeave(TObject *Sender);
	void __fastcall Icon_MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Icon_MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
};

//TWindow* NewIcon(TWinControl* parent, int id, String title, String shell, String icon, int index)
//�о��ļ�
void __fastcall ShellView(vector<TWindow*>& List, LPITEMIDLIST lpPIDL, TWinControl* owner,
	int iconsize, bool filter = false);

void __fastcall ViewFolder(vector<TWindow*>& List, TWinControl* owner, String Path);
void InsertLink(vector<TWindow*>& list, int clsid, TWinControl* owner);

//���ͼ���б�
void IconList_Clear(vector<TWindow*>& List);

//�����˵�
int __fastcall InvokeContextMenu(HWND owner, const idlist_t& id, int X, int Y);
int __fastcall InvokeContextMenu(HWND owner, String File, int X, int Y);

#endif
