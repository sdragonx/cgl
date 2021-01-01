//---------------------------------------------------------------------------


#pragma hdrstop

#include "icon.h"
#include "main.h"
//#include "title.h"
#include "shellview.h"
#include "panel.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
//icon_image

icon_image::icon_image():m_fimage()
{
	m_bitmap = NULL;
	m_icon = NULL;
	m_index = -1;
	m_sheard = false;
	m_size = 48;

	alpha = 0xFF;
}
icon_image::~icon_image()
{
	close();
}

int icon_image::open_image(String fname, int id, int sz)
{
	String ext = ExtractFileExt(fname).LowerCase();
	this->close();
	m_fimage = fname;
	m_size = sz;
	if(!fname.IsEmpty() && fname[1]=='@')
	{
		int csidl = CSIDL_Index(fname);
		if(csidl!=-1)
		{
			SHFILEINFO fi = {0};
			LPITEMIDLIST lpidl;
			SHGetSpecialFolderLocation(NULL, CSIDL_LIST[csidl].id, &lpidl);
			SHGetFileInfo((TCHAR*)lpidl, 0, &fi, sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_SYSICONINDEX);
			ILFree(lpidl);
			m_index = fi.iIcon;
			m_sheard = true;
		}
	}
	else if(ext==L".png")
	{
		m_sheard = false;
		m_index = -1;
		//默认加载png缩放大小
		m_bitmap = bm_load(m_fimage.c_str(), ICON_RES_SIZE, ICON_RES_SIZE);
		#ifdef _DEBUG
		if(!m_bitmap)ShowMessage(m_fimage + "load err");
		#endif
	}
	else if((ext==L".exe" || ext==L".dll") && id>0)
	{
		m_sheard = false;
		m_index = id;
		CallSHDefExtractIcon(m_fimage.c_str(), m_index, 0, &m_icon, NULL, m_size);
	}
	else if(!m_fimage.IsEmpty())//sheard system icon
	{
		m_sheard = true;
		m_index = GetSysIconIndex(m_fimage.c_str());
	}
	else
	{
		m_sheard = true;
		m_index = id;
	}
	return 0;
}

void icon_image::close()
{
	if(m_bitmap)
	{
		DeleteObject(m_bitmap); m_bitmap = NULL;
	}
	if(m_icon)
	{
		DestroyIcon(m_icon); m_icon = NULL;
	}
}

int draw_icon(HDC hdc, int x, int y, int size, int index)
{
	IImageList *imageList = NULL;
	HRESULT hResult = S_FALSE;
	//IMAGELISTDRAWPARAMS dp = {0};

	switch(size)
	{
	case 32:
		hResult = CallSHGetImageList(SHIL_LARGE, __uuidof(IImageList), (void**)&imageList);
		break;
	case 48:
	default:
		hResult = CallSHGetImageList(SHIL_EXTRALARGE, __uuidof(IImageList), (void**)&imageList);
		break;
	}

	if(S_OK == hResult)
	{
		//imagelist->DrawIcon(
		ImageList_DrawEx((HIMAGELIST)imageList, index, hdc, x, y, size, size, CLR_NONE, CLR_NONE,//CLR_DEFAULT,
			//ILD_PRESERVEALPHA);
			//ILD_IMAGE|
			ILD_TRANSPARENT);
			//ILD_TRANSPARENT);//|ILD_PRESERVEALPHA);//
			//|ILD_PRESERVEALPHA);
			//ILD_IMAGE|ILD_PRESERVEALPHA ILD_TRANSPARENT);        ILD_SCALE

			//ILD_TRANSPARENT 显示没有问题，但是256色图标还是全透
			//ILD_SCALE|ILD_TRANSPARENT|ILD_PRESERVEALPHA);//win7没有问题。xp下面普通绘制

		//leayered window ok
		//ImageList_DrawEx((HIMAGELIST)imageList, index, hdc, x, y, size, size, CLR_NONE, CLR_NONE, ILD_PRESERVEALPHA);
	}
	return 0;
}

int icon_image::paint(HDC handle, int x, int y, int w, int h)
{
	IImageList* imagelist = 0;

	if(m_bitmap)
	{
		bm_paint(handle, x, y, w, h, m_bitmap, 0, 0, ICON_RES_SIZE, ICON_RES_SIZE, alpha);
	}
	#if 0
	else
	{//锯齿、layered Windows 穿透问题
		draw_icon(handle, x, y, w, m_index);
	}
	#else
	else
	{
		BITMAP bm = {0};
		HDC dc = CreateCompatibleDC(NULL);
		HBITMAP bmp = bm_create(m_size, m_size, 32);
		//HBITMAP bmp = bm_create(48, 48, 32);
		SelectObject(dc, bmp);
		bm_clear(bmp, GDI_ALPHA_MARK);
		//bm_clear(bmp, 0);
		if(m_icon)
			DrawIconEx(dc, 0, 0, m_icon, m_size, m_size, 0, NULL, DI_NORMAL);
		else if(m_index!=-1)
			draw_icon(dc, 0, 0, m_size, m_index);
		GetObject(bmp, sizeof(BITMAP), &bm);
		bm_mkalpha(bm);
		if(m_size!=w)
		{
			HBITMAP bmpx = bm_create(w, h, 32);
			SelectObject(dc, bmpx);
			bilinear_blt(bmpx, bmp);
			alphablend(handle, x, y, w, h, dc, 0, 0, w, h, alpha);
			DeleteObject(bmpx);bmpx = NULL;
		}
		else
		{
			alphablend(handle, x, y, w, h, dc, 0, 0, w, h, alpha);
		}
		DeleteObject(bmp); bmp = NULL;
		DeleteDC(dc); dc = NULL;
	}
	#endif
	return 0;
}

int icon_image::fastpaint(HDC handle, int x, int y, int w, int h)
{
	int imagelist;
	if(m_bitmap)
	{
		bm_paint(handle, x, y, w, h, m_bitmap, 0, 0, ICON_RES_SIZE, ICON_RES_SIZE, alpha);
	}
	else
	{
		if(m_icon)
		{
			DrawIconEx(handle, 0, 0, m_icon, m_size, m_size, 0, NULL, DI_NORMAL);
        }
		else if(m_index!=-1)
		{
			CallSHGetImageList(SHIL_EXTRALARGE, __uuidof(IImageList), (void**)&imagelist);
			ImageList_DrawEx((HIMAGELIST)imagelist, m_index, handle, x, y, m_size, m_size, CLR_NONE, CLR_NONE,//CLR_DEFAULT,
				ILD_TRANSPARENT);
        }
	}
	return 0;
}

//---------------------------------------------------------------------------
//TWindow

__fastcall TWindow::TWindow(TComponent* AOwner)
	:TCustomLabel(AOwner), image(), idlist()
{
	this->Transparent = true;
	this->AutoSize = false;
	this->ParentFont = true;
	this->Tag = 0;
	this->Cursor = crHandPoint;
	this->ShowHint = true;

	this->Size = ICON_SIZE;

	//itemid = NULL;
	hotspot	 = false;
	selected = false;
	lock = false;
	show_title = false;
	mode = ICON_RANDER_ZOOM;

	this->OnMouseDown = Icon_MouseDown;
	this->OnMouseEnter = Icon_MouseEnter;
	this->OnMouseLeave = Icon_MouseLeave;
	this->OnMouseMove = Icon_MouseMove;
	this->OnMouseUp = Icon_MouseUp;
}

__fastcall TWindow::~TWindow()
{
	//if(itemid)PIDL_Free(itemid);

}

int TWindow::config_save(TIniFile* ini)
{
	if(!ini)return -1;
	String section = group+IntToStr(id);
	ini->WriteString(section, L"title", Caption);
	ini->WriteString(section, L"shell", shell);
	ini->WriteString(section, L"icon", image.get_image());
	ini->WriteInteger(section, L"index", image.get_index());
	ini->WriteString(section, L"comment", Hint);
	ini->WriteBool(section, L"lock", lock);
	return 0;
}


//绘制图标，分层窗口
int TWindow::PaintTo(HDC dc, int left, int top)
{
	TWindow* window = this;
	//绘制一个alpha只有1的背景，防止鼠标穿透
	alpha_rect(dc, left+window->Left, top+window->Top, window->Width, window->Height, 0xFFFFFFFF, 1);
	//绘制图标
	window->image.paint(dc,
		left+window->Position.x-window->Size/2,
		top+window->Position.y-window->Size/2,
		window->Size, window->Size);
	//绘制标题
	if(window->show_title && !window->Caption.IsEmpty())
	{
		TRect rc = window->BoundsRect;
		//int offset = (ICON_SIZE+PANEL_ICON_STEP-window->Width)/2;
		int offset = (TITLE_WIDTH - window->Size)/2;
		OffsetRect(rc, left+0, top+window->Size+window->Size/16);
		rc.left -= offset;
		rc.right += offset;
		draw_text(dc, window->Caption.c_str(), -1, rc, clWhite, DT_CENTER|DT_WORDBREAK|DT_EDITCONTROL|DT_CALCRECT);
		draw_text(dc, window->Caption.c_str(), -1, rc, clWhite);
	}
	return 0;
}

int TWindow::GetTextOffsetY()
{
	return ClientWidth-(ClientWidth-Size)/2+Size/8;
}

int TWindow::SetDisplayRect()
{
	if(hotspot)
	{
		TRect rc = ClientRect;
		rc.top = GetTextOffsetY();
		int h = draw_text(Canvas->Handle, Caption.c_str(), -1, rc, clWhite,
				DT_CENTER|DT_WORDBREAK|DT_EDITCONTROL|DT_CALCRECT);
		if(h>=rc.Height())
		{
			this->BringToFront();
			this->Height = rc.top+h + 8;
		}
	}
	else
	{
		TWinControl* powner = static_cast<TWinControl*>(Owner);
		if(GetWindowLong(powner->Handle, GWL_EXSTYLE)&WS_EX_LAYERED)
		{
			this->Height = FSize*1.5;
		}
		else
		{
			this->Height = FSize*2+20;
		}
	}
	return 0;
}

void __fastcall TWindow::DoDrawText(Types::TRect &Rect, int Flags)
{
	if(hotspot)// || selected)
	{
		Canvas->Pen->Color = clSkyBlue;
		Canvas->Rectangle(Rect);
		alpha_rect(Canvas->Handle, Rect, clSkyBlue, 64);
    }

	//绘制图标
	image.paint(Canvas->Handle,	(ClientWidth-Size)/2,	(ClientWidth-Size)/2,	Size, Size);
	//绘制标题
	if(!Caption.IsEmpty())
	{
		TRect rc = ClientRect;
		rc.top = GetTextOffsetY();
		if(!hotspot)
		{
            draw_text(Canvas->Handle, Caption.c_str(), -1, rc, clWhite,
				DT_CENTER|DT_END_ELLIPSIS);
		}
		else
		{
			draw_text(Canvas->Handle, Caption.c_str(), -1, rc, clWhite,
				DT_CENTER|DT_WORDBREAK|DT_EDITCONTROL);
        }
	}
	//this->PaintTo(Canvas->Handle, -Left, -Top);
}

void __fastcall TWindow::WndProc(Messages::TMessage &Message)
{

	switch(Message.Msg)
	{
	case WM_DESTROY:
		break;
	case WM_ERASEBKGND:
		Message.Result = 1;
		break;
	case 45075://MOUSEENTER
		//hotspot	= true;
		//this->Repaint();
		Icon_MouseEnter(this);
		break;
	case 45076://MOUSELEAVE
		//hotspot	= false;
		//this->Repaint();
		Icon_MouseLeave(this);
		break;
	case 45066://MOUSEMOVE
		break;
	default:
		break;
	}
	TCustomLabel::WndProc(Message);
}

//按钮缩放效果
void Icon_zoom_in(TWindow* window)
{//放大
	window->Size = ICON_MAX_SIZE;
	//window->Parent->Perform(WM_PAINT, 0, 0);
}

void Icon_zoom_out(TWindow* window)
{//缩小
	window->Size = ICON_SIZE;
	//window->Parent->Perform(WM_PAINT, 0, 0);
}

//按钮偏移效果
void Icon_offset(TWindow* window, int x, int y)
{//
	int size = ICON_SIZE/2;
	window->Left = window->Position.x-size+x;
	window->Top = window->Position.y-size+y;
	//window->Parent->Perform(WM_PAINT, 0, 0);
}

void __fastcall TWindow::Icon_MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	TWindow* window = static_cast<TWindow*>(Sender);
	selected = true;
	if(mbLeft == Button)
	{
		if(mode == ICON_RANDER_ZOOM)
			Icon_zoom_out(window);
		else if(mode == ICON_RANDER_OFFSET)
			Icon_offset(window, 1, 2);
		this->ReDraw();
	}
}

void __fastcall TWindow::Icon_MouseEnter(TObject *Sender)
{//
	TWindow* window = static_cast<TWindow*>(Sender);
	window->hotspot = true;
	if(mode == ICON_RANDER_ZOOM)
		Icon_zoom_in(window);
	else if(mode == ICON_RANDER_OFFSET)
		Icon_offset(window, -1, -2);

	SetDisplayRect();

	this->ReDraw();

	PlaySound(MAKEINTRESOURCE(300), GetModuleHandle(0), SND_RESOURCE|SND_ASYNC);

	#ifdef titleH
	if(!window->show_title)//是否显示标题
	{
		frmTitle->Left = window->Parent->Left+window->Left-(TITLE_WIDTH-window->Size)/2;
		frmTitle->Top = window->Parent->Top+window->Top+window->Height;
		frmTitle->Caption = window->Caption;
		frmTitle->Show();
	}
	#endif
}

void __fastcall TWindow::Icon_MouseLeave(TObject *Sender)
{//
	TWindow* window = static_cast<TWindow*>(Sender);
	window->hotspot = false;
	if(mode == ICON_RANDER_ZOOM)
		Icon_zoom_out(window);
	else if(mode == ICON_RANDER_OFFSET)
		Icon_offset(window, 0, 0);
	SetDisplayRect();
	this->ReDraw();
	#ifdef titleH
	frmTitle->Hide();
	#endif
}

void __fastcall TWindow::Icon_MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{//

}

void __fastcall TWindow::Icon_MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{//
	TWindow* window = static_cast<TWindow*>(Sender);
	selected = false;
	if(mbLeft == Button)
	{
		if(mode == ICON_RANDER_ZOOM)
			Icon_zoom_in(window);
		else if(mode == ICON_RANDER_OFFSET)
			Icon_offset(window, -1, -2);
		this->ReDraw();

		//Shell
		if(PtInRect(ClientRect, TPoint(X, Y)))
		{
			this->shellexec();
		}
	}
	else if(window->Owner->ClassName() == TfrmShellView::ClassName() ||
		window->Owner->ClassName() == TfrmMain::ClassName())
	{
		TPoint pt;
		GetCursorPos(&pt);
		if(!this->PopupMenu)
			InvokeContextMenu(this->Parent->Handle, idlist, pt.x, pt.y);
	}
}


#if 0//创建和连接“宽带连接”
if(window->shell == L"@rasdial")
{
	//ConnectionCreate();//创建连接
	//CallShellExecute<TCHAR>(NULL, NULL, TEXT("rasphone"), TEXT("-d \"宽带连接\""), NULL, SW_SHOW);
	TCHAR buf[MAX_PATH]={0};
	SHGetFolderPath(NULL, CSIDL_NETHOOD, NULL, SHGFP_TYPE_CURRENT, buf);
	ShowMessage(buf);
}
else
#endif

//执行命令
int TWindow::shellexec()
{
	TPoint pt;
	if(shell.IsEmpty())
	{
		return -1;
	}
	DWORD attrib = GetFileAttributes(shell.c_str());
	if(attrib == INVALID_FILE_ATTRIBUTES)attrib = 0;
	if(shell[1] == '@')
	{
		int csidl = CSIDL_Index(shell);
		if(csidl<0)return -1;
		csidl = CSIDL_LIST[csidl].id;

		switch(csidl)
		{
		case CSIDL_NETWORK:
			CallShellExecute<wchar_t>(NULL, NULL,
				TEXT("::{7007ACC7-3202-11D1-AAD2-00805FC1270E}"), NULL, NULL, SW_SHOW);
			return 0;
		default:
			break;
		}

		TfrmShellView *shellview = new TfrmShellView(Application->MainForm);
		shellview->Parent = NULL;
		GetCursorPos(&pt);
		shellview->Caption = Caption;
		shellview->Position = pt;
		shellview->ShellView(csidl);
	}
	else if((attrib & FILE_ATTRIBUTE_DIRECTORY) &&
		this->Owner->ClassName() == TfrmPanel::ClassName())
	{
		TfrmShellView *shellview = new TfrmShellView(Application->MainForm);
		shellview->Parent = NULL;
		GetCursorPos(&pt);
		shellview->Caption = Caption;
		shellview->Position = pt;//this->Owner->ClientToParent(Position);//ClientToScreen(Position);
		shellview->ShellView(shell);
	}
	else
	{
		CallShellExecute<wchar_t>(NULL, NULL, shell.c_str(), NULL, NULL, SW_SHOW);
	}
	return 0;
}

int __fastcall TWindow::GetAlpha()
{
	return image.alpha;
}

void __fastcall TWindow::SetAlpha(int alpha)
{
	if(alpha <= 0)
	{
		image.alpha = 0;
		this->Visible = false;
		return ;
	}
	else
	{
		if(alpha >=255)
		{
			image.alpha = 255;
		}
		else
		{
			image.alpha = alpha;
		}
		this->Visible = true;
	}
}

void __fastcall TWindow::SetPosition(TPoint pt)
{
	FPosition = pt;
	this->Left = pt.x - this->Width/2;
	this->Top = pt.y - this->Width/2;
}

void __fastcall TWindow::SetSize(int ASize)
{
	FSize = ASize;
	TWinControl* powner = static_cast<TWinControl*>(Owner);
	if(GetWindowLong(powner->Handle, GWL_EXSTYLE)&WS_EX_LAYERED)
	{
		this->Width = ASize;
		this->Height = ASize*1.5;
	}
	else
	{
		this->Width = ASize*2;
		this->Height = ASize*2+20;
	}
	this->Left = Position.x - this->Width/2;
	this->Top = Position.y - this->Width/2;
}

void __fastcall TWindow::SetShell(String path)
{
	Fshell = path;
	if(Fshell.IsEmpty())
	{
		idlist.free();
		return ;
    }
	if(Fshell[1] == '@')
	{
		int csidl = CSIDL_Index(shell);
		if(csidl<0)return ;
		SHGetSpecialFolderLocation(NULL, csidl, &idlist);
	}
	else
	{
		idlist.assign(path.c_str());
    }
}

int __fastcall TWindow::GetSize()
{
	return FSize;
}

//重绘函数
void TWindow::ReDraw()
{
	if(GetWindowLong(Parent->Handle, GWL_EXSTYLE)&WS_EX_LAYERED)
	{
		Parent->Perform(WM_PAINT, 0, 0);
	}
	else
	{
		this->Repaint();
    }
}

//---------------------------------------------------------------------------
//通用函数

void __fastcall ShellView(vector<TWindow*>& List, LPITEMIDLIST lpPIDL, TWinControl* owner, int iconsize, bool filter)
{
	_di_IShellFolder desktop;
	_di_IShellFolder ShellFolder;
	PItemIDList ID;
	_di_IEnumIDList EnumList;
	unsigned long NumIDs;
	TCursor SaveCursor;
	SHFILEINFO fi = {0};
	LPITEMIDLIST full_idl;
	//WIN32_FIND_DATA wfd = {0};
	UINT Flags = SHCONTF_STORAGE|SHCONTF_FOLDERS|SHCONTF_INCLUDEHIDDEN;//SHCONTF_FASTITEMS;

	SHGetDesktopFolder(&desktop);
	if(FAILED(desktop->BindToObject(lpPIDL, NULL, IID_IShellFolder, (void**)&ShellFolder)))
	{
		//SHGetDesktopFolder(&ShellFolder);
		ShellFolder = desktop;
	}

	auto_ptr<TStringList> sl(new TStringList);
	bool show = false;
	if(filter)
	{//读取过滤词
		auto_ptr<TIniFile> ini(new TIniFile(CONFIG_FILE));
		sl->Delimiter = ',';
		sl->DelimitedText = ini->ReadString(CONFIG_MAIN, L"iconlist", STRING_NULL);
	}

	SaveCursor = Screen->Cursor;
	try
	{
		Screen->Cursor = crHourGlass;
		OleCheck(ShellFolder->EnumObjects(NULL, Flags, &EnumList));

		while(EnumList->Next(1, &ID, &NumIDs) == S_OK)
		{
			full_idl = ILCombine(lpPIDL, ID);
			ILFree(ID);
			SHGetFileInfo((TCHAR*)full_idl, 0, &fi, sizeof(fi),
				SHGFI_PIDL|SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);

			if(!_tcsicmp(fi.szDisplayName, TEXT("desktop.ini")) )
			{
				ILFree(full_idl);
				continue;
			}

			if(filter)
			{//过滤代码
				show = true;
				for(int i=0; i<sl->Count; ++i)
				{
					//if(String(fi.szDisplayName).LowerCase() == sl->Strings[i].LowerCase())//完整匹配
					if(String(fi.szDisplayName).LowerCase().Pos(sl->Strings[i].LowerCase()))//简单的模糊匹配
					{
						show = false;
						break;
					}
				}
				if(!show){
					ILFree(full_idl);
					continue;
				}
			}

			TWindow *Icon = new TWindow(owner);
			Icon->Parent = owner;
			Icon->Size = iconsize;
			Icon->mode = 0;
			Icon->show_title = true;
			//Icon->PopupMenu = frmMain->mnuContext;

			Icon->Caption = fi.szDisplayName;
			Icon->image.open_image(STRING_NULL, fi.iIcon, iconsize);
			Icon->shell = GetDisplayName(desktop, full_idl, SHGDN_FORPARSING);
			#if 0 //网络连接只返回一个GUID，不知道怎么应用
			if(Icon->shell[1] == '{')
			{
				Icon->shell = L"::{7007ACC7-3202-11D1-AAD2-00805FC1270E}\\::" + Icon->shell;
				frmMain->Memo1->Lines->Add(Icon->shell);
			}
			//frmMain->Memo1->Lines->Add(Icon->shell);
			//Icon->PopupMenu = frmMain->mnuIconMenu;
			/*
			frmMain->Memo1->Lines->Add(GetDisplayName(desktop, full_idl, SHGDN_NORMAL));
			frmMain->Memo1->Lines->Add(GetDisplayName(desktop, full_idl, SHGDN_INFOLDER));
			frmMain->Memo1->Lines->Add(GetDisplayName(desktop, full_idl, SHGDN_FOREDITING));
			frmMain->Memo1->Lines->Add(GetDisplayName(desktop, full_idl, SHGDN_FORADDRESSBAR));
			*/
			frmMain->Memo1->Lines->Add(GetDisplayName(desktop, full_idl, SHGDN_FORPARSING));
			#endif

			List.push_back(Icon);
			ILFree(full_idl);
		}//end while
	}
	__finally
	{
		Screen->Cursor = SaveCursor;
	}
}

void __fastcall ViewFolder(vector<TWindow*>& List, TWinControl* owner, String Path)
{
	SHFILEINFO fi = {0};
	HANDLE hfind;
	WIN32_FIND_DATA wfd = {0};
	UINT Flags = SHCONTF_FOLDERS;// | SHCONTF_INCLUDEHIDDEN;
	//DWORD attrib = 0;

	if(*Path.LastChar()!=TEXT('\\'))Path+=TEXT('\\');
	hfind = FindFirstFile((Path+TEXT('*')).c_str(), &wfd);
	if(hfind == INVALID_HANDLE_VALUE)
	{
		ShowMessage(TEXT("ShellView 文件查找失败！"));
		return ;
	}
	do
	{
		_wcslwr(wfd.cFileName);
		if(!_tcscmp(wfd.cFileName, TEXT(".")) || !_tcscmp(wfd.cFileName, TEXT("..")) ||
			!_tcsicmp(wfd.cFileName, TEXT("desktop.ini")) )
			continue;

		String fullpath = Path + wfd.cFileName;

		TWindow *Icon = new TWindow(owner);
		//Icon->Parent = pnlView;
		Icon->Size = 32;
		Icon->mode = 0;

		SHGetFileInfo(fullpath.c_str(), 0, &fi, sizeof(SHFILEINFO),
			SHGFI_DISPLAYNAME|SHGFI_SYSICONINDEX);

		Icon->Caption = fi.szDisplayName;
		Icon->id = fi.iIcon;
		Icon->image.open_image(STRING_NULL, fi.iIcon, 32);

		Icon->shell = fullpath;
		Icon->show_title = true;
		List.push_back(Icon);
	}while(FindNextFile(hfind, &wfd));//end while
}

//插入宽带连接
#if 0
void InsertLink(vector<TWindow*>& list)
{
	TCHAR path[MAX_PATH] = {0};
	SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, path);
	_tcscat(path, L"\\rasdlg.dll");
	TWindow* win = new TWindow(frmMain);
	win->Parent = frmMain;
	win->Size = 48;
	win->mode = 0;
	win->Caption = L"宽带连接";
	win->shell = L"@rasdial";
	DWORD ver = GetVersion();
	win->image.open_image(path, 19+(GetVersion()&0xFF)-5, 48);
	//win->Hint = ini->ReadString(section, CONFIG_COMMENT, STRING_NULL);
	win->lock = true;
	win->show_title = true;
	list.push_back(win);
}
#endif



int __fastcall InvokeContextMenu(HWND Owner, String File, int X, int Y)
{
	idlist_t id;
	id.assign(File.c_str());
    return InvokeContextMenu(Owner, id, X, Y);
}

int __fastcall InvokeContextMenu(HWND owner, const idlist_t& id, int X, int Y)
{
	_di_IShellFolder IDesktop;
	_di_IShellFolder IParent;
	_di_IContextMenu CM;
	HMENU hMenu;
	int Command;

	idlist_t id_dir;

	if(id.is_empty())
	{
		return -1;
	}

	try
	{
		id_dir = id;
		SHGetDesktopFolder(&IDesktop);
		if(id_dir.pop())
		{
			if(IDesktop->BindToObject(id_dir, NULL, IID_IShellFolder, (void**)&IParent) != S_OK)
			{
				 IParent = IDesktop;
			}
			LPITEMIDLIST id_file = ILFindLastID(id);
			if(id_file)
			{
				IParent->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&id_file, IID_IContextMenu, NULL, (void**)&CM);
			}
		}
	}
	__finally
	{
	}

	if(CM == NULL)return -2;

	hMenu = CreatePopupMenu();
	try
    {
		CM->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_VERBSONLY);//|CMF_NOVERBS|CMF_EXPLORE);
		try
		{
			Command = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON |
						  TPM_RETURNCMD, X, Y, 0, owner, NULL);
		}
		__finally
		{
		}

		if(Command>0)
        {
            int ICmd = Command - 1;

			CMINVOKECOMMANDINFO ICI;
			memset(&ICI, 0, sizeof(CMINVOKECOMMANDINFO));
			ICI.cbSize = sizeof(CMINVOKECOMMANDINFO);
			if (owner)
				ICI.hwnd = owner;
			else
                ICI.hwnd = Application->Handle;
			ICI.lpVerb = MakeIntResourceA(ICmd);
            ICI.nShow = SW_SHOWNORMAL;
			CM->InvokeCommand(&ICI);
		}
    }
	__finally
    {
		DestroyMenu(hMenu); hMenu = NULL;
	}
	return 0;
}


void IconList_Clear(vector<TWindow*>& List)
{
	for(size_t i=0; i<List.size(); ++i)
	{
		delete List[i];
		List[i] = NULL;
	}
	List.clear();
}
