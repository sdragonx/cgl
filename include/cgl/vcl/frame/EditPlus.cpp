//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditPlus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditPlus *EditPlus;

static HANDLE sci_handle = LoadLibrary(TEXT("SciLexer.dll"));


typedef string_t SCIString;
//---------------------------------------------------------------------------

bool IsWordChar(int ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

//¹Ø¼ü×Ö²éÑ¯Àà
class keywords_query
{
public:
    std::vector<string_t> keywords;
    std::vector<string_t> functions;
    std::map<string_t, string_t> tips;

    string_t find(const string_t& str)
    {
        std::vector<string_t> ls;
        if(!str.empty()){
            find_items(ls, keywords, str);
            find_items(ls, functions, str);
        }
        return join(ls);
    }

    string_t ls_keywords()
    {
        std::vector<string_t> ls;
        list_items(ls, keywords);
        return join(ls);
    }

    string_t ls_functions()
    {
        std::vector<string_t> ls;
        list_items(ls, functions);
        return join(ls);
    }

    string_t all()
    {
        std::vector<string_t> ls;
        list_items(ls, keywords);
        list_items(ls, functions);
        return join(ls);
    }

private:
    void find_items(std::vector<string_t>& ls, const std::vector<string_t>& items, const string_t& str)
    {
        for(size_t i=0; i<items.size(); ++i){
            if(!strnicmp(items[i].c_str(), str.c_str(), str.length())){
                ls.push_back(items[i]);
            }
        }
    }

    void list_items(std::vector<string_t>& ls, const std::vector<string_t>& items)
    {
        for(size_t i=0; i<items.size(); ++i){
            ls.push_back(items[i]);
        }
    }

    string_t join(std::vector<string_t>& ls)
    {
        std::sort(ls.begin(), ls.end());
        string_t text;
        text.join(ls, ' ');
        return text;
    }
};

//---------------------------------------------------------------------------
__fastcall TEditPlus::TEditPlus(TComponent* Owner)
    : TFrame(Owner)
{
}
//---------------------------------------------------------------------------
