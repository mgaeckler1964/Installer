/*
		Project:		MG Setup
		Module:			IconFrm.cpp
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

#include "IconFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIconForm *IconForm;
//---------------------------------------------------------------------------
__fastcall TIconForm::TIconForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TIconForm::ButtonCloseClick(TObject *)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TIconForm::FormResize(TObject *)
{
	int colWidth = (StringGridIcons->Width - 85) / 2;

	StringGridIcons->ColWidths[0] = colWidth;
	StringGridIcons->ColWidths[1] = colWidth;
	StringGridIcons->ColWidths[2] = 30;
	StringGridIcons->ColWidths[3] = 30;

}
//---------------------------------------------------------------------------
void __fastcall TIconForm::FormShow(TObject *)
{
	StringGridIcons->Cells[0][0] = "Title";
	StringGridIcons->Cells[1][0] = "Target";
	StringGridIcons->Cells[2][0] = "Main";
	StringGridIcons->Cells[3][0] = "DBUpd";
}
//---------------------------------------------------------------------------

void __fastcall TIconForm::StringGridIconsMouseDown(TObject *,
	  TMouseButton , TShiftState , int X, int Y)
{
	int Column, Row;
	StringGridIcons->MouseToCell(X, Y, Column, Row);
	if( Column == 2 && Row >= 1 )
	{
		if( StringGridIcons->Cells[2][Row] == "true" )
			StringGridIcons->Cells[2][Row] = "";
		else if( StringGridIcons->Cells[0][Row] != ""
		&& StringGridIcons->Cells[1][Row] != "" )
		{
			for( int i=1; i<StringGridIcons->RowCount; i++ )
			{
				StringGridIcons->Cells[2][i] = "";
			}
			StringGridIcons->Cells[2][Row] = "true";
		}
	}
	else if( Column == 3 && Row >= 1 )
	{
		if( StringGridIcons->Cells[3][Row] == "true" )
			StringGridIcons->Cells[3][Row] = "";
		else if( StringGridIcons->Cells[0][Row] != ""
		&& StringGridIcons->Cells[1][Row] != "" )
		{
			for( int i=1; i<StringGridIcons->RowCount; i++ )
			{
				StringGridIcons->Cells[3][i] = "";
			}
			StringGridIcons->Cells[3][Row] = "true";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TIconForm::StringGridIconsSetEditText(TObject *,
	  int ACol, int ARow, const AnsiString Value)
{
	if( ACol == 2 || ACol == 3 )
	{
		if( Value > "" )
		{
			for( int i=1; i<StringGridIcons->RowCount; i++ )
			{
				StringGridIcons->Cells[ACol][i] = "";
			}
			StringGridIcons->Cells[ACol][ARow] = "true";
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TIconForm::ButtonRemoveClick(TObject *)
{
	if( StringGridIcons->RowCount > 2 )
	{
		for( int i=StringGridIcons->Row; i<StringGridIcons->RowCount-1; i++ )
		{
			StringGridIcons->Cells[0][i] = StringGridIcons->Cells[0][i+1];
			StringGridIcons->Cells[1][i] = StringGridIcons->Cells[1][i+1];
			StringGridIcons->Cells[2][i] = StringGridIcons->Cells[2][i+1];
			StringGridIcons->Cells[3][i] = StringGridIcons->Cells[3][i+1];
		}
		StringGridIcons->RowCount = StringGridIcons->RowCount-1;
	}
	else
	{
		StringGridIcons->Cells[0][2] = "";
		StringGridIcons->Cells[1][2] = "";
		StringGridIcons->Cells[2][2] = "";
		StringGridIcons->Cells[3][2] = "";
	}
}
//---------------------------------------------------------------------------
void __fastcall TIconForm::ButtonAddClick(TObject *)
{
	StringGridIcons->RowCount = StringGridIcons->RowCount+1;
}
//---------------------------------------------------------------------------
