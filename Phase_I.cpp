#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <thread>
using namespace std;
int ARRAY_LEN = 9;
pthread_mutex_t mtlock = PTHREAD_MUTEX_INITIALIZER;
// int *resultArray; //[9]={0,0,0,0,0,0,0,0,0};
int *resultArray = new int[9];
int *entriesArray = new int[9];
// In checking vaidty we assumed that user cant enter number smaller than 1 and greater than 9
int MAINARRAY[9][9] = {
    {6, 2, 4, 7, 2, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 10, 6, 3, 4},
    {8, 3, 7, 11, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}};
struct rowcheck
{ // int columnnumber;
    int rownumber;
};
struct BoxCheck
{
    int startRow;
    int startColumn;
};

void *Validrow(void *vargp)
{

    //   resultArray={0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 9; i++)
    {
        resultArray[i] = 0;
    }

    rowcheck *ptr = (rowcheck *)vargp;
    int RN = (*ptr).rownumber;
    int tempindex = 0;
    RN = RN - 1;
    pthread_mutex_lock(&mtlock);

    for (int i = RN; i == RN; i++) // row
    {
        for (int j = 0; j < 9; j++)
        {
            entriesArray[tempindex] = MAINARRAY[i][j];
            tempindex += 1;
            resultArray[MAINARRAY[i][j] - 1] += 1;
        }
    }
    pthread_mutex_unlock(&mtlock);
    return resultArray;

    pthread_exit(0);
}
void *ValidColumn(void *vargp)
{
    //   resultArray={0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 9; i++)
    {
        resultArray[i] = 0;
    }
    rowcheck *ptr = (rowcheck *)vargp;
    int RN = (*ptr).rownumber;
    int tempindex = 0;
    RN = RN - 1;
    pthread_mutex_lock(&mtlock);
    for (int j = 0; j < 9; j++) // row
    {
        for (int i = RN; i == RN; i++)
        {
            entriesArray[tempindex] = MAINARRAY[j][i];
            tempindex += 1;
            resultArray[MAINARRAY[j][i] - 1] += 1; // -1 to go one index at back
        }
    }
    pthread_mutex_unlock(&mtlock);
    return resultArray;
    pthread_exit(0);
}

void *ValidBox(void *vargp)
{
    // int resultArray[9]={0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 9; i++)
    {
        resultArray[i] = 0;
    }

    BoxCheck *ptr = (BoxCheck *)vargp;
    int RN = (*ptr).startRow;
    int CN = (*ptr).startColumn;

    //   cout<<"RN="<<RN<<endl;
    //   cout<<"CN="<<CN<<endl;
    RN = RN - 1;
    CN = CN - 1;
    int tempindex = 0;
    pthread_mutex_lock(&mtlock);
    for (int i = RN; i < RN + 3; i++) // row
    {

        for (int j = CN; j < CN + 3; j++)
        {
            entriesArray[tempindex] = MAINARRAY[i][j];
            tempindex += 1;
            //   cout << MAINARRAY[i][j] << "  ";   // uncomment to see box values

            resultArray[(MAINARRAY[i][j]) - 1] += 1; // -1 to go one index at back
        }

        // cout<<endl;
    }
    pthread_mutex_unlock(&mtlock);
    // return ( void * ) resultArray;

    pthread_exit(0);
}

int main()
{
    int boxInvalidEntryCount = 0;
    int invalidOfRangeGreaterValues = 0, invalidOfrepeatingValues = 0;
    int invalidentryValue;
    int tempcheckrow = 1; // true
    //  resultArray  = (int*) malloc(sizeof(int) * ARRAY_LEN);
    ///////////Checking For Rows
    int invalidEntriesInRowCount = 0;
    for (int i = 1; i <= 9; i++)
    {
        struct rowcheck p1 = {i};
        pthread_t thread_id1;
        pthread_create(&thread_id1, NULL, Validrow, (void *)&p1);
        pthread_join(thread_id1, NULL);
        cout << "\n*************************Checking For RowNumber " << i << " *************************\n";
       
        cout<<"Thread ID for checking invalid enteries in Row:  "<<pthread_self()<<endl;
        invalidEntriesInRowCount = 0;
        tempcheckrow = 1;
        invalidentryValue = 0;
        invalidOfRangeGreaterValues = 0, invalidOfrepeatingValues = 0;
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] == 1)
                tempcheckrow = 1;
            else
            {
                tempcheckrow = 0;
                if (resultArray[i] != 0)
                {
                    invalidentryValue = i + 1;
                    cout << "Invalid entry Value is = " << i + 1 << "\n";
                    for (int k = 0; k < 9; k++)
                    {
                        //   cout<<"entriesArray[k]="<<entriesArray[k];
                        if (entriesArray[k] == invalidentryValue)
                        {
                            invalidOfrepeatingValues = 1;
                            cout << "Invalid entry = " << invalidentryValue << " Index is = " << k + 1 << "\n";
                        }
                    }
                    invalidEntriesInRowCount += resultArray[i] - 1;
                }
            }
        }
        for (int i = 0; i < 9; i++)
        {

            if (entriesArray[i] > 9)
            {
                invalidEntriesInRowCount++;
                invalidOfRangeGreaterValues = 1;
                cout << "Invalid Entry = " << entriesArray[i] << "at index =" << i + 1 << endl;
            }
            else if (entriesArray[i] < 1)
            {
                invalidEntriesInRowCount++;
                invalidOfRangeGreaterValues = 1;
                cout << "Invalid Entry = " << entriesArray[i] << "at index=" << i + 1 << endl;
            }
        }

        if (tempcheckrow == 1 && invalidEntriesInRowCount == 0)
        {
            cout << "Valid Row\n";
        }
        else
        {
            cout << "Invalid Row ...\n"
                 << "Count of invalid entries in rows is = " << invalidEntriesInRowCount << "\n";
            if (invalidOfRangeGreaterValues == 1)
            {
                cout << "invalid Because Of Range Greater Values";
            }
            if (invalidOfrepeatingValues == 1)
            {
                cout << "invalid Because Of repeating Values\n";
            }
        }
         cout<<"Cancelling Row thread...\n"<<endl;
    pthread_cancel (thread_id1) ;
    }
   

    ///////////Checking For Columns
    for (int j = 1; j <= 9; j++)
    {
        struct rowcheck p1 = {j};
        pthread_t thread_idCols;
        pthread_create(&thread_idCols, NULL, ValidColumn, (void *)&p1);
        pthread_join(thread_idCols, NULL);
        cout << "\n*************************Checking For Column Number " << j << "*************************\n";
        cout<<"Thread ID for checking invalid enteries in Column:  "<<pthread_self()<<endl;
        
        invalidEntriesInRowCount = 0;
        tempcheckrow = 1;
        invalidOfRangeGreaterValues = 0, invalidOfrepeatingValues = 0;
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] == 1)
                tempcheckrow = 1;
            else
            {
                tempcheckrow = 0;
                if (resultArray[i] != 0)
                {
                    invalidentryValue = i + 1;
                    cout << "Invalid entry is = " << i + 1 << " \n";
                    for (int k = 0; k < 9; k++)
                    {
                        //   cout<<"entriesArray[k]="<<entriesArray[k];
                        if (entriesArray[k] == invalidentryValue)
                        {
                            cout << "Invalid entry = " << invalidentryValue << " Index is = " << k + 1 << " \n";
                            invalidOfrepeatingValues = 1;
                        }
                    }
                    //   invalidEntriesInRowCount+=resultArray[i]-1;
                    invalidEntriesInRowCount += resultArray[i] - 1;
                }
            }
        }
        for (int m = 0; m < 9; m++)
        {

            if (entriesArray[m] > 9)
            {
                invalidOfRangeGreaterValues = 1;
                invalidEntriesInRowCount++;
                cout << "Invalid Entry = " << entriesArray[m] << "   at index = " << m + 1 << endl;
            }
            else if (entriesArray[m] < 1)
            {
                invalidOfRangeGreaterValues = 1;
                invalidEntriesInRowCount++;
                cout << "Invalid Entry = " << entriesArray[m] << "at index = " << m + 1 << endl;
            }
        }

        if (tempcheckrow == 1 && invalidEntriesInRowCount == 0)
        {
            cout << "Valid Column\n";
        }
        else
        {
            cout << "Invalid Column ...\n"
                 << "Count of invalid entries in Column is = " << invalidEntriesInRowCount << "\n";

            if (invalidOfRangeGreaterValues == 1)
            {
                cout << "invalid Because Of Range Greater Values\n";
            }
            if (invalidOfrepeatingValues == 1)
            {
                cout << "invalid Because Of repeating Values\n";
            }
        }
        cout<<"Cancelling Column thread...\n"<<endl;
    pthread_cancel (thread_idCols) ;
    }

    /////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    int tempinvalidentryCount = 0;
    int tempinvalidentry = 0;

    int tempBoxNum = 1;
    int CheckBoxRowNum = 1;
    int CheckBoxColNum = 1;
    ///// Checking for box 1
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    struct BoxCheck boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox1;
    pthread_create(&threadbox1, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox1, NULL);
    cout << "\n*************************Checking Validty For Box 1************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 1:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }

    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
    cout<<"Cancelling Box 1 thread...\n"<<endl;
    pthread_cancel (threadbox1) ;
    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1; // used if no invalid entry for validation
    CheckBoxRowNum = 1;
    CheckBoxColNum = 4;

    ///// Checking for box 2
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox2;
    pthread_create(&threadbox2, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox2, NULL);
    cout << "\n*************************Checking Validty For Box 2************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 2:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is=" << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column=" << (i % 6) + 1;
                            cout << "    Invalid Entries on row=" << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    // printing invalid indexes

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
    
    cout<<"Cancelling Box 2 thread...\n"<<endl;
    pthread_cancel (threadbox2) ;
    //////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1;
    CheckBoxRowNum = 1;
    CheckBoxColNum = 7;
    ///// Checking for box 3
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox3;
    pthread_create(&threadbox3, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox3, NULL);
    cout << "\n*************************Checking Validty For Box 3************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 3:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    // printing invalid indexes

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
     cout<<"Cancelling Box 3 thread...\n"<<endl;
    pthread_cancel (threadbox3) ;
    //////////////////////////////////////////////////////////////////////
    //****************************************************************////
    tempBoxNum = 1;
    CheckBoxRowNum = 4;
    CheckBoxColNum = 1;
    tempinvalidentryCount = 0;
    tempinvalidentry = 0;
    ///// Checking for box 4
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox4;
    pthread_create(&threadbox4, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox4, NULL);
    cout << "\n*************************Checking Validty For Box 4************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 4:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
     cout<<"Cancelling Box 4 thread...\n"<<endl;
    pthread_cancel (threadbox4) ;
    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1;
    CheckBoxRowNum = 4;
    CheckBoxColNum = 4;
    ///// Checking for box 5
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox5;
    pthread_create(&threadbox5, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox5, NULL);
    cout << "\n*************************Checking Validty For Box 5*************************"<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 5:  "<<pthread_self()<<endl;
    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
 cout<<"Cancelling Box 5 thread...\n"<<endl;
    pthread_cancel (threadbox5) ;
    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1;
    CheckBoxRowNum = 4;
    CheckBoxColNum = 7;
    ///// Checking for box 6
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox6;
    pthread_create(&threadbox6, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox6, NULL);
    cout << "\n*************************Checking Validty For Box 6************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 6:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is = " << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
     cout<<"Cancelling Box 6 thread...\n"<<endl;
    pthread_cancel (threadbox6) ;
    //////////////////////////////////////////////////////////////////////
    //****************************************************************////
    tempBoxNum = 1;
    CheckBoxRowNum = 7;
    CheckBoxColNum = 1;
    tempinvalidentryCount = 0;
    tempinvalidentry = 0;
    ///// Checking for box 7
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox7;
    pthread_create(&threadbox7, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox7, NULL);
    cout << "\n*************************Checking Validty For Box 7************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 7:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is=" << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    // printing invalid indexes

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
     cout<<"Cancelling Box 7 thread...\n"<<endl;
    pthread_cancel (threadbox7) ;
    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1;
    CheckBoxRowNum = 7;
    CheckBoxColNum = 4;
    ///// Checking for box 8
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox8;
    pthread_create(&threadbox8, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox8, NULL);
    cout << "\n*************************Checking Validty For Box 8************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 8:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is = " << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }
 cout<<"Cancelling Box 8 thread...\n"<<endl;
    pthread_cancel (threadbox8) ;
    //////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////
    tempBoxNum = 1;
    CheckBoxRowNum = 7;
    CheckBoxColNum = 7;
    ///// Checking for box 9
    invalidOfrepeatingValues = 0;
    invalidOfRangeGreaterValues = 0;
    boxInvalidEntryCount = 0;
    tempinvalidentry = 0;

    boxcheck = {CheckBoxRowNum, CheckBoxColNum};
    pthread_t threadbox9;
    pthread_create(&threadbox9, NULL, ValidBox, (void *)&boxcheck);
    pthread_join(threadbox9, NULL);
    cout << "\n*************************Checking Validty For Box 9************************* "<<endl;
    cout<<"Thread ID for checking invalid enteries in Box 9:  "<<pthread_self()<<endl;

    for (int j = 0; j < 9; j++)
    {
        if (resultArray[j] == 1)
            tempBoxNum = 1;
        else
        {
            tempBoxNum = 0;
            if (resultArray[j] != 0) // to find missing entry ...put == 0 aand print index+1
            {
                tempinvalidentry = j + 1;
                cout << "\nInvalid Entry is = " << j + 1; // index having more than value means that indexnumber occuring more than one time
                boxInvalidEntryCount++;
                tempinvalidentryCount = 0; // it storescount of each repeating number // when count will be 2 or greater for that it will print location
                for (int i = 0; i < 9; i++)
                {
                    if (entriesArray[i] == tempinvalidentry)
                        tempinvalidentryCount += 1;
                    if (entriesArray[i] == tempinvalidentry && tempinvalidentryCount > 1)
                    {
                        invalidOfrepeatingValues = 1;
                        if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
                        {
                            cout << "\n1Invalid Entries on column = " << i + 1;
                            cout << "    Invalid Entries on row = " << i + 1;
                        }
                        if (i > 2 && i <= 5) // when missing entries found in row 2
                        {
                            cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                            cout << "    Invalid Entries on row = " << 2;
                        }
                        if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
                        {
                            cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                            cout << "    Invalid Entries on row = " << 3;
                        }
                        // index having more than value means that indexnumber occuring more than one time
                        // break;
                        //  }
                    }
                }
            }
        }
    }
    // for values greater than 9 and smaller than 0

    for (int i = 0; i < 9; i++)
    {
        if (entriesArray[i] > 9 || entriesArray[i] < 1)
        {
            boxInvalidEntryCount++;
            cout << "\nInvalid Entry is = " << entriesArray[i];
            invalidOfRangeGreaterValues = 1;
            if (i >= 0 && i <= 2) // when missing entries found inrow 1 // on first 3 indexes
            {
                cout << "\n1Invalid Entries on column = " << i + 1;
                cout << "    Invalid Entries on row = " << i + 1;
            }
            if (i > 2 && i <= 5) // when missing entries found in row 2
            {
                cout << "\n2Invalid Entries on column = " << (i % 3) + 1;
                cout << "    Invalid Entries on row = " << 2;
            }
            if (i > 5 && i <= 8) // when missing entries found in row 3---7 8 9
            {
                cout << "\n3Invalid Entries on column = " << (i % 6) + 1;
                cout << "    Invalid Entries on row = " << 3;
            }
            // index having more than value means that indexnumber occuring more than one time
            //  }
        }
    }

    // printing invalid indexes

    if (tempBoxNum == 1 and boxInvalidEntryCount == 0)
    {
        cout << "\nValid Box\n";
    }
    else
    {
        cout << "\nComparison results of invalidation:\n";
        cout << "Repeating value in box is = ";
        for (int i = 0; i < 9; i++)
        {

            if (resultArray[i] > 1)
            {
                cout << " " << i + 1;
            }
        }
        cout << "\nMissing value in box is = ";
        for (int i = 0; i < 9; i++)
        {
            if (resultArray[i] == 0)
            {
                cout << " " << i + 1;
            }
        }

        cout << "\nboxInvalidEntryCount = " << boxInvalidEntryCount << endl;
        if (invalidOfRangeGreaterValues == 1)
        {
            cout << "invalidOfRangeGreaterValues\n";
        }
        if (invalidOfrepeatingValues == 1)
        {
            cout << "invalidOfrepeatingValues\n";
        }
        cout << "Invalid Box\n";
    }

     cout<<"Cancelling Box 9 thread...\n"<<endl;
    pthread_cancel (threadbox9) ;
    pthread_exit(NULL);
    return 0;
}
