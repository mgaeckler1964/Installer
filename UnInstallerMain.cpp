/*
		Project:		MG Setup
		Module:			UnInstallerMain.cpp
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
#include <windows.h>

#include <vcl.h>
#include <vcl/registry.hpp>

#include <gak/string.h>
#include <gak/xmlParser.h>
#include <gak/directory.h>


#pragma hdrstop

#include "UnInstallerMain.h"
#include "Setup.h"
#include "tags.h"
#include "common.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TUninstallMainForm *UninstallMainForm;
//---------------------------------------------------------------------------
__fastcall TUninstallMainForm::TUninstallMainForm(TComponent* Owner)
	: TForm(Owner)
{
	m_needBDE = true;
}
//---------------------------------------------------------------------------
void __fastcall TUninstallMainForm::ButtonCancelClick(TObject *)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TUninstallMainForm::FormShow(TObject *)
{
	STRING	setupXml = makeFullPath( Application->ExeName.c_str(), "setup.xml" );
	xml::Parser	theParser( setupXml );
	m_theSetupDoc = theParser.readFile( false );
	if( !m_theSetupDoc )
	{
		Application->MessageBox(
			LoadStr( ERR_READ_SETUP_XML ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONSTOP
		);
		Close();
		Application->Terminate();
/*@*/	return;
	}
	m_setupElement = m_theSetupDoc->getElement( SETUP_TAG );
	if( !m_setupElement )
	{
		Application->MessageBox(
			LoadStr( ERR_BAD_SETUP_XML ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONSTOP
		);
		Close();
		Application->Terminate();
/*@*/	return;
	}
	m_appInfo = m_setupElement->getElement( APPLICATION_TAG );
	if( !m_appInfo )
	{
		Application->MessageBox(
			LoadStr( ERR_BAD_SETUP_APP_INFO ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONSTOP
		);
		Close();
		Application->Terminate();
/*@*/	return;
	}
	m_uninstall = m_setupElement->getElement( UNINSTALL_TAG );
	if( !m_uninstall )
	{
		Application->MessageBox(
			LoadStr( ERR_BAD_SETUP_UNINSTALL_INFO ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONSTOP
		);
		Close();
		Application->Terminate();
/*@*/	return;
	}
	m_filesCopied = m_uninstall->getElement( FILES_TAG );
	if( !m_filesCopied )
	{
		Application->MessageBox(
			LoadStr( ERR_BAD_SETUP_FILES_INFO ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONSTOP
		);
		Close();
		Application->Terminate();
/*@*/	return;
	}
	if( m_setupElement->getElement( BDE_TAG ) )
	{
		TRegistry	*theRegistry = new TRegistry;
		theRegistry->RootKey = HKEY_LOCAL_MACHINE;
		if( theRegistry->OpenKey( BDE_REGISTRY_KEY, true ) )
		{
			theRegistry->CloseKey();
		}
		else
		{
			Application->MessageBox(
				LoadStr( ERR_NO_UNINSTALL_PERM ).c_str(),
				LoadStr( ERR_CAPTION ).c_str(),
				MB_OK|MB_ICONSTOP
			);
			Close();
			Application->Terminate();
/*@*/		return;
		}
	}
	else
	{
		m_needBDE = false;
	}


	AppInfo	appInfo( m_appInfo );
	STRING	appName = appInfo.getApplicationName();

	MemoStatus->Lines->Clear();
	AnsiString	message;
	message.printf(
		LoadStr( UNINSTALL_MESSAGE ).c_str(),
		static_cast<const char *>(appName)
	);

	MemoStatus->Lines->Add( message );
}
//---------------------------------------------------------------------------
void __fastcall TUninstallMainForm::ButtonRemoveClick(TObject *)
{
	doEnterFunction("TUninstallMainForm::ButtonRemoveClick");

	STRING	tag, source;
	int		result;

	int		bdeUseCount = 0;

	AppInfo	appInfo( m_appInfo );
	STRING	appName = appInfo.getApplicationName();

	AnsiString	message;
	message.printf(
		LoadStr( UNINSTALL_START ).c_str(),
		static_cast<const char *>(appName)
	);
	if( Application->MessageBox(
		message.c_str(),
		LoadStr( UNINSTALL_CAPTION ).c_str(),
		MB_YESNO|MB_ICONQUESTION
	) != IDYES )
	{
/***/	return;
	}

	setcwd( "C:\\" );
	MemoStatus->Lines->Clear();

	if( m_needBDE )
	{
		std::auto_ptr<TRegistry>	theRegistry( new TRegistry() );
		theRegistry->RootKey = HKEY_LOCAL_MACHINE;

		if( theRegistry->OpenKey( BDE_REGISTRY_KEY, true ) )
		{
			AnsiString useCountStr = "0";
			if( theRegistry->ValueExists( BDE_USECOUNT_KEY ) )
			{
				useCountStr = theRegistry->ReadString( BDE_USECOUNT_KEY );
			}
			bdeUseCount = atoi( useCountStr.c_str() ) - 1;
			useCountStr.printf( "%d", bdeUseCount );
			theRegistry->WriteString( BDE_USECOUNT_KEY, useCountStr );

			theRegistry->CloseKey();
			if( !bdeUseCount )
			{
				theRegistry->DeleteKey( BDE_REGISTRY_KEY );
			}
		}
		else
		{
			Application->MessageBox(
				LoadStr( ERR_NO_UNINSTALL_PERM ).c_str(),
				LoadStr( ERR_CAPTION ).c_str(),
				MB_OK|MB_ICONSTOP
			);
			Close();
			Application->Terminate();
/*@*/		return;
		}
	}


	for( std::size_t i = m_filesCopied->getNumObjects()-1; i!= size_t(-1); i-- )
	{
		xml::Element	*theEntry = m_filesCopied->getElement(i);
		if( theEntry )
		{
			tag = theEntry->getTag();
			source = theEntry->getAttribute( SOURCE_TAG );
			if( (source != BDE_DIR || bdeUseCount == 0) && source != DATA_DIR )
			{
				if( tag == FILE_TAG )
				{
					const STRING destination = theEntry->getAttribute(
						DESTINATION_ATTR
					);
					result = remove( destination );
					if( result )
					{
						MemoStatus->Lines->Add(
							static_cast<const char *>(destination)
						);
					}
				}
				else if( tag == DIRECTORY_TAG )
				{
					const STRING path = theEntry->getAttribute(
						PATH_ATTR
					);
					result = RemoveDirectory( path );
					if( !result )
					{
						MemoStatus->Lines->Add(static_cast<const char *>(path));
					}
				}
			}
		}
	}


	message.printf(
		LoadStr( UNINSTALL_END ).c_str(),
		static_cast<const char *>(appName)
	);
	Application->MessageBox(
		message.c_str(),
		LoadStr( UNINSTALL_CAPTION ).c_str(),
		MB_ICONINFORMATION
	);
	ButtonRemove->Enabled = false;
	ButtonCancel->Caption = LoadStr( UNINSTALL_CLOSE );
}
//---------------------------------------------------------------------------
