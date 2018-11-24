#pragma once
#include <string>
using std::string;

class simpletron
{
public:
	simpletron();
	~simpletron();
	int getwrd(string);		// returns the correct wrd or -1
	void prog_file(string);	// input prog from file
	void prog_load();		// input prog from keyboard
	void prog_run();		// run load prog
	void prog_dump();		// show memory dump
private:
	bool ldst;          // access control
	int nxt;            // next inst
	int reg;            // load inst 
	int cmd;            // load command 
	int adr;            // load address 
	int tmp;		    // temp buffer 
	int size = 1000;    // size of memory
	int mem[1000];      // memory space 
};

