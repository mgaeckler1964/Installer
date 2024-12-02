/*
		Project:		MG Setup
		Module:			tags.h
		Description:	XML tags and attributes	
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

#ifndef SETUP_XML_TAGS_H
#define SETUP_XML_TAGS_H

// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

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

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static const gak::STRING SETUP_TAG="setup";
static const gak::STRING GROUP_TAG="group";
static const gak::STRING SOURCE_TAG="source";
static const gak::STRING FILE_TAG="file";
static const gak::STRING APPLICATION_TAG="application";
static const gak::STRING BDE_TAG="bde";
static const gak::STRING ICONS_TAG="icons";
static const gak::STRING ICON_TAG="icon";

static const gak::STRING UNINSTALL_TAG="uninstall";
static const gak::STRING FILES_TAG="files";
static const gak::STRING DIRECTORY_TAG="directory";

static const gak::STRING TITLE_ATTR="title";
static const gak::STRING AUTHOR_ATTR="author";
static const gak::STRING VERSION_ATTR="version";

static const gak::STRING DBVERSION_ATTR="dbVersion";
static const gak::STRING ALIASNAME_ATTR="AliasName";
static const gak::STRING DATAPATH_ATTR="DataPath";
static const gak::STRING GROUP_ATTR="Group";
static const gak::STRING TARGET_ATTR="target";
static const gak::STRING MAIN_ATTR="main";
static const gak::STRING DBUPD_ATTR="dbUpd";
static const gak::STRING NAME_ATTR="name";
static const gak::STRING DESTINATION_ATTR="destination";
static const gak::STRING PATH_ATTR="path";

static const gak::STRING RESOURCE_ATTR="RESOURCE";
static const gak::STRING BDE_ENGLISH="0009";
static const gak::STRING BDE_GERMAN="0007";
static const gak::STRING BDE_NONE="NONE";

static const gak::STRING PROGRAM_DIR="PROGRAM";
static const gak::STRING BDE_DIR="BDE";
static const gak::STRING DATA_DIR="DATA";
static const gak::STRING MENU_DIR="STARTMENU";

static const gak::STRING PROGRAM32="$ProgramFiles32";
static const gak::STRING PROGRAM64="$ProgramFiles64";
static const gak::STRING VST32="$VstPlugins32Bit";
static const gak::STRING VST64="$VstPlugins64Bit";
static const gak::STRING VSTPRESETS="$VstPresets";

static const gak::STRING BDE_REGISTRY_KEY="SOFTWARE\\BORLAND\\Database Engine";
static const gak::STRING BDE_USECOUNT_KEY="UseCount";

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

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

#endif	// SETUP_XML_TAGS_H
