// Author : Anil Kumar Saini
// Console App for File management

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

// Function declaration
void menu();
void copyFile();
void moveFile();
void deleteFile();
void appendFile();
string getFileLocation();
int operationCopy(string FunctionalityType);

// Global declarations used for files
HANDLE hFile;
HANDLE hAppend;
DWORD  dwBytesRead, dwBytesWritten, dwPos;
BYTE   buff[4096];

// Entry point of application
void main()
{
	// Welcome message
	cout << "Welcome to File Management System" << endl << endl;	
	
	// Displaying menu for file operation
	menu();	

	cout << "Thanks for using this application. ";	
}

// Funtion defined to show File operation functions to user.
void menu()
{
	// Taking input from user to perform selected operation.
	int choice = 0;
	cout << endl << endl;
	cout << "_________________________________" << endl;
	cout << "Menu : " << endl;
	cout << "_________________________________" << endl;
	cout << "1 - Copy File" << endl;
	cout << "2 - Move File" << endl;
	cout << "3 - Append File" << endl;
	cout << "4 - Delete File" << endl;
	cout << "5 - Exit" << endl << endl;
	cout << "Please input your choice from above menu" << endl << "Your Input :";
	cin >> choice;

	switch (choice)
	{
		case 1:
			copyFile();
			menu();
			break;

		case 2:
			moveFile();
			menu();
			break;

		case 3:
			appendFile();
			menu();
			break;

		case 4:
			deleteFile();
			menu();
			break;

		case 5:
			return;

		default:
			cout << "Invalid input" << endl;
			menu();
			break;
	}
}

// Function defination for Copy file operation
void copyFile()
{
	int result = operationCopy("copy");
	
	if(result)
		cout << "File copied successfully" << endl;
}

// Function defination for Delete file operation
void deleteFile()
{
	cout << "Please enter file name with location and it's extention to perform delete operation." << endl << "For example : C:\\\\foldername\\filename.cpp" << endl;	
	if (remove(getFileLocation().c_str()) != 0)
		cout << "Error deleting file";
	else
		cout << "File successfully deleted";
}

// Function defination for Append file operation
void appendFile()
{
	operationCopy("append");
	cout << "File appended successfully" << endl;
}

// Function defination for Move file operation
void moveFile()
{
	cout << "Enter old file location. Example : C:\\\\foldername\\filename.cpp" << endl;
	string OldFileName = getFileLocation();
	cout << "Enter new file location. Example : C:\\\\foldername\\filename.cpp" << endl;
	string NewFileName = getFileLocation();

	int result;
	result = MoveFileA(OldFileName.c_str(), NewFileName.c_str());	
	if (result == 0)
		cout << "Error moving file" << endl;
	else
		cout << "File moved successfully" << endl;
}

// Common function defination used for Copy and append operation
// Param Used : FunctionalityType
// Param Type : String
// Param Purpose : Passing which type of operation called this function
int operationCopy(string FunctionalityType)
{
	cout << "Please enter source file location with it's extention." << endl << "For example : C:\\\\foldername\\filename.cpp" << endl;

	// Opening existing file.
	hFile = CreateFileA(getFileLocation().c_str(), // open file
		GENERIC_READ,             // open for reading
		0,                        // do not share
		NULL,                     // no security
		OPEN_EXISTING,            // existing file only
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		cout << "Could not open file. No such file on specified directory.";
		return 0;
	}

	cout << "Please enter destination file location with it's extention." << endl << "For example : C:\\\\foldername\\filename.cpp" << endl;
	// Open the existing file, or if the file does not exist,
	// create a new file.
	string DestinationFileLocation = getFileLocation();

	if (FunctionalityType == "copy")
	{
		if (std::ifstream(DestinationFileLocation))
		{
			cout << "File already exists" << endl;

			// Close file.
			CloseHandle(hFile);

			return 0;
		}
	}
	hAppend = CreateFileA(DestinationFileLocation.c_str(), // open file
		FILE_APPEND_DATA,         // open for writing
		FILE_SHARE_READ,          // allow multiple readers
		NULL,                     // no security
		OPEN_ALWAYS,              // open or create
		FILE_ATTRIBUTE_NORMAL,    // normal file
		NULL);                    // no attr. template

	if (hAppend == INVALID_HANDLE_VALUE)
	{
		cout << "Could not open file. No such file on specified directory.";
		return 0;
	}

	// Append the first file to the end of the second file.
	// Lock the second file to prevent another process from
	// accessing it while writing to it. Unlock the
	// file when writing is complete.

	while (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL)
		&& dwBytesRead > 0)
	{
		dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
		LockFile(hAppend, dwPos, 0, dwBytesRead, 0);
		WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
		UnlockFile(hAppend, dwPos, 0, dwBytesRead, 0);
	}

	// Close both files.

	CloseHandle(hFile);
	CloseHandle(hAppend);
	return 1;
}

// Function defination to input file name and location from user
string getFileLocation()
{
	string fileName;
	cout << "Enter file location with name : ";		 
	cin >> fileName;
	return fileName;
}
