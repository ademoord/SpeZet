//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmMain *fmMain;

TDateTime myStart, myElapsed;

//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
	: TForm(Owner)
{
    // read system parameters
	TIniFile *ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
	dirHome = ini->ReadString("System", "Home", "");
	dirEmpl = ini->ReadString("System", "Empl", "");

	myHost = ini->ReadString("MySQL", "Host", "");
	myUser = ini->ReadString("MySQL", "User", "");
	myPassword = ini->ReadString("MySQL", "Password", "");
	myDatabase = ini->ReadString("MySQL", "Database", "");

	delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormShow(TObject *Sender)
{
int	i;

	mmForm->Lines->Add(dirHome);
	mmForm->Lines->Add(dirEmpl);

	// insert the host name
	i = dbMySQL->Params->IndexOfName("HostName");
	if (i > -1)
		dbMySQL->Params->Values[dbMySQL->Params->Names[i]] = myHost;
	else
		dbMySQL->Params->Add("HostName=" + myHost);

	// insert the user name
	i = dbMySQL->Params->IndexOfName("User_Name");
	if (i > -1)
		dbMySQL->Params->Values[dbMySQL->Params->Names[i]] = myUser;
	else
		dbMySQL->Params->Add("User_Name=" + myUser);

	// insert the password
	i = dbMySQL->Params->IndexOfName("Password");
	if (i > -1)
		dbMySQL->Params->Values[dbMySQL->Params->Names[i]] = myPassword;
	else
		dbMySQL->Params->Add("Password=" + myPassword);

	// insert the database name
	i = dbMySQL->Params->IndexOfName("Database");
	if (i > -1)
		dbMySQL->Params->Values[dbMySQL->Params->Names[i]] = myDatabase;
	else
		dbMySQL->Params->Add("Database=" + myDatabase);

	mmForm->Lines->Add(dbMySQL->Params->Text);

	dbMySQL->LoginPrompt = False;		// no login prompt
	dbMySQL->Open();

	mmForm->Lines->Add("Database server connected");

	// prepare SQLDataSet
	qryMySQL->SQLConnection = dbMySQL;
	qryMySQL->CommandType = ctQuery;
	qryMySQL->Close();

	qryLookup->SQLConnection = dbMySQL;
	qryLookup->CommandType = ctQuery;
	qryLookup->Close();

	// prepare paradox database
	qryPdox->DatabaseName = dirHome;

	// set the date conversion format
	FormatSettings.DateSeparator = '-';
	FormatSettings.ShortDateFormat = "yyyy/m/d";
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	dbMySQL->Close();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::buJobClick(TObject *Sender)
{
AnsiString	s, v;						// variable for SQL command

	myStart = Now();                    // get start time

	// empty the tables tbJob and cnvJob
	qryMySQL->CommandText = "DELETE FROM tbJob;";
	qryMySQL->ExecSQL();
	qryMySQL->CommandText = "DELETE FROM cnvJob;";
	qryMySQL->ExecSQL();

	// prepare paradox table mjob.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mjob WHERE (IDJob IS NOT NULL);");

	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT into tbJob (";
			v = " VALUE (";

			if (!qryPdox->FieldByName("NrJob")->IsNull) {
				s += "shortJob";
				v += "'" + qryPdox->FieldByName("NrJob")->AsString + "'";
			}

			if (!qryPdox->FieldByName("Job")->IsNull) {
				s += ", Job";
				v += ", '" + qryPdox->FieldByName("Job")->AsString + "'";
			}

			if (!qryPdox->FieldByName("Mode")->IsNull) {
				s += ", Mode";
				v += ", '" + qryPdox->FieldByName("Mode")->AsString + "'";
			}

			s += ")" + v + ");";
			mmForm->Lines->Add(s);

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			// get the last idJob
			qryMySQL->CommandText = "SELECT idJob FROM tbJob ORDER BY idJob DESC;";
			qryMySQL->Open();       // IMPORTANT: use Open() instead of ExecSQL()
										//            to get return values back
			s = "INSERT into cnvJob (idJobOld, idJobNew) VALUE ("
				+ qryPdox->FieldByName("IDJob")->AsString
				+ ", " + qryMySQL->Fields->Fields[0]->AsString + ");";
			qryMySQL->Close();

			// insert the old and new idJob into the cnvJob
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	myElapsed = Now() - myStart;        // show the elapsed time
	mmForm->Lines->Add("Elapsed time = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------


void __fastcall TfmMain::buEmployeeClick(TObject *Sender)
{
AnsiString		s, v;					// variable for SQL command

	myStart = Now();

	// empty the tables tbEmployee and cnvEmployee
	qryMySQL->CommandText = "DELETE FROM tbEmployee;";
	qryMySQL->ExecSQL();
	qryMySQL->CommandText = "DELETE FROM cnvEmployee;";
	qryMySQL->ExecSQL();

	// prepare paradox table mempl.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mempl WHERE (IDJob IS NOT NULL);");

	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {		// scan through the whole record

			// SQL preambles
			s = "INSERT into tbEmployee (";
			v = " VALUE (";

			if (!qryPdox->FieldByName("ShortName")->IsNull) {
				s += "shortName";
				v += "'" + qryPdox->FieldByName("ShortName")->AsString + "'";
			}

			if (!qryPdox->FieldByName("Name")->IsNull) {
				s += ", Name";
				v += ", '" + qryPdox->FieldByName("Name")->AsString + "'";
			}

			if (!qryPdox->FieldByName("DBegin")->IsNull) {
				s += ", dtBegin";
				v += ", '" + DateToStr(qryPdox->FieldByName("DBegin")->AsDateTime) + "'";
			}

			if (!qryPdox->FieldByName("DEnd")->IsNull) {
				s += ", dtEnd";
				v += ", '" + DateToStr(qryPdox->FieldByName("DEnd")->AsDateTime) + "'";
			}

			if (!qryPdox->FieldByName("Mode")->IsNull) {
				s += ", Mode";
				v += ", '" + qryPdox->FieldByName("Mode")->AsString + "'";
			}

			if (!qryPdox->FieldByName("PartTime")->IsNull) {
				s += ", PartTime";
				v += ", " + IntToStr(qryPdox->FieldByName("PartTime")->AsInteger);
			}

			if (!qryPdox->FieldByName("IDJob")->IsNull) {
				s += ", idJob";

				// get the new idJob
				qryMySQL->CommandText = "SELECT idJobNew FROM cnvJob WHERE idJobOld = "
						+ qryPdox->FieldByName("IDJob")->AsString + ";";
				qryMySQL->Open();
				v += ", " + qryMySQL->Fields->Fields[0]->AsString;
				qryMySQL->Close();
			}

			s += ")" + v + ");";
			mmForm->Lines->Add(s);

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			// get the last idEmployee
			qryMySQL->CommandText = "SELECT idEmployee FROM tbEmployee ORDER BY idEmployee DESC;";
			qryMySQL->Open();       // IMPORTANT: use Open() instead of ExecSQL()

			// insert the old and new idEmployee into the cnvEmployee
			s = "INSERT into cnvEmployee (idEmplOld, idEmplNew) VALUE ("
				+ qryPdox->FieldByName("IDEmployee")->AsString + ", "
				+ qryMySQL->Fields->Fields[0]->AsString + ");";
			qryMySQL->Close();
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}


	// empty the tables tbEmployeeJob
	qryMySQL->CommandText = "DELETE FROM tbEmployeeJob;";
	qryMySQL->ExecSQL();

	// prepare paradox table mejob.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mejob WHERE (IDEmployee IS NOT NULL);");

	// Run set the idOrder for the first run
	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT into tbEmployeeJob (idEmployee, idJob) VALUE (";

			// get the converted idEmployee
			qryMySQL->CommandText = "SELECT idEmplNew FROM cnvEmployee WHERE idEmplOld = "
						+ qryPdox->FieldByName("IDEmployee")->AsString + ";";
			qryMySQL->Open();
			if (!qryMySQL->Fields->Fields[0]->IsNull) {
				s += qryMySQL->Fields->Fields[0]->AsString + ", ";
				qryMySQL->Close();

				// get the converted idJob
				qryMySQL->CommandText = "SELECT idJobNew FROM cnvJob WHERE idJobOld = "
							+ qryPdox->FieldByName("IDJob")->AsString + ";";
				qryMySQL->Open();
				if (!qryMySQL->Fields->Fields[0]->IsNull) {
					s += qryMySQL->Fields->Fields[0]->AsString + ");";
					qryMySQL->Close();

					// execute the SQL command
					qryMySQL->CommandText = s;
					qryMySQL->ExecSQL();

					mmForm->Lines->Add(s);
				}
			}
			qryMySQL->Close();

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	myElapsed = Now() - myStart;
	mmForm->Lines->Add("Elapsed time  = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------



void __fastcall TfmMain::buOwnerClick(TObject *Sender)
{
AnsiString	s, v;				// variable for SQL command

	myStart = Now();

	// empty the tables tbOwner and cnvOwner
	qryMySQL->CommandText = "DELETE FROM tbOwner;";
	qryMySQL->ExecSQL();
	qryMySQL->CommandText = "DELETE FROM cnvOwner;";
	qryMySQL->ExecSQL();

	// prepare paradox table mownr.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mownr;");

	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT into tbOwner (";
			v = " VALUE (";

			if (!qryPdox->FieldByName("Company")->IsNull) {
				s += "Company";
				v += "\"" + qryPdox->FieldByName("Company")->AsString + "\"";
			}
			else {						// workaround for first entry is NULL
				s += "Company";
				v += "\" \"";
            }

			if (!qryPdox->FieldByName("Attention")->IsNull) {
				s += ", Attention";
				v += ", \"" + qryPdox->FieldByName("Attention")->AsString + "\"";
			}

			if (!qryPdox->FieldByName("Address")->IsNull) {
				s += ", Address";
				v += ", \"" + qryPdox->FieldByName("Address")->AsString + "\"";
			}

			if (!qryPdox->FieldByName("Postcode")->IsNull) {
				s += ", Postcode";
				v += ", \"" + qryPdox->FieldByName("Postcode")->AsString + "\"";
			}

			if (!qryPdox->FieldByName("City")->IsNull) {
				s += ", City";
				v += ", \"" + qryPdox->FieldByName("City")->AsString + "\"";
			}

			if (!qryPdox->FieldByName("Mode")->IsNull) {
				s += ", Mode";
				v += ", \"" + qryPdox->FieldByName("Mode")->AsString + "\"";
			}

			s += ")" + v + ");";
			mmForm->Lines->Add(s);

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			// get the last idOwner
			qryMySQL->CommandText = "SELECT idOwner FROM tbOwner ORDER BY idOwner DESC;";
			qryMySQL->Open();

			// insert the old and new idEmployee into the cnvEmployee
			s = "INSERT into cnvOwner (idOwnOld, idOwnNew) VALUE ("
				+ qryPdox->FieldByName("IDClient")->AsString + ", "
				+ qryMySQL->Fields->Fields[0]->AsString + ");";
			qryMySQL->Close();

			// insert the old and new idJob into the cnvJob
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	myElapsed = Now() - myStart;
	mmForm->Lines->Add("Elapsed time = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------


void __fastcall TfmMain::buOrderClick(TObject *Sender)
{
AnsiString	s, v;						// variable for SQL command

	myStart = Now();

	// empty the tables tbOrder and cnvOrder
	qryMySQL->CommandText = "DELETE FROM tbOrder;";
	qryMySQL->ExecSQL();
	qryMySQL->CommandText = "DELETE FROM cnvOrder;";
	qryMySQL->ExecSQL();

	// prepare paradox table morder.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM morder WHERE Mode IS NOT NULL;");

	// Run set the idOrder for the first run
	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT INTO tbOrder (";
			v = " VALUE (";

			s += "Mode";
			v += "'" + qryPdox->FieldByName("Mode")->AsString + "'";

			if (!qryPdox->FieldByName("NrOrder")->IsNull) {
				s += ", shortOrder";
				v += ", '" + qryPdox->FieldByName("NrOrder")->AsString + "'";
			}

			if (!qryPdox->FieldByName("OrderDesc")->IsNull) {
				s += ", Description";
				v += ", '" + qryPdox->FieldByName("OrderDesc")->AsString + "'";
			}

			if (!qryPdox->FieldByName("IDOwner")->IsNull) {
				// get the last idOwner
				qryMySQL->CommandText = "SELECT idOwnNew FROM cnvOwner WHERE idOwnOld = "
						+ qryPdox->FieldByName("IDOwner")->AsString + ";";
				qryMySQL->Open();
				if (!qryMySQL->Fields->Fields[0]->IsNull) {
					s += ", idOwner";
					v += ", " + qryMySQL->Fields->Fields[0]->AsString;
				}
				qryMySQL->Close();
			}

			if (!qryPdox->FieldByName("Budget")->IsNull) {
				s += ", Budget";
				v += ", " + qryPdox->FieldByName("Budget")->AsString;
			}

			if (!qryPdox->FieldByName("DBegin")->IsNull) {
				s += ", dtBegin";
				v += ", '" + DateToStr(qryPdox->FieldByName("DBegin")->AsDateTime) + "'";
			}

			if (!qryPdox->FieldByName("DEnd")->IsNull) {
				s += ", dtEnd";
				v += ", '" + DateToStr(qryPdox->FieldByName("DEnd")->AsDateTime) + "'";
			}

			if (!qryPdox->FieldByName("ArtCost")->IsNull) {
				s += ", artCost";
				v += ", '" + qryPdox->FieldByName("ArtCost")->AsString + "'";
			}

			if (!qryPdox->FieldByName("BaseRate")->IsNull) {
				s += ", baseRate";
				v += ", " + qryPdox->FieldByName("BaseRate")->AsString;
			}

			if (!qryPdox->FieldByName("BaseItem")->IsNull) {
				s += ", baseItem";
				v += ", " + qryPdox->FieldByName("BaseItem")->AsString;
			}

			s += ")" + v + ");";

			mmForm->Lines->Add(s);

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			// get the last idOrder
			qryMySQL->CommandText = "SELECT idOrder FROM tbOrder ORDER BY idOrder DESC;";
			qryMySQL->Open();
			int 		idNew = qryMySQL->Fields->Fields[0]->AsInteger;
			qryMySQL->Close();

			int 		idOld = qryPdox->FieldByName("IDOrder")->AsInteger;

			mmForm->Lines->Add(s);

			// insert the old and new idJob into the cnvJob
			s.sprintf("INSERT into cnvOrder (idOrderOld, idOrderNew) VALUE (%d, %d);",
				idOld, idNew);
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	// Update the baseRate for the second run
	qryMySQL->CommandText = "SELECT * FROM tbOrder;";
	qryMySQL->Open();					// open the data base
	try {
		qryMySQL->First();				// goto the first record
		while (!qryMySQL->Eof) {			// scan through the whole record

			AnsiString idOrder = qryMySQL->FieldByName("idOrder")->AsString;

			// replace the baseRate with the new one from cnvOrder
			if (!qryMySQL->FieldByName("baseRate")->IsNull) {

				// get the new idOrder
				qryLookup->CommandText = "SELECT idOrderNew FROM cnvOrder WHERE idOrderOld = "
					+ qryMySQL->FieldByName("baseRate")->AsString + ";";
				qryLookup->Open();

				// check if there is any entry for this idOrder
				if (qryLookup->Fields->Fields[0]->IsNull) {
					qryLookup->Close();
					qryLookup->CommandText = "UPDATE tbOrder SET baseRate = NULL WHERE idOrder = "
						+ qryMySQL->FieldByName("idOrder")->AsString
						+ ";";
				}
				else {
					int	newRate = qryLookup->Fields->Fields[0]->AsInteger;
					qryLookup->Close();

					// substitute the old baseRate with the new one
					qryLookup->CommandText = "UPDATE tbOrder SET baseRate = "
						+ IntToStr(newRate) + " WHERE idOrder = "
						+ qryMySQL->FieldByName("idOrder")->AsString
						+ ";";
				}

				qryLookup->ExecSQL();

				mmForm->Lines->Add(idOrder);

				// make sure that the query closed
				qryLookup->Close();
			}

			qryMySQL->Next();			// next record
		}
	}
	__finally {
		qryMySQL->Close();				// always close database
	}

	// Update the baseItem for the second run
	qryMySQL->CommandText = "SELECT * FROM tbOrder;";
	qryMySQL->Open();					// open the data base
	try {
		qryMySQL->First();				// goto the first record
		while (!qryMySQL->Eof) {			// scan through the whole record

			AnsiString idOrder = qryMySQL->FieldByName("idOrder")->AsString;

			// replace the baseItem with the new one from cnvOrder
			if (!qryMySQL->FieldByName("baseItem")->IsNull) {

				// get the new idOrder
				qryLookup->CommandText = "SELECT idOrderNew FROM cnvOrder WHERE idOrderOld = "
					+ qryMySQL->FieldByName("baseItem")->AsString + ";";
				qryLookup->Open();

				// check if there is any entry for this idOrder
				if (qryLookup->Fields->Fields[0]->IsNull) {
					qryLookup->Close();
					qryLookup->CommandText = "UPDATE tbOrder SET baseItem = NULL WHERE idOrder = "
						+ qryMySQL->FieldByName("idOrder")->AsString
						+ ";";
				}
				else {
					int	newRate = qryLookup->Fields->Fields[0]->AsInteger;
					qryLookup->Close();

					// substitute the old baseRate with the new one
					qryLookup->CommandText = "UPDATE tbOrder SET baseItem = "
						+ IntToStr(newRate) + " WHERE idOrder = "
						+ qryMySQL->FieldByName("idOrder")->AsString
						+ ";";
				}

				qryLookup->ExecSQL();
				mmForm->Lines->Add(idOrder);

				// make sure that the query closed
				qryLookup->Close();
			}

			qryMySQL->Next();			// next record
		}
	}
	__finally {
		qryMySQL->Close();				// always close database
	}

	myElapsed = Now() - myStart;
	mmForm->Lines->Add("Elapsed time = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------


void __fastcall TfmMain::buRateClick(TObject *Sender)
{
AnsiString	s, v;						// variable for SQL command

	myStart = Now();

	// empty the tables tbRate
	qryMySQL->CommandText = "DELETE FROM tbRate;";
	qryMySQL->ExecSQL();

	// prepare paradox table mrate.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mrate "
			"WHERE (IDOrder IN (SELECT IDOrder FROM morder)) "
			"AND (IDJob IN (SELECT IDJob FROM mjob)) "
			"AND (DBegin IS NOT NULL);");

	// Run set the idOrder for the first run
	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT into tbRate (";
			v = " VALUE (";

			if ( (!qryPdox->FieldByName("IDOrder")->IsNull) &&
				 (!qryPdox->FieldByName("IDJob")->IsNull) &&
				 (!qryPdox->FieldByName("DBegin")->IsNull) )
			{
				// get the last idOrder
				s += "idOrder";
				qryMySQL->CommandText = "SELECT idOrderNew FROM cnvOrder WHERE idOrderOld = "
						+ qryPdox->FieldByName("IDOrder")->AsString + ";";
				qryMySQL->Open();
				v += qryMySQL->Fields->Fields[0]->AsString;
				qryMySQL->Close();

				// get the last idJob
				s += ", idJob";
				qryMySQL->CommandText = "SELECT idJobNew FROM cnvJob WHERE idJobOld = "
						+ qryPdox->FieldByName("IDJob")->AsString + ";";
				qryMySQL->Open();
				v += ", " + qryMySQL->Fields->Fields[0]->AsString;
				qryMySQL->Close();

				// get the date begin
				s += ", dtBegin";
				v += ", \"" + DateToStr(qryPdox->FieldByName("DBegin")->AsDateTime) + "\"";

				if (!qryPdox->FieldByName("DEnd")->IsNull) {
					s += ", dtEnd";
					v += ", \"" + DateToStr(qryPdox->FieldByName("DEnd")->AsDateTime) + "\"";
				}

				if (!qryPdox->FieldByName("Rate")->IsNull) {
					s += ", Rate";
					v += ", " + qryPdox->FieldByName("Rate")->AsString;
				}
			}
			s += ")" + v + ");";

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	myElapsed = Now() - myStart;
	mmForm->Lines->Add("Elapsed time = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------



void __fastcall TfmMain::buEmployeeJobClick(TObject *Sender)
{
AnsiString	s, v;						// variable for SQL command

	myStart = Now();

	// empty the tables tbRate
	qryMySQL->CommandText = "DELETE FROM tbEmployeeJob;";
	qryMySQL->ExecSQL();

	// prepare paradox table mejob.DB
	qryPdox->SQL->Clear();
	qryPdox->SQL->Add("SELECT * FROM mejob;");

	// Run set the idOrder for the first run
	// access the paradox file
	qryPdox->Open();					// open the data base
	try {
		qryPdox->First();				// goto the first record
		while (!qryPdox->Eof) {			// scan through the whole record

			// SQL preambles
			s = "INSERT into tbEmployeeJob (";
			v = " VALUE (";

 			if ( (!qryPdox->FieldByName("IDEmployee")->IsNull) &&
				 (!qryPdox->FieldByName("IDJob")->IsNull) )
			{
				// get the last idEmployee
				s += "idEmployee";
				qryMySQL->CommandText = "SELECT idEmplNew FROM cnvEmployee WHERE idEmplOld = "
						+ qryPdox->FieldByName("IDEmployee")->AsString + ";";
				qryMySQL->Open();
				if (!qryMySQL->Fields->Fields[0]->IsNull) {
					v += qryMySQL->Fields->Fields[0]->AsString;
				}
				else {
					v += "0";
				}
//				v += ", " + qryMySQL->Fields->Fields[0]->AsString;
				qryMySQL->Close();

				// get the last idJob
				s += ", idJob";
				qryMySQL->CommandText = "SELECT idJobNew FROM cnvJob WHERE idJobOld = "
						+ qryPdox->FieldByName("IDJob")->AsString + ";";
				qryMySQL->Open();
				v += ", " + qryMySQL->Fields->Fields[0]->AsString;
				qryMySQL->Close();
			}

			s += ")" + v + ");";

			// execute the SQL command
			qryMySQL->CommandText = s;
			qryMySQL->ExecSQL();

			mmForm->Lines->Add(s);

			qryPdox->Next();			// next record
		}
	}
	__finally {
		qryPdox->Close();				// always close database
	}

	myElapsed = Now() - myStart;
	mmForm->Lines->Add("Elapsed time = " + myElapsed.TimeString());
}
//---------------------------------------------------------------------------

