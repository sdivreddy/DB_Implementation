
#include <iostream>
#include "Record.h"
#include <stdlib.h>
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct AndList *final;

int main () {

	// Write
    FILE *tableFile = fopen ("/cise/homes/dreddy/P1/git/tpch-dbgen/lineitem.tbl", "r");
	Schema mySchema ("catalog", "lineitem");
	Record record;
	Page page;
	File file;	
	while(record.SuckNextRecord (&mySchema, tableFile) == 1) {
		record.Print (&mySchema);
		if(page.Append(&record) == 0) break;
	}
	file.Open(0, "output.tbl");
	file.AddPage(&page, 1);
	file.Close();

	// Read
	file.Open(1, "output.tbl");
	Page getPage;
	Record record1;
	file.GetPage(&getPage, 1);
	while(getPage.GetFirst(&record1) != 0) {
		record1.Print(&mySchema);
	}

	// try to parse the CNF
	// cout << "Enter in your CNF: ";
  	// if (yyparse() != 0) {
	// 	cout << "Can't parse your CNF.\n";
	// 	exit (1);
	// }

	// suck up the schema from the file
	// Schema lineitem ("catalog", "lineitem");

	// // grow the CNF expression from the parse tree 
	// CNF myComparison;
	// Record literal; // Read and parse CNF Text
	// myComparison.GrowFromParseTree (final, &lineitem, literal);
	
	// // print out the comparison to the screen - CNF Structure
	// myComparison.Print (); 

	// now open up the text file and start procesing it
	//char *bits = literal.GetBits ();
	//cout << " numbytes in rec " << ((int *) bits)[0] << endl;
	//literal.Print (&supplier);

	// read in all of the records from the text file and see if they match
	// the CNF expression that was typed in
	// int counter = 0;
	// ComparisonEngine comp;
    //     while (temp.SuckNextRecord (&mySchema, tableFile) == 1) {
	// 		counter++;
	// 		if (counter % 10000 == 0) {
	// 			cerr << counter << "\n";
	// 		}

	// 		if (comp.Compare (&temp, &literal, &myComparison))
	// 					temp.Print (&mySchema);

    //     }

}


