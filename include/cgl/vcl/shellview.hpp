
//列表壁纸
int ListView_ListImage(TListView* ListView)
{
	Gdiplus::Bitmap *bitmap = NULL;
    TListItem* item = NULL;
	WIN32_FIND_DATA wfd = {0};
    String fname = L"../wallpaper/";
	HANDLE f = FindFirstFile(L"../wallpaper/*.*", &wfd);
    if(f==INVALID_HANDLE_VALUE)
    {
    	return -1;
    }

	ListView->Items->BeginUpdate();
    do
    {
    	if(wcscpy(L".", wfd.cFileName) && wcscpy(L"..", wfd.cFileName))
        {
            bitmap = image_load((fname+wfd.cFileName).c_str(), IMAGE_WIDTH, IMAGE_HEIGHT);
            if(bitmap)
            {
                item = ListView->Items->Add();
                item->Caption = fname+wfd.cFileName;//名字保存路径
                item->Data = bitmap;
            }
        }

	}while(FindNextFile(f, &wfd));
	ListView->Items->EndUpdate();

    return 0;
}


void ShellView(TListView* ListView, LPITEMIDLIST lpPIDL)
{
	_di_IShellFolder desktop;
	_di_IShellFolder ShellFolder;
	_di_IEnumIDList EnumList;
	PItemIDList ID;
	unsigned long NumIDs;
	TCursor SaveCursor;
	SHFILEINFO fi = {0};
	LPITEMIDLIST full_idl;

	UINT Flags = SHCONTF_STORAGE|SHCONTF_FOLDERS|SHCONTF_INCLUDEHIDDEN;//SHCONTF_FASTITEMS;

	SHGetDesktopFolder(&desktop);
	if(FAILED(desktop->BindToObject(lpPIDL, NULL, IID_IShellFolder, (void**)&ShellFolder)))
	{
		ShellFolder = desktop;
	}

	SaveCursor = Screen->Cursor;
	try
	{
		Screen->Cursor = crHourGlass;
		ShellFolder->EnumObjects(NULL, Flags, &EnumList);

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

			TListItem* Icon = ListView->Items->Add();
			Icon->Caption = fi.szDisplayName;
			Icon->ImageIndex = fi.iIcon;
			//Icon->shell = GetDisplayName(desktop, full_idl, SHGDN_FORPARSING);

			ILFree(full_idl);
		}//end while
	}
	__finally
	{
		Screen->Cursor = SaveCursor;
	}
}