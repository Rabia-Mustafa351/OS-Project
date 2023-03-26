#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <thread>
#include <fstream>
#include <semaphore.h>
using namespace std;
int moves = 0;
int *checkRowArray = new int[9];
int *checkColumnArray = new int[9];
int *checkBoxArray = new int[9];
int *RowvalidtyArray = new int[9];
int invaildentriesCount = 0;
int *BoxvalidtyArray = new int[9];
int *ColumnvalidtyArray = new int[9];
int boxWarning = 0, columnWarning = 0, rowWarning = 0;
int RN, CN, valuetobereplaced; // struct
int threadCount = 0;
sem_t sem;

struct rowcheck
{
    // int columnnumber;
    int rownumber;
};
struct ValueCheck
{
    int Row;
    int Column;
};

int MAINARRAY[9][9] = {
                     {6, 2, 4, 5, 3, 4, 4, 8, 7},
                     {5, 1, 9, 7, 2, 8, 6, 3, 4},
                     {8, 3, 7, 6, 1, 4, 2, 9, 5},
                     {1, 4, 3, 8, 6, 5, 7, 2, 9},
                     {9, 5, 8, 2, 4, 7, 3, 6, 1},
                     {7, 6, 2, 3, 9, 1, 4, 5, 8},
                     {3, 7, 1, 9, 1, 6, 8, 4, 2},
                     {4, 9, 6, 1, 8, 2, 5, 7, 3},
                     {2, 8, 5, 4, 7, 3, 9, 1, 6}
                    };

void *setInvalid(void *vargp)
{
    for (int i = 0; i < 9; i++)
    {

        if (RowvalidtyArray[i] == 0 && BoxvalidtyArray[i] == 0 && ColumnvalidtyArray[i] == 0)
        {
            cout << "Value to be changed is: " << MAINARRAY[RN][CN] << " at Row: " << RN + 1 << " at Column: " << CN + 1 << endl;
            valuetobereplaced = i + 1;
            MAINARRAY[RN][CN] = valuetobereplaced;
            moves++;
            threadCount++;
            invaildentriesCount++;
        }
    }

    pthread_exit(0);
    return 0;
}

void *Check(void *vargp)
{
    for (int i = 0; i < 9; i++)
    {
        RowvalidtyArray[i] = 0;
        ColumnvalidtyArray[i] = 0;
        BoxvalidtyArray[i] = 0;
    }
    // row column box
    boxWarning = 0, columnWarning = 0, rowWarning = 0;
    ValueCheck *ptr = (ValueCheck *)vargp;
    RN = (*ptr).Row;
    CN = (*ptr).Column;
    RN = RN - 1;
    CN = CN - 1;

    for (int i = RN; i <= RN; i++)
    { 
        // adding values of rows
        for (int j = 0; j < 9; j++)
        {

            checkRowArray[j] = MAINARRAY[i][j];
        }
    }

    for (int i = 0; i <= 9; i++)
    { // adding values of Columns
        for (int j = CN; j <= CN; j++)
        {
            checkColumnArray[i] = MAINARRAY[i][j];
        }
    }
    // Sub Box1
    if (RN <= 2 && CN <= 2)
    {
        int tempcount = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box2 horizontal
    if ((RN <= 2) && (CN > 2 && CN < 5))
    {
        int tempcount = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 3; j < 6; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box3 horizontal
    if ((RN <= 2) && (CN > 5 && CN < 9))
    {
        int tempcount = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 6; j < 9; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box4 verticle
    if ((RN > 2 && RN < 6) && (CN <= 2))
    {
        int tempcount = 0;
        for (int i = 3; i < 6; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box5 horizontal
    if ((RN > 2 && RN < 6) && (CN > 2 && CN < 5))
    {
        int tempcount = 0;
        for (int i = 3; i < 6; i++)
        {
            for (int j = 3; j < 6; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box6 horizontal
    if ((RN > 2 && RN < 6) && (CN > 5 && CN < 9))
    {
        int tempcount = 0;
        for (int i = 3; i < 6; i++)
        {
            for (int j = 6; j < 9; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box7 verticle
    if ((RN > 6 && RN <= 8) && (CN <= 2))
    {
        int tempcount = 0;
        for (int i = 6; i < 9; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box 8 horizontal
    if ((RN > 6 && RN <= 8) && (CN > 2 && CN < 5))
    {
        int tempcount = 0;
        for (int i = 6; i < 9; i++)
        {
            for (int j = 3; j < 6; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    // Sub Box9 horizontal
    if ((RN > 6 && RN <= 8) && (CN > 5 && CN < 9))
    {
        int tempcount = 0;
        for (int i = 6; i < 9; i++)
        {
            for (int j = 6; j < 9; j++)
            {
                checkBoxArray[tempcount] = MAINARRAY[i][j];
                tempcount++;
            }
        }
    }

    for (int i = 0; i < 9; i++)
    { // checking desire rows
        RowvalidtyArray[checkRowArray[i] - 1] += 1;
    }

    for (int i = 0; i < 9; i++)
    { // checking desire Columns
        ColumnvalidtyArray[checkColumnArray[i] - 1] += 1;
    }

    for (int i = 0; i < 9; i++)
    { // checking desire Columns
        BoxvalidtyArray[checkBoxArray[i] - 1] += 1;
    }

    int valuetobereplaced = 0;
    for (int i = 0; i < 9; i++)
    {
        if (RowvalidtyArray[i] != 1)
        { // we have to do changes
            rowWarning = 1;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (ColumnvalidtyArray[i] != 1)
        {
            columnWarning = 1;
        }
    }
    for (int i = 0; i < 9; i++)
    {
        if (BoxvalidtyArray[i] != 1)
        {
            boxWarning = 1;
        }
    }

    if (boxWarning == 1 && columnWarning == 1 && rowWarning == 1)
    {   
        // actual value will be changed here
        

        pthread_t callSetThread;
        pthread_create(&callSetThread, NULL, setInvalid, NULL);
        pthread_join(callSetThread, NULL);
        cout<<"Threads setting the invalid values: "<<pthread_self()<<" at location "<<RN+1<<" and "<<CN+1<<endl;
        //cout<<endl;

        // sem_wait(&sem) ;
        // threadCount++;
        // cout<<"threadCount="<<threadCount;
        // sem_post(&sem) ;
    }

    return 0;
}

int main()
{
    cout << "Original Puzzlee: " << endl;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << MAINARRAY[i][j] << " ";
        }
        cout << endl;
    }
    // Checking the validty Of Matrix

    // ///////////
    // /////////
    pthread_t thread_id1;
    struct ValueCheck p1;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            //  sem_wait(&sem) ;
            p1 = {i + 1, j + 1};
            pthread_create(&thread_id1, NULL, Check, (void *)&p1);
            pthread_join(thread_id1, NULL);
            //  sem_post(&sem) ;
            
            cout<<"Threads checking Invalid positions: "<<pthread_self()<<" at location "<<i<<" and "<<j<<endl;
            
        }
    }
    

    cout << "\nInvalid entries count: " << invaildentriesCount << endl;
    cout << "\nmoves count: " << moves << endl;
    cout << "Thread count: " << threadCount << endl;

    
    cout<<"\nReplaced Matrix: "<<endl;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cout << MAINARRAY[i][j] << " ";
        }
        cout << endl;
    }
    pthread_exit(0);
}
