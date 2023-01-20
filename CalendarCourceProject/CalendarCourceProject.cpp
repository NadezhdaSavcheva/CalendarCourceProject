/**
*
* Solution to course project # 10
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Nadezhda Savcheva
* @idnumber 2MI0600155
* @compiler VC
*
* <main program file>
*
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<ctime>
#include <string>
#include <fstream>

using namespace std;

void calendarStartMenu();
void showCalendar(int, int, string);
void showSchedule(int, int);
void listEvents();
void addEvent();
void addDurationToEvents(string, string, string);
void removeEvent();
void removeEventFromEventsByDaysFile(string);
void setFirstWeekDay();

const int MAX_VALID_YEAR = 2099;
const int MIN_VALID_YEAR = 1901;

bool isLeap(int year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool dateValidation(int day, int month, int year)
{
    if (year > MAX_VALID_YEAR || year < MIN_VALID_YEAR)
    {
        cout << "Sorry, the year entered is invalid. Please try again." << endl;
        return false;
    }
    if (month < 1 || month > 12)
    {
        cout << "Sorry, the month entered is invalid. Please try again." << endl;
        return false;
    }
    if (day < 1 || day > 31)
    {
        cout << "Sorry, the day entered is invalid. Please try again." << endl;
        return false;
    }
    if (month == 2)
    {
        if (isLeap(year))
        {
            if (day > 29)
            {
                cout << "Sorry, the day entered is invalid. Please try again." << endl;
                return false;
            }
        }
        else
        {
            if (day > 28)
            {
                cout << "Sorry, the day entered is invalid. Please try again." << endl;
                return false;
            }
        }
    }

    return true;
}

bool monthValidation(int month)
{
    if (month < 1 || month > 12)
    {
        cout << endl;
        cout << "Sorry, the month entered is invalid. Please try again.";
        return false;
    }

    return true;
}

bool yearValidation(int year)
{
    if (year > MAX_VALID_YEAR || year < MIN_VALID_YEAR)
    {
        cout << endl;
        cout << "Sorry, the year entered is invalid. Please try again.";
        return false;
    }

    return true;
}

void choiceFromMenuValidation(int choice)
{
    if (choice < 1 || choice > 7)
    {
        cout << endl;
        cout << "Sorry, the choice entered is invalid. Please try again with number from 1 to 7." << endl << endl;;
        return;
    }
}

bool eventNameValidation(string eventName)
{
    if (eventName.find("-") != string::npos || eventName.find("/") != string::npos)
    {
        cout << endl;
        cout << "Sorry, the name of the event must not contain \"-\" or \"/\". Please try again with another name." << endl << endl;
        return false;
    }

    return true;
}

bool uniqueEventNameValidation(string eventName)
{
    string wholeLine;

    fstream eventsFile;
    eventsFile.open("events.txt", ios::in | ios::app);

    if (eventsFile.is_open())
    {
        while (getline(eventsFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) == eventName)
            {
                cout << endl;
                cout << "Sorry, there is already an event with this name. Please try again." << endl << endl;
                eventsFile.close();
                return false;
            }
        }
    }

    return true;
}

bool isEventInList(string eventName)
{
    string wholeLine;

    fstream eventsFile;
    eventsFile.open("events.txt", ios::in | ios::app);

    if (eventsFile.is_open())
    {
        while (getline(eventsFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) == eventName)
            {
                eventsFile.close();
                return true;
            }
        }
    }

    return false;
}

bool firstWeekDayValidation(string firstWeekDay)
{
    if (firstWeekDay != "Sun" && firstWeekDay != "Mon")
    {
        cout << endl;
        cout << "Sorry, the first day of the week must be \"Sun\" or \"Mon\". Please try again.";
        return false;
    }

    return true;
}

void calendarStartMenu()
{
    const string DAY[] = { "Sunday","Monday","Tuesday",
     "Wednesday","Thursday","Friday","Saturday" };

    const string MONTH[] = { "January","February","March",
     "April","May","June","July","August","September","October","November","December" };

    time_t rawtime;
    tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    int weekDay = timeinfo->tm_wday;
    int month = timeinfo->tm_mon;
    int monthDay = timeinfo->tm_mday;
    int year = 1900 + timeinfo->tm_year;

    cout << "Welcome! Today is " << DAY[weekDay] << ", " << monthDay << " " << MONTH[month] << " " << year << "." << endl;
    cout << endl;

    int eventsToday = 0;
    int eventsTomorrow = 0;

    string line;
    ifstream eventsFile;
    eventsFile.open("events.txt", ios::app);

    if (eventsFile.is_open())
    {
        while (getline(eventsFile, line, '\n'))
        {
            size_t delim = line.find_first_of("-");

            if (monthDay < 10 && month < 10)
            {
                if (line.substr(delim + 2, 1) == to_string(monthDay) &&
                    line.substr(delim + 5, 1) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsToday++;
                }

                if (line.substr(delim + 2, 1) == to_string(monthDay + 1) &&
                    line.substr(delim + 5, 1) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsTomorrow++;
                }
            }
            else if (monthDay < 10 && month >= 10)
            {
                if (line.substr(delim + 2, 1) == to_string(monthDay) &&
                    line.substr(delim + 4, 2) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsToday++;
                }

                if (line.substr(delim + 2, 1) == to_string(monthDay + 1) &&
                    line.substr(delim + 4, 2) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsTomorrow++;
                }
            }
            else if (monthDay >= 10 && month < 10)
            {
                if (line.substr(delim + 1, 2) == to_string(monthDay) &&
                    line.substr(delim + 5, 1) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsToday++;
                }

                if (line.substr(delim + 1, 2) == to_string(monthDay + 1) &&
                    line.substr(delim + 5, 1) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsTomorrow++;
                }
            }
            else
            {
                if (line.substr(delim + 1, 2) == to_string(monthDay) &&
                    line.substr(delim + 4, 2) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsToday++;
                }

                if (line.substr(delim + 1, 2) == to_string(monthDay + 1) &&
                    line.substr(delim + 4, 2) == to_string(month + 1) &&
                    line.substr(delim + 7, 4) == to_string(year))
                {
                    eventsTomorrow++;
                }
            }
        }
    }

    if (eventsToday == 1)
    {
        cout << "You have " << eventsToday << " event" << " today." << endl;
        cout << endl;
    }
    else if (eventsToday > 1)
    {
        cout << "You have " << eventsToday << " events" << " today." << endl;
        cout << endl;
    }
    else if (eventsTomorrow == 1)
    {
        cout << "You have " << eventsTomorrow << " event" << " tomorrow." << endl;
        cout << endl;
    }
    else if (eventsTomorrow > 1)
    {
        cout << "You have " << eventsTomorrow << " events" << " tomorrow." << endl;
        cout << endl;
    }
    else
    {
        cout << "You have " << eventsTomorrow << " events" << " tomorrow." << endl;
        cout << endl;
    }

    cout << "Choose an option:" << endl
        << "    1. Show calendar" << endl
        << "    2. Show schedule" << endl
        << "    3. List events" << endl
        << "    4. Add event" << endl
        << "    5. Remove event" << endl
        << "    6. Set first weekday" << endl
        << "    7. Exit" << endl;
    cout << endl;

    int option;
    cout << "Enter your choice: ";
    cin >> option;

    choiceFromMenuValidation(option);

    string fileLine;
    string dayOfWeek;
    fstream firstDayOfTheWeek;
    firstDayOfTheWeek.open("first_day_of_the_week.txt", ios::in | ios::app);

    getline(firstDayOfTheWeek, fileLine);
    dayOfWeek = fileLine.substr(strlen(fileLine.c_str()) - 4, 3);
    firstDayOfTheWeek.close();

    while (true)
    {
        switch (option)
        {
        case 1:
            showCalendar(month + 1, year, dayOfWeek);
            cout << endl << endl;
            break;
        case 2:
            showSchedule(month + 1, year);
            cout << endl;
            break;
        case 3:
            listEvents();
            cout << endl;
            break;
        case 4:
            addEvent();
            break;
        case 5:
            eventsFile.close();
            firstDayOfTheWeek.close();
            removeEvent();
            break;
        case 6:
            setFirstWeekDay();
            cout << endl << endl;
            break;
        case 7:
            return;
        }

        cout << "Choose an option:" << endl
            << "    1. Show calendar" << endl
            << "    2. Show schedule" << endl
            << "    3. List events" << endl
            << "    4. Add event" << endl
            << "    5. Remove event" << endl
            << "    6. Set first weekday" << endl
            << "    7. Exit" << endl;
        cout << endl;

        cout << "Enter your choice: ";
        cin >> option;
    }

    eventsFile.close();
    firstDayOfTheWeek.close();
}

int dayNumber(int day, int month, int year)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

    year -= month < 3;

    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

string getMonthName(int monthNumber)
{
    string months[] = { "January", "February", "March", "April", "May", "June",
                        "July", "August", "September", "October", "November", "December" };

    return (months[monthNumber]);
}

string getWeekDayName(int dayOfWeek, string firstWeekDay)
{
    string daysFromSunToSat[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    string daysFromMonToSun[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };

    if (firstWeekDay == "Sun")
    {
        return (daysFromSunToSat[dayOfWeek]);
    }
    else
    {
        return (daysFromMonToSun[dayOfWeek]);
    }
}

int numberOfDays(int monthNumber, int year)
{
    if (monthNumber == 0)
    {
        return (31);
    }

    if (monthNumber == 1)
    {
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        {
            return (29);
        }
        else
        {
            return (28);
        }
    }

    if (monthNumber == 2)
    {
        return (31);
    }

    if (monthNumber == 3)
    {
        return (30);
    }

    if (monthNumber == 4)
    {
        return (31);
    }

    if (monthNumber == 5)
    {
        return (30);
    }

    if (monthNumber == 6)
    {
        return (31);
    }

    if (monthNumber == 7)
    {
        return (31);
    }

    if (monthNumber == 8)
    {
        return (30);
    }

    if (monthNumber == 9)
    {
        return (31);
    }

    if (monthNumber == 10)
    {
        return (30);
    }

    if (monthNumber == 11)
    {
        return (31);
    }
}

int getFirstDayOfMonth(int month, int year)
{
    unsigned a, y, m;

    a = (14 - month) / 12;
    y = year - a;
    m = month + (12 * a) - 2;

    return (1 + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7;
}

int findNumberOfEventsForEachDayOfTheMonth(int month, int year, int dayOfMonth)
{
    int eventsNum = 0;
    int MAX_DAYS_IN_MONTH = 31;
    int* numOfDayEvents = new int[MAX_DAYS_IN_MONTH] {0};

    string line;
    ifstream eventsFile;
    eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

    int day = 0;

    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++)
    {
        day = i + 1;

        eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

        if (eventsFile.is_open())
        {
            while (getline(eventsFile, line, '\n'))
            {
                size_t delim = line.find_first_of("-");

                if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) != "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else if (line.substr(delim + 1, 1) != "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 1, 2) == to_string(day) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else
                {
                    if (line.substr(delim + 1, 2) == to_string(day) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
            }

            eventsFile.close();
        }

        numOfDayEvents[i] = eventsNum;
        eventsNum = 0;
    }

    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++)
    {
        if (i == dayOfMonth - 1)
        {
            return numOfDayEvents[i];
        }
    }
}

int* findNumbersOfEventsForAMonth(int month, int year)
{
    int eventsNum = 0;
    int MAX_DAYS_IN_MONTH = 31;
    int* numOfDayEvents = new int[MAX_DAYS_IN_MONTH] {0};

    string line;
    ifstream eventsFile;
    eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

    int day = 0;
    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++)
    {
        day = i + 1;

        eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

        if (eventsFile.is_open())
        {
            while (getline(eventsFile, line, '\n'))
            {
                size_t delim = line.find_first_of("-");

                if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day - 1) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) != "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day - 1) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else if (line.substr(delim + 1, 1) != "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 1, 2) == to_string(day - 1) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
                else
                {
                    if (line.substr(delim + 1, 2) == to_string(day) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        eventsNum++;
                    }
                }
            }

            eventsFile.close();
        }

        numOfDayEvents[i] = eventsNum;
        eventsNum = 0;
    }

    return numOfDayEvents;
}

string getDayOfWeekFromDate(int day, int month, int year)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

    year -= month < 3;

    int dayOfWeek = (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;

    return getWeekDayName(dayOfWeek, "Sun");
}

void printEvents(int month, int year)
{
    string line;

    ifstream eventsFile;
    eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

    const int MAX_DAYS_IN_MONTH = 31;

    string dayFromWeek;

    int day = 0;
    int dayEventsCount = 1;

    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++)
    {
        day = i + 1;
        dayEventsCount = 1;

        eventsFile.open("events_day_by_day.txt", ios::in | ios::app);

        if (eventsFile.is_open())
        {
            while (getline(eventsFile, line, '\n'))
            {
                size_t delim = line.find_first_of("-");

                if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day - 1) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        dayFromWeek = getDayOfWeekFromDate(day - 1, month, year);

                        if (findNumberOfEventsForEachDayOfTheMonth(month, year, i) >= 1)
                        {
                            if (dayEventsCount == 1)
                            {
                                cout << dayFromWeek << ", 0" << day - 1;
                                cout << "     " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                                continue;
                            }

                            if (dayEventsCount > 1)
                            {
                                cout << "            " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                            }
                        }
                        else
                        {
                            cout << dayFromWeek << ", 0" << day - 1 << "     " << line.substr(0, line.find("-")) << endl;
                        }
                    }
                }
                else if (line.substr(delim + 1, 1) == "0" && line.substr(delim + 4, 1) != "0")
                {
                    if (line.substr(delim + 2, 1) == to_string(day) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        dayFromWeek = getDayOfWeekFromDate(day - 1, month, year);

                        if (findNumberOfEventsForEachDayOfTheMonth(month, year, i) >= 1)
                        {
                            if (dayEventsCount == 1)
                            {
                                cout << dayFromWeek << ", 0" << day - 1;
                                cout << "     " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                                continue;
                            }

                            if (dayEventsCount > 1)
                            {
                                cout << "            " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                            }
                        }
                        else
                        {
                            cout << dayFromWeek << ", 0" << day - 1 << "     " << line.substr(0, line.find("-")) << endl;
                        }
                    }
                }
                else if (line.substr(delim + 1, 1) != "0" && line.substr(delim + 4, 1) == "0")
                {
                    if (line.substr(delim + 1, 2) == to_string(day) &&
                        line.substr(delim + 5, 1) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        dayFromWeek = getDayOfWeekFromDate(day, month, year);

                        if (findNumberOfEventsForEachDayOfTheMonth(month, year, day) >= 1)
                        {
                            if (dayEventsCount == 1)
                            {
                                cout << dayFromWeek << ", " << day;
                                cout << "     " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                                continue;
                            }

                            if (dayEventsCount > 1)
                            {
                                cout << "            " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                            }
                        }
                        else
                        {
                            cout << dayFromWeek << ", " << day << "     " << line.substr(0, line.find("-")) << endl;
                        }
                    }
                }
                else
                {
                    if (line.substr(delim + 1, 2) == to_string(day) &&
                        line.substr(delim + 4, 2) == to_string(month) &&
                        line.substr(delim + 7, 4) == to_string(year))
                    {
                        dayFromWeek = getDayOfWeekFromDate(day, month, year);

                        if (findNumberOfEventsForEachDayOfTheMonth(month, year, day) >= 1)
                        {
                            if (dayEventsCount == 1)
                            {
                                cout << dayFromWeek << ", " << day;
                                cout << "     " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                                continue;
                            }

                            if (dayEventsCount > 1)
                            {
                                cout << "            " << line.substr(0, line.find("-")) << endl;
                                dayEventsCount++;
                            }
                        }
                        else
                        {
                            cout << dayFromWeek << ", " << day << "     " << line.substr(0, line.find("-")) << endl;
                        }
                    }
                }
            }

            eventsFile.close();
        }
    }
}

void showCalendar(int month, int year, string firstWeekDay)
{
    char ch;

    cout << endl;
    cout << "Enter month (MM/YYYY): ";
    cin >> month >> ch >> year;

    if (ch != '/')
    {
        cout << endl;
        cout << "Entered date is not in the correct format. Please try again.";
        return;
    }

    if (monthValidation(month) == false)
    {
        return;
    }

    if (yearValidation(year) == false)
    {
        return;
    }

    int days;

    time_t theTime = time(NULL);
    struct tm* aTime = localtime(&theTime);

    int currentDay = aTime->tm_mday;
    int currentMonth = aTime->tm_mon + 1;
    int currentYear = aTime->tm_year + 1900;


    int eventsNumForADay = 0;

    string line;
    ifstream eventsFile;
    eventsFile.open("events_day_by_day.txt", ios::app);

    for (int i = 0; i < 12; i++)
    {
        if (i == month - 1)
        {
            days = numberOfDays(i, year);

            printf("\n                         %s ", getMonthName(i).c_str());
            printf("%d            \n", year);

            if (firstWeekDay == "Sun")
            {
                cout << "  ";

                for (int i = 0; i < 7; i++)
                {
                    if ((i == currentDay % 7 - 2) && currentMonth == month && currentYear == year)
                    {
                        printf("%s     ", getWeekDayName(i, firstWeekDay).c_str());
                        continue;
                    }

                    if ((i == currentDay % 7 - 1) && currentMonth == month && currentYear == year)
                    {
                        cout << "[" << getWeekDayName(i, firstWeekDay).c_str() << "]" << "     ";
                    }
                    else
                    {
                        printf("%s      ", getWeekDayName(i, firstWeekDay).c_str());
                    }

                    if (i == 6)
                    {
                        cout << endl;
                    }
                }

                int firstDayOfMonth = getFirstDayOfMonth(month, year);

                int i;
                int spaces = 0;

                for (i = 0; i < firstDayOfMonth; i++)
                {
                    printf("         ");
                    spaces++;
                }

                int eventsForTheDay;

                for (int j = 1; j <= days; j++)
                {
                    eventsForTheDay = findNumbersOfEventsForAMonth(month, year)[j];

                    if (eventsForTheDay > 0)
                    {
                        if (j == currentDay && currentMonth == month && currentYear == year)
                        {
                            if (j < 10)
                            {
                                cout << "   [" << j << "]" << "(" << eventsForTheDay << ")";
                            }
                            else
                            {
                                cout << "  [" << j << "]" << "(" << eventsForTheDay << ")";
                            }
                        }
                        else
                        {
                            printf("%5d (%u)", j, eventsForTheDay);
                        }
                        spaces++;
                    }
                    else
                    {
                        if (j == currentDay && currentMonth == month && currentYear == year)
                        {
                            if (j < 10)
                            {
                                cout << "   [" << j << "]" << "   ";
                            }
                            else
                            {
                                cout << "  [" << j << "]" << "   ";
                            }
                        }
                        else
                        {
                            printf("%5d    ", j);
                        }
                        spaces++;
                    }

                    if (spaces > 6)
                    {
                        spaces = 0;
                        printf("\n");
                    }
                }
            }
            else if (firstWeekDay == "Mon")
            {
                currentMonth = aTime->tm_mon + 1;

                if (currentMonth != month || currentYear != year)
                {
                    cout << "  ";
                }

                for (int i = 0; i < 7; i++)
                {
                    if ((i == currentDay % 7 - 3) && currentMonth == month && currentYear == year)
                    {
                        printf("  %s     ", getWeekDayName(i, firstWeekDay).c_str());
                        continue;
                    }
                    if ((i == currentDay % 7 - 2) && currentMonth == month && currentYear == year)
                    {
                        cout << "[" << getWeekDayName(i, firstWeekDay).c_str() << "]" << "     ";
                    }
                    else
                    {
                        printf("%s      ", getWeekDayName(i, firstWeekDay).c_str());
                    }

                    if (i == 6)
                    {
                        cout << endl;
                    }
                }

                int firstDayOfMonth = getFirstDayOfMonth(month, year) + 6;

                int i;
                int spaces = 0;

                for (i = 0; i < firstDayOfMonth; i++)
                {
                    spaces++;

                    if (spaces == 7)
                    {
                        spaces = 0;
                        printf("\n");
                    }
                    else
                    {
                        printf("         ");
                    }
                }

                int eventsForTheDay = 0;
                for (int j = 1; j <= days; j++)
                {
                    eventsForTheDay = findNumbersOfEventsForAMonth(month, year)[j];

                    if (eventsForTheDay > 0)
                    {
                        if (j == currentDay && currentMonth == month && currentYear == year)
                        {
                            if (j < 10)
                            {
                                cout << "   [" << j << "]" << "(" << eventsForTheDay << ")";
                            }
                            else
                            {
                                cout << "  [" << j << "]" << "(" << eventsForTheDay << ")";
                            }
                        }
                        else
                        {
                            printf("%5d (%u)", j, eventsForTheDay);
                        }
                        spaces++;
                    }
                    else
                    {
                        if (j == currentDay && currentMonth == month && currentYear == year)
                        {
                            if (j < 10)
                            {
                                cout << "   [" << j << "]" << "   ";
                            }
                            else
                            {
                                cout << "  [" << j << "]" << "   ";
                            }
                        }
                        else
                        {
                            printf("%5d    ", j);
                        }
                        spaces++;
                    }

                    if (spaces > 6)
                    {
                        spaces = 0;
                        printf("\n");
                    }
                }
            }

            break;
        }
    }

    eventsFile.close();
}

void showSchedule(int month, int year)
{
    char ch;

    cout << endl;
    cout << "Enter month (MM/YYYY): ";
    cin >> month >> ch >> year;

    if (ch != '/')
    {
        cout << endl;
        cout << "Entered date is not in the correct format. Please try again.";
        return;
    }

    if (monthValidation(month) == false)
    {
        return;
    }

    if (yearValidation(year) == false)
    {
        return;
    }

    bool eventsForTheMonth = false;

    for (int i = 0; i < 31; i++)
    {
        if (findNumbersOfEventsForAMonth(month, year)[i] > 0)
        {
            eventsForTheMonth = true;
        }
    }

    if (eventsForTheMonth == false)
    {
        cout << endl << "Sorry, you don't have any events for this month." << endl;
        return;
    }

    cout << endl;

    for (int i = 0; i < 12; i++)
    {
        if (i == month - 1)
        {
            cout << getMonthName(i).c_str() << " ";
            cout << year << endl;
            for (int j = 0; j < strlen(getMonthName(i).c_str()) + sizeof(char) + sizeof(year); j++)
            {
                cout << "-";
            }
            cout << endl;
        }
    }

    printEvents(month, year);
}

void sortEventsByStartDate()
{

}

void sortEventsByName()
{

}

void listEvents()
{
    cout << endl;

    cout << "You have the following events:" << endl << endl;

    int eventNumInList = 0;
    int numOfLines = 0;
    string line;
    fstream eventsFile;
    eventsFile.open("events.txt", ios::in | ios::app);

    if (eventsFile.is_open())
    {
        while (getline(eventsFile, line, '\n'))
        {
            numOfLines++;
        }
    }

    eventsFile.close();
    eventsFile.open("events.txt", ios::in | ios::app);

    if (eventsFile.is_open())
    {
        if (numOfLines == 0)
        {
            cout << "Sorry, your list of events is empty. If you want to add current or future events to your calendar, enter 4." << endl;
        }
        else
        {
            numOfLines = 1;

            while (getline(eventsFile, line, '\n'))
            {
                cout << numOfLines << ". " << line << endl;
                numOfLines++;
            }
        }
    }

    eventsFile.close();
}

int eventDuration(string eventName)
{
    int eventDuration = 0;
    string wholeLine;

    ifstream eventsByDaysFile;
    eventsByDaysFile.open("events_day_by_day.txt", ios::in | ios::app);

    if (eventsByDaysFile.is_open())
    {
        while (getline(eventsByDaysFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) == eventName)
            {
                eventDuration++;
            }
        }
    }

    eventsByDaysFile.close();

    return eventDuration;
}

void addEvent()
{
    cout << endl;

    ofstream eventsFile;
    eventsFile.open("events.txt", ios::app);

    ofstream eventsByDaysFile;
    eventsByDaysFile.open("events_day_by_day.txt", ios::app);

    int startDay = 0;
    int endDay = 0;
    int startMonth = 0;
    int endMonth = 0;
    int startYear = 0;
    int endYear = 0;
    int lastDayOfTheMonth = 0;

    string eventName;
    string startDate;
    string endDate;

    cout << "Enter name: ";
    getline(cin >> ws, eventName, '\n');

    if (uniqueEventNameValidation(eventName) == false)
    {
        return;
    }

    if (eventNameValidation(eventName) == false)
    {
        return;
    }

    cout << "Enter start date (DD/MM/YYYY): ";
    cin >> startDate;

    startDay = stoi(startDate.substr(0, 2).c_str());
    startMonth = stoi(startDate.substr(3, 2).c_str());
    startYear = stoi(startDate.substr(6, 4).c_str());

    if (dateValidation(startDay, startMonth, startYear) == false)
    {
        return;
    }

    cout << "Enter end date (DD/MM/YYYY): ";
    cin >> endDate;

    endDay = stoi(endDate.substr(0, 2).c_str());
    endMonth = stoi(endDate.substr(3, 2).c_str());
    endYear = stoi(endDate.substr(6, 4).c_str());

    if (dateValidation(endDay, endMonth, endYear) == false)
    {
        return;
    }

    lastDayOfTheMonth = numberOfDays(startMonth - 1, startYear);

    if (startDate == endDate)
    {
        eventsFile << eventName << "-" << startDate << endl;

        eventsByDaysFile << eventName << "-" << startDate << endl;
    }
    else
    {
        eventsFile << eventName << "-" << startDate << "-" << endDate << endl;

        while (!(startDay == endDay + 1 && startMonth == endMonth && startYear == endYear))
        {
            if (startDay == lastDayOfTheMonth + 1)
            {
                if (startMonth == 12)
                {
                    startDay = 1;
                    startMonth++;
                    startYear++;
                }
                else
                {
                    startDay = 1;
                    startMonth++;
                }
            }

            if (startDay < 10 && startMonth < 10)
            {
                eventsByDaysFile << eventName << "-0" << startDay << "/0" << startMonth << "/" << startYear << endl;
                startDay++;
            }
            else if (startDay < 10 && startMonth >= 10)
            {
                eventsByDaysFile << eventName << "-0" << startDay << "/" << startMonth << "/" << startYear << endl;
                startDay++;
            }
            else if (startDay >= 10 && startMonth < 10)
            {
                eventsByDaysFile << eventName << "-" << startDay << "/0" << startMonth << "/" << startYear << endl;
                startDay++;
            }
            else
            {
                eventsByDaysFile << eventName << "-" << startDay << "/" << startMonth << "/" << startYear << endl;
                startDay++;
            }
        }
    }

    if (!eventsFile.fail() || !eventsByDaysFile.fail())
    {
        cout << endl;
        cout << "Event added successfully!";
    }
    else
    {
        cout << endl;
        cout << "Sorry, event was not added successfully. Please, try again.";
    }

    eventsFile.close();
    eventsByDaysFile.close();

    addDurationToEvents(eventName, startDate, endDate);
}

void addDurationToEvents(string eventName, string startDate, string endDate)
{
    int startDay = 0;
    int endDay = 0;
    int startMonth = 0;
    int endMonth = 0;
    int startYear = 0;
    int endYear = 0;
    int lastDayOfTheMonth = 0;
    int durationOfEvent;
    string wholeLine;

    fstream eventsByDaysFile;
    eventsByDaysFile.open("events_day_by_day.txt", ios::in | ios::app);

    fstream secTempFile;
    secTempFile.open("sec_temp.txt", ios::out);

    startDay = stoi(startDate.substr(0, 2).c_str());
    endDay = stoi(endDate.substr(0, 2).c_str());
    startMonth = stoi(startDate.substr(3, 2).c_str());
    endMonth = stoi(endDate.substr(3, 2).c_str());
    startYear = stoi(startDate.substr(6, 4).c_str());
    endYear = stoi(endDate.substr(6, 4).c_str());

    lastDayOfTheMonth = numberOfDays(startMonth - 1, startYear);
    durationOfEvent = eventDuration(eventName);

    if (secTempFile.is_open())
    {
        while (getline(eventsByDaysFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) == eventName && durationOfEvent > 1)
            {
                int eventDaysCount = 1;

                while (!(startDay == endDay + 1 && startMonth == endMonth && startYear == endYear))
                {
                    if (startDay == lastDayOfTheMonth + 1)
                    {
                        if (startMonth == 12)
                        {
                            startDay = 1;
                            startMonth++;
                            startYear++;
                        }
                        else
                        {
                            startDay = 1;
                            startMonth++;
                        }
                    }

                    if (startDay < 10 && startMonth < 10)
                    {
                        secTempFile << eventName << " (Day " << eventDaysCount << "/" << durationOfEvent << ")"
                            << "-0" << startDay << "/0" << startMonth << "/" << startYear << endl;
                        secTempFile.flush();
                        startDay++;
                        eventDaysCount++;
                        continue;
                    }
                    else if (startDay < 10 && startMonth >= 10)
                    {
                        secTempFile << eventName << " (Day " << eventDaysCount << "/" << durationOfEvent << ")"
                            << "-0" << startDay << "/" << startMonth << "/" << startYear << endl;
                        secTempFile.flush();
                        startDay++;
                        eventDaysCount++;
                        continue;
                    }
                    else if (startDay >= 10 && startMonth < 10)
                    {
                        secTempFile << eventName << " (Day " << eventDaysCount << "/" << durationOfEvent << ")"
                            << "-" << startDay << "/0" << startMonth << "/" << startYear << endl;
                        secTempFile.flush();
                        startDay++;
                        eventDaysCount++;
                        continue;
                    }
                    else
                    {
                        secTempFile << eventName << " (Day " << eventDaysCount << "/" << durationOfEvent << ")"
                            << "-" << startDay << "/" << startMonth << "/" << startYear << endl;
                        secTempFile.flush();
                        startDay++;
                        eventDaysCount++;
                        continue;
                    }
                }
            }
            else
            {
                secTempFile << wholeLine << endl;
                secTempFile.flush();
            }
        }
    }

    eventsByDaysFile.close();
    secTempFile.close();

    cout << endl;

    remove("events_day_by_day.txt");

    if (rename("sec_temp.txt", "events_day_by_day.txt") == 0)
    {
        puts("");
    }
    else
    {
        perror("");
    }
}

void removeEvent()
{
    cout << endl;

    string eventName;
    string wholeLine;

    cout << "Enter name: ";
    getline(cin >> ws, eventName, '\n');

    if (isEventInList(eventName) == false)
    {
        cout << endl;
        cout << "Sorry, there is no event with this name. Please try again." << endl << endl;
        return;
    }

    fstream eventsFile;
    eventsFile.open("events.txt", ios::in | ios::app);

    fstream tempFile;
    tempFile.open("temp.txt", ios::out);

    if (tempFile.is_open())
    {
        while (getline(eventsFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) != eventName)
            {
                tempFile << wholeLine << endl;
                tempFile.flush();
            }
        }
    }

    eventsFile.close();
    tempFile.close();

    cout << endl;

    remove("events.txt");

    if (rename("temp.txt", "events.txt") == 0)
    {
        puts("Event deleted successfully!");
    }
    else
    {
        perror("Sorry, event was not deleted successfully. Please, try again.");
    }

    removeEventFromEventsByDaysFile(eventName);
}

void removeEventFromEventsByDaysFile(string eventName)
{
    string wholeLine;

    fstream eventsByDaysFile;
    eventsByDaysFile.open("events_day_by_day.txt", ios::in | ios::app);

    fstream secTempFile;
    secTempFile.open("sec_temp.txt", ios::out);

    if (secTempFile.is_open())
    {
        while (getline(eventsByDaysFile, wholeLine, '\n'))
        {
            if (wholeLine.substr(0, eventName.size()) != eventName)
            {
                secTempFile << wholeLine << endl;
                secTempFile.flush();
            }
        }
    }

    eventsByDaysFile.close();
    secTempFile.close();

    cout << endl;

    remove("events_day_by_day.txt");

    if (rename("sec_temp.txt", "events_day_by_day.txt") == 0)
    {
        puts("");
    }
    else
    {
        perror("");
    }
}

void setFirstWeekDay()
{
    cout << endl;

    string line;
    string firstWday;
    string newFirstWday;
    fstream firstDayOfTheWeek;
    firstDayOfTheWeek.open("first_day_of_the_week.txt", ios::out | ios::app);

    firstDayOfTheWeek.close();
    firstDayOfTheWeek.open("first_day_of_the_week.txt", ios::in | ios::app);

    getline(firstDayOfTheWeek, line);

    firstWday = line.substr(strlen(line.c_str()) - 4, 3);

    if (firstWday == "Sun")
    {
        printf("The first day of the week is currently Sunday.\n");
    }
    else if (firstWday == "Mon")
    {
        printf("The first day of the week is currently Monday.\n");
    }

    cout << endl;

    firstDayOfTheWeek.close();
    firstDayOfTheWeek.open("first_day_of_the_week.txt", ios::out | ios::app);

    cout << "Enter new (Mon / Sun): ";
    cin >> newFirstWday;

    if (firstWeekDayValidation(newFirstWday) == false)
    {
        return;
    }

    firstDayOfTheWeek << newFirstWday << " ";

    if (!firstDayOfTheWeek.fail())
    {
        cout << endl;
        cout << "Saved!";
    }
    else
    {
        cout << endl;
        cout << "Change was not saved. Please try again.";
    }

    firstDayOfTheWeek.close();
}

int main()
{
    calendarStartMenu();

    return 0;
}
