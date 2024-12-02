/*
		Project:		MG Setup
		Module:			common.cpp
		Description:	Common structures and functions
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



// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <vcl/registry.hpp>

#include <gak/xml.h>
#include <gak/directory.h>

#include "common.h"
#include "tags.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

using namespace gak;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

static STRING getApplicationVersion( const STRING &programFile )
{
	DWORD	dummy1, versionInfoSize;
	UINT	dummy2;
	char	tmpBuffer[128];
	STRING	versionString;

	versionInfoSize = GetFileVersionInfoSize(
		programFile, &dummy1
	);
	if( versionInfoSize )
	{
		void *data = malloc( versionInfoSize );
		if( data )
		{
			VS_FIXEDFILEINFO	*fixedFileInfo;

			GetFileVersionInfo( programFile, 0L, versionInfoSize, data );
			if( VerQueryValue( data, "\\", (void **)&fixedFileInfo, &dummy2 ) )
			{
				sprintf(
					tmpBuffer,
					"%u.%u.%u.%u",
					(unsigned int)(fixedFileInfo->dwFileVersionMS >> 16),
					(unsigned int)(fixedFileInfo->dwFileVersionMS & 0xFFFF),
					(unsigned int)(fixedFileInfo->dwFileVersionLS >> 16),
					(unsigned int)(fixedFileInfo->dwFileVersionLS & 0xFFFF)
				);
				versionString = tmpBuffer;
			}
			free( data );
		}
	}

	return versionString;
}

static inline void Add(
	InstallerFiles &installerFiles, 
	const STRING &group, const F_STRING &fileName 
)
{
	installerFiles[group].files.addElement( fileName );
}

static inline void SetDestination( 
	InstallerFiles &installerFiles, 
	const STRING &group, const STRING &destination 
)
{
	installerFiles[group].destination = destination;
}

static STRING readFiles(
	InstallerFiles &installerFiles,
	const gak::STRING &installerFileName, gak::xml::Element *sourceInfo,
	const gak::STRING &group
)
{
	STRING 			applicationVersion;
	xml::Element	*theFile;
	xml::XmlArray	theFiles;
	sourceInfo->getChildElements( &theFiles, FILE_TAG );
	for(
		theFiles.first();
		(theFile = theFiles.current()) != NULL;
		theFiles.next()
	)
	{
		F_STRING	sourceFile = theFile->getAttribute( SOURCE_TAG );
		if( !sourceFile.isEmpty() )
		{
			sourceFile = makeFullPath( installerFileName, sourceFile );
			if( applicationVersion.isEmpty() )
			{
				applicationVersion = getApplicationVersion( sourceFile );
			}
			Add( installerFiles, group, sourceFile );
		}
	}

	return applicationVersion;
}
// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

gak::STRING AppInfo::getCommonAppData()
{
	STRING commonAppData;

	std::auto_ptr<TRegistry>	theRegistry( new TRegistry );
	theRegistry->RootKey = HKEY_LOCAL_MACHINE;
	if(
		theRegistry->OpenKey(
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\Shell Folders",
			false
		)
	)
	{
		if( theRegistry->ValueExists( "Common AppData" ) )
		{
			commonAppData = theRegistry->ReadString( "Common AppData" ).c_str();
		}

		theRegistry->CloseKey();
	}

	return commonAppData;
}

gak::STRING AppInfo::getCommonStartMenu()
{
	STRING commonStartMenu;

	std::auto_ptr<TRegistry>	theRegistry( new TRegistry );
	theRegistry->RootKey = HKEY_LOCAL_MACHINE;
	if(
		theRegistry->OpenKey(
			"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\explorer\\Shell Folders",
			false
		)
	)
	{
		if( theRegistry->ValueExists( "Common Programs" ) )
		{
			commonStartMenu = theRegistry->ReadString( "Common Programs" ).c_str();
		}

		theRegistry->CloseKey();
	}

	return commonStartMenu;
}

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

gak::STRING AppInfo::getDefaultDestination( const gak::STRING &groupDestination ) const
{
	STRING	defaultDestination;

	if( groupDestination == VSTPRESETS )
	{
		defaultDestination = getCommonAppData();
		if( !defaultDestination.isEmpty() )
		{
			defaultDestination += DIRECTORY_DELIMITER_STRING "VST3 Presets";
		}
	}
	else
	{
		std::auto_ptr<TRegistry>	theRegistry( new TRegistry );
		theRegistry->RootKey = HKEY_LOCAL_MACHINE;
		if(
			theRegistry->OpenKey(
				"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
				false
			)
		)
		{
			if( groupDestination == PROGRAM32
			&& theRegistry->ValueExists( "ProgramFilesDir" ) )
			{
				defaultDestination = theRegistry->ReadString( "ProgramFilesDir" ).c_str();
			}
			else if( groupDestination == PROGRAM64
			&& theRegistry->ValueExists( "ProgramW6432Dir" ) )
			{
				defaultDestination = theRegistry->ReadString( "ProgramW6432Dir" ).c_str();
			}
			else if( groupDestination == VST32
			&& theRegistry->ValueExists( "CommonFilesDir" ) )
			{
				defaultDestination = theRegistry->ReadString( "CommonFilesDir" ).c_str();
				defaultDestination += DIRECTORY_DELIMITER_STRING "VST3";
			}
			else if( groupDestination == VST64
			&& theRegistry->ValueExists( "CommonW6432Dir" ) )
			{
				defaultDestination = theRegistry->ReadString( "CommonW6432Dir" ).c_str();
				defaultDestination += DIRECTORY_DELIMITER_STRING "VST3";
			}

			theRegistry->CloseKey();
		}
	}

	if( !defaultDestination.isEmpty() )
	{
		defaultDestination += DIRECTORY_DELIMITER_STRING + author + DIRECTORY_DELIMITER + title;
	}
	return defaultDestination;
}

gak::STRING AppInfo::getDefaultDBDestination() const
{
	STRING	defaultDestination = getCommonAppData();
	if( !defaultDestination.isEmpty() )
	{
		defaultDestination += DIRECTORY_DELIMITER_STRING + author + DIRECTORY_DELIMITER + title;
	}
	return defaultDestination;
}

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

STRING readInstallerFiles(
	InstallerFiles &installerFiles,
	xml::Element *sourceInfo, const gak::STRING &installerFileName
)
{
	/*
		legacy file entries
	*/
	STRING applicationVersion = readFiles(
		installerFiles, installerFileName, sourceInfo, PROGRAM_DIR
	);

	xml::XmlArray	theGroups;
	sourceInfo->getChildElements( &theGroups, GROUP_TAG );
	for(
		xml::XmlArray::const_iterator it = theGroups.cbegin(),
			endIT = theGroups.cend();
		it != endIT;
		++it
	)
	{
		xml::Element	*theGroup = *it;
		STRING	groupName = theGroup->getAttribute( NAME_ATTR );
		STRING	destination = theGroup->getAttribute( DESTINATION_ATTR );
		SetDestination( installerFiles, groupName, destination );
		STRING	tmpVersion = readFiles(
			installerFiles, installerFileName, theGroup, groupName
		);
		if( applicationVersion.isEmpty() )
		{
			applicationVersion = tmpVersion;
		}
	}

	return applicationVersion;
}

void saveInstallerFiles(
	const gak::STRING &installerFileName, xml::Element *setupElement,
	const InstallerFiles &files
)
{
	xml::Element *sourceInfo = setupElement->getElement( SOURCE_TAG );
	if( !sourceInfo )
	{
		sourceInfo = setupElement->addObject( new xml::Any( SOURCE_TAG ) );
	}
	if( sourceInfo )
	{
		while( sourceInfo->getNumObjects() )
		{
			delete sourceInfo->removeObject( std::size_t(0) );
		}

		for(
			InstallerFiles::const_iterator it = files.cbegin(),
				endIT = files.cend();
			it != endIT;
			++it
		)
		{
			const STRING		&group = it->getKey();
			xml::Element		*groupInfo = sourceInfo->addObject( new xml::Any( GROUP_TAG ) );
			const GroupFiles	&groupFiles = it->getValue().files;
			const STRING		&destination = it->getValue().destination;

			groupInfo->setStringAttribute( NAME_ATTR, group );
			groupInfo->setStringAttribute( DESTINATION_ATTR, destination );
			for(
				GroupFiles::const_iterator it = groupFiles.cbegin(),
					endIT = groupFiles.cend();
				it != endIT;
				++it
			)
			{
				const F_STRING	&sourceFile = *it;
				if( !sourceFile.isEmpty() )
				{
					xml::Element	*fileInfo = groupInfo->addObject(
						new xml::Any( FILE_TAG )
					);
					fileInfo->setStringAttribute(
						SOURCE_TAG, makeRelPath( installerFileName, sourceFile )
					);
				}
			}
		}
	}
}

std::size_t getNumFiles( const InstallerFiles &installerFiles )
{
	std::size_t	numFiles = 0;

	for(
		InstallerFiles::const_iterator it = installerFiles.cbegin(),
			endIT = installerFiles.cend();
		it != endIT;
		++it
	)
	{
		const GroupFiles	&groupFiles = it->getValue().files;

		numFiles += groupFiles.size();
	}

	return numFiles;
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

