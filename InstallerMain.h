/*
		Project:		MG Setup
		Module:			InstallerMain.h
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

#ifndef InstallerMainH
#define InstallerMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include <gak/directory.h>

#include "common.h"

//---------------------------------------------------------------------------
typedef enum
{
	imNONE_INSTALLED, imNEW_INSTALL, imUPDATE_INSTALL
} InstallMode;

//---------------------------------------------------------------------------
class TInstallerForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TLabel *Label1;
	TEdit *EditDestination;
	TLabel *Label2;
	TEdit *EditBDE;
	TLabel *Label3;
	TEdit *EditDataPath;
	TButton *ButtonSelectDestination;
	TButton *ButtonSelectBDE;
	TButton *ButtonSelectDataPath;
	TButton *ButtonInstall;
	TButton *ButtonCancel;
	TProgressBar *ProgressBar;
	TLabel *Label4;
	TEdit *EditStartMenu;
	TButton *ButtonSelectStartMenu;
	TLabel *LabelStatus;
	void __fastcall SelectPath(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall PathChanged(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ButtonInstallClick(TObject *Sender);
private:	// Anwender-Deklarationen
	gak::xml::Document	*m_theSetupDoc;
	gak::xml::Element	*m_setupElement, *m_appInfo, *m_sourceInfo, *m_bdeInfo,
						*m_iconInfo;
	gak::xml::Element	*m_uninstall, *m_filesCopied;
	gak::STRING		  	m_aliasName, m_oldAliasName, m_backupAliasName, m_bdeResource;
	int			  		m_dbVersion;
	gak::DirectoryList	m_bdeSource, m_dataSource, m_uninstallSource;
	InstallerFiles		m_installerFiles;
	gak::ArrayOfStrings	m_oldFiles;
	bool		  		m_needDataPath, m_needBDE;

	void installBDE( std::ostream &logOut, InstallMode installMode );
	bool installDatabase( std::ostream &logOut );
	void installMenu(
		const AppInfo &appInfo,
		gak::F_STRING *mainProgram, gak::F_STRING *mainPath,
		gak::F_STRING *updateProgram, gak::F_STRING *updatePath
	);

	gak::STRING getPath( TEdit *editControl ) const;
	InstallMode check4Mode(
		const gak::STRING &sourceOrig, gak::DirectoryList &dirList,
		const gak::STRING &destinationPath
	);
	void copyFile(
		std::ostream &logOut,
		InstallMode installMode,
		const gak::F_STRING &name,
		const gak::STRING &sourceOrig, const gak::STRING &sourcePath,
		const gak::STRING &destinationPath
	);
	void copyFiles(
		std::ostream &logOut,
		InstallMode installMode,
		const gak::STRING &sourceOrig, gak::DirectoryList &dirList,
		const gak::STRING &destinationPath
	);
	InstallMode copyData(
		std::ostream &logOut,
		const gak::STRING &sourceOrig, gak::DirectoryList &dirList,
		const gak::STRING &destinationPath
	);
	void cleanDirectory( const gak::STRING &dirName );

	bool makepath( const char *path, const gak::STRING &source );
	void loadOldFiles( const gak::STRING &setupFile );
	void checkInstall();
	void doInstall();
public:		// Anwender-Deklarationen
	__fastcall TInstallerForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInstallerForm *InstallerForm;
//---------------------------------------------------------------------------
#endif
