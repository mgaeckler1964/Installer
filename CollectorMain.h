/*
		Project:		MG Setup
		Module:			CollectorMain.h
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

//---------------------------------------------------------------------------

#ifndef CollectorMainH
#define CollectorMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
namespace gak
{
namespace xml
{
	class Document;
	class Element;
}
}

class TCollectorForm : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonCreateInstaller;
	TMainMenu *MainMenu;
	TMenuItem *File1;
	TMenuItem *New;
	TMenuItem *Open;
	TMenuItem *Save;
	TMenuItem *SaveAs;
	TMenuItem *Exit;
	TMenuItem *N1;
	TLabel *Label1;
	TRadioGroup *RadioGroupBDE;
	TLabel *Label3;
	TEdit *EditDataPath;
	TButton *ButtonSelectDataPath;
	TLabel *Label2;
	TEdit *EditApplicationTitle;
	TLabel *Label4;
	TEdit *EditApplicationVersion;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *EditAuthor;
	TButton *ButtonFiles;
	TButton *ButtonIcons;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TStatusBar *StatusBar;
	TButton *ButtonOpen;
	TButton *ButtonNew;
	TComboBox *ComboBoxBDEalias;
	TMenuItem *N2;
	TMenuItem *TargetDirectory;
	void __fastcall ButtonFilesClick(TObject *Sender);
	void __fastcall ButtonIconsClick(TObject *Sender);
	void __fastcall OpenClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall SaveAsClick(TObject *Sender);
	void __fastcall ButtonSelectDataPathClick(TObject *Sender);
	void __fastcall ExitClick(TObject *Sender);
	void __fastcall ButtonCreateInstallerClick(TObject *Sender);
	void __fastcall NewClick(TObject *Sender);
	void __fastcall TargetDirectoryClick(TObject *Sender);
	void __fastcall RadioGroupBDEClick(TObject *Sender);
private:	// User declarations
	long				m_dbVersion;
	gak::xml::Document	*m_theSetupDoc;
	gak::xml::Element	*m_setupElement;

	void readApplicationInfo();
	void saveApplicationInfo();
	void readBdeInfo( const gak::STRING &installerFileName );
	void saveBdeInfo( const gak::STRING &installerFileName );
	
	void readIcons();
	void saveIcons();

	void readSource( const gak::STRING &installerFileName );
	void saveSource( const gak::STRING &installerFileName );

	bool checkSettings();
	void cleanDirectory( const gak::STRING &dirName );
	gak::STRING getDataPath( void );
	gak::STRING getTargetPath( void );
	void setTargetPath( const AnsiString &newTarget );

	void copyInstallerFiles( const gak::STRING &directory, const gak::STRING &pattern );
	void copyBdeFiles( const gak::STRING &directory );
	void copyDatabase( const gak::STRING &dataPath, const gak::STRING &directory );
	void createSchemaXml( const gak::STRING &dataPath, const gak::STRING &directory );
	void copyProgramFiles( const gak::STRING &directory );

public:		// User declarations
	__fastcall TCollectorForm(TComponent* Owner);

	void openFile( const gak::STRING &installerFileName );
};
//---------------------------------------------------------------------------
extern PACKAGE TCollectorForm *CollectorForm;
//---------------------------------------------------------------------------
#endif
