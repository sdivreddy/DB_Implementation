#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "DBFile.h"
#include "Comparison.h"
#include "ComparisonEngine.h"

typedef enum {heap, sorted, tree} myType;

class DBFile {

	private:
		File *fileHeap;
		Page *pageHeap;
		Page *readingPage;
		myType fType;
		bool pageSeen, fileOpened;
		int prePageNum;
		char ftype[10];

	public:
		DBFile (); 
		~DBFile ();

		// Creates the DBFile under fpath folder with type matching file_type
		// which is heap for the first project
		int Create (const char *fpath, myType file_type, void *startup);

		// Opens an already existing file present that was closed. 
		// Auxillary text file having additional information about 
		// the file is also opened
		int Open (const char *name);

		// Closes the DBFile. Returns 1 on success and 0 on failure.
		int Close ();

		// Write to the page while dirty writes are possible
		void WritetoPage();

		// Consumes the addMe record, puts the same into the file and  
		// later can't be used 
		void Add (Record &addme);

		// Loads DBFile instance from the records present in file under 
		// loadpath location
		void Load (Schema &myschema, const char *loadpath);

		// Adds new record to the end of file for the first project
		void MoveFirst ();

		// Gets next record from file and returns 0 when there are no more
		// valid records are present 
		int GetNext (Record &fetchme);

		// Gets next record from file that matches the selection predicate,
		// returns 0 on failure
		int GetNext (Record &fetchme, CNF &cnf, Record &literal);
		

};
#endif
