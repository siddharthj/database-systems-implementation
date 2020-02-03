#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {
    totalPageCount = 0;
    currentPage = 0;
    filepath = NULL;
}

DBFile::~DBFile () {
    delete(filepath);
}
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    if(f_path == NULL || f_path[0] == '\0') {
        cerr<<"Path to create file is null!!!"<<endl;
        return 0;
    }
    if(f_type == heap)
    {
        char *b = new char[strlen(f_path) + 1]{};
        copy(f_path, f_path + strlen(f_path), b);
        filepath = b;
        file.Open(0, filepath);
        file.Close();
        return 1;
    } else {
        cerr<<"Only heap file type supported!!!"<<endl;
        return 0;
    } 
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {
    if(filepath == NULL || filepath[0] == '\0') {
        cerr<<"First call create() and then load()."<<endl;
        return;
    }

    FILE *tableFile = fopen(loadpath,"r");
    if(tableFile == NULL) {
        cerr<<"Failed to open file at loadpath:"<<loadpath<<" . Maybe the file doesn't exist at that location!!!"<<endl;
        return;
    }
    off_t pageCount=0;
    Record temp;
    page.EmptyItOut();
    file.Open(1, filepath);
    off_t numberOfrecords = 0;
    while (temp.SuckNextRecord (&f_schema, tableFile)) {
        numberOfrecords++;
        if (!page.Append(&temp)) {
            file.AddPage(&page, pageCount);
            pageCount++;
            page.EmptyItOut();
            if(!page.Append(&temp)) {
                cout<<"Something wrong happended while adding record to new page";
                exit(0);
            }
        }
    }
    file.AddPage(&page, pageCount);
    page.EmptyItOut();
    cout<<"Number of records loaded :"<<numberOfrecords<<endl;
}

int DBFile::Open (const char *f_path) {
    if(f_path == NULL || f_path[0] == '\0') {
        cerr<<"Empty file path!!!"<<endl;
        return 0;
    }

    char *path = new char[strlen(f_path) + 1]{};
    copy(f_path, f_path + strlen(f_path), path);
    cout<<f_path<<endl;
    file.Open(1, path);
    delete(path);
    return 1;
}

void DBFile::MoveFirst () {
    page.EmptyItOut();
    file.GetPage(&page, 0);
    currentPage = 0;
}

int DBFile::Close () {
    file.Close();
    return 1;
}

void DBFile::Add (Record &rec) {
    off_t lastPage;
    int fileLength = file.GetLength();
    if (file.GetLength() > 3) {
        lastPage = fileLength - 2;
    }
    if (fileLength >3 ) {
        file.GetPage(&page, lastPage);

        if (!page.Append(&rec)) {
            page.EmptyItOut();
            page.Append(&rec);
            lastPage++;
        }
    } else { 
        page.Append(&rec);
        lastPage = 0;
    }
    file.AddPage(&page, lastPage);
}

int DBFile::GetNext (Record &fetchme) {
    totalPageCount = file.GetLength() - 1;
    cout<<"Total Page Count :"<<totalPageCount<<endl;
    if (page.GetFirst(&fetchme)) {
        return 1;
    } else {
        if (currentPage + 1 < totalPageCount) {
            currentPage++;
            cout<<"Current Page :"<<currentPage<<endl;
            file.GetPage(&page, currentPage);
            page.GetFirst(&fetchme);
            return 1;
        } else {
            return 0;
        }
    }
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    Record temp;
    totalPageCount = file.GetLength() - 1;
    while (currentPage < totalPageCount) {
        if (page.GetFirst(&temp)) {
            if (comparator.Compare(&temp, &literal, &cnf)) {
                fetchme.Consume(&temp);
                return 1;
            }
        } else {
            if (currentPage + 1 < totalPageCount) {
                currentPage++;
                file.GetPage(&page, currentPage);
            } else {
                return 0;
            }
        }
    }
    return 0;
}
