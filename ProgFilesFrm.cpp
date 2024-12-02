/*
		Project:		MG Setup
		Module:			ProgFilesFrm.cpp
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

#include <vcl.h>
#pragma hdrstop

#include "ProgFilesFrm.h"
#include "DirSelFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TProgramFilesForm *ProgramFilesForm;
//---------------------------------------------------------------------------
__fastcall TProgramFilesForm::TProgramFilesForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TProgramFilesForm::ButtonCloseClick(TObject *)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TProgramFilesForm::ButtonRemoveClick(TObject *)
{
	if( ListBoxGroups->ItemIndex >= 0 && ListBoxProgramFiles->ItemIndex >= 0 )
	{
		const std::size_t	itemIndex = ListBoxProgramFiles->ItemIndex;
		ListBoxProgramFiles->Items->Delete(itemIndex);
		GroupFiles	&currentGroup = m_installerFiles[m_currentGroup].files;
		currentGroup.removeElementAt(itemIndex);
	}
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ButtonAddFileClick(TObject *)
{
	if( ListBoxGroups->ItemIndex >= 0 && OpenDialog->Execute() )
	{
		GroupFiles	&currentGroup = m_installerFiles[m_currentGroup].files;

		for( int i=0; i<OpenDialog->Files->Count; i++ )
		{
			ListBoxProgramFiles->Items->Add(OpenDialog->Files->Strings[i] );
			currentGroup.addElement(OpenDialog->Files->Strings[i].c_str());
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ButtonAddDirectoryClick(TObject *)
{
	if( ListBoxGroups->ItemIndex >= 0 && DirectorySelector->ShowModal( "" ) == mrOk )
	{
		ListBoxProgramFiles->Items->Add(
			DirectorySelector->DirectoryListBox->Directory
		);
		GroupFiles	&currentGroup = m_installerFiles[m_currentGroup].files;
		currentGroup.addElement(
			DirectorySelector->DirectoryListBox->Directory.c_str()
		);
	}
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ButtonAddGroupClick(TObject *Sender)
{
	STRING groupName = EditGroupName->Text.c_str();
	if( !groupName.isEmpty() && !m_installerFiles.hasElement( groupName ) )
	{
		ListBoxGroups->Items->Add( static_cast<const char*>( groupName ) );
		ListBoxGroups->ItemIndex = ListBoxGroups->Items->Count-1;
		ListBoxGroupsClick(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ListBoxGroupsClick(TObject *)
{
	if( ListBoxGroups->ItemIndex >= 0 )
	{
		m_currentGroup = ListBoxGroups->Items->Strings[ListBoxGroups->ItemIndex].c_str();
		ListBoxProgramFiles->Clear();
		GroupFiles	&currentGroup = m_installerFiles[m_currentGroup].files;
		for(
			GroupFiles::const_iterator it = currentGroup.cbegin(),
				endIT = currentGroup.cend();
			it != endIT;
			++it
		)
		{
			const F_STRING &fileName = *it;
			ListBoxProgramFiles->Items->Add( static_cast<const char*>( fileName ) );
		}

		ComboBoxDestination->Text = static_cast<const char *>(
			m_installerFiles[m_currentGroup].destination
		);
	}
}

//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::FormShow(TObject *)
{
	ListBoxGroups->Clear();
	for(
		InstallerFiles::const_iterator it = m_installerFiles.cbegin(),
			endIT = m_installerFiles.cend();
		it != endIT;
		++it
	)
	{
		ListBoxGroups->Items->Add( static_cast<const char*>( it->getKey() ) );
	}
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ComboBoxDestinationChange(TObject *)
{
	m_installerFiles[m_currentGroup].destination = ComboBoxDestination->Text.c_str();
}
//---------------------------------------------------------------------------

void __fastcall TProgramFilesForm::ButtonDeleteClick(TObject *)
{
	if( ListBoxGroups->Items->Count > 1 && ListBoxGroups->ItemIndex >= 0 )
	{
		m_installerFiles.removeElement(m_currentGroup);
		ListBoxGroups->Items->Delete(ListBoxGroups->ItemIndex);
		ListBoxProgramFiles->Clear();
	} 
}
//---------------------------------------------------------------------------

