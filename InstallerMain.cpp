/*
		Project:		MG Setup
		Module:			InstallerMain.cpp
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
#define NO_WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <aclapi.h>
#include <shlobj.h>
#include <shlguid.h>

#include <vcl.h>
#include <vcl/registry.hpp>
#include <bde.hpp>
#include <dbtables.hpp>


#include <gak/gaklib.h>
#include <gak/ci_string.h>
#include <gak/xmlParser.h>
#include <gak/directory.h>
#include <gak/acls.h>
#include <gak/numericString.h>
#include <gak/fcopy.h>
#include <gak/vcl_tools.h>

#include <winlib/winlib.h>
#pragma hdrstop

#include "InstallerMain.h"
#include "DirSelFrm.h"
#include "Setup.h"
#include "Config.h"
#include "tags.h"

//---------------------------------------------------------------------------
using namespace gak;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInstallerForm *InstallerForm;
//---------------------------------------------------------------------------

#define IDAPI_CFG "IDAPI32.CFG"

//----- Typedefs -------------------------------------------------------

typedef IShellLink *LPSHELLLINK;
typedef IPersistFile *LPPERSISTFILE;
/**********************************************************************/
/* CreateShortcut : Hilfsfunktion zur Erzeugung eines Shortcuts       */
/*--------------------------------------------------------------------*/
/* Parameter :    pszPath       - Pfad, auf den der Shortcut verweist */
/*                pszArguments  - Programmargumente                   */
/*                pszLocation   - Wo und unter welchem Namen soll der */
/*                                Shortcut gespeichert werden?        */
/*                pszWorkingDir - Arbeitsverzeichnis                  */
/*                        pszIcon          - Icon                                             */
/*                nCmdShow      - Wie soll das Applikationsfenster    */
/*                                nach dem Start dargestellt werden?  */
/* Rückgabewert : TRUE  - Shortcut wurde erzeugt                      */
/*                FALSE - Fehler                                      */
/**********************************************************************/
static BOOL CreateShortcut( LPSTR pszPath,
					 LPSTR pszArguments,
					 LPSTR pszLocation,
					 LPSTR pszWorkingDir,
					 LPSTR pszIcon,
					 int   nCmdShow )
{
  doEnterFunctionEx(gakLogging::llInfo,"CreateShortcut");

  LPSHELLLINK pShellLink;
  HRESULT hrCoInit;                        // OLE-Installations-Ergebnis
  HRESULT hr;                             // Ergebnis der Objektmethoden

  hrCoInit = CoInitialize( NULL );                 // OLE initialisieren
  hr = CoCreateInstance(   CLSID_ShellLink, // ShellLink-Objekt erzeugen
						   NULL,
						   CLSCTX_INPROC_SERVER,
						   IID_IShellLink,
						   (void**)&pShellLink );
  if( SUCCEEDED( hr ) )
  {
	LPPERSISTFILE pPersistFile;

	// PersistFile Interface ermitteln   -------------------------------
	if(SUCCEEDED(pShellLink->QueryInterface(IID_IPersistFile,(void**)&pPersistFile)))
	{
	  wchar_t wsz[ MAX_PATH ];

	  // Programmpfad setzen  ------------------------------------------
	  hr = pShellLink->SetPath(pszPath);

	  // Argumente setzen ----------------------------------------------
	  if( SUCCEEDED( hr ) )
		hr = pShellLink->SetArguments(pszArguments);

	  // Arbeitsverzeichnis setzen -------------------------------------
	  if( SUCCEEDED( hr ) )
		hr = pShellLink->SetWorkingDirectory(pszWorkingDir );

	  // Icon setzen ---------------------------------------------------
	  if (SUCCEEDED(hr))
		  hr = pShellLink->SetIconLocation(pszIcon,0);

	  // ShowCommand setzen --------------------------------------------
	  if( SUCCEEDED( hr ) )
		hr = pShellLink->SetShowCmd(nCmdShow );

	  // Pfad setzen, an dem Shortcut abgespeichert werden soll  -------
	  if( SUCCEEDED( hr ) )
	  {
		MultiByteToWideChar( CP_ACP, 0, pszLocation, -1, wsz, MAX_PATH);
		pPersistFile->Save( wsz, TRUE );
		hr = pPersistFile->SaveCompleted( wsz );
	  }
//    *************************************
//
//      hr = pPersistFileRelease( );
//
//    *************************************
	}
	pShellLink->Release( );
  }
  // Wenn OLE hier initialisiert wurde, muß es auch hier wieder de-
  // installiert werden.
  if( SUCCEEDED( hrCoInit ) ) CoUninitialize();

  return SUCCEEDED( hr );
}
//---------------------------------------------------------------------------
__fastcall TInstallerForm::TInstallerForm(TComponent* Owner)
	: TForm(Owner)
{
	m_needBDE = m_needDataPath = true;
}
//---------------------------------------------------------------------------
void TInstallerForm::cleanDirectory( const STRING &dirName )
{
	STRING				fileName;
	DirectoryList		content;

	content.dirlist( dirName );
	for(
		DirectoryList::iterator it = content.begin(), endIT = content.end();
		it != endIT;
		++it
	)
	{
		const STRING &entry = it->fileName;
		if( entry != "." && entry != ".." )
		{
			fileName = dirName + DIRECTORY_DELIMITER + entry;
			if( isFile( fileName ) )
			{
				remove( fileName );
			}
			else
			{
				cleanDirectory( fileName );
				rmdir( fileName );
			}
		}
	}
}
//---------------------------------------------------------------------------
bool TInstallerForm::makepath( const char *path, const STRING &source )
{
	doEnterFunctionEx(gakLogging::llInfo, "TInstallerForm::makepath");
	bool	success = true;
	char	*cp;

	doLogValueEx( gakLogging::llInfo, path );
	cp = const_cast<char *>(path);
	while( 1 )
	{
		cp = strchr( cp, DIRECTORY_DELIMITER );
		if( !cp )
		{
/*v*/		break;
		}

		*cp = 0;
		size_t	len = strlen( path );
		if( len && (path[1] != ':' ||  len > 3) && !exists(path) )
		{
			xml::Element	*directory = m_filesCopied->addObject(
				new xml::Any( DIRECTORY_TAG )
			);
			directory->setStringAttribute( PATH_ATTR, path );
			directory->setStringAttribute( SOURCE_TAG, source );
			m_oldFiles.removeElementVal( path );
			if( mkdir( path ) )
			{
				doLogMessageEx(gakLogging::llError, STRING("Cannot create ") + path );
				success = false;
			}
		}
		*cp = DIRECTORY_DELIMITER;
		cp++;
	}

	return success;
}

//---------------------------------------------------------------------------
void __fastcall TInstallerForm::SelectPath(TObject *Sender)
{
	STRING selDirectory;

	if( Sender == ButtonSelectDestination )
	{
		selDirectory = EditDestination->Text.c_str();
	}
	else if( Sender == ButtonSelectBDE )
	{
		selDirectory = EditBDE->Text.c_str();
	}
	else if( Sender == ButtonSelectDataPath )
	{
		selDirectory = EditDataPath->Text.c_str();
	}
	else if( Sender == ButtonSelectStartMenu )
	{
		selDirectory = EditStartMenu->Text.c_str();
	}

	while( !selDirectory.isEmpty() && !exists( selDirectory ) )
	{
		size_t	slashPos = selDirectory.searchRChar( DIRECTORY_DELIMITER );
		if( slashPos != selDirectory.no_index )
		{
			selDirectory.cut( slashPos );
		}

	}
	if( selDirectory.isEmpty() || !exists( selDirectory ) )
	{
		selDirectory = NULL_STRING;
	}
	if(
		DirectorySelector->ShowModal(
			static_cast<const char *>(selDirectory)
		) == mrOk
	)
	{
		AnsiString selPath = DirectorySelector->DirectoryListBox->Directory;
		if( Sender == ButtonSelectDestination )
		{
			EditDestination->Text = selPath;
		}
		else if( Sender == ButtonSelectBDE )
		{
			EditBDE->Text = selPath;
		}
		else if( Sender == ButtonSelectDataPath )
		{
			EditDataPath->Text = selPath;
		}
		else if( Sender == ButtonSelectStartMenu )
		{
			EditStartMenu->Text = selPath;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TInstallerForm::ButtonCancelClick(TObject *)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TInstallerForm::PathChanged(TObject *)
{
	if( EditDestination->Text == ""
	|| (EditBDE->Text == ""  && m_needBDE)
	|| (EditDataPath->Text == "" && m_needDataPath)
	|| EditStartMenu->Text == "" )
	{
		ButtonInstall->Enabled = false;
	}
	else
	{
		ButtonInstall->Enabled = true;
	}
}
//---------------------------------------------------------------------------


void __fastcall TInstallerForm::FormShow(TObject *)
{
	bool	bdeAvail = false;

	STRING	setupXml = makeFullPath( Application->ExeName.c_str(), "setup.xml" );
	xml::Parser	theParser( setupXml );

	/*
	-------------------------------------
		check the XML file
	-------------------------------------
	*/
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

	m_sourceInfo = m_setupElement->getElement( SOURCE_TAG );
	if( !m_sourceInfo )
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

	m_iconInfo = m_setupElement->getElement( ICONS_TAG );

	/*
		check we have a BDE application
	*/
	m_bdeInfo = m_setupElement->getElement( BDE_TAG );
	if( m_bdeInfo )
	{
		m_aliasName = m_bdeInfo->getAttribute( ALIASNAME_ATTR );
		if( m_aliasName.isEmpty() )
		{
			Application->MessageBox(
				LoadStr( ERR_BAD_SETUP_ALIAS_INFO ).c_str(),
				LoadStr( ERR_CAPTION ).c_str(),
				MB_OK|MB_ICONSTOP
			);
			Close();
			Application->Terminate();
/*@*/		return;
		}
		m_oldAliasName = m_aliasName + "_OLD";
		m_backupAliasName = m_aliasName + "_BACKUP";
		STRING	dbVersionStr = m_bdeInfo->getAttribute( DBVERSION_ATTR );
		if( dbVersionStr.isEmpty() )
		{
			Application->MessageBox(
				LoadStr( ERR_BAD_SETUP_ALIAS_INFO ).c_str(),
				LoadStr( ERR_CAPTION ).c_str(),
				MB_OK|MB_ICONSTOP
			);
			Close();
			Application->Terminate();
/*@*/		return;
		}
		m_dbVersion = dbVersionStr.getValueE<unsigned>();
		m_bdeResource = m_bdeInfo->getAttribute( RESOURCE_ATTR );
		if( m_bdeResource.isEmpty() )
		{
			Application->MessageBox(
				LoadStr( ERR_BAD_SETUP_RESOURCE_INFO ).c_str(),
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
		m_needDataPath = false;

		EditBDE->Enabled = false;
		ButtonSelectBDE->Enabled = false;
	}

	readInstallerFiles(
		m_installerFiles,
		m_sourceInfo, setupXml
	);

	if( m_needBDE )
	{
		m_bdeSource.dirlist(
			makeFullPath( Application->ExeName.c_str(), BDE_DIR )
		);
	}

	if( m_needDataPath )
	{
		m_dataSource.dirlist(
			makeFullPath( Application->ExeName.c_str(), DATA_DIR )
		);
	}

	m_uninstall = m_setupElement->addObject( new xml::Any( UNINSTALL_TAG ) );
	m_filesCopied = m_uninstall->addObject( new xml::Any( FILES_TAG ) );

	STRING		group = m_iconInfo->getAttribute( GROUP_ATTR );
	AppInfo		appInfo( m_appInfo );
	STRING		registry = appInfo.getRegistry();

	m_uninstallSource.findFiles(
		makeFullPath( Application->ExeName.c_str(), "uninstall.*" )
	);

	std::auto_ptr<TRegistry>	theRegistry( new TRegistry );
	theRegistry->RootKey = HKEY_LOCAL_MACHINE;

	/*
	-----------------------------------------------------------------------
		Programs
	-----------------------------------------------------------------------
	*/
	{
		/*
			Test for update
		*/
		STRING	appPath;
		if(
			theRegistry->OpenKey(
				static_cast<const char *>(registry), true
			)
		)
		{
			if( theRegistry->ValueExists( "path" ) )
			{
				appPath = theRegistry->ReadString( "path" ).c_str();
			}

			theRegistry->CloseKey();
		}
		else
		{
			Application->MessageBox(
				LoadStr( ERR_NO_INSTALL_PERM ).c_str(),
				LoadStr( ERR_CAPTION ).c_str(),
				MB_OK|MB_ICONSTOP
			);
			Close();
			Application->Terminate();
/*@*/		return;
		}

		if( appPath.isEmpty() )
		{
			appPath = appInfo.getDefaultDestination( m_installerFiles[PROGRAM_DIR].destination );
		}

		if( !appPath.isEmpty() )
		{
			EditDestination->Text = static_cast<const char *>(appPath);
		}
	}

	/*
	-----------------------------------------------------------------------
		BDE
	-----------------------------------------------------------------------
	*/
	{
		/*
			Test existing BDE
		*/
		T_STRING	bdePath;
		if( m_needBDE )
		{
			if( theRegistry->OpenKey( BDE_REGISTRY_KEY, true ) )
			{
				if( theRegistry->ValueExists( "DLLPATH" ) )
				{
					bdePath = theRegistry->ReadString( "DLLPATH" ).c_str();
					bdePath = bdePath.getFirstToken( ";" );
					bdeAvail = true;
				}

				theRegistry->CloseKey();
			}
			else
			{
				Application->MessageBox(
					LoadStr( ERR_NO_INSTALL_PERM ).c_str(),
					LoadStr( ERR_CAPTION ).c_str(),
					MB_OK|MB_ICONSTOP
				);
				Close();
				Application->Terminate();
/*@*/			return;
			}
		}

		if(
			theRegistry->OpenKey(
				"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
				false
			)
		)
		{
			if( m_needBDE && bdePath.isEmpty() && theRegistry->ValueExists( "CommonFilesDir" ) )
			{
				bdePath = theRegistry->ReadString( "CommonFilesDir" ).c_str();
				bdePath += DIRECTORY_DELIMITER;
				bdePath += BDE_DIR;
			}

			theRegistry->CloseKey();
		}

		if( m_needBDE && !bdePath.isEmpty() )
		{
			EditBDE->Text = static_cast<const char *>(bdePath);
		}
	}

	/*
	-----------------------------------------------------------------------
		DB direcxtory
	-----------------------------------------------------------------------
	*/
	{
		STRING		dataPath;
		if( bdeAvail )
		{
			const DBDesc	databaseInfo = vcl::getDatabaseDesc( m_aliasName );
			dataPath = databaseInfo.szPhyName;
		}

		if( dataPath.isEmpty() )
		{
			dataPath = appInfo.getDefaultDBDestination();
		}


		if( !m_dataSource.size() )
		{
			EditDataPath->Text = "";
			EditDataPath->Enabled = false;
			ButtonSelectDataPath->Enabled = false;
			m_needDataPath = false;
		}
		else if( !dataPath.isEmpty() )
		{
			EditDataPath->Text = static_cast<const char *>(dataPath);
		}
	}

	/*
	-----------------------------------------------------------------------
		Start Menu
	-----------------------------------------------------------------------
	*/

	{
		STRING menuPath = AppInfo::getCommonStartMenu();
		menuPath += DIRECTORY_DELIMITER;
		menuPath += group;
		EditStartMenu->Text = static_cast<const char *>(menuPath);
	}
}
//---------------------------------------------------------------------------
static bool getVersion(
	const STRING &versionFile,
	unsigned long *major, unsigned long *minor
)
{
	bool	vInfoFound = false;
	DWORD	dummy1, versionInfoSize;
	UINT	dummy2;

	versionInfoSize = GetFileVersionInfoSize(
		versionFile, &dummy1
	);
	if( versionInfoSize )
	{
		void *data = malloc( versionInfoSize );
		if( data )
		{
			VS_FIXEDFILEINFO	*fixedFileInfo;

			GetFileVersionInfo( versionFile, 0L, versionInfoSize, data );
			if( VerQueryValue( data, "\\", (void **)&fixedFileInfo, &dummy2 ) )
			{
				*major = fixedFileInfo->dwFileVersionMS;
				*minor = fixedFileInfo->dwFileVersionLS;
				vInfoFound = true;
			}
			free( data );
		}
	}

	return vInfoFound;
}
//---------------------------------------------------------------------------
InstallMode TInstallerForm::check4Mode(
	const STRING &sourceOrig, DirectoryList &dirList, const STRING &destinationPath
)
{
	CI_STRING		name;
	STRING			source, destination;
	unsigned long	sMajor, sMinor, dMajor, dMinor;
	bool			sVersionFound, dVersionFound;
	STRING			sourcePath = makeFullPath(
		Application->ExeName.c_str(), sourceOrig
	);

	InstallMode		installMode = imNONE_INSTALLED;
	AnsiString		status;

	status.printf(
		LoadStr( INSTALL_CHECK ).c_str(), static_cast<const char *>(sourceOrig)
	);
	LabelStatus->Caption = status;
	idleLoop();
	for(
		DirectoryList::iterator it = dirList.begin(), endIT = dirList.end();
		it != endIT;
		++it
	)
	{
		name = it->fileName;
		if( name != "." && name != ".." )
		{
			if( name.beginsWith( sourcePath ) )
			{
				source = name;
				name += sourcePath.strlen();
			}
			else
			{
				source = sourcePath;
				source += DIRECTORY_DELIMITER;
				source += name;
			}

			destination = destinationPath;
			if( !destination.endsWith( DIRECTORY_DELIMITER ) )
			{
				destination += DIRECTORY_DELIMITER;
			}
			destination += name;


			if( isFile( source ) )
			{
				if( exists( destination ) )
				{
					sVersionFound = getVersion( source, &sMajor, &sMinor );
					dVersionFound = getVersion( destination, &dMajor, &dMinor );

					if( sVersionFound && dVersionFound )
					{
						if( sMajor > dMajor
						|| (sMajor == dMajor && sMinor > dMinor ) )
						{
							installMode = imUPDATE_INSTALL;
/*v*/						break;
						}
					}
				}
				else if( installMode < imNEW_INSTALL )
				{
					installMode = imNEW_INSTALL;
				}
			}
		}
	}

	return installMode;
}

//---------------------------------------------------------------------------
void TInstallerForm::copyFile(
	std::ostream &logOut,
	InstallMode installMode,
	const F_STRING &iName,
	const STRING &sourceOrig, const STRING &sourcePath,
	const STRING &destinationPath
)
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::copyFile");

	logOut << __FILE__ << __LINE__ << ": iName=" << iName << std::endl << std::flush;
	logOut << __FILE__ << __LINE__ << ": sourceOrig=" << sourceOrig << std::endl << std::flush;
	logOut << __FILE__ << __LINE__ << ": sourcePath=" << sourcePath << std::endl << std::flush;
	logOut << __FILE__ << __LINE__ << ": destinationPath=" << destinationPath << std::endl << std::flush;

	unsigned long	sMajor, sMinor, dMajor, dMinor;
	bool			sVersionFound, dVersionFound;
	F_STRING		name = iName;
	STRING			source, destination;

	if( name.beginsWith( sourcePath ) )
	{
		source = name;
		name += sourcePath.strlen()+1;
	}
	else
	{
		source = sourcePath;
		source += DIRECTORY_DELIMITER;
		source += name;
	}

	destination = destinationPath;
	if( !destination.endsWith( DIRECTORY_DELIMITER ) )
	{
		destination += DIRECTORY_DELIMITER;
	}
	destination += name;

	logOut << __FILE__ << __LINE__ << ": name=" << name << std::endl << std::flush;
	logOut << __FILE__ << __LINE__ << ": source=" << source << std::endl << std::flush;
	logOut << __FILE__ << __LINE__ << ": destination=" << destination << std::endl << std::flush;

	if( isFile( source ) )
	{
		STRING fileMode = "skipped";

		if( installMode != imNONE_INSTALLED )
		{
			if( !makepath( destination, sourceOrig ) )
			{
				AnsiString		status;

				status.printf(
					LoadStr( ERR_FAILED_CREATE ).c_str(),
					static_cast<const char *>(destination)
				);
				throw Exception( status );
			}
			if( name != IDAPI_CFG || !exists( destination ) )
			{
				AnsiString		status;
				
				status.printf(
					LoadStr( INSTALL_COPY ).c_str(),
					static_cast<const char *>(destination)
				);
				LabelStatus->Caption = status;
				idleLoop();

				if( exists( destination ) )
				{
					FileSecurity	fs;

					fs.loadFromFile( destination );
					sVersionFound = getVersion( source, &sMajor, &sMinor );
					dVersionFound = getVersion( destination, &dMajor, &dMinor );

					if( !sVersionFound || !dVersionFound )
					{
						remove( destination );

						logOut << __FILE__ << __LINE__ << ": Copy source to destination\n" << std::flush;
						fcopy( source, destination );
						fileMode = "update";
						logOut << __FILE__ << __LINE__ << ": Copied source to destination\n" << std::flush;
					}
					else
					{
						if( sMajor > dMajor
						|| (sMajor == dMajor && sMinor > dMinor ) )
						{
							const F_STRING backup = destination + ".bak";

							if( !makepath( backup, sourceOrig ) )
							{
								status.printf(
									LoadStr( ERR_FAILED_CREATE ).c_str(),
									static_cast<const char *>(backup)
								);
								throw Exception( status );
							}
							rename( destination, backup );


							logOut << __FILE__ << __LINE__ << ": Copy source to destination\n" << std::flush;
							fcopy( source, destination );
							fileMode = "updateVersion";
							logOut << __FILE__ << __LINE__ << ": Copied source to destination\n" << std::flush;
						}
					}
					fs.saveToFile( destination );
				}
				else
				{
					logOut << __FILE__ << __LINE__ << ": Copy source to destination\n" << std::flush;
					fcopy( source, destination );
					fileMode = "new";
					logOut << __FILE__ << __LINE__ << ": Copied source to destination\n" << std::flush;
				}
			}
		}

		xml::Element	*file = m_filesCopied->addObject( new xml::Any( FILE_TAG ) );
		file->setStringAttribute( DESTINATION_ATTR, destination );
		file->setStringAttribute( "fileMode", fileMode );
		file->setStringAttribute( SOURCE_TAG, sourceOrig );
		m_oldFiles.removeElementVal( destination );
	}
}

//---------------------------------------------------------------------------
void TInstallerForm::copyFiles(
	std::ostream &logOut,
	InstallMode installMode,
	const STRING &sourceOrig, DirectoryList &dirList, const STRING &destinationPath
)
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::copyFiles");
	int				pos =ProgressBar->Position;
	STRING			sourcePath = makeFullPath(
		Application->ExeName.c_str(), sourceOrig
	);

	for(
		DirectoryList::iterator it = dirList.begin(), endIT = dirList.end();
		it != endIT;
		++it, ++pos
	)
	{
		const F_STRING name = it->fileName;
		if( name != "." && name != ".." )
		{
			try
			{
				copyFile(
					logOut,
					installMode,
					name, sourceOrig, sourcePath, destinationPath
				);
			}
			catch( std::exception & )
			{
				AnsiString	status;

				status.printf(
					LoadStr( ERR_FAILED_CREATE ).c_str(),
					static_cast<const char *>(name)
				);
				throw Exception( status );
			}

		}
		ProgressBar->Position = pos;
	}
}

//---------------------------------------------------------------------------
InstallMode TInstallerForm::copyData(
	std::ostream &logOut,
	const STRING &sourceOrig, DirectoryList &dirList, const STRING &destinationPath
)
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::copyData");

	InstallMode	installMode = check4Mode( sourceOrig, dirList, destinationPath );

	copyFiles(
		logOut,
		installMode, sourceOrig, dirList, destinationPath
	);

	return installMode;
}

//---------------------------------------------------------------------------

void TInstallerForm::loadOldFiles( const gak::STRING &setupFile )
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::loadOldFiles");

	if( exists( setupFile ) )
	{
		xml::Parser		theParser( setupFile );
		xml::Document	*theSetupDoc = theParser.readFile( false );

		if( theSetupDoc )
		{
			xml::Element *setupElement = theSetupDoc->getElement( SETUP_TAG );
			if( setupElement )
			{
				xml::Element *uninstall = setupElement->getElement( UNINSTALL_TAG );
				if( uninstall )
				{
					xml::Element *filesCopied = uninstall->getElement( FILES_TAG );
					if( filesCopied )
					{
						for(
							size_t i = 0;
							i < filesCopied->getNumObjects();
							i++ )
						{
							xml::Element	*theEntry = filesCopied->getElement(i);
							if( theEntry )
							{
								const STRING source = theEntry->getAttribute( SOURCE_TAG );
								const STRING destination = theEntry->getAttribute( DESTINATION_ATTR );
								if( source != DATA_DIR && source != BDE_DIR )
								{
									m_oldFiles.addElement( destination );
								}
							}
						}
					}
				}
			}

			delete theSetupDoc;
		}
	}
}

//---------------------------------------------------------------------------

gak::STRING TInstallerForm::getPath( TEdit *editControl ) const
{
	STRING	result = editControl->Text.c_str();
	if( result.endsWith( DIRECTORY_DELIMITER ) )
	{
		result.cut(result.strlen()-1);
	}

	return result;
}

//---------------------------------------------------------------------------

bool TInstallerForm::installDatabase( std::ostream &logOut )
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::installDatabase");
	TDateTime		now = TDateTime::CurrentDateTime();
	const STRING	newDataPath = getPath( EditDataPath );
	F_STRING		oldDataPath, backupDataPath;
	bool			updateDatabase = false;

	logOut << __FILE__ << __LINE__ << ": Installing database\n" << std::flush;
	logOut << __FILE__ << __LINE__ << ": newDataPath=" << newDataPath << std::endl << std::flush;

	DBIResult result = DbiInit( NULL );
	if( result == DBIERR_NONE )
	{
		DBIResult result = DbiInit( NULL );
		if( result == DBIERR_NONE )
		{
			DBDesc	DatabaseInfo;

			result = DbiGetDatabaseDesc( m_aliasName, &DatabaseInfo );
			if( result == DBIERR_NONE )
			{
				oldDataPath = DatabaseInfo.szPhyName;
				logOut << __FILE__ << __LINE__ << ": oldDataPath=" << oldDataPath << std::endl << std::flush;
			}
			else
			{
				const STRING	bdeAliasPath = "PATH:" + newDataPath;
				result = DbiAddAlias(
					NULL, m_aliasName, NULL, (char *)bdeAliasPath, true
				);
				if( result != DBIERR_NONE )
				{
					Application->MessageBox(
						LoadStr( ERR_FAILED_ALIAS ).c_str(),
						LoadStr( ERR_CAPTION ).c_str(),
						MB_OK|MB_ICONERROR
					);
				}
			}

			result = DbiGetDatabaseDesc( m_backupAliasName, &DatabaseInfo );
			if( result == DBIERR_NONE )
			{
				backupDataPath = DatabaseInfo.szPhyName;
				logOut << __FILE__ << __LINE__ << ": backupDataPath=" << backupDataPath << std::endl << std::flush;
			}
			else
			{
				backupDataPath = newDataPath + DIRECTORY_DELIMITER_STRING "backup";

				const STRING	bdeAliasPath = "PATH:" + backupDataPath;
				result = DbiAddAlias(
					NULL, m_backupAliasName, NULL, (char *)bdeAliasPath, true
				);
				if( result != DBIERR_NONE )
				{
					Application->MessageBox(
						LoadStr( ERR_FAILED_ALIAS ).c_str(),
						LoadStr( ERR_CAPTION ).c_str(),
						MB_OK|MB_ICONERROR
					);
				}
			}
		}
		DbiExit();
	}
	else
	{
		Application->MessageBox(
			LoadStr( ERR_FAILED_BDE ).c_str(),
			LoadStr( ERR_CAPTION ).c_str(),
			MB_OK|MB_ICONERROR
		);
	}

	if( !oldDataPath.isEmpty() )
	{
		int	oldDbVersion = ConfigDataModule->GetDBVersionByAlias( m_aliasName );
		logOut << __FILE__ << __LINE__ << ": oldDbVersion=" << oldDbVersion << std::endl << std::flush;
		logOut << __FILE__ << __LINE__ << ": m_dbVersion=" << m_dbVersion << std::endl << std::flush;
		if( oldDbVersion != m_dbVersion )
		{
			updateDatabase = true;

			TSession	*instSession = new TSession( NULL );
			TStringList	*aliasParams = new TStringList;

			instSession->SessionName = "CresdInstaller";
			instSession->ConfigMode = cmPersistent;
			instSession->Open();
			instSession->GetAliasParams( static_cast<const char *>(m_aliasName), aliasParams );
			if( instSession->IsAlias( static_cast<const char *>(m_oldAliasName) ) )
				instSession->DeleteAlias( static_cast<const char *>(m_oldAliasName) );
			instSession->AddAlias(
				static_cast<const char *>(m_oldAliasName),
				instSession->GetAliasDriverName( static_cast<const char *>(m_aliasName) ),
				aliasParams
			);

			if( oldDataPath == newDataPath )
			{
				const F_STRING	oldBackupPath = oldDataPath
					+ DIRECTORY_DELIMITER_STRING "old_"
					+ now.FormatString("dd_mm_yyyy_hh_nn_ss").c_str()
				;
				logOut << __FILE__ << __LINE__ << ": oldBackupPath=" << oldBackupPath << std::endl << std::flush;

				mkdir( oldBackupPath );

				STRING tmpPath = "PATH=";
				tmpPath += oldBackupPath;

				aliasParams->Clear();
				aliasParams->Add( static_cast<const char *>(tmpPath) );
				instSession->ModifyAlias( static_cast<const char *>(m_oldAliasName), aliasParams );

				instSession->SaveConfigFile();
				instSession->Close();

				delete instSession;
				delete aliasParams;

				STRING	oldPattern = oldDataPath;
				oldPattern += DIRECTORY_DELIMITER_STRING "*.*";

				WIN32_FIND_DATA	ffData;
				HANDLE ff = FindFirstFile( oldPattern, &ffData );
				if( ff != INVALID_HANDLE_VALUE )
				{
					do
					{
						if(!(ffData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
						{
							oldPattern = oldDataPath + DIRECTORY_DELIMITER + ffData.cFileName;
							tmpPath = oldBackupPath + DIRECTORY_DELIMITER + ffData.cFileName;

							logOut	<< __FILE__ << __LINE__ << ": Rename "
									<< oldPattern << " to " << tmpPath
									<< std::endl << std::flush;
							rename( oldPattern, tmpPath );
							logOut	<< __FILE__ << __LINE__ << ": Renameed "
									<< oldPattern << " to " << tmpPath
									<< std::endl << std::flush;
						}

					}
					while( FindNextFile( ff, &ffData ) );
					FindClose( ff );
				}
			}
			copyData( logOut, DATA_DIR, m_dataSource, newDataPath );
		}
	}
	else
	{
		copyData( logOut, DATA_DIR, m_dataSource, newDataPath );
	}

	cleanDirectory( backupDataPath );
	copyData( logOut, DATA_DIR, m_dataSource, backupDataPath );

	logOut << __FILE__ << __LINE__ << ": Database installed\n" << std::flush;
	return updateDatabase;
}

//---------------------------------------------------------------------------

void TInstallerForm::installBDE( std::ostream &logOut, InstallMode installMode )
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::installBDE");

	const STRING bdePath = getPath( EditBDE );
	LabelStatus->Caption = LoadStr( INSTALL_BDE );
	idleLoop();

	InstallMode bdeMode = copyData( logOut, BDE_DIR, m_bdeSource, bdePath );

	std::auto_ptr<TRegistry>	theRegistry( new TRegistry() );
	theRegistry->RootKey = HKEY_LOCAL_MACHINE;
	if( theRegistry->OpenKey( BDE_REGISTRY_KEY, true ) )
	{
		STRING	configPath = bdePath;
		theRegistry->WriteString( "DLLPATH", static_cast<const char *>(configPath) );
		configPath += DIRECTORY_DELIMITER_STRING IDAPI_CFG;

		if( !theRegistry->ValueExists( "CONFIGFILE01" ) )
		{
			theRegistry->WriteString( "CONFIGFILE01", static_cast<const char *>(configPath) );
		}
		if( !theRegistry->ValueExists( "RESOURCE" ) )
		{
			theRegistry->WriteString( "RESOURCE", m_bdeResource );
		}
		if( !theRegistry->ValueExists( "SAVECONFIG" ) )
		{
			theRegistry->WriteString( "SAVECONFIG", "WIN32" );
		}

		if( installMode == imNEW_INSTALL )
		{
			AnsiString useCountStr = "0";
			if( theRegistry->ValueExists( BDE_USECOUNT_KEY ) )
			{
				useCountStr = theRegistry->ReadString( BDE_USECOUNT_KEY );
			}
			int useCount = atoi( useCountStr.c_str() );
			useCount++;
			useCountStr.printf( "%d", useCount );
			theRegistry->WriteString( BDE_USECOUNT_KEY, useCountStr );
		}
		else if( !theRegistry->ValueExists( BDE_USECOUNT_KEY ) )
		{
			AnsiString useCountStr = "1";
			theRegistry->WriteString( BDE_USECOUNT_KEY, useCountStr );
		}
		theRegistry->CloseKey();

		if( bdeMode != imNONE_INSTALLED )
		{
			STRING cmd = "regedit /s \"";
			cmd += makeFullPath(
				Application->ExeName.c_str(), "BDE\\registry\\bde.reg"
			);
			cmd += '\"';
			system( cmd );
		}
	}
}

//---------------------------------------------------------------------------

void TInstallerForm::installMenu(
	const AppInfo &appInfo,
	gak::F_STRING *mainProgram, gak::F_STRING *mainPath,
	gak::F_STRING *updateProgram, gak::F_STRING *updatePath
)
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::installMenu");

	F_STRING		destFile, workingDir;
	const F_STRING	menuPath = getPath( EditStartMenu );
	const F_STRING	programDestination = getPath( EditDestination );

	LabelStatus->Caption = LoadStr( INSTALL_MENU );
	idleLoop();

	xml::XmlArray	theIcons;
	xml::Element	*theIcon;

	const STRING bdePath = getPath( EditBDE );
	m_iconInfo->getChildElements( &theIcons, ICON_TAG );
	for(
		theIcons.first();
		(theIcon = theIcons.current()) != NULL;
		theIcons.next()
	)
	{
		STRING	title = theIcon->getAttribute( TITLE_ATTR );
		STRING	target = theIcon->getAttribute( TARGET_ATTR );
		if( !title.isEmpty() && !target.isEmpty() )
		{
			CI_STRING	mainAttr = theIcon->getAttribute( MAIN_ATTR );
			CI_STRING	dbUpdAttr = theIcon->getAttribute( DBUPD_ATTR );
			std::size_t	slashPos = target.searchChar( DIRECTORY_DELIMITER );

			if( slashPos != target.no_index )
			{
				STRING sourceDir = target.leftString( slashPos );
				STRING	destinationPath = sourceDir == PROGRAM_DIR
					? programDestination
					: sourceDir == BDE_DIR
						? bdePath
						: appInfo.getDefaultDestination(
							m_installerFiles[sourceDir].destination
						)
				;

				if( destinationPath.endsWith( DIRECTORY_DELIMITER ) )
				{
					destinationPath.cut(destinationPath.strlen()-1);
				}
				destFile = destinationPath;
				target += sourceDir.strlen() + std::size_t(1);
			}

			destFile += DIRECTORY_DELIMITER;
			workingDir = destFile;
			destFile += target;

			const STRING linkFile = menuPath + DIRECTORY_DELIMITER + title + ".lnk";

			if( !makepath( linkFile, MENU_DIR ) )
			{
				AnsiString status;
				status.printf(
					LoadStr( ERR_FAILED_CREATE ).c_str(),
					static_cast<const char *>(linkFile)
				);
				throw Exception( status );
			}

			if( mainAttr == "yes" || mainAttr == "true" )
			{
				*mainProgram = destFile;
				*mainPath = workingDir;
			}

			if( dbUpdAttr == "yes" || dbUpdAttr == "true" )
			{
				*updateProgram = destFile;
				*updatePath = workingDir;
			}

			CreateShortcut(
				destFile, "", linkFile, workingDir, "",  SW_SHOW
			);
			xml::Element	*file = m_filesCopied->addObject(
				new xml::Any( FILE_TAG )
			);
			file->setStringAttribute( DESTINATION_ATTR, linkFile );
			file->setStringAttribute( SOURCE_TAG, MENU_DIR );
			m_oldFiles.removeElementVal( linkFile );
		}
	}
	workingDir = destFile = programDestination;
	destFile += DIRECTORY_DELIMITER_STRING "Uninstall.exe";

	const STRING linkFile = menuPath + DIRECTORY_DELIMITER_STRING "Uninstall.lnk";

	CreateShortcut(
		destFile, "", linkFile, workingDir, "",  SW_SHOW
	);
	xml::Element	*file = m_filesCopied->addObject( new xml::Any(FILE_TAG) );
	file->setStringAttribute( DESTINATION_ATTR, linkFile );
	file->setStringAttribute( SOURCE_TAG, MENU_DIR );
	m_oldFiles.removeElementVal( linkFile );

	ShellExecute(
		NULL, "open", EditStartMenu->Text.c_str(),
		NULL, NULL, SW_SHOWNOACTIVATE
	);
}

//---------------------------------------------------------------------------

void TInstallerForm::checkInstall()
{
	AppInfo	appInfo( m_appInfo );
	const F_STRING programDestination = getPath( EditDestination );

	for(
		InstallerFiles::const_iterator it = m_installerFiles.cbegin(),
			endIT = m_installerFiles.cend();
		it != endIT;
		++it
	)
	{
		const STRING	sourceDir = it->getKey();
		const STRING	destinationPath = sourceDir == PROGRAM_DIR
			? programDestination
			: appInfo.getDefaultDestination( it->getValue().destination );

		if( destinationPath.isEmpty() )
		{
			throw Exception(LoadStr(ERR_BAD_DESTINATION));
		}
	}
}

//---------------------------------------------------------------------------

void TInstallerForm::doInstall()
{
	AppInfo	appInfo( m_appInfo );
	STRING	appName = appInfo.getApplicationName();

	AnsiString	message;
	message.printf(
		LoadStr( INSTALL_START ).c_str(),
		static_cast<const char *>(appName)
	);
	if( Application->MessageBox(
		message.c_str(),
		LoadStr( INSTALL_CAPTION ).c_str(),
		MB_YESNO|MB_ICONQUESTION
	) != IDYES )
	{
/***/	return;
	}

	const F_STRING programDestination = getPath( EditDestination );
	const F_STRING setupFile = programDestination + DIRECTORY_DELIMITER_STRING "setup.xml";
	const F_STRING logFile = programDestination + DIRECTORY_DELIMITER_STRING "setup.log";

	makePath(logFile);
	std::ofstream	logOut( logFile );

	if( !logOut.is_open() )
	{
		Application->MessageBox(
			LoadStr( ERR_FAILED_CREATE_LOG ).c_str(), LoadStr( INSTALL_CAPTION ).c_str(),
			MB_OK
		);
/***/	return;
	}

	xml::Element	*file = m_filesCopied->addObject( new xml::Any( FILE_TAG ) );
	file->setStringAttribute( DESTINATION_ATTR, logFile );
	file->setStringAttribute( "fileMode", "new" );
	file->setStringAttribute( SOURCE_TAG, PROGRAM_DIR );
	m_oldFiles.removeElementVal( logFile );

	F_STRING	mainProgram, mainPath, updateProgram, updatePath;
	AnsiString	status;
	size_t		numFiles =
		getNumFiles( m_installerFiles ) +
		m_bdeSource.size() +
		m_dataSource.size()*2 +
		m_uninstallSource.size();

	ProgressBar->Max = numFiles;
	ProgressBar->Position = 0;

	loadOldFiles( setupFile );

	std::auto_ptr<TRegistry>	theRegistry( new TRegistry() );
	theRegistry->RootKey = HKEY_LOCAL_MACHINE;

	InstallMode	installMode = imNONE_INSTALLED;
	for(
		InstallerFiles::const_iterator it = m_installerFiles.cbegin(),
			endIT = m_installerFiles.cend();
		it != endIT;
		++it
	)
	{
		const STRING	sourceDir = it->getKey();
		const STRING	destinationPath = sourceDir == PROGRAM_DIR
			? programDestination
			: appInfo.getDefaultDestination( it->getValue().destination );

		logOut	<< __FILE__ << __LINE__ << ": Installing " << sourceDir
				<< " to " << destinationPath << std::endl << std::flush;
		doLogValueEx( gakLogging::llInfo, destinationPath );
		DirectoryList	programSource;
		programSource.dirtree(
			makeFullPath( Application->ExeName.c_str(), sourceDir )
		);

		InstallMode im = copyData( logOut, sourceDir, programSource, destinationPath );
		if( im > installMode )
		{
			installMode = im;
		}
		logOut	<< __FILE__ << __LINE__ << ": Installed " << sourceDir
				<< " to " << destinationPath << std::endl << std::flush;
	}
	copyData( logOut, ".", m_uninstallSource, programDestination );
	STRING	registry = appInfo.getRegistry();
	if( theRegistry->OpenKey( static_cast<const char *>(registry), true ) )
	{
		theRegistry->WriteString(
			"path", static_cast<const char *>(programDestination)
		);
		theRegistry->CloseKey();
	}

	if( m_needBDE )
	{
		logOut << __FILE__ << __LINE__ << ": Installing BDE\n" << std::flush;
		installBDE( logOut, installMode );
		logOut << __FILE__ << __LINE__ << ": BDE installed\n" << std::flush;
	}

	bool updateDatabase = m_needDataPath ? installDatabase(logOut) : false;

	if( m_iconInfo )
	{
		logOut << __FILE__ << __LINE__ << ": Installing menu\n" << std::flush;
		installMenu(
			appInfo,
			&mainProgram, &mainPath,
			&updateProgram, &updatePath
		);
		logOut << __FILE__ << __LINE__ << ": Menu Installed\n" << std::flush;
	}

	for(
		ArrayOfStrings::const_iterator it = m_oldFiles.cbegin(),
			endIT = m_oldFiles.cend();
		it != endIT;
		++it
	)
	{
		logOut << __FILE__ << __LINE__ << ": Deleting " << *it << std::endl << std::flush;
		remove( *it );
		logOut << __FILE__ << __LINE__ << ": Deleted " << *it << std::endl << std::flush;
	}

	logOut << __FILE__ << __LINE__ << ": Writing setup.xml\n" << std::flush;
	const STRING	setupXML = m_setupElement->generateDoc();
	setupXML.writeToFile( setupFile );
	logOut << __FILE__ << __LINE__ << ": setup.xml written\n" << std::flush;

	if( updateDatabase && !updateProgram.isEmpty() )
	{
		logOut << __FILE__ << __LINE__ << ": Change directory to "
			   << updatePath << std::endl << std::flush;
		setcwd( updatePath );
		logOut << __FILE__ << __LINE__ << ": Executing "
			   << updateProgram << std::endl << std::flush;
		SyncExec( updateProgram, "UPGRADE", SW_SHOW );
		logOut << __FILE__ << __LINE__ << ": Updated DB\n" << std::flush;
	}
	else
	{
		SetFocus();
		BringToFront();
		LabelStatus->Caption = LoadStr( INSTALL_FINISH );
		message.printf( LoadStr( INSTALL_END ).c_str(), static_cast<const char *>(appName) );
		if( !mainProgram.isEmpty() )
		{
			message += LoadStr( INSTALL_EXEC );
		}

		if( Application->MessageBox(
			message.c_str(),
			LoadStr( INSTALL_CAPTION ).c_str(),
			mainProgram.isEmpty() ? MB_ICONINFORMATION : MB_YESNO|MB_ICONQUESTION
		) == IDYES )
		{
			setcwd( mainPath );
			WinExec( mainProgram, SW_SHOW );
		}
	}

	Close();
	logOut << __FILE__ << __LINE__ << ": Setup completed.\n" << std::flush;
}

//---------------------------------------------------------------------------
void __fastcall TInstallerForm::ButtonInstallClick(TObject *)
{
	doEnterFunctionEx(gakLogging::llInfo,"TInstallerForm::ButtonInstallClick");

	try
	{
		checkInstall();
		doInstall();
	}
	catch( std::exception &e )
	{
		throw Exception( e.what() );
	}
	catch( Exception &e )
	{
		throw;
	}
	catch( ... )
	{
		throw Exception( "Unkown Error" );
	}
}
//---------------------------------------------------------------------------




