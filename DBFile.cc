#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"

using namespace std;

// Constructor 
DBFile::DBFile () {
    prePageNum = 0;
    fileHeap = new File();
    pageHeap = new Page();
    readingPage = new Page();
    fileOpened = false;
}

// Destructor
DBFile :: ~DBFile () {
    delete fileHeap;
    delete pageHeap;
    delete readingPage;
}

// Creates a new file present under f_path and returns 1 on success if 
// no file is opened by this object. If the file type is not heap, then
// return 0. If the specified file is already created, this overwrites it
int DBFile::Create (const char *f_path, myType f_type, void *startup) {
    if(!fileOpened) {
        if(f_type != 0) return 0;
        fileHeap->Open(0, (char*)f_path);
        readingPage->EmptyItOut();
        pageHeap->EmptyItOut();
        fileOpened = true;
        prePageNum = 0;
        return 1;
    }
    return 0;
}

// Dirty write is performed by writing the records to the new page and 
// adding it to the file
void DBFile :: WritetoPage() {
    if(!pageHeap->IsPageEmpty()) {
        int pageNum = 0;
        int fileLen = fileHeap->GetLength();
        if(fileLen != 0) {
            pageNum = fileLen-1;
        }
        fileHeap->AddPage(pageHeap, pageNum);
        pageHeap->EmptyItOut();
    }
    return;
}

// Adds the new record to the end of the page 
void DBFile::Add (Record &rec) {
    int pageNum = 0;
    if(pageHeap->Append(&rec) == 0) {
        int fileLen = fileHeap->GetLength();
        if(fileLen != 0) {
            pageNum = fileLen-1;
        }
        fileHeap->AddPage(pageHeap, pageNum);
        pageHeap->EmptyItOut();
        pageHeap->Append(&rec);
    }
    return;
}

// Loads the record from the file to the .bin file and appends them to the page
void DBFile::Load (Schema &f_schema, const char *loadpath) {
    Record record;
    FILE *loadedFile = fopen (loadpath, "r");
    if(loadedFile) {
        while(record.SuckNextRecord (&f_schema, loadedFile) == 1) {
            Add(record);   
	    }
    }
    else {
        cout << "File can't be loaded from the given path" << endl;
    }
    return;
}

// Opens the already created file and returns 1 on success and 0 on failure
// If the object already opened a file then returns 0
int DBFile::Open (const char *f_path) {
    if(!fileOpened) {
        fileHeap->Open(1, (char*)f_path);
        fileOpened = true;
        readingPage->EmptyItOut();
        prePageNum = 0;
        return 1;
    }
    else {
        return 0;
    }
}

// Move to the first page present in the file and then move to the beginning of the page
// Also performs dirty write before that
void DBFile::MoveFirst () {
    WritetoPage();
    readingPage->EmptyItOut();
    prePageNum = 0;
    fileHeap->GetPage(readingPage, prePageNum);
    readingPage->StartFromBeginning();
    return;
}

// Close the file instance and returns 1 on success and 0 on failure 
// If no file is opened by this object, returns 0
// Also, performs dirty write before that
int DBFile::Close () {
    if(fileOpened) {
        WritetoPage();
        readingPage->EmptyItOut();
        fileHeap->Close();
        prePageNum = 0;
        return 1;
    }
    else {
        return 0;
    }
}

// Performs dirty write first and get the next record from page 
int DBFile::GetNext (Record &fetchme) {
    // Dirty writes and reads check
    WritetoPage();
    while(readingPage->GetFirst(&fetchme) == 0){
        prePageNum += 1;
        if(prePageNum < fileHeap->GetLength()-1) {
            readingPage->EmptyItOut();
            fileHeap->GetPage(readingPage, prePageNum);
            readingPage->StartFromBeginning();
        }
        else {
            return 0;
        }
    }
    return 1;
}

// Get the next record from the page and if it matches the CNF given
// will update the fetchme with the same and returns 1 on success and 
// 0 on failure
int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    int compVal = 0, nextFetch = 0;
    ComparisonEngine compare;

    nextFetch = GetNext(fetchme);
    if (nextFetch == 1) {
        compVal = compare.Compare(&fetchme, &literal, &cnf); 
    }
	else {
        return 0;
    }
    while(compVal == 0) {
        nextFetch = GetNext(fetchme);
	    if (nextFetch == 1) {
            compVal = compare.Compare(&fetchme, &literal, &cnf); 
        }
	    else {
            return 0;
        }
	}
    return compVal;
}
