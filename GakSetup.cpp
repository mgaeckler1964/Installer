/*
		Project:		MG Setup
		Module:			GakSetup.cpp
		Description:	
		Author:			Martin G�ckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#include <gak/string.h>
#include <gak/logfile.h>

#include "CollectorMain.h"

#pragma hdrstop
USERES("GakSetup.res");
USEFORM("CollectorMain.cpp", CollectorForm);
USEFORM("ProgFilesFrm.cpp", ProgramFilesForm);
USEFORM("IconFrm.cpp", IconForm);
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USEFORM("..\GAKLIB\Repository\DirSelFrm.cpp", DirectorySelector);
USEFORM("ErrorFrm.cpp", ErrorForm);
USEFORM("..\GAKLIB\Repository\Config.cpp", ConfigDataModule); /* TDataModule: File Type */
USEFORM("..\GAKLIB\Repository\StatusFrm.cpp", StatusForm);
USELIB("..\..\Object\winlib\winlibBCB.lib");
USEUNIT("common.cpp");
//---------------------------------------------------------------------------
using namespace gak;

//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR cmdLine, int)
{
	doEnableLogEx(gakLogging::llInfo);
	try
	{
		Application->Initialize();
		Application->Title = "MG Setup Collector";
		Application->CreateForm(__classid(TCollectorForm), &CollectorForm);
		Application->CreateForm(__classid(TProgramFilesForm), &ProgramFilesForm);
		Application->CreateForm(__classid(TIconForm), &IconForm);
		Application->CreateForm(__classid(TDirectorySelector), &DirectorySelector);
		Application->CreateForm(__classid(TErrorForm), &ErrorForm);
		Application->CreateForm(__classid(TConfigDataModule), &ConfigDataModule);
		Application->CreateForm(__classid(TStatusForm), &StatusForm);
		STRING	file = cmdLine;
		if( file.beginsWith( '\"' ) && file.endsWith( '\"' ) )
		{
			file = file.subString( 1, file.strlen() -2 );
		}
		if( !file.isEmpty() )
		{
			CollectorForm->openFile( file );
		}
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
