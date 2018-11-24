#include "stdafx.h"
#include "simpletron.h"


simpletron::simpletron()
{
	for (adr = 0; adr < size; adr++) mem[adr] = 0;
	nxt = reg = cmd = adr = 0; tmp = 0; // varibales 
	ldst  = false; // access 
}

simpletron::~simpletron()
{
	for (adr = 0; adr < size; adr++) mem[adr] = 0;
	nxt = reg = cmd = adr = 0; tmp = 0; // variables 
	ldst = false; // access
}

int simpletron::getwrd(string str)
{
	// = = = = = = = = = > 15:05 11/8/2018 - by Yevhen Shugaev ;) 
	// getwrd(): ver 0.2 - extracting the correct word from the str:
	//	- check the empty str 
	//	- find  the wrd and check legnth  
	//	- check the wrd symbols
	//	- check the wrd commands
	// -----------------------------------------------------------------------
	// return 0;  - if the str is empty							 ( empty_str ) 
	// return -1; - if the str does not contain a correct wrd    ( error_wrd )
	// return 00~99; - if the wrd does not contain a correct cmd ( error_cmd ) 
	// return +-10000 <= wrd <= +-99999; - if struct its correct ( good_wrd  )
	// -----------------------------------------------------------------------
	// !!!!! To include new cmd, add 0xCMD_ID to the Tcmd[], and Tsz++; !!!!!

	// CHECK THE EMPTY STRING 
	if (str.length() <= 1) return 0;								 // empty_str 

	// init variables 
	char ltr[] = "+-0123456789"; // ltr allow
	int wrd_l = 0, wrd_l2 = 0;	 // wrd length
	int wrd_s = -1, wrd_e = -1;  // wrd post 

	// FIND THE WRD AND CHECK LENGTH 
	for (unsigned adr_w = 0; adr_w < str.length(); adr_w++) { // str space 
		wrd_l2 = wrd_l; // check ltr allow count
		// check current wrd[ltr]
		for (int l_adr = 0; l_adr < 13; l_adr++) // ltr space 
			if (str[adr_w] == ltr[l_adr]) { wrd_l++; break; } // search wrd 
		// check current find wrd 
		if (wrd_l == 6) { wrd_s = adr_w - 4; wrd_e = adr_w; break; }
		else if (wrd_l2 == wrd_l) wrd_l = 0; } // if wrd is to short 

	// if the correct wrd - is not found
	if (wrd_s == -1 && wrd_e == -1) return -1;						 // error_wrd

	// CHECK THE WRD SYMBOLS 
	int wrd = 0; // new wrd
	for (; wrd_s <= wrd_e; wrd_s++) { // space of wrd
		if (48 <= str[wrd_s] && str[wrd_s] <= 57) { // check ltr
			wrd  = (wrd * 10) + str[wrd_s] - '0';   // build wrd 
		} else return -1; }											 // error_wrd 

	// CHECK THE WRD COMMANDS
	int Tsz = 21; cmd = wrd / 1000; bool Tst = false; // <- !!! update Tsz count !!!
	const int Tcmd[] = { 0x00, 0x0A, 0x0B, 0x14, 0x15, 0x1E, 0x1F, 0x20, 0x21, 0x22, 
						 0x23, 0x24, 0x28, 0x29, 0x32, 0x2A, 0x2B, 0x32, 0x33, 0x34, 0x63 };

	for (int T = 0; T < Tsz; T++) // space of allowed cmd 
		if (cmd == Tcmd[T]) Tst = true;
	if (Tst == false) return cmd;									 // error_cmd 

	// PREPARATION OF THE WORD
	if (str[wrd_e - 5] == '-') return  wrd * -1; else return wrd;	 // good_wrd!!!
}

void simpletron::prog_file(string name)
{
	//system("cls");
	cout << "SIMPLETRON ~ PROG_FILE : \n\n";

	// show current file name
	cout << " ~$ name : " << name << endl; // show name 

	// check current file
	ifstream infile(name); ldst = infile.good(); // run access

	// !load_st: exiting
	if (!ldst) { cout << " ~$ error: path or name - is bad! \n\n"; return; }

	// else: loading file
	ifstream file(name); // open for read-only

	int wrd, line;	// storage current wrd
	string str;		// storage current str

	for (line = 0; line < size ; line++) { // file space

		getline(file, str);// take new string
		wrd = getwrd(str); // extraction str -> wrd

		if (wrd == 0) // if the str does not contain the correct wrd ->
		{ line--; continue; } 

		if (wrd == -1) // if the str contain the bad wrd ~ EXIT POINT ->
		{ cout << " ~$ error: str[ " << line << " ] - is bad! \n\n"; 
		ldst = false; return; }
		
		if (to_string(abs(wrd)).length() == 2) // if the wrd contain the bad cmd ~ EXIT POINT ->
		{ cout << " ~$ error: line[ " << line  << " ] cmd[ " << wrd << " ] - is bad! \n\n"; 
		ldst = false; return; }
	
		// /* writing a wrd into memory */
		if (wrd != +99999) mem[line] = wrd; // write to mem line
		else { mem[line] = 0; break; };  }  // else EOF

	// complid load
	cout << " ~$ prog : size[ " << line << " ] - wrd is loaded!\n\n";
	file.close(); // close file 
}

void simpletron::prog_load()
{
	//system("cls");
	cout << "SIMPLETRON ~ PROG_LOAD : \n\n";

	int wrd;	  // storage current wrd
	string str;   // storage current str
	ldst = false; // preset access status 

	for (int line = 0; line < size; line++) { // mem space 

		do { // read current str
			cout << ' ' << setfill('0') << setw(3) << line << "? "; // print msg
			getline(cin, str); wrd = getwrd(str);   // get && chek wrd
		} while (to_string(abs(wrd)).length() < 5); // while wrd is bad

		// processing of the correct wrd
		if (wrd != +99999) mem[line] = wrd; // writing a wrd into memory 
		else { mem[line] = 0; break; } /* end of prog */ } // end of write 

	ldst = true; // run access ok
	cout << endl; // offset
}

void simpletron::prog_run()
{
	//system("cls");
	cout << "SIMPLETRON ~ PROG_RUN  : \n\n";

	// access control 
	if (!ldst) { cout << " ~$ error: access is denied \n\n"; return; }

	string str;

	for (nxt = 0; nxt < size; nxt++) { // mem space 

		// LOAD
		reg = mem[nxt];		 // current word
		cmd = reg / 1000;    // command
		adr = reg % 1000;    // address
		
		// CHECK
		if (cmd < 0 || cmd > 99) { prog_dump(); break; }  // cheking cmd 
		if (adr < 0 || adr > 999) { prog_dump(); break; } // cheking adr

		// RUN
		switch (cmd) { // cmd space 

		case 0x00: break; // 00 - zero str
			
		case 0x0A: // 10 -  write: set value to adr
			do { cout << " ~$ ";
				if (cin.fail()) { // only int data
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
				cin >> mem[adr]; // write verified data
			} while (cin.fail() || mem[adr] < -32767 || 32767 < mem[adr]);
			break;

		// 11 - read: get value from adr
		case 0x0B: cout << " ~$ " << mem[adr]; break; 

		// 20 - copy: mem -> tmp
		case 0x14: tmp = mem[adr]; break;

		// 21 - past: tmp -> mem
		case 0x15: mem[adr] = tmp; break;

		// 30 - add : tmp + adr
		case 0x1E: tmp += mem[adr]; break;

		// 31 - sub : tmp - adr
		case 0x1F: tmp -= mem[adr]; break;

		case 0x20: // 32 - div : tmp / adr
			if (tmp != 0 && mem[adr] != 0) { tmp /= mem[adr]; }
			else { cout << " ~$ error: an exception !(tmp / 0) \n\n";
				prog_dump(); nxt = size; } break;

		// 33 - mul : tmp * adr
		case 0x21: tmp *= mem[adr]; break;

		// 34 - abs : |-1| = 1 && 1 = 1
		case 0x22: tmp = abs(tmp); break;

		// 35 - pow : tmp ^ 2
		case 0x23: tmp = pow(tmp, adr); break;

		// 40 - gt_adr : goto adr
		case 0x28: nxt = adr-1; break;

		// 41 - gt_neg : if( tmp < 0 )
		case 0x29: if (tmp < 0) { nxt = adr - 1; } break;

		// 42 - gt_zer : if( tmp == 0  )
		case 0x2A: if (tmp == 0) { nxt = adr - 1; } break;

		// 43 - gt_hig : if( tmp > 0   )
		case 0x2B: if (tmp > 0) { nxt = adr - 1; } break;

		// 50 - str: new str
		case 0x32: cout << endl; break;

		// 51 - str: write str to mem
		case 0x33: 
			cout << " ~$ "; getline(cin, str);
			for (int ltr = 0; ltr < str.length() && adr < size; adr++, ltr++)
				mem[adr] = (str.length() * 1000) + int(str[ltr]);
			break; 

		// 52 - str: read str from mem	
		case 0x34:
			cout << " ~$ ";
			for (int lng = mem[adr] / 1000; lng == mem[adr] / 1000; adr++)
				cout << char(mem[adr] % 1000); 
			break;

		// 99 - halt   : exit 
		case 0x63: nxt = size; break;

		default: // error code 
			cout << " ~$ error: unsupported cmd \n\n";
			prog_dump(); nxt = size; break;
		} // end cmd space 
	} // end mem space 
	cout << endl << endl; // offset
}

void simpletron::prog_dump()
{
	//system("cls");
	cout << "SIMPLETRON ~ PROG_DUMP : \n";

	cout << setfill('0') // show registers 
		<< "\n   nxt: " << setw(4) << nxt
		<< "\n   reg: " << setw(5) << reg
		<< "\n   cmd: " << setw(2) << cmd
		<< "\n   adr: " << setw(3) << adr
		<< "\n   tmp: " << setw(5) << tmp << endl;

	cout << "\n  ->  CMD ADR "; // show mem cells 
	for (adr = 0; adr < size; adr++) { // mem space

		reg = mem[adr]; /*<- load; check ->*/if (reg != 0) 
			cout << endl << ' ' << setw(3) << adr << ":  " // count
			<< setw(2) << reg / 1000 << ' '  /*cmd*/
			<< setw(3) << reg % 1000 << ' '; /*adr*/ }

	cout << endl << endl; // offset
}