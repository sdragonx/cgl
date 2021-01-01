//---------------------------------------------------------------------------

#ifndef ZipViewH
#define ZipViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include <cgl/io/zip_archive.hpp>
using namespace cgl;
//---------------------------------------------------------------------------
class TZipView : public TFrame
{
__published:	// IDE-managed Components
    TTreeView *tvwFiles;
    void __fastcall FrameClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TZipView(TComponent* Owner);
    io::zip_archive<char> m_archive;

    int LoadArchive(String filename);

    void CloseArchive();
};
//---------------------------------------------------------------------------
extern PACKAGE TZipView *ZipView;
//---------------------------------------------------------------------------
#endif