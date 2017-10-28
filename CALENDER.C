/*-------------------------------------------------------------------------
This program used to print the simple month calender of the giving date.
e.g:
	Input: 2006 09 22
	Ouput:
	2006/09/22
	---------------------------
	Sun Mon Tue Wed Thu Fri Sat
	                      1   2
	  3   4   5   6   7   8   9
	 10  11  12  13  14  15  16
	 17  18  19  20  21  22  23
	 24  25  26  27  28  29  30
	---------------------------
Author: zhanghongda@gcrj.com
date: 2006/09/23
Compiled by Turbe C 2.0
* ATTENTION: This calender respect to the history before the Year 1582 A.D.
--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*the const used in the program*/
#define MAX_ERR_MSG_STRING 50
#define ERROR 0
#define SUCCESS 1
#define DAYS_OF_YEAR 365
#define START_YEAR 1582
#define ESPECIAL_YEAR 4
/*the variable used to store the input date*/
int _intYear = 0;	/*input year*/
int _intMonth = 0;	/*input month*/
int _intDay = 0;	/*input day*/
/*Month Table*/
int _intMonthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int _intMonthTableLeap[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
/*this function returns the days of February*/
/*attention: there is no check of the parameters*/
int getDaysOfFebruary(int intYear)
{
	if(intYear > START_YEAR)	/*after the year 1582*/
	{
		if(((intYear%100 != 0)&&(intYear%4 == 0))||((intYear%100 == 0)&&(intYear%400 == 0)))
		{
			return 29;
		}
		else
		{
			return 28;
		}
	}
	else				/*before the year 1582*/
	{
		if(intYear == ESPECIAL_YEAR)	/*the Year 4 A.D. */
		{
			return 28;
		}
		else
		{
			if(intYear % 4 == 0)
			{
				return 29;
			}
			else
			{
				return 28;
			}
		}
	}
}
/*this function returns the days of the month*/
/*attention: there is no check of the parameters*/
int getDaysOfMonth(int intYear, int intMonth)
{
	if(intMonth == 2)
	{
		return getDaysOfFebruary(intYear);
	}
	else
	{
		return _intMonthTable[intMonth - 1];
	}
}
/*this function checked the input validity*/
int checkInput(int intYear, int intMonth, int intDay, char *strErrMsg)
{
	if(!strErrMsg)
	{
		return ERROR;
	}
	/*check Year Valid*/
	if((intYear < 1) || (intYear > 9999))
	{
		strcat(strErrMsg, "\nInvalid Year Input.\n");
		return ERROR;
	}
	/*check Month Valid*/
	if((intMonth < 1) || (intMonth > 12))
	{
		strcat(strErrMsg, "\nInvalid Month Input.\n");
		return ERROR;
	}
	/*check Day Valid*/
	if((intDay < 1) || (intDay > getDaysOfMonth(intYear, intMonth)))
	{
		strcat(strErrMsg, "\nInvalid Day Input.\n");
		return ERROR;
	}
	/*check the Oct. 1582*/
	if((intYear == START_YEAR) && (intMonth == 10))
	{
		if((intDay > 4) && (intDay < 15))
		{
			strcat(strErrMsg, "\nInvalid Day Input.\n");
			return ERROR;
		}
	}
	return SUCCESS;
}
/*this function used to receive the input by the user*/
int receiveInput(void)
{
	int intYear = 0;
	int intMonth = 0;
	int intDay = 0;
	char strErrMsg[MAX_ERR_MSG_STRING] = "";	/*the error message string*/
	fflush(stdin);	/*clear the input buffer*/
	/*
	I searched it by BaiDu.com.
	Someone said that it is better not using fflush(stdin).
	Because it cannot be used in Linux gcc.
	So this programme may cannot be compiled in Linux gcc....
	*/
	/*display the message and wait for the input*/
	printf("\nInput the date you want to display(use form like \"yyyy mm dd\"): ");
	if(!scanf("%d %d %d", &intYear, &intMonth, &intDay))
	{
		printf("\nBad form of your input.\n");
		return ERROR;
	}
	if(!checkInput(intYear, intMonth, intDay, strErrMsg))
	{
		/*print the error message*/
		printf("%s",strErrMsg);
		return ERROR;
	}
	else
	{
		/*let the value pass*/
		_intYear = intYear;
		_intMonth = intMonth;
		_intDay = intDay;
		return SUCCESS;
	}
}
/*this function used to print the fixed head infomation of the calender*/
void printHead(void)
{
	printf("\n%d/%d/%d\n", _intYear, _intMonth, _intDay);
	printf("-----------------------------\n");
	printf(" Sun Mon Tue Wed Thu Fri Sat\n");
}
/*this function used to calculate the weekday of the first day of the year and month*/
/*---------------------------------------------------------------------------------------
Calculate isn't a hard business,
But I think calulate from Year 1 A.D. to Year 9999 A.D is very hard.
Because the Gregorian calendar (we are now using) is published in Mar. 1, 1582.
There is detail infomation in the document from hongen.com.
So respect to the history, we have to calculate the weekday before 1582 especially.
Between 1 A.D to 1582, there is a predecessor of the Gregorian calendar.
All the years that can divide exactly by 4 are the leap years.
And the Year 4 A.D. is especial, It is not a leap year in history.
Another especial is the Year 1582 A.D.
The next day of Oct. 4, 1582 Thu is Oct. 15, 1582 Fri.
So I have to calculate respectively of this year, before this year, and after this year.
The benchmark of the weekday is Oct. 4, 1582 Thu and Oct. 15, 1582 Fri.
Sorry about that this function is such long....
---------------------------------------------------------------------------------------*/
int calculateWeekday(void)
{
	int intYear = _intYear;
	int intMonth = _intMonth;
	long longTotalDays = 0;
	/*after the year 1582, using the benchmark Oct. 15, 1582 Fri*/
	if(intYear > START_YEAR)
	{
		longTotalDays = ((long)intYear - START_YEAR - 1) * DAYS_OF_YEAR;
			/*leap days*/
		longTotalDays += (((intYear-1)/4 - START_YEAR/4) + ((intYear-1)/400 - START_YEAR/400));
			/*correct it*/
		longTotalDays -= ((intYear-1)/100 - START_YEAR/100);
		longTotalDays += (_intMonthTable[10] + _intMonthTable[11]);
		longTotalDays += 16;	/*calculate to Oct. 15*/
		if(((intYear%100 != 0)&&(intYear%4 == 0))||((intYear%100 == 0)&&(intYear%400 == 0)))
		{
			int i = 0;
			for(i = 0; i < (intMonth - 1); i++)
			{
				longTotalDays += _intMonthTableLeap[i];
			}
		}
		else
		{
			int i = 0;
			for(i = 0; i < (intMonth - 1); i++)
			{
				longTotalDays += _intMonthTable[i];
			}
		}
		/*printf("%ld\n", longTotalDays);*/
		return (longTotalDays + 6)%7;
	}
	/*before the year 1582 after the year 4, using the benchmark Oct. 4, 1582 Thu*/
	if(intYear < START_YEAR)
	{
		int i = 0;
		longTotalDays = (START_YEAR - ((long)intYear + 1)) * DAYS_OF_YEAR;
			/*leap days*/
		longTotalDays += (((START_YEAR + 2) - (intYear + 1))/4);
			/*before the year 4, calculate respectively*/
		if(intYear < ESPECIAL_YEAR)
		{
			longTotalDays--;
		}
		for(i = 0; i < 9; i++)
		{
			longTotalDays += _intMonthTable[i];
		}
		longTotalDays += 4;	/*calculate to Oct. 4*/
		if(intYear % 4 == 0)
		{
			if(intYear != ESPECIAL_YEAR)
			{
				for(i = 12; i > (intMonth - 1); i--)
				{
					longTotalDays += _intMonthTableLeap[i-1];
				}
			}
			else
			{
				for(i = 12; i > (intMonth - 1); i--)
				{
					longTotalDays += _intMonthTable[i-1];
				}
			}
		}
		else
		{
			for(i = 12; i > (intMonth - 1); i--)
			{
				longTotalDays += _intMonthTable[i-1];
			}
		}
		/*printf("%d\n", i);*/
		/*printf("%ld\n", longTotalDays);*/
		if((longTotalDays + 2) % 7 != 0)
		{
			return (7 - (longTotalDays + 2) % 7);
		}
		else
		{
			return 0;
		}
	}
	/*the year 1582, calculate respectively*/
	if(intYear == START_YEAR)
	{
		if(intMonth < 10)
		{
			int i = 0;
			for(i = 9; i > (intMonth - 1); i--)
			{
				longTotalDays += _intMonthTable[i-1];
			}
			/*printf("%d\n", i);*/
			longTotalDays += 4;	/*calculate to Oct. 4*/
			/*printf("%ld\n", longTotalDays);*/
			if((longTotalDays + 2) % 7 != 0)
			{
				return (7 - (longTotalDays + 2) % 7);
			}
			else
			{
				return 0;
			}
		}
		if(intMonth > 10)
		{
			int i = 10;
			longTotalDays = 16;	/*calculate to Oct. 15*/
			for(i = 10; i < (intMonth - 1); i++)
			{
				longTotalDays += _intMonthTable[i];
			}
			/*printf("%ld\n", longTotalDays);*/
			return (longTotalDays + 6) % 7;
		}
		return 1;
	}
	return ERROR;	/*the programme will never run here, only to disable the warning*/
}
/*this function used to print the the calender*/
void printCalender(void)
{
	int intWeekday = calculateWeekday();
	int i = 0;
	int intYear = _intYear;
	int intMonth = _intMonth;
	int intDaysOfMonth = getDaysOfMonth(intYear, intMonth);
	if((intYear == 1582) && (intMonth == 10))	/*the very especial month*/
	{
		printf("       1   2   3   4  15  16\n");
		printf("  17  18  19  20  21  22  23\n");
		printf("  24  25  26  27  28  29  30\n");
		printf("  31\n");
		return;
	}
	/*print the space of the first date*/
	for(i = 0; i < intWeekday; i++)
	{
		printf("    ");
	}
	/*print the calender*/
	for(i = 1; i <= intDaysOfMonth; i++)
	{
		if(i < 10)
		{
			printf("   %d", i);
		}
		else
		{
			printf("  %d", i);
		}
		if((intWeekday + i)%7 == 0)
		{
			printf("\n");
		}
	}
	if((intWeekday + i - 1)%7 != 0)
	{
		printf("\n");
	}
}
/*this function used to print the fixed foot infomation of the calender*/
void printFoot(void)
{
	printf("-----------------------------\n");
	return;
}
/*the main function of the program*/
int main()
{
	while(!receiveInput());
	printHead();
	/*printf("%d", calculateWeekday());*/
	printCalender();
	printFoot();
	return 0;
}