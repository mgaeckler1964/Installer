/*
		Project:		MG Setup
		Module:			ProgFilesFrm.h
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

#ifndef ProgFilesFrmH
#define ProgFilesFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

#include <gak/map.h>

#include "common.h"

//---------------------------------------------------------------------------
class TProgramFilesForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBoxProgramFiles;
	TPanel *Panel1;
	TButton *ButtonAddFile;
	TButton *ButtonRemove;
	TButton *ButtonClose;
	TOpenDialog *OpenDialog;
	TButton *ButtonAddDirectory;
	TListBox *ListBoxGroups;
	TLabel *Label1;
	TComboBox *ComboBoxDestination;
	TPanel *Panel2;
	TEdit *EditGroupName;
	TButton *ButtonAddGroup;
	TButton *ButtonDelete;
	TLabel *Label2;
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall ButtonRemoveClick(TObject *Sender);
	void __fastcall ButtonAddFileClick(TObject *Sender);
	void __fastcall ButtonAddDirectoryClick(TObject *Sender);
	void __fastcall ButtonAddGroupClick(TObject *Sender);
	void __fastcall ListBoxGroupsClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxDestinationChange(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
private:	// User declarations
	InstallerFiles	m_installerFiles;
	gak::STRING		m_currentGroup;
public:		// User declarations
	__fastcall TProgramFilesForm(TComponent* Owner);

	const InstallerFiles &getInstallerFiles() const
	{
		return m_installerFiles;
	}
	InstallerFiles &getInstallerFiles()
	{
		return m_installerFiles;
	}
	void Clear()
	{
		m_installerFiles.clear();
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TProgramFilesForm *ProgramFilesForm;
//---------------------------------------------------------------------------
#endif
