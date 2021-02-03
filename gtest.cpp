#include <iostream>
#include "gtest/gtest.h"
#include "DBFile.h"
#include "test.h"
#include "Schema.h"
using namespace std;

const char *dbfile_dir = ""; // dir where binary heap files should be stored
const char *tpch_dir ="/Users/divyareddy/Desktop/Sem3/DBI/Projects/A1/git/tpch-dbgen/"; // dir where dbgen tpch files (extension *.tbl) can be found
const char *catalog_path = "catalog"; // full path of the catalog file

relation *rel;

DBFile dFCreate, dFCreateNew, dFOpen, dFClose;

// Create - Check if we can create a new file
TEST(HeapFile, CheckNewFileCreate) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[1];
    ASSERT_EQ(1, dFCreate.Create(rel->path(), heap, NULL));
}

// Create - Check if we can create an already existent file or any
// other file while the DBFile object holds another file
TEST(HeapFile, CheckAlreadyFileCreate) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[1];    
    ASSERT_EQ(0, dFCreate.Create(rel->path(), heap, NULL));
    rel = rel_ptr[5];
    ASSERT_EQ(0, dFCreate.Create(rel->path(), heap, NULL));
    dFCreate.Close();
}

// Create - Check if we can create a new file with a new object
TEST(HeapFile, CheckNewFileCreateWithNewObject) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[6];    
    ASSERT_EQ(1, dFCreateNew.Create(rel->path(), heap, NULL));
    dFCreateNew.Close();
}

// Create - Check if we can create a new file other than heap
TEST(HeapFile, CheckNewFileCreateOtherFileType) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[3];
    ASSERT_EQ(0, dFCreate.Create(rel->path(), sorted, NULL));
}

// Create - Invalid File Path Create
TEST(DBFile_Test_Create, CheckInvalidFileCreatePath)
{
    DBFile dbfileInvalidCreate;
    EXPECT_EXIT(dbfileInvalidCreate.Create( "", heap, NULL), ::testing::ExitedWithCode(1), "BAD.*");
}

// Open - Check if we can open a file which we was created earlier
TEST(HeapFile, CheckFileOpenAgainAfterCreate) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[1];
    ASSERT_EQ(1, dFOpen.Open(rel->path()));
}

// Open - Check if we can open a file immediately after we create
TEST(HeapFile, CheckFileOpenAfterCreate) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[2];
    dFOpen.Close();
    dFOpen.Create(rel->path(), heap, NULL);
    ASSERT_EQ(0, dFOpen.Open(rel->path()));
}

// Open - Check if we can open a file which we had opened previously
TEST(HeapFile, CheckFileOpenAgainAfterOpen) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[2];
    dFOpen.Close();
    dFOpen.Open(rel->path());
    ASSERT_EQ(0, dFOpen.Open(rel->path()));
}

// Open - Invalid File Open
TEST(HeapFile, CheckInvalidFileOpen) {
    DBFile dbfileInvalid;
	EXPECT_EXIT(dbfileInvalid.Open("invalid_file.bin"), ::testing::ExitedWithCode(1), "BAD.*");
}

// Create - Check if we can create any new file if a file is already opened by the object
TEST(HeapFile, CheckNewFileCreateAfterOpen) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[3];    
    ASSERT_EQ(0, dFOpen.Create(rel->path(), heap, NULL));
}

// Close - Check if we can close a file before we either create or open
TEST(HeapFile, CheckFileCloseBeforeCreateOrOpen) {
    ASSERT_EQ(0, dFClose.Close());
}

// Close - Check if we can close a created file 
TEST(HeapFile, CheckFileCloseAfterCreate) {
    relation *rel_ptr[] = {n, r, c, p, ps, o, li};
    rel = rel_ptr[4];
    dFClose.Create(rel->path(), heap, NULL);
    ASSERT_EQ(1, dFClose.Close());
}

// Close - Check if we can close an opened file
TEST(HeapFile, CheckFileCloseAfterOpen) {
    ASSERT_EQ(1, dFOpen.Close());
}

// Entry Point
int main(int argc, char **argv) {
    setup(catalog_path, dbfile_dir, tpch_dir);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
