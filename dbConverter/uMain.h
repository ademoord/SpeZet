//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <IniFiles.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <DBClient.hpp>
#include <DBGrids.hpp>
#include <DBXMySQL.hpp>
#include <FMTBcd.hpp>
#include <Grids.hpp>
#include <Provider.hpp>
#include <SqlExpr.hpp>
//---------------------------------------------------------------------------
class TfmMain : public TForm
{
__published:	// IDE-managed Components
	TMemo *mmForm;
	TButton *buJob;
	TSQLConnection *dbMySQL;
	TSQLDataSet *qryMySQL;
	TButton *buEmployee;
	TButton *buOwner;
	TButton *buOrder;
	TQuery *qryPdox;
	TSQLDataSet *qryLookup;
	TButton *buRate;
	TButton *buEmployeeJob;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall buJobClick(TObject *Sender);
	void __fastcall buEmployeeClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall buOwnerClick(TObject *Sender);
	void __fastcall buOrderClick(TObject *Sender);
	void __fastcall buRateClick(TObject *Sender);
	void __fastcall buEmployeeJobClick(TObject *Sender);
private:	// User declarations
	AnsiString dirEmpl, dirHome;
	AnsiString myHost, myUser, myPassword, myDatabase;

public:		// User declarations
	__fastcall TfmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
