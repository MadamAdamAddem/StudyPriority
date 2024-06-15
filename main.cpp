#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <string>

using namespace std;

//finished ~5/5/2024 in Morocco!
//needed something to do over vacation
//Ernesto Adam Lugo

bool isLeap = false;

enum Month
{
    January = 1,
    February, March, April, May, June, July, August, September, October, November, December
};


class Assignment
{
public:
    int daysTillDue;
    int workHours;
    
    bool isNextYear;

    float daysToHoursRatio;

    string name;

    Assignment()
    {
        daysTillDue = 0;
        workHours = 0;
        daysToHoursRatio = 0;
        isNextYear = false;
    }

};


void assignmentSwap(Assignment** list, int in1, int in2)
{
    Assignment* tmp = list[in1];
    list[in1] = list[in2];
    list[in2] = tmp;
}

//sort assignmentList from least to most using daysToHours ratio
void assignmentSelectionSort(Assignment** assignmentList, int len)
{
    float min = 9999;
    int mindex = 0;
    int minspot = 0;
    int flag = 0;


    //when she tells me we have to sort things out
    for(int i=0; i<=len; ++i)
    {
        if(minspot == len-1)
        {
            break;
        }
        else if(i >= len)
        {
            assignmentSwap(assignmentList, mindex, minspot);
            ++minspot;
            i = minspot;
            flag = 0;
            min = 9999;
            mindex = minspot;
        }

        if(assignmentList[i]->daysToHoursRatio < min)
        {
            mindex = i;
            min = assignmentList[i]->daysToHoursRatio;
            ++flag;
        }


    }

}


/*one of the worst functions ive ever written
int stringToMonth(string month)
{
    string puremonth;
    puremonth[0] = month[0];
    puremonth[1] = month[1];
    puremonth[2] = month[2];

    if(puremonth[0] == 'A')
    {
        if(puremonth[1] == 'p')
            return 4;

        return 8;
    }
    else if(puremonth[0] == 'J')
    {
        if(puremonth[2] == 'l')
            return 7;
        else if(puremonth[1] == 'a')
            return 1;

        return 6;
    }
    else if(puremonth[0] == 'M')
    {
        if(puremonth[2] == 'r')
            return 3;
        
        return 5;
    }
    else
    {
        switch(puremonth[0])
        {
            case 'F':
                return 2;
            case 'S':
                return 9;
            case 'O':
                return 10;
            case 'N':
                return 11;
            case 'D':
                return 12;
        }
    }

    return -1;
}
*/

//fast but stupid lol
//each month coincidentally adds up to different numbers
// B)
int stringToMonth(string month)
{
    int total = month[0] + month[1] + month[2];
    switch(total)
    {
        case 281:
            return January;
        case 269:
            return February;
        case 288:
            return March;
        case 291:
            return April;
        case 295:
            return May;
        case 301:
            return June;
        case 299:
            return July;
        case 285:
            return August;
        case 296:
            return September;
        case 294:
            return October;
        case 307:
            return November;
        default:  
            return December;
    }
}


//returns an array with all the months in between month1 and month2 (inclusive both)
//arr[-1] will be size of the array
int* allMonthsBetween(int month1, int month2)
{
    int* arr = NULL;
    int i;

    if(month1 <= month2)
    {
        arr = (int*)malloc((month2-month1+2)*sizeof(int));

        for(i=month1; i<=month2; ++i)
        {
            arr[i-month1+1] = i;
        }

        arr[0] = i-month1;
        return &arr[1];
    }

    //runs this code if month2 is next year

    i = month1;
    arr = (int*)malloc((12 - (month1-month2)+2)*sizeof(int));
    arr[0] = 12-(month1-month2)+1;
    int j = 1;

    //what the flip
    while(1)
    {
        arr[j] = i;
        if(i == month2)
            break;

        ++i;
        ++j;
        if(i >= 13)
        {
            i = 1;
        }
    }

    return &arr[1];

    
}

//September April June November have 30 days (9, 4, 6, 11 respectively)
//Assume 28 for february (2)

int findDaysUntilDueMain(int currmonth, int duemonth, int currday, int dueday)
{
    int* monthArr = allMonthsBetween(currmonth, duemonth);
    int daysTillDue = 0, monthArrSize = monthArr[-1];

    for(int i=0; i<monthArrSize; ++i)
    {
        //if we're iterating in the current month
        if(monthArr[i] == currmonth)
        {
            switch(currmonth)
            {
                case September:
                case April:
                case June:
                case November:
                    daysTillDue += 30 - currday;
                    break;
                case February:
                    if(isLeap)
                        ++daysTillDue;
                    daysTillDue += 28 - currday;
                    break;
                default:
                    daysTillDue += 31 - currday;
                    break;
            }
            continue;
        }

        //if we're iterating in the due month
        else if(monthArr[i] == duemonth)
        {
            daysTillDue += dueday;
            break;
        }

        //anywhere in between
        switch(monthArr[i])
        {
            case September:
            case April:
            case June:
            case November:
                daysTillDue += 30;
                break;
            case February:
                if(isLeap)
                    ++daysTillDue;
                daysTillDue += 28;
                break;
            default:
                daysTillDue += 31;
                break;
        }
    }

    free(&monthArr[-1]);

    return daysTillDue;

}

int findDaysUntilDue(string datestring, string timestring)
{
    
    if(datestring.compare(timestring) == 0)
        return 0;

    //todays day and month, the assignments due day and month
    int currday, currmonth, dueday, duemonth;

    currday = (timestring[4] - '0')*10 + (timestring[5] - '0');
    dueday = (datestring[4] - '0')*10 + (datestring[5] - '0');
    currmonth = stringToMonth(timestring);
    duemonth = stringToMonth(datestring);

    if(currmonth > duemonth)
    {
        return findDaysUntilDueMain(currmonth, duemonth, currday, dueday);
    }

    if(currmonth == duemonth)
    {
        if(currday > dueday)
            return dueday - currday + 365;

        return dueday - currday;
        
    }

    return findDaysUntilDueMain(currmonth, duemonth, currday, dueday);

    
}

bool isLeapF(string year)
{
    int trueYear = (year[0] - '0')*1000 + (year[1] - '0')*100 + (year[2] - '0')*10 + year[3] - '0';
    
    if(trueYear%4 == 0)
    {
        if(trueYear%100 == 0)
        {
            if(trueYear%400 == 0)
                return true;
            
            return false;
        }
        return true;

    }

    return false;

}

int main()
{
    ifstream file;
    file.open("input.txt", ios::in);
    if(file.bad())
    {
        printf("FILE ERROR\n");
        exit(1);
    }

    //get current time
    time_t currtime = time(NULL);
    
    //timestring holds current time, datestring holds due dates, inputstrings holds names of assignments
    string timestring, datestring[15];

    int i;

    string yearstring;

    Assignment** assignmentList = (Assignment**)new Assignment*[15];
    
    //give timestring and yearstring the time in the correct format
    timestring.assign(ctime(&currtime));
    timestring.erase(0, 4);
    yearstring = timestring;
    timestring.erase(6, 50);

    yearstring.erase(7, 7);
    yearstring.erase(0, 9);

    isLeap = isLeapF(yearstring);

    
    string input;

    //fill arrays with file, max 15 assignments
    for(i=0; i<=15; ++i)
    {
        
        //take in name
        file >> input;

        

        if(input.compare(".") == 0)
            break;


        //set name
        assignmentList[i] = (Assignment*)new Assignment;
        assignmentList[i]->name = input;



        //take in workhours
        file >> assignmentList[i]->workHours;

        //take the rest of the date string
        getline(file, datestring[i]);

        //gets rid of pesky space in beginning
        datestring[i].erase(0, 1);

        //does all the cool stuff
        assignmentList[i]->daysTillDue = findDaysUntilDue(datestring[i], timestring);

        //sets ratio
        assignmentList[i]->daysToHoursRatio = (float)assignmentList[i]->daysTillDue / (float)assignmentList[i]->workHours;

        //cout << assignmentList[i]->name << " due in " << assignmentList[i]->daysTillDue << " days" << " with ratio " << assignmentList[i]->daysToHoursRatio << endl;



    }

    file.close();
    
    assignmentSelectionSort(assignmentList, i);

    cout << "Printing sorted list, left is highest priority" << endl;

    for(int j=0; j<i; ++j)
    {
        printf("%s ", assignmentList[j]->name.c_str());
        //cout << assignmentList[j]->name << endl;
        delete assignmentList[j];
    }

    delete[] assignmentList;

    std::cout << "\n\nDone With Program" << endl;
    return 0;
}