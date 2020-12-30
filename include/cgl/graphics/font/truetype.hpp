/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 font_ttf.hpp

 2020-02-29 00:57:29

*/
#ifndef FONT_TTF_HPP_20200229005729
#define FONT_TTF_HPP_20200229005729

#include <cgl/public.h>

namespace cgl{

struct TTF_HEADER
{
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint16_t numTables;
    uint16_t searchRange;
    uint16_t entrySelector;
    uint16_t rangeShift;
};

struct TTF_TABLE
{
    char name[4];       //table name
    uint32_t checkSum;
    uint32_t offset;    //Offset from beginning of file
    uint32_t length;    //length of the table in bytes
};

struct TTF_NAME_HEADER {
    uint16_t format;    //format selector. Always 0
    uint16_t count;     //Name Records count
    uint16_t offset;    //Offset for strings storage, from start of the table
};

struct TTF_NAME_TABLE {
    uint16_t platformID;
    uint16_t encodingID;
    uint16_t languageID;
    uint16_t nameID;
    uint16_t length;
    uint16_t offset;    //from start of storage area
};

unistring GetFontNameFromFile(LPCTSTR lpszFilePath)
{
	FILE* f = _tfopen(lpszFilePath, TEXT("rb"));
	unistring csRetVal;

	if(f){
		TTF_HEADER ttfEntry;
		fread(&ttfEntry, sizeof(TTF_HEADER), 1, f);
		ttfEntry.numTables = bswap16(ttfEntry.numTables);
		ttfEntry.majorVersion = bswap16(ttfEntry.majorVersion);
		ttfEntry.minorVersion = bswap16(ttfEntry.minorVersion);

		//check is this is a true type font and the version is 1.0
		if(ttfEntry.majorVersion != 1 || ttfEntry.minorVersion != 0)
			return csRetVal;

		TTF_TABLE ttfTab;
		BOOL bFound = FALSE;
	    union{
            char    csTemp[256];
            wchar_t wcsTemp[128];
        };

		for(int i=0; i< ttfEntry.numTables; i++){
			fread(&ttfTab, sizeof(TTF_TABLE), 1, f);
			strncpy(csTemp, ttfTab.name, 4);
            csTemp[4] = 0;
			if(strcmp(csTemp, "name") == 0){
				bFound = TRUE;
				ttfTab.length = bswap32(ttfTab.length);
				ttfTab.offset = bswap32(ttfTab.offset);
				break;
			}
		}

		if(bFound){
			fseek(f, ttfTab.offset, SEEK_SET);
			TTF_NAME_HEADER nameEntry;
			fread(&nameEntry, sizeof(TTF_NAME_HEADER), 1, f);
			nameEntry.count = bswap16(nameEntry.count);
			nameEntry.offset = bswap16(nameEntry.offset);
			TTF_NAME_TABLE nameTab;
			bFound = FALSE;

			for(int i=0; i<nameEntry.count; i++){
				fread(&nameTab, sizeof(TTF_NAME_TABLE), 1, f);
				nameTab.nameID = bswap16(nameTab.nameID);
				if(nameTab.nameID == 1){
					nameTab.length = bswap16(nameTab.length);
					nameTab.offset = bswap16(nameTab.offset);
					int nPos = ftell(f);// f.GetPosition();
					fseek(f, ttfTab.offset + nameTab.offset + nameEntry.offset, SEEK_SET);
                    //nameTab.length + 1//
					fread(csTemp, nameTab.length, 1, f);
					csTemp[nameTab.length + 0] = 0;
                    csTemp[nameTab.length + 1] = 0;

                    csswap16((char16_t*)wcsTemp, nameTab.length / 2);
                    if(wcslen(wcsTemp)){
                        csRetVal = wcsTemp;
                        break;
                    }
					fseek(f, nPos, SEEK_SET);
				}
			}
		}
		fclose(f);
	}
	return csRetVal;
}

unistring GetFontNameFromData(byte_t* data, size_t size)
{
	unistring csRetVal;
    size_t offset = 0;

    TTF_HEADER ttfEntry;
    memcpy(&ttfEntry, data, sizeof(TTF_HEADER));
    offset += sizeof(TTF_HEADER);
    ttfEntry.numTables = bswap16(ttfEntry.numTables);
    ttfEntry.majorVersion = bswap16(ttfEntry.majorVersion);
    ttfEntry.minorVersion = bswap16(ttfEntry.minorVersion);

    //check is this is a true type font and the version is 1.0
    if(ttfEntry.majorVersion != 1 || ttfEntry.minorVersion != 0)
        return csRetVal;

    TTF_TABLE ttfTab;
    BOOL bFound = FALSE;
    union{
        char    csTemp[256];
        wchar_t wcsTemp[128];
    };

    for(int i=0; i< ttfEntry.numTables; i++){
        memcpy(&ttfTab, data + offset, sizeof(TTF_TABLE));
        offset += sizeof(TTF_TABLE);
        strncpy(csTemp, ttfTab.name, 4);
        csTemp[4] = 0;
        //print("table", csTemp);
        if(strcmp(csTemp, "name") == 0){
            bFound = TRUE;
            ttfTab.length = bswap32(ttfTab.length);
            ttfTab.offset = bswap32(ttfTab.offset);
            break;
        }
    }

    if(bFound){
        offset = ttfTab.offset;
        TTF_NAME_HEADER nameEntry;
        memcpy(&nameEntry, data + offset, sizeof(TTF_NAME_HEADER));
        offset += sizeof(TTF_NAME_HEADER);
        nameEntry.count = bswap16(nameEntry.count);
        nameEntry.offset = bswap16(nameEntry.offset);
        TTF_NAME_TABLE nameTab;
        bFound = FALSE;

        for(int i=0; i<nameEntry.count; i++){
            memcpy(&nameTab, data + offset, sizeof(TTF_NAME_TABLE));
            offset += sizeof(TTF_NAME_HEADER);
            nameTab.nameID = bswap16(nameTab.nameID);
            if(nameTab.nameID == 1){//font name
                nameTab.length = bswap16(nameTab.length);
                nameTab.offset = bswap16(nameTab.offset);

                if(nameTab.length == 0){
                    continue;
                }

                if(nameTab.platformID == 0x0300){//ms
                    int string_offset = ttfTab.offset + nameTab.offset + nameEntry.offset;
                    memcpy(csTemp, data + string_offset, nameTab.length);

                    csTemp[nameTab.length + 0] = 0;
                    csTemp[nameTab.length + 1] = 0;

                    csswap16((char16_t*)wcsTemp, nameTab.length / 2);
                    //print(nameTab.uLanguageID, nameTab.length);

                    if(wcslen(wcsTemp)){
                        csRetVal = wcsTemp;
                        break;
                    }
                }
            }
        }
    }

	return csRetVal;
}

}//end namespace cgl

#endif //FONT_TTF_HPP_20200229005729
