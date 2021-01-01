/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 treeview.hpp

 2018-09-07 13:55:05

*/
#ifndef VCL_TREEVIEW_HPP_20180907135505
#define VCL_TREEVIEW_HPP_20180907135505

#include <cgl/filesystem.hpp>
#include "../string.hpp"
#include <vector>

namespace cgl{
namespace vcl{

//
// TTreeNode
//

//���ؽڵ�ĸ��ڵ�
TTreeNode* TTreeNode_Root(TTreeNode* Node)
{
    if(Node){
        return null;
    }
    while(Node->Parent){
        Node = Node->Parent;
    }
    return Node;
}

//����ͬ���ڵ��еĵ�һ���ڵ�
TTreeNode* TTreeNode_FirstNode(TTreeNode* Node)
{
    CGL_ASSERT(Node);
    if(Node->Parent){
        return Node->Parent->getFirstChild();
    }
    else{
        return Node->Owner->GetFirstNode();
    }
}

//����ͬ���ڵ��е����һ���ڵ�
TTreeNode* TTreeNode_LastNode(TTreeNode* Node)
{
    CGL_ASSERT(Node);
    if(Node->Parent)
    {
        return Node->Parent->GetLastChild();
    }
    else
    {
        while(Node->getNextSibling())
        {
            Node = Node->getNextSibling();
        }
        return Node;
    }
}

//
// TTreeView
//

//�����ַ������ҵ�ǰ�ӽڵ�
TTreeNode* TTreeView_FindChild(TTreeView* TreeView, String Name)
{
    if(!TreeView){
        return null;
    }

    TTreeNode* Node = TreeView->Items->GetFirstNode();
    while(Node){
        if(!StrICmp(Node->Text, Name)){
            break;
        }
        Node = Node->getNextSibling();
    }

    return Node;
}

TTreeNode* TTreeView_FindChild(TTreeNode* Node, String Name)
{
    if(!Node){
        return null;
    }

    Node = Node->getFirstChild();
    while(Node){
        if(!StrICmp(Node->Text, Name)){
            break;
        }
        Node = Node->getNextSibling();
    }
    return Node;
}

//
//�����ӽڵ�
//
TTreeNode* TTreeView_Find(TTreeView* TreeView, TTreeNode* Node, String Name)
{
    TTreeNode* SubNode = Node ? Node->getFirstChild() : TreeView->Items->GetFirstNode();

    while(SubNode){
        if(!StrICmp(SubNode->Text, Name)){
            break;
        }
        SubNode = SubNode->getNextSibling();
    }
    return SubNode;
}

//����TreeView����ң������ִ�Сд
//���⻹��Ҫ��һ��TTreeView_FindNext(Nodes, Node, Name)
int TTreeView_FindNodes(std::vector<TTreeNode*> ls, TTreeView* treeview, String name)
{
    for(int i=0; i<treeview->Items->Count; ++i)
    {
        if(StrICmp(treeview->Items->Item[i]->Text, name)){
            ls.push_back(treeview->Items->Item[i]);
        }
    }
    return ls.size();
}

//���ؽڵ��·��
//aaa/bbb/ccc
String TTreeView_FullPath(TTreeNode* Node, Char Delimiter = '/')
{
    if(!Node){
        return String();
    }
    String str = Node->Text;
    while(Node->Parent)
    {
        Node = Node->Parent;
        str  = Node->Text + Delimiter + str;
    }
    return str;
}

//ͨ��·��չ���ڵ㡣
//�����ɹ��������ҵ���Node
//Path        "aaa/bbb/ccc"
//Expand    �Ƿ�չ��
TTreeNode* TTreeView_OpenPath(TTreeView* TreeView,
    String Path,
    bool Expand = true,
    int Delimiter = '/')
{
    if(Path.IsEmpty()){
        return null;
    }

    alone_ptr<TStringList> ls(new TStringList);
    ls->Delimiter = Delimiter;
    ls->DelimitedText = Path;

    TTreeNode* node = null;
    TTreeNode* find_node;
    for(int i=0; i<ls->Count; ++i){
        find_node = TTreeView_Find(TreeView, node, ls->Strings[i]);
        if(find_node){
            if(node){
                node->Expanded = Expand;
            }
            node = find_node;
        }
        else{
            break;
        }
    }

    return find_node;
}

//ͨ��·������node
/*
TTreeNode* TTreeView_CreatePath(TTreeView* TreeView, String Path, int Delimiter = '/')
{
    alone_ptr<TStringList> ls(new TStringList);
    ls->Delimiter = Delimiter;
    ls->DelimitedText = Path;
    TTreeNode* node = null;
    TTreeNode* find_node;
    for(int i=0; i<ls->Count; ++i)
    {
        find_node = TTreeView_Find(TreeView, node, ls->Strings[i]);
        if(find_node){
            node = find_node;
        }
        else{
            node = TreeView->Items->AddChild(node, ls->Strings[i]);
        }
    }
    return node;
}
/*/

TTreeNode* TTreeView_CreatePath(TTreeView* TreeView, String Path, int Delimiter = '/')
{
    unistring name;
    TTreeNode* node = null;
    TTreeNode* find_node;

    for(int i=1; i<=Path.Length(); ++i){
        if(Path[i] == Delimiter){
            if(name.empty()){
                continue;
            }
            find_node = TTreeView_Find(TreeView, node, name.c_str());
            if(find_node){
                node = find_node;
            }
            else{
                node = TreeView->Items->AddChild(node, name.c_str());
            }
            name.clear();
        }
        else{
            name.push_back(Path[i]);
        }
    }

    if(!name.empty()){
        node = TreeView->Items->AddChild(node, name.c_str());
    }

    return node;
}

TTreeNode* TTreeView_CreatePath(std::map<unistring, TTreeNode*> &itembuf,
    TTreeView* TreeView, String Path, int Delimiter = '/')
{
    unistring name;
    TTreeNode* node = null;
    std::map<unistring, TTreeNode*>::iterator itr;

    for(int i=1; i<=Path.Length(); ++i){
        if(Path[i] == Delimiter){
            if(name.empty()){
                continue;
            }
            //find_node = TTreeView_Find(TreeView, node, name.c_str());
            itr = itembuf.find(name);
            if(itr != itembuf.end()){
                node = itr->second;
            }
            else{
                node = TreeView->Items->AddChild(node, name.c_str());
                itembuf[name] = node;
            }
            name.clear();
        }
        else{
            name.push_back(Path[i]);
        }
    }

    if(!name.empty()){
        node = TreeView->Items->AddChild(node, name.c_str());
    }

    return node;
}
//*/

void TTreeView_ListFolder(TTreeView* TreeView, TTreeNode* Parent, TString Path)
{
    TTreeNode* SubNode;
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile((Path + "*").c_str(), &wfd);
    if(hFind != INVALID_HANDLE_VALUE){
        do{
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if(!filesystem::is_vtdir(wfd.cFileName)){
                    SubNode = Parent->Owner->AddChild(Parent, wfd.cFileName);
                    TTreeView_ListFolder(TreeView, SubNode, Path + wfd.cFileName + "\\");
				}
			}
			else{
                //TTreeNode* SubNode = Node->Owner->AddChild(Node, wfd.cFileName);
			}
        }while(FindNextFile(hFind, &wfd));
    }
}

//�о�Ŀ¼�µ��ļ���TreeNode
void TTreeView_ListFile(TTreeNode* Node, TString Path)
{
    TTreeNode* SubNode;
    WIN32_FIND_DATA wfd;
    HANDLE hFind = FindFirstFile((Path + "*").c_str(), &wfd);
    if(hFind != INVALID_HANDLE_VALUE){
        do{
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				if(!filesystem::is_vtdir(wfd.cFileName)){
                    SubNode = Node->Owner->AddChild(Node, wfd.cFileName);
                    TTreeView_ListFile(SubNode, Path + wfd.cFileName + "\\");
				}
			}
			else{
                SubNode = Node->Owner->AddChild(Node, wfd.cFileName);
			}
        }while(FindNextFile(hFind, &wfd));
    }
}


}//end namespace vcl
}//end namespace cgl

#endif //VCL_TREEVIEW_HPP_20180907135505