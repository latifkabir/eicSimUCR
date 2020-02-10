// Filename: help.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Feb  9 20:53:28 2020 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include "TString.h"

using namespace std;


void help()
{
    string str;
    TString lineStr;
    str = (string)"src/";
	
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream myFile(str.c_str());
    if(!myFile)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\n\t\t=========================================================================" <<endl;

    cout << "\t\t|\t\t\t        Functions implemented       \t \t|" <<endl;
    cout << "\t\t=========================================================================" <<endl;
    while(getline(myFile,str))
    {
	//cout << str <<endl;
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	if(str == "#pragma link off all globals;" || str == "#pragma link off all classes;" || str == "#pragma link off all functions;")
	    continue;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }

    myFile.close();
}

