/*
		Project:		MG Setup
		Module:			CollectorMain.cpp
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
#include <vcl/registry.hpp>

#include <gak/fcopy.h>
#include <gak/vcl_tools.h>
#include <gak/xmlParser.h>
#include <winlib/f_type.h>
#include <gak/directory.h>

#pragma hdrstop

#include "CollectorMain.h"
#include "ProgFilesFrm.h"
#include "IconFrm.h"
#include "DirSelFrm.h"
#include "ErrorFrm.h"
#include "Config.h"
#include "StatusFrm.h"
#include "tags.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCollectorForm *CollectorForm;

//---------------------------------------------------------------------------
__fastcall TCollectorForm::TCollectorForm(TComponent* Owner)
	: TForm(Owner)
{
	m_dbVersion = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCollectorForm::ButtonFilesClick(TObject *)
{
	ProgramFilesForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TCollectorForm::ButtonIconsClick(TObject *)
{
	IconForm->ShowModal();
}
//---------------------------------------------------------------------------

void TCollectorForm::readApplicationInfo()
{
	doEnterFunction("TCollectorForm::readApplicationInfo");
	xml::Element *appInfoXml = m_setupElement->getElement( APPLICATION_TAG );
	if( appInfoXml )
	{
		AppInfo	appInfo( appInfoXml );

		EditApplicationTitle->Text = static_cast<const char *>(
			appInfo.title
		);
		EditApplicationVersion->Text = static_cast<const char *>(
			appInfo.version
		);
		EditAuthor->Text = static_cast<const char *>(
			appInfo.author
		);
	}
	else
	{
		EditApplicationTitle->Text =  "";
		EditApplicationVersion->Text =  "";
		EditAuthor->Text = "";
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::saveApplicationInfo()
{
	doEnterFunction("TCollectorForm::saveApplicationInfo");
	xml::Element *appInfoXml = m_setupElement->getElement( APPLICATION_TAG );
	if( !appInfoXml )
	{
		appInfoXml = m_setupElement->addObject( new xml::Any( APPLICATION_TAG ) );
	}

	if( appInfoXml )
	{
		AppInfo	appInfo(
			EditApplicationTitle->Text,
			EditAuthor->Text,
			EditApplicationVersion->Text

		);
		appInfo.setAttributes( appInfoXml );

		// remove no longer used attributes
		appInfoXml->deleteAttribute( "path" );
		appInfoXml->deleteAttribute( "registry" );
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::readBdeInfo( const gak::STRING &installerFileName )
{
	doEnterFunction("TCollectorForm::readBdeInfo");
	xml::Element *bdeInfo = m_setupElement->getElement( BDE_TAG );
	if( bdeInfo )
	{
		ComboBoxBDEalias->Text = static_cast<const char *>(
			bdeInfo->getAttribute(ALIASNAME_ATTR)
		);
		STRING resource = static_cast<const char *>(
			bdeInfo->getAttribute( RESOURCE_ATTR )
		);

		if( resource == BDE_GERMAN )
		{
			RadioGroupBDE->ItemIndex = 1;
			ComboBoxBDEalias->Enabled = true;
			EditDataPath->Enabled = true;
			ButtonSelectDataPath->Enabled = true;
		}
		else if( resource == BDE_ENGLISH )
		{
			RadioGroupBDE->ItemIndex = 0;
			ComboBoxBDEalias->Enabled = true;
			EditDataPath->Enabled = true;
			ButtonSelectDataPath->Enabled = true;
		}
		else
		{
			RadioGroupBDE->ItemIndex = 2;
			ComboBoxBDEalias->Enabled = false;
			EditDataPath->Enabled = false;
			ButtonSelectDataPath->Enabled = false;
		}

		STRING	dataPath = bdeInfo->getAttribute( DATAPATH_ATTR );
		if( !dataPath.isEmpty() )
			EditDataPath->Text = static_cast<const char *>(
				makeFullPath(installerFileName, dataPath)
			);
		else
			EditDataPath->Text = "";
	}
	else
	{
		ComboBoxBDEalias->Text = "";
		RadioGroupBDE->ItemIndex = 2;
		EditDataPath->Text = "";

		ComboBoxBDEalias->Enabled = false;
		EditDataPath->Enabled = false;
		ButtonSelectDataPath->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void TCollectorForm::saveBdeInfo( const gak::STRING &installerFileName )
{
	doEnterFunction("TCollectorForm::saveBdeInfo");
	xml::Element *bdeInfo = m_setupElement->getElement( BDE_TAG );
	if( RadioGroupBDE->ItemIndex != 2 )
	{
		if( !bdeInfo )
		{
			bdeInfo = m_setupElement->addObject( new xml::Any( BDE_TAG ) );
		}
		if( bdeInfo )
		{
			bdeInfo->setStringAttribute( ALIASNAME_ATTR, ComboBoxBDEalias->Text.c_str() );
			if( RadioGroupBDE->ItemIndex == 1 )
			{
				bdeInfo->setStringAttribute( RESOURCE_ATTR, BDE_GERMAN );
			}
			else if( RadioGroupBDE->ItemIndex == 0 )
			{
				bdeInfo->setStringAttribute( RESOURCE_ATTR, BDE_ENGLISH );
			}
			else if( RadioGroupBDE->ItemIndex == 2 )
			{
				bdeInfo->setStringAttribute( RESOURCE_ATTR, BDE_NONE);
			}
			STRING dataPath = EditDataPath->Text.c_str();
			if( !dataPath.isEmpty() )
			{
				dataPath = makeRelPath( installerFileName, dataPath );
			}
			bdeInfo->setStringAttribute( DATAPATH_ATTR, dataPath );

			m_dbVersion = ConfigDataModule->GetDBVersionByPath( getDataPath() );
			doLogValue(m_dbVersion);
			bdeInfo->setIntegerAttribute( DBVERSION_ATTR, m_dbVersion );
		}
		else
		{
			m_dbVersion = 0;
		}
	}
	else if( bdeInfo )
	{
		m_setupElement->removeObject( bdeInfo );
	}
}

//---------------------------------------------------------------------------
void TCollectorForm::readIcons()
{
	doEnterFunction("TCollectorForm::readIcons");
	xml::Element *iconInfo = m_setupElement->getElement( ICONS_TAG );
	if( iconInfo )
	{
		IconForm->EditIconGroup->Text =
			static_cast<const char *>(iconInfo->getAttribute( GROUP_ATTR ))
		;
		xml::XmlArray	theIcons;
		xml::Element	*theIcon;

		iconInfo->getChildElements( &theIcons, ICON_TAG );
		int row = 1;
		IconForm->StringGridIcons->RowCount = 2;
		for(
			theIcons.first();
			(theIcon = theIcons.current()) != NULL;
			theIcons.next()
		)
		{
			IconForm->StringGridIcons->Cells[0][row] =
				static_cast<const char *>(theIcon->getAttribute( TITLE_ATTR ))
			;
			IconForm->StringGridIcons->Cells[1][row] =
				static_cast<const char *>(theIcon->getAttribute( TARGET_ATTR ))
			;
			IconForm->StringGridIcons->Cells[2][row] =
				static_cast<const char *>(theIcon->getAttribute( MAIN_ATTR ))
			;
			IconForm->StringGridIcons->Cells[3][row] =
				static_cast<const char *>(theIcon->getAttribute( DBUPD_ATTR ))
			;
			row++;
			IconForm->StringGridIcons->RowCount = row+1;
		}
	}
	else
	{
		IconForm->EditIconGroup->Text =  "";
		IconForm->StringGridIcons->RowCount = 2;
		IconForm->StringGridIcons->Cells[0][1] = "";
		IconForm->StringGridIcons->Cells[1][1] = "";
		IconForm->StringGridIcons->Cells[2][1] = "";
		IconForm->StringGridIcons->Cells[3][1] = "";
	}
}

//---------------------------------------------------------------------------
void TCollectorForm::saveIcons()
{
	doEnterFunction("TCollectorForm::saveIcons");
	xml::Element *iconInfo = m_setupElement->getElement( ICONS_TAG );
	if( !iconInfo )
	{
		iconInfo = m_setupElement->addObject( new xml::Any( ICONS_TAG ) );
	}
	if( iconInfo )
	{
		iconInfo->setStringAttribute(
			GROUP_ATTR, IconForm->EditIconGroup->Text.c_str()
		);
		while( iconInfo->getNumObjects() )
		{
			delete iconInfo->removeObject( size_t(0) );
		}

		for( int i=1; i<IconForm->StringGridIcons->RowCount; i++ )
		{
			STRING	title =IconForm->StringGridIcons->Cells[0][i].c_str();
			STRING	target =IconForm->StringGridIcons->Cells[1][i].c_str();
			STRING	mainAttr =IconForm->StringGridIcons->Cells[2][i].c_str();
			STRING	dbUpdAttr =IconForm->StringGridIcons->Cells[3][i].c_str();
			if( !title.isEmpty() && !target.isEmpty() )
			{
				xml::Element *icon = iconInfo->addObject(
					new xml::Any( ICON_TAG )
				);
				icon->setStringAttribute( TITLE_ATTR, title );
				icon->setStringAttribute( TARGET_ATTR, target );
				icon->setStringAttribute( MAIN_ATTR, mainAttr );
				icon->setStringAttribute( DBUPD_ATTR, dbUpdAttr );
			}
		}
	}
}
//---------------------------------------------------------------------------
void TCollectorForm::readSource( const gak::STRING &installerFileName )
{
	doEnterFunction("TCollectorForm::readSource");
	xml::Element *sourceInfo = m_setupElement->getElement( SOURCE_TAG );
	if( sourceInfo )
	{
		ProgramFilesForm->Clear();

		STRING applicationVersion = readInstallerFiles(
			ProgramFilesForm->getInstallerFiles(),
			sourceInfo,
			installerFileName
		);

		if( !applicationVersion.isEmpty() )
		{
			EditApplicationVersion->Text = static_cast<const char *>(
				applicationVersion
			);
		}
	}
}
//---------------------------------------------------------------------------
void TCollectorForm::saveSource( const gak::STRING &installerFileName )
{
	doEnterFunction("TCollectorForm::saveSource");
	saveInstallerFiles(
		installerFileName, m_setupElement,
		ProgramFilesForm->getInstallerFiles()
	);
}
//---------------------------------------------------------------------------
void TCollectorForm::openFile( const gak::STRING &installerFileName )
{
	doEnterFunction("TCollectorForm::openFile");
	if( m_theSetupDoc )
	{
		SaveClick( NULL );
		delete m_theSetupDoc;
		m_theSetupDoc = NULL;
	}

	m_dbVersion = 0;
	StatusBar->SimpleText = static_cast<const char *>(installerFileName);
	xml::Parser	theParser( installerFileName );
	m_theSetupDoc = theParser.readFile( false );
	if( !m_theSetupDoc )
	{
		Application->MessageBox(
			"Failed to read file",
			"Error",
			MB_OK|MB_ICONSTOP
		);
		NewClick( NULL );
/*@*/	return;
	}
	m_setupElement = m_theSetupDoc->getElement( SETUP_TAG );
	if( !m_setupElement )
	{
		Application->MessageBox(
			"Bad collection file",
			"Error",
		MB_OK|MB_ICONSTOP
		);
		NewClick( NULL );
/*@*/	return;
	}

	readApplicationInfo();
	readBdeInfo( installerFileName );
	readIcons();
	readSource( installerFileName );
}

//---------------------------------------------------------------------------

void __fastcall TCollectorForm::OpenClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::OpenClick");
	if( OpenDialog->Execute() )
	{
		openFile( OpenDialog->FileName.c_str() );
	}
	else
		NewClick( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TCollectorForm::FormCreate(TObject *)
{
	doEnterFunction("TCollectorForm::FormCreate");
	m_theSetupDoc = NULL;
	m_setupElement = NULL;

	winlib::FileTypeRegistry fType;

	getFileType( "csc", "open", &fType );
	if( fType.type.isEmpty() )
	{
		fType.type = "csc_file";
	}
	if( fType.type_description.isEmpty() )
	{
		fType.type_description = "GAK Setup Collection";
	}

	fType.cmd = "open";
	fType.cmd_description = "&Open";
	fType.commandLine = '"';
	fType.commandLine += Application->ExeName.c_str();
	fType.commandLine += "\" \"%1\"";
	fType.icon = Application->ExeName.c_str();
	fType.icon += ",1";

	fType.ddeCommand = "";
	fType.ddeIfExec = "";
	fType.application = "";
	fType.topic = "";
	addFileType( &fType );

	TStringList *MyStringList = new TStringList();

	try
	{
		Session->GetAliasNames(MyStringList);
		// fill a list box with alias names for the user to select from
		for( int i = 0; i < MyStringList->Count; i++ )
			ComboBoxBDEalias->Items->Add(MyStringList->Strings[i]);

		ComboBoxBDEalias->Text = "";
	}
	__finally
	{
		delete MyStringList;
	}
}
//---------------------------------------------------------------------------
STRING TCollectorForm::getDataPath( void )
{
	doEnterFunction("TCollectorForm::getDataPath");
	STRING	dataPath = EditDataPath->Text.c_str();

	doLogValue(dataPath);
	if( dataPath.isEmpty() )
	{
		STRING AliasName = ComboBoxBDEalias->Text.c_str();
		doLogValue(AliasName);
		const DBDesc	databaseInfo = getDatabaseDesc( AliasName );
		dataPath = databaseInfo.szPhyName;
		doLogValue(dataPath);
	}

	doLogValue(dataPath);
	return dataPath;
}
//---------------------------------------------------------------------------

void __fastcall TCollectorForm::SaveClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::SaveClick");
	if( !m_theSetupDoc )
	{
		m_theSetupDoc = new xml::Document( "" );
		m_setupElement = m_theSetupDoc->addObject( new xml::Any( SETUP_TAG ) );
	}
	if( m_theSetupDoc && m_setupElement )
	{
		STRING installerFileName = m_theSetupDoc->getFilename();
		if( installerFileName.isEmpty() )
		{
			SaveAsClick( Sender );
/***/		return;
		}

		saveApplicationInfo();
		saveBdeInfo( installerFileName );
		saveIcons();
		saveSource( installerFileName );

		STRING xmlData = m_setupElement->generateDoc();
		xmlData.writeToFile( installerFileName );
	}
}

//---------------------------------------------------------------------------

void __fastcall TCollectorForm::SaveAsClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::SaveAsClick");
	if( !m_theSetupDoc )
	{
		m_theSetupDoc = new xml::Document( "" );
		m_setupElement = m_theSetupDoc->addObject( new xml::Any( SETUP_TAG ) );
	}
	if( m_theSetupDoc && m_setupElement && SaveDialog->Execute() )
	{
		StatusBar->SimpleText = SaveDialog->FileName;
		m_theSetupDoc->setFilename( SaveDialog->FileName.c_str() );
		SaveClick( Sender );
	}
}
//---------------------------------------------------------------------------

void __fastcall TCollectorForm::ButtonSelectDataPathClick(TObject *)
{
	doEnterFunction("TCollectorForm::ButtonSelectDataPathClick");
	AnsiString dataPath = static_cast<const char *>(getDataPath());
	if( DirectorySelector->ShowModal( dataPath ) == mrOk )
	{
		EditDataPath->Text = DirectorySelector->DirectoryListBox->Directory;
	}
}
//---------------------------------------------------------------------------

void __fastcall TCollectorForm::ExitClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::ExitClick");
	if( m_theSetupDoc )
	{
		SaveClick( Sender );
	}
	Close();
}
//---------------------------------------------------------------------------
void TCollectorForm::cleanDirectory( const STRING &dirName )
{
	doEnterFunction("TCollectorForm::cleanDirectory");

	STRING		  	fileName;
	DirectoryList	content;

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
				remove( fileName );
			else
			{
				cleanDirectory( fileName );
				rmdir( fileName );
			}
		}
	}
}

//---------------------------------------------------------------------------
static void myCopy( const STRING &source, const STRING &destination )
{
	doEnterFunction("TCollectorForm::myCopy");

	StatusForm->setStatus( "Copying", source );

	try
	{
		if( isDirectory( source ) )
			dcopy( source, destination );
		else
			fcopy( source, destination );
	}
	catch( std::exception &e )
	{
		AnsiString	msg = "Error copying ";
		msg += static_cast<const char *>(source);
		msg += " to ";
		msg += static_cast<const char *>(destination);
		msg += " ";
		msg += e.what();
		ErrorForm->MemoErrors->Lines->Add( msg );
	}
	catch( ... )
	{
		AnsiString	msg = "Error copying ";
		msg += static_cast<const char *>(source);
		msg += " to ";
		msg += static_cast<const char *>(destination);

		ErrorForm->MemoErrors->Lines->Add( msg );
	}
}
//---------------------------------------------------------------------------
bool TCollectorForm::checkSettings()
{
	doEnterFunction("TCollectorForm::checkSettings");

	if( EditApplicationTitle->Text == "" )
	{
		Application->MessageBox(
			"No Application Title defined!",
			"Error",
			MB_ICONSTOP );
		EditApplicationTitle->SetFocus();
/*@*/	return false;
	}

	if( EditApplicationVersion->Text == "" )
	{
		Application->MessageBox(
			"No Application Version defined!",
			"Error",
			MB_ICONSTOP );
		EditApplicationVersion->SetFocus();
/*@*/	return false;
	}

	if( EditAuthor->Text == "" )
	{
		Application->MessageBox(
			"No author defined!",
			"Error",
			MB_ICONSTOP );
		EditAuthor->SetFocus();
/*@*/	return false;
	}

	if( RadioGroupBDE->ItemIndex < 2 && ComboBoxBDEalias->Text == "" )
	{
		Application->MessageBox(
			"No BDE Alias defined!",
			"Error",
			MB_ICONSTOP );
		ComboBoxBDEalias->SetFocus();
/*@*/	return false;
	}

#if 0
	STRING	dataPath = getDataPath();
	if( dataPath == "" )
	{
		Application->MessageBox(
			"No Data Path defined!",
			"Error",
			MB_ICONSTOP );
		EditDataPath->SetFocus();
/*@*/	return false;
	}
#endif

	if( RadioGroupBDE->ItemIndex < 0 )
	{
		Application->MessageBox(
			"No BDE Language selected!",
			"Error",
			MB_ICONSTOP );
		RadioGroupBDE->SetFocus();
/*@*/	return false;
	}

	return true;
}

//---------------------------------------------------------------------------

void TCollectorForm::copyInstallerFiles( const gak::STRING &directory, const gak::STRING &pattern )
{
	doEnterFunction("TCollectorForm::copyInstallerFiles");
	DirectoryList	content;
	const STRING	appExe = Application->ExeName.c_str();

	content.findFiles( makeFullPath( appExe, pattern ) );
	for(
		DirectoryList::iterator it = content.begin(), endIT = content.end();
		it != endIT;
		++it
	)
	{
		STRING	destination = directory + DIRECTORY_DELIMITER + it->fileName;
		makePath( destination );
		myCopy( makeFullPath(appExe, it->fileName ), destination );
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::copyBdeFiles( const gak::STRING &directory )
{
	doEnterFunction("TCollectorForm::copyBdeFiles");
	if( RadioGroupBDE->ItemIndex != 2 )
	{
		DirectoryList	content;
		STRING			bdeSource = makeFullPath(
			Application->ExeName.c_str(),
			(RadioGroupBDE->ItemIndex == 0)
			? "BDE.ENG"
			: "BDE.DEU"
		);
		content.dirlist( bdeSource );
		for(
			DirectoryList::iterator it = content.begin(), endIT = content.end();
			it != endIT;
			++it
		)
		{
			STRING source = bdeSource + DIRECTORY_DELIMITER + it->fileName;
			if( isFile( source ) )
			{
				STRING destination = directory +
					DIRECTORY_DELIMITER + BDE_DIR +
					DIRECTORY_DELIMITER_STRING +
					it->fileName
				;
				makePath( destination );

				myCopy( source, destination );
			}
		}
		STRING source = bdeSource + DIRECTORY_DELIMITER_STRING "registry"
			DIRECTORY_DELIMITER_STRING "bde.reg"
		;
		STRING destination = directory +
			DIRECTORY_DELIMITER + BDE_DIR + DIRECTORY_DELIMITER_STRING
			"registry" DIRECTORY_DELIMITER_STRING "bde.reg";
		makePath( destination );
		myCopy( source, destination );
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::copyDatabase( const gak::STRING &dataPath, const gak::STRING &directory )
{
	doEnterFunction("TCollectorForm::copyDatabase");
	if( !dataPath.isEmpty() )
	{
		DirectoryList	content;
		content.dirlist( dataPath );
		for(
			DirectoryList::iterator it = content.begin(), endIT = content.end();
			it != endIT;
			++it
		)
		{
			F_STRING name = it->fileName;
			F_STRING extension = fsplit( name );
			if( extension != "CMD"
			&&  extension != "CSV"
			&&  extension != "INI"
			&&  extension != "LCK"
			&&  extension != "SQL"
			&&  extension != "SSL" )
			{
				F_STRING source = dataPath;
				if( !source.endsWith( DIRECTORY_DELIMITER ) )
				{
					source += DIRECTORY_DELIMITER;
				}
				source += name;
				if( isFile( source ) )
				{
					F_STRING destination = directory +
						DIRECTORY_DELIMITER_STRING + DATA_DIR +
						DIRECTORY_DELIMITER_STRING +
						it->fileName
					;
					makePath( destination );

					doLogValue(source);
					doLogValue(destination);
					myCopy( source, destination );
					doLogPosition();
				}
			}
		}
		StatusForm->setStatus( "Emptying", directory );
		doLogPosition();
		ConfigDataModule->emptyTables( directory + DIRECTORY_DELIMITER + DATA_DIR );
		doLogPosition();
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::createSchemaXml( const gak::STRING &dataPath, const gak::STRING &directory )
{
	doEnterFunction("TCollectorForm::createSchemaXml");
	if( !dataPath.isEmpty() )
	{
		StatusForm->setStatus( "Schema", directory );

		DatabaseSchema	theSchema;
		DBconnector		dbConnect;
		STRING			alias = ComboBoxBDEalias->Text.c_str();

		doLogValue(alias);
		dbConnect.m_aliasName = alias;
		doLogPosition();
		theSchema.readSchema( dbConnect );
		doLogPosition();
		theSchema.setDbVersion( m_dbVersion );
		doLogPosition();

		xml::Element	*schemaXML = theSchema.getSchema();
		doLogPosition();
		STRING		schemaXmlString = schemaXML->generateDoc();
		doLogPosition();
		delete schemaXML;

		F_STRING source = *(
			ProgramFilesForm->getInstallerFiles().cbegin()->
			getValue().files.cbegin()
		);
		doLogPosition();
		F_STRING xmlFileName = makeFullPath( source, alias + ".XML" );
		schemaXmlString.writeToFile( xmlFileName );
		std::size_t	slashPos = xmlFileName.searchRChar( DIRECTORY_DELIMITER );
		if( slashPos != xmlFileName.no_index )
		{
			F_STRING	destination = directory +
				DIRECTORY_DELIMITER + PROGRAM_DIR +
				xmlFileName.subString( slashPos )
			;
			makePath( destination );

			doLogPosition();
			myCopy( xmlFileName, destination );
			doLogPosition();
		}
	}
}

//---------------------------------------------------------------------------

void TCollectorForm::copyProgramFiles( const gak::STRING &directory )
{
	doEnterFunction("TCollectorForm::copyProgramFiles");
	const InstallerFiles &files = ProgramFilesForm->getInstallerFiles();

	for(
		InstallerFiles::const_iterator it = files.cbegin(),
			endIT = files.cend();
		it != endIT;
		++it
	)
	{
		const STRING		&group = it->getKey();
		const GroupFiles	&groupFiles = it->getValue().files;

		for(
			GroupFiles::const_iterator it = groupFiles.cbegin(),
				endIT = groupFiles.cend();
			it != endIT;
			++it
		)
		{
			const F_STRING	&source = *it;
			if( !source.isEmpty() )
			{
				std::size_t	slashPos = source.searchRChar( DIRECTORY_DELIMITER );
				if( slashPos != source.no_index )
				{
					F_STRING	destination = directory +
						DIRECTORY_DELIMITER + group +
						source.subString( slashPos )
					;
					makePath( destination );

					myCopy( source, destination );
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TCollectorForm::ButtonCreateInstallerClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::ButtonCreateInstallerClick");
	if( !checkSettings() )
	{
		return;
	}

	if( m_theSetupDoc )
	{
		SaveClick( Sender );
		STRING fileName = m_theSetupDoc->getFilename();
		if( !fileName.isEmpty() )
		{
			try
			{
				Enabled = false;
				StatusForm->Show();
				StatusForm->setStatus( "Creating", fileName );
				/*
					clean up
					========
				*/
				ErrorForm->MemoErrors->Lines->Clear();

				DirectoryList		content;
				STRING				source, destination, name;
				STRING 				directory = getTargetPath();

				size_t	slashPos = fileName.searchRChar( DIRECTORY_DELIMITER );
				if( slashPos != fileName.no_index )
				{
					fileName += slashPos;
				}
				directory += fileName + '.' + EditApplicationVersion->Text.c_str();

				if( m_dbVersion )
				{
					directory += ".db";
					directory += formatNumber( m_dbVersion );
				}
				directory += ".setupDir";

				StatusForm->setStatus( "Cleaning", directory );
				cleanDirectory( directory );

				/*
					copy setup files
					================
				*/
				copyInstallerFiles( directory, "setup.*" );

				/*
					copy uninstall files
					====================
				*/
				copyInstallerFiles( directory, "uninstall.*" );

				/*
					copy bde files
					==============
				*/
				copyBdeFiles( directory );


				/*
					copy database files
					===================
				*/

				STRING	dataPath = getDataPath();
				copyDatabase( dataPath, directory );

				/*
					create schema XML
					=================
				*/
				doLogMessage("*gak*: create Schema");
				createSchemaXml( dataPath, directory );

				/*
					copy program files
					==================
				*/
				doLogMessage("*gak*: copy programm files");
				copyProgramFiles( directory );

				/*
					create the setup XML
					====================
				*/
				doLogMessage("*gak*: create the setup XML");
				StatusForm->setStatus( "Setup", directory );
				destination = directory + DIRECTORY_DELIMITER_STRING "setup.xml";
				STRING xmlData = m_setupElement->generateDoc();
				if( xmlData.writeToFile( destination ) )
				{
					AnsiString	msg = "Error writing ";
					msg += static_cast<const char *>(destination);
					ErrorForm->MemoErrors->Lines->Add( msg );
				}

				/*
					check the icons
					===============
				*/
				StatusForm->setStatus( "Icons", directory );
				xml::Element *iconInfo = m_setupElement->getElement( ICONS_TAG );
				if( iconInfo )
				{
					xml::XmlArray	theIcons;
					xml::Element*theIcon;

					iconInfo->getChildElements( &theIcons, ICON_TAG );
					for(
						theIcons.first();
						(theIcon = theIcons.current()) != NULL;
						theIcons.next()
					)
					{
						source = theIcon->getAttribute( TARGET_ATTR );
						if( !exists( makeFullPath( destination, source ) ) )
						{
							AnsiString	msg = "Short Cut target ";
							msg += static_cast<const char *>(source);
							msg += " does not exist";

							ErrorForm->MemoErrors->Lines->Add( msg );
						}
					}
				}

				/*
					show result
					===========
				*/
				ShellExecute( NULL, "open", directory, "", directory, SW_SHOW );
				StatusForm->Close();
				Enabled = true;
				if( ErrorForm->MemoErrors->Lines->Count )
				{
					ErrorForm->ShowModal();
				}
				else
				{
					ShowMessage( "Finish" );
				}
			}
			catch( ... )
			{
				StatusForm->Close();
				Enabled = true;
				throw;
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TCollectorForm::NewClick(TObject *Sender)
{
	doEnterFunction("TCollectorForm::NewClick");
	if( m_theSetupDoc )
	{
		SaveClick( Sender );
		delete m_theSetupDoc;
		m_theSetupDoc = NULL;
	}

	EditApplicationTitle->Text =  "";
	EditApplicationVersion->Text =  "";
	EditAuthor->Text = "";

	ComboBoxBDEalias->Text = "";
	RadioGroupBDE->ItemIndex = 2;
	EditDataPath->Text = "";
	ComboBoxBDEalias->Enabled = false;
	EditDataPath->Enabled = false;
	ButtonSelectDataPath->Enabled = false;

	ProgramFilesForm->Clear();

	IconForm->EditIconGroup->Text =  "";
	IconForm->StringGridIcons->RowCount = 2;
	IconForm->StringGridIcons->Cells[0][1] = "";
	IconForm->StringGridIcons->Cells[1][1] = "";
	IconForm->StringGridIcons->Cells[2][1] = "";

	StatusBar->SimpleText = "";
	m_dbVersion = 0;
}
//---------------------------------------------------------------------------

STRING TCollectorForm::getTargetPath( void )
{
	doEnterFunction("TCollectorForm::getTargetPath");
	size_t		slashPos;
	STRING		targetPath;

	TRegistry	*newRegistry = new TRegistry;
	if( newRegistry->OpenKeyReadOnly( "Software\\GAK\\Installer" )
	&& newRegistry->ValueExists( "TargetPath" ) )
	{
		targetPath = newRegistry->ReadString( "TargetPath" ).c_str();
	}

	delete newRegistry;

	if( targetPath.isEmpty() )
	{
		if( m_theSetupDoc )
		{
			targetPath = m_theSetupDoc->getFilename();
		}

		if( targetPath.isEmpty() )
		{
			targetPath = Application->ExeName.c_str();
		}

		slashPos = targetPath.searchRChar( DIRECTORY_DELIMITER );
		if( slashPos != targetPath.no_index )
		{
			targetPath.cut( slashPos );
		}
	}

	return targetPath;
}
//---------------------------------------------------------------------------
void TCollectorForm::setTargetPath( const AnsiString &newTarget )
{
	doEnterFunction("TCollectorForm::setTargetPath");
	TRegistry	*newRegistry = new TRegistry;
	if( newRegistry->OpenKey( "Software\\GAK\\Installer", true ) )
	{
		newRegistry->WriteString( "TargetPath", newTarget );
	}

	delete newRegistry;
}
//---------------------------------------------------------------------------
void __fastcall TCollectorForm::TargetDirectoryClick(TObject *)
{
	doEnterFunction("TCollectorForm::TargetDirectoryClick");
	AnsiString targetPath = static_cast<const char *>(getTargetPath());
	if( DirectorySelector->ShowModal( targetPath ) == mrOk )
	{
		setTargetPath( DirectorySelector->DirectoryListBox->Directory );
	}
}
//---------------------------------------------------------------------------



void __fastcall TCollectorForm::RadioGroupBDEClick(TObject *)
{
	if( RadioGroupBDE->ItemIndex == 2 || RadioGroupBDE->ItemIndex < 0 )
	{
		ComboBoxBDEalias->Text = "";
		RadioGroupBDE->ItemIndex = 2;
		EditDataPath->Text = "";

		ComboBoxBDEalias->Enabled = false;
		EditDataPath->Enabled = false;
		ButtonSelectDataPath->Enabled = false;
	}
	else
	{
		ComboBoxBDEalias->Enabled = true;
		EditDataPath->Enabled = true;
		ButtonSelectDataPath->Enabled = true;
	}
}
//---------------------------------------------------------------------------

