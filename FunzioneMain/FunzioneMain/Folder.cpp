#include "stdafx.h"
#include "Folder.h"
#include "fstream"
#include <windows.h>
#include <string>
#include <string.h>
#include <iostream>
#include <tchar.h>
#include <wchar.h>
#include <future>
#include <list>
#include <chrono>

using namespace std;




Folder::Folder(std::wstring* cartella_origine, std::wofstream& f, std::list <Oggetto*>& allthefiles)
{		
	this->name = *cartella_origine;
	WIN32_FIND_DATA find_file_data;
	HANDLE Ffile = FindFirstFile((*cartella_origine+L"\\*").c_str(), &find_file_data);		
	
	if (Ffile == INVALID_HANDLE_VALUE){
		std::wcout << L"Non ho trovato nulla, la cartella esiste?!\n" << *cartella_origine << std::endl;
		//lancia una eccezione
	}

	//cartella_origine->pop_back();
	//cartella_origine->pop_back();

	for (bool i; GetLastError() != ERROR_NO_MORE_FILES;i=FindNextFile(Ffile, &find_file_data)){
			
		wstring filepath = *cartella_origine + L"\\" + find_file_data.cFileName;
		
		if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wcscmp(find_file_data.cFileName, L"..") != 0 && wcscmp(find_file_data.cFileName, L".") != 0){
				
			this->contains.push_front(new Folder(&filepath, f, allthefiles));
			SetLastError(0);
		}
		else if (! (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
			this->files.push_front(new Oggetto(filepath, find_file_data.cFileName));
			allthefiles.push_front(new Oggetto(filepath, find_file_data.cFileName));
		}

	}
	
	FindClose(Ffile); //ricordati di chiudere sempre le HANDLE
}

//Visto che creiamo il distruttore specifico � ovvio che abbiamo bisogno del costruttore di copia e assegnazione
Folder::~Folder()
{
	Folder* p;
	while (!this->contains.empty()){
		p = this->contains.front();
		this->contains.pop_front();
		delete p;
		p = NULL;
	}

	Oggetto* p2;
	while (!this->files.empty()){
		p2 = this->files.front();
		this->files.pop_front();
		delete p2;
		p2 = NULL;
	}
}



