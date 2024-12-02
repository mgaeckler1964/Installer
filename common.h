/*
		Project:		Installer
		Module:			common.h
		Description:	Common structures and functions
		Author:			Martin Gäckler
		Address:		Kirchenstr. 26, D-81675 München
		Web:			http://www.gäckler.de/

		Copyright:		(c) 2011-2018 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
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

#ifndef INSTALLER_COMMON_H
#define INSTALLER_COMMON_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <gak/Array.h>
#include <gak/Map.h>
#include <gak/xml>

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

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

typedef gak::Array<gak::F_STRING>				GroupFiles;
struct GroupInfo
{
	gak::STRING	destination;
	GroupFiles  files;
};
typedef gak::PairMap<gak::STRING, GroupInfo>	InstallerFiles;

struct AppInfo
{
	gak::STRING	title, author, version;

	AppInfo( gak::xml::Element *appInfo )
	{
		title = appInfo->getAttribute( TITLE_ATTR );
		author = appInfo->getAttribute( AUTHOR_ATTR );
		version = appInfo->getAttribute( VERSION_ATTR );
	}
	AppInfo( const AnsiString &title, const AnsiString &author, const AnsiString &version )
	{
		this->title = title.c_str();
		this->author = author.c_str();
		this->version = version.c_str();
	}

	gak::STRING getRegistry() const
	{
		return "SOFTWARE\\" + author + '\\' + title;
	}

	static gak::STRING getCommonAppData();
	static gak::STRING getCommonStartMenu();
	gak::STRING getDefaultDestination( const gak::STRING &groupDestination ) const;
	gak::STRING getDefaultDBDestination() const;
	gak::STRING getDefaultMenuDestination() const;

	gak::STRING getApplicationName() const
	{
		return title + ' ' + version;
	}
	void setAttributes( gak::xml::Element *appInfo ) const
	{
		appInfo->setStringAttribute( TITLE_ATTR, title );
		appInfo->setStringAttribute( AUTHOR_ATTR, author );
		appInfo->setStringAttribute( VERSION_ATTR, version );
	}
};

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

gak::STRING readInstallerFiles(
	InstallerFiles &installerFiles,
	gak::xml::Element *sourceInfo, const gak::STRING &installerFileName
);

void saveInstallerFiles(
	const gak::STRING &installerFileName, gak::xml::Element *setupElement,
	const InstallerFiles &files
);

std::size_t getNumFiles( const InstallerFiles &installerFiles );

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

#endif	// INSTALLER_COMMON_H
