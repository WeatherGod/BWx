#ifndef _BMOS_C
#define _BMOS_C

using namespace std;
/*
 *  BMOS.C
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>		// for strftime(), strptime()
#include <cstdio>		// for sscanf(), snprintf()
#include <cmath>		// for nan() and isnan()
#include "BMOS.h"

BMOS::BMOS()
	:	myIsValid(false),
		myTableStartPos(0),
		myTime(),
		myStationID(""),
		myModelType(""),
		myHours(0),
                myMinTemps(),
		myMaxTemps(),
                myTemperatures(),
                myDewPoints(),
                myCloudCovers(),
                myWindDirs(),
                myWindSpeeds(),
                myProbOfPrecips_6hr(),
                myProbOfPrecips_12hr(),
		myHourLabel(""),
		myMinMaxLabel(""),
		myMaxMinLabel(""),
		myTemperatureLabel(""),
		myDewPointLabel(""),
		myCloudLabel(""),
		myWindDirLabel(""),
		myWindSpeedLabel(""),
		myPOP6Label(""),
		myPOP12Label("")
{
}

BMOS::BMOS(const BMOS &MOSCopy)
	:	myIsValid( MOSCopy.myIsValid ),
		myTableStartPos( MOSCopy.myTableStartPos ),
		myTime( MOSCopy.myTime ),
		myStationID( MOSCopy.myStationID ),
		myModelType( MOSCopy.myModelType ),
		myHours( MOSCopy.myHours ),
                myMinTemps( MOSCopy.myMinTemps ),
		myMaxTemps( MOSCopy.myMaxTemps ),
                myTemperatures( MOSCopy.myTemperatures ),
                myDewPoints( MOSCopy.myDewPoints ),
                myCloudCovers( MOSCopy.myCloudCovers ),
                myWindDirs( MOSCopy.myWindDirs ),
                myWindSpeeds( MOSCopy.myWindSpeeds ),
                myProbOfPrecips_6hr( MOSCopy.myProbOfPrecips_6hr ),
                myProbOfPrecips_12hr( MOSCopy.myProbOfPrecips_12hr ),
		myHourLabel( MOSCopy.myHourLabel ),
                myMinMaxLabel( MOSCopy.myMinMaxLabel ),
		myMaxMinLabel( MOSCopy.myMaxMinLabel ),
                myTemperatureLabel( MOSCopy.myTemperatureLabel ),
                myDewPointLabel( MOSCopy.myDewPointLabel ),
                myCloudLabel( MOSCopy.myCloudLabel ),
                myWindDirLabel( MOSCopy.myWindDirLabel ),
                myWindSpeedLabel( MOSCopy.myWindSpeedLabel ),
                myPOP6Label( MOSCopy.myPOP6Label ),
                myPOP12Label( MOSCopy.myPOP12Label )
{
}

BMOS::BMOS(const string &MOSString)
	:	myIsValid(false),
		myTableStartPos(0),
		myTime(),
		myStationID(""),
		myModelType(""),
		myHours(0),
		myMinTemps(),
                myMaxTemps(),
                myTemperatures(),
                myDewPoints(),
                myCloudCovers(),
                myWindDirs(),
                myWindSpeeds(),
                myProbOfPrecips_6hr(),
                myProbOfPrecips_12hr(),
		myHourLabel(""),
                myMinMaxLabel(""),
		myMaxMinLabel(""),
                myTemperatureLabel(""),
                myDewPointLabel(""),
                myCloudLabel(""),
                myWindDirLabel(""),
                myWindSpeedLabel(""),
                myPOP6Label(""),
                myPOP12Label("")
{
	Decode(MOSString);
}


bool BMOS::IsModelType(const string &CodeStr) const
{
	return(CodeStr == "NGM" || CodeStr == "ETA" || CodeStr == "GFS" || 
	       CodeStr == "AVN" || CodeStr == "MRF" || CodeStr == "MM5");
}

bool BMOS::IsStationID(const string &StationID) const
{
	if (StationID.length() < 3 || StationID.length() > 4)
	{
		return(false);
	}

	size_t CharPos = 0;


	if (StationID.length() == 4)
	{
		// If it is 4 characters long, then the first char must be a letter.
		if (isalpha(StationID[CharPos]) == 0)
		{
			return(false);
		}
		
		CharPos++;
	}
	
	// The last 3 characters must be an alphanumeric character.
	return(isalnum(StationID[CharPos]) != 0 && isalnum(StationID[CharPos + 1]) != 0 && isalnum(StationID[CharPos + 2]) != 0);
}

bool BMOS::TakeModelType(const string &CodeType)
{
	if (IsModelType(CodeType))
	{
		myModelType = CodeType;

		if (myModelType == "NGM")
		{
			myHourLabel = "HOUR";
			myMinMaxLabel = "MN/MX";
			myMaxMinLabel = "MX/MN";
			myTemperatureLabel = "TEMP";
			myDewPointLabel = "DEWPT";
			myCloudLabel = "CLDS";
			myWindDirLabel = "WDIR";
			myWindSpeedLabel = "WSPD";
			myPOP6Label = "POP06";
			myPOP12Label = "POP12";
		}
		else
		{
			myHourLabel = "HR";
                        myMinMaxLabel = "N/X";
			myMaxMinLabel = "X/N";
                        myTemperatureLabel = "TMP";
                        myDewPointLabel = "DPT";
                        myCloudLabel = "CLD";
                        myWindDirLabel = "WDR";
                        myWindSpeedLabel = "WSP";
                        myPOP6Label = "P06";
                        myPOP12Label = "P12";
		}

		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMOS::TakeStationID(const string &StationID)
{
	if (IsStationID(StationID))
	{
		if (StationID.length() == 3)
		{
			myStationID = "K" + StationID;
		}
		else
		{
			myStationID = StationID;
		}

		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMOS::Decode(const string &MOSReport)
{
	size_t CharPos = 0;
	size_t CharCount = 0;

	try
	{
	
	char StationIDStr[5];
	memset(StationIDStr, '\0', 5);
	char ModelTypeStr[6];
        memset(ModelTypeStr, '\0', 6);
        char DateStr[11];
        memset(DateStr, '\0', 11);
        char TimeStr[5];
        memset(TimeStr, '\0', 5);

	
	if (sscanf(MOSReport.c_str() + CharPos, "%4s %n", StationIDStr, &CharCount) != 1)
	{
		myIsValid = false;
		return(false);
	}

	CharPos += CharCount;
	CharCount = 0;

	if (strlen(StationIDStr) == 3)
	{
		if (sscanf(MOSReport.c_str() + CharPos, "%*3s %5s MOS GUIDANCE %10s %4s UTC %*4s %n", ModelTypeStr, DateStr, TimeStr, &CharCount) != 3)
		{
			myIsValid = false;
			return(false);
		}
	
		CharPos += CharCount;
	}
	else
	{
		if (sscanf(MOSReport.c_str() + CharPos, "%5s MOS GUIDANCE %10s %4s UTC %*4s %n", ModelTypeStr, DateStr, TimeStr, &CharCount) != 3)
		{
			myIsValid = false;
			return(false);
		}

		CharPos += CharCount;
	}

	if (!TakeStationID(StationIDStr))
	{
		myIsValid = false;
		return(false);
	}
	
	if (!TakeModelType(ModelTypeStr))
	{
		myIsValid = false;
		return(false);
	}

	if (!DecodeTime(((string) DateStr) + ' ' + ((string) TimeStr)))
	{
		myIsValid = false;
		return(false);
	}

	CharCount = 0;
	char MonStr[5];
	memset(MonStr, '\0', 5);

	// reading the date line, which only gets thrown out...
	sscanf(MOSReport.c_str() + CharPos, "/%4s %*2d %n", MonStr, &CharCount);
	if (strcmp(MonStr, myHourLabel.c_str()) == 0)
	{
		// This is just in case the MonStr accidentially grabs the next label
		CharCount = 0;
	}

	while (CharCount != 0)
	{
		CharPos += CharCount;
		CharCount = 0;
		memset(MonStr, '\0', 4);	// only need to memset the first four characters.

		sscanf(MOSReport.c_str() + CharPos, "/%4s %*2d %n", MonStr, &CharCount);

		if (strcmp(MonStr, myHourLabel.c_str()) == 0)
	        {
			// This is just in case the MonStr accidentially grabs the next label.
        	        CharCount = 0;
        	}
	}


	CharCount = 0;
	// Sometimes, there might be a trailing slash that wouldn't be found above
	sscanf(MOSReport.c_str() + CharPos, "/ %n", &CharCount);
	CharPos += CharCount;

	// At this point, CharPos should be the position of the first character of the
	// hour label.

	// Each Load function should set CharPos to the position of the first character
        // of the next label

        // Each of the substr calls you see in these if statements are trying to find the
        // space character after the label, in order to verify the line


	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myHourLabel)
	{
		LoadHours(MOSReport, CharPos);

		// Until proven otherwise, this is a valid MOS
		myIsValid = true;
	}
	else
	{
		myIsValid = false;
		return(false);
	}

	// Each Load function should set CharPos to the position of the first character
	// of the next label

	// Each of the substr calls you see in these if statements are trying to find the
	// space character after the label, in order to verify the line


	// Determine if the extreme line is Maximum temperature first or Minimum temperature first
	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myMinMaxLabel)
	{
		LoadExtremeTemps(MOSReport, CharPos, myMinTemps, myMaxTemps, 999);
	}
	else if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myMaxMinLabel)
	{
		LoadExtremeTemps(MOSReport, CharPos, myMaxTemps, myMinTemps, 999);
	}

	
	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myTemperatureLabel)
        {
		LoadTableRow(MOSReport, CharPos, myTemperatures, 999);
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myDewPointLabel)
        {
		LoadTableRow(MOSReport, CharPos, myDewPoints, 999);
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myCloudLabel)
        {
		LoadTableRow(MOSReport, CharPos, myCloudCovers);
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myWindDirLabel)
        {
		LoadTableRow(MOSReport, CharPos, myWindDirs, 99);	// 99 is the error number, not the default 999
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myWindSpeedLabel)
        {
		LoadTableRow(MOSReport, CharPos, myWindSpeeds, 99);	// 99 is the error number, not the default 999
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myPOP6Label)
        {
		LoadTableRow(MOSReport, CharPos, myProbOfPrecips_6hr);
	}

	if (MOSReport.substr(CharPos, MOSReport.find(' ', CharPos) - CharPos) == myPOP12Label)
        {
		LoadTableRow(MOSReport, CharPos, myProbOfPrecips_12hr);
	}


	

	}
	catch (...)
	{
		cerr << "ERROR: Exception Caught during decoding MOS\n";
		myIsValid = false;
		throw;
	}
		
	
	return(myIsValid);
}



void BMOS::LoadHours(const string &MOSReport, size_t &CharPos)
{
	size_t CharCount = 0;
	// Jumping past the Hour label to the first non-whitespace character
	sscanf(MOSReport.c_str() + CharPos, "%*4s %n", &CharCount);

        // sort of like an offset for the beginning of the line to find data.
        // Should be the position of the space character before the first hour.
	// It indicates the number of characters from the first character of
	// label to the first character of the first value position.
        myTableStartPos = CharCount - 1;

	CharPos += myTableStartPos;
        int HourVal;

        CharCount = 0;
	int HourOffset = myTime.tm_hour;

        while (sscanf(MOSReport.c_str() + CharPos, " %3d %n", &HourVal, &CharCount) == 1)
        {
                CharPos += CharCount;
                CharCount = 0;

		if (HourVal == 0)
		{
			HourOffset -= 24;
		}

		myHours.push_back(HourVal - HourOffset);
        }
}

void BMOS::LoadExtremeTemps(const string &MOSReport, size_t &CharPos, map <int, float> &Array_1, map <int, float> &Array_2, const int ErrNum)
{
	CharPos += myTableStartPos;
        int SomeVal;

	char TempStr[4];
        memset(TempStr, '\0', 4);
        char* EndPoint = NULL;


        size_t CharCount = 0;
	
	bool DoArray_1 = true;

	for (size_t Index = 0; Index < myHours.size(); Index++)
        {
                // The CharCount is just in case the sscanf can only pick up less than three characters.
                if (sscanf(MOSReport.c_str() + CharPos, "%3c%n", TempStr, &CharCount) != 1)
                {
                        // most likely, we reached the end of the line...
                        break;
                }

                SomeVal = strtol(TempStr, &EndPoint, 10);

                if (TempStr != EndPoint)
                {
			float FloatVal = ((SomeVal != ErrNum) ? (float) SomeVal : nanf("nan"));

			if (DoArray_1)
			{
                       		Array_1.insert( make_pair(myHours[Index],
	                               			  FloatVal) );
			}
			else
			{
				Array_2.insert( make_pair(myHours[Index],
                	                                  FloatVal) );
			}

			DoArray_1 = !DoArray_1;
		}
                
		memset(TempStr, '\0', 3);
                EndPoint = NULL;

                CharPos += CharCount;
        }

	// This little bit gets us to the beginning of the next line.
        CharCount = 0;
        sscanf(MOSReport.c_str() + CharPos, " %n", &CharCount);
        CharPos += CharCount;
}



void BMOS::LoadTableRow(const string &MOSReport, size_t &CharPos, map <int, int> &TheArray)
{
	CharPos += myTableStartPos;
	char TempStr[4];
	memset(TempStr, '\0', 4);
	char* EndPoint = NULL;

	int SomeVal;

	size_t CharCount = 0;

	for (size_t Index = 0; Index < myHours.size(); Index++)
	{
		// The CharCount is just in case the sscanf can only pick up less than three characters.
		if (sscanf(MOSReport.c_str() + CharPos, "%3c%n", TempStr, &CharCount) != 1)
		{
			// most likely, we reached the end of the line...
			break;
		}

		SomeVal = strtol(TempStr, &EndPoint, 10);

		if (TempStr != EndPoint)
		{
			TheArray.insert( make_pair(myHours[Index],
					           SomeVal) );
		}

		memset(TempStr, '\0', 3);
		EndPoint = NULL;

		CharPos += CharCount;
        }

	// This little bit gets us to the beginning of the next line.
        CharCount = 0;
        sscanf(MOSReport.c_str() + CharPos, " %n", &CharCount);
        CharPos += CharCount;
}


void BMOS::LoadTableRow(const string &MOSReport, size_t &CharPos, map <int, string> &TheArray)
{
	CharPos += myTableStartPos;
	char SomeVal[4];
	memset(SomeVal, '\0', 4);

	size_t CharCount = 0;

	for (size_t Index = 0; Index < myHours.size(); Index++)
	{
		if (sscanf(MOSReport.c_str() + CharPos, "%3c%n", SomeVal, &CharCount) != 1)
		{
			// most likely, we reached the end of the line...
			break;
		}

		TheArray.insert( make_pair(myHours[Index],
				           SomeVal) );
		memset(SomeVal, '\0', 3);

		CharPos += CharCount;
	}

	// This little bit gets us to the beginning of the next line.
	CharCount = 0;
	sscanf(MOSReport.c_str() + CharPos, " %n", &CharCount);
	CharPos += CharCount;
}


void BMOS::LoadTableRow(const string &MOSReport, size_t &CharPos, map <int, float> &TheArray, const int ErrNum)
{
	CharPos += myTableStartPos;
        char TempStr[4];
        memset(TempStr, '\0', 4);
        char* EndPoint = NULL;

	int SomeVal;

        size_t CharCount = 0;

        for (size_t Index = 0; Index < myHours.size(); Index++)
        {
                // The CharCount is just in case the sscanf can only pick up less than three characters.
                if (sscanf(MOSReport.c_str() + CharPos, "%3c%n", TempStr, &CharCount) != 1)
                {
                        // most likely, we reached the end of the line...
                        break;
                }

                SomeVal = strtol(TempStr, &EndPoint, 10);

                if (TempStr != EndPoint)
                {
			if (SomeVal != ErrNum)
			{
				TheArray.insert( make_pair(myHours[Index],
                                                           SomeVal) );
			}
			else
			{
				TheArray.insert( make_pair(myHours[Index],
							   nanf("nan")) );
			}
                }

                memset(TempStr, '\0', 3);
                EndPoint = NULL;

                CharPos += CharCount;
        }

        // This little bit gets us to the beginning of the next line.
        CharCount = 0;
        sscanf(MOSReport.c_str() + CharPos, " %n", &CharCount);
        CharPos += CharCount;
}



string BMOS::Encode() const
{
	return(myStationID + "     " + myModelType + " MOS GUIDANCE  " + EncodeTime() + " UTC\n"
	       + EncodeHours() + '\n' + EncodeExtremeTemps(999) + '\n' + EncodeTableRow(myTemperatureLabel, myTemperatures, 999) + '\n'
	       + EncodeTableRow(myDewPointLabel, myDewPoints, 999) + '\n' + EncodeTableRow(myCloudLabel, myCloudCovers) + '\n'
	       + EncodeTableRow(myWindDirLabel, myWindDirs, 99) + '\n' + EncodeTableRow(myWindSpeedLabel, myWindSpeeds, 99) + '\n'
	       + EncodeTableRow(myPOP6Label, myProbOfPrecips_6hr) + '\n' + EncodeTableRow(myPOP12Label, myProbOfPrecips_12hr));
}


void BMOS::TakeTime(const struct tm &MOSTime)
// may need to be more advanced, so as to replace default values?
{
	myTime = MOSTime;
}


bool BMOS::DecodeTime(const string &MOSTime)
{
	char* CharPoint;
	struct tm NewTime;
	NewTime.tm_sec = 0;
	
	if (MOSTime.length() >= 14)
	{
		if ((CharPoint = strptime(MOSTime.c_str(), "%m/%d/%Y %H%M", &NewTime)) == NULL)
		{
			return(false);
		}
	}
	else if (MOSTime.length() >= 12)
	{
		if ((CharPoint = strptime(MOSTime.c_str(), "%m/%d/%y %H%M", &NewTime)) == NULL)
		{
			return(false);
		}
	}
	else
	{
		return(false);
	}

	
	if (*CharPoint != '\0')
	{
		// it was an incomplete parsing...
		return(false);
	}
	
	myTime = NewTime;
	
	return(true);
}



struct tm BMOS::GiveTime() const
{
	return(myTime);
}


bool BMOS::IsValid() const
{
	return(myIsValid);
}


string BMOS::GiveStationID() const
{
	return(myStationID);
}

string BMOS::GiveModelType() const
{
	return(myModelType);
}


vector <int> BMOS::GiveHours() const
{
	return(myHours);
}

float BMOS::GiveMinTemp(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myMinTemps));
}

float BMOS::GiveMaxTemp(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myMaxTemps));
}


vector <float> BMOS::GiveMinTemps(const vector <int> &ForecastHours) const
{
	return(GiveVals(ForecastHours, myMinTemps));
}

vector <float> BMOS::GiveMaxTemps(const vector <int> &ForecastHours) const
{
	return(GiveVals(ForecastHours, myMaxTemps));
}


float BMOS::GiveTemperature(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myTemperatures));
}

vector <float> BMOS::GiveTemperatures(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myTemperatures));
}


float BMOS::GiveDewPoint(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myDewPoints));
}


vector <float> BMOS::GiveDewPoints(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myDewPoints));
}




string BMOS::GiveCloudCover(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myCloudCovers));
}


vector <string> BMOS::GiveCloudCovers(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myCloudCovers));
}



float BMOS::GiveWindDir(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myWindDirs));
}


vector <float> BMOS::GiveWindDirs(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myWindDirs));
}


float BMOS::GiveWindSpeed(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myWindSpeeds));
}

vector <float> BMOS::GiveWindSpeeds(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myWindSpeeds));
}


int BMOS::GiveProbOfPrecip_6hr(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myProbOfPrecips_6hr));
}

vector <int> BMOS::GiveProbOfPrecips_6hr(const vector <int> &ForecastHours) const
{
	return(GiveVals(ForecastHours, myProbOfPrecips_12hr));
}


int BMOS::GiveProbOfPrecip_12hr(const int ForecastHour) const
{
	return(GiveVal(ForecastHour, myProbOfPrecips_6hr));
}


vector <int> BMOS::GiveProbOfPrecips_12hr(const vector <int> &ForecastHours) const
{
        return(GiveVals(ForecastHours, myProbOfPrecips_12hr));
}


int BMOS::GiveVal(const int ForecastHour, const map <int, int> &TheArray) const
{
	map<int, int>::const_iterator AMatch = TheArray.find(ForecastHour);

        if (AMatch != TheArray.end())
        {
                return(AMatch->second);
        }
        else
        {
                throw((string) "BMOS::GiveVal(): Invalid forecast hour");
	}
}

float BMOS::GiveVal(const int ForecastHour, const map <int, float> &TheArray) const
{
	map<int, float>::const_iterator AMatch = TheArray.find(ForecastHour);

        if (AMatch != TheArray.end())
        {
                return(AMatch->second);
        }
        else
        {
                return(nanf("NAN"));
        }
}

string BMOS::GiveVal(const int ForecastHour, const map <int, string> &TheArray) const
{
	map<int, string>::const_iterator AMatch = TheArray.find(ForecastHour);

        if (AMatch != TheArray.end())
        {
                return(AMatch->second);
        }
        else
        {
                return("");
        }
}


vector <int> BMOS::GiveVals(const vector <int> &ForecastHours, const map <int, int> &TheArray) const
{
	vector <int> TheValues(ForecastHours.size());

        try
        {
                vector<int>::const_iterator AnHour = ForecastHours.begin();
                for (vector<int>::iterator AVal = TheValues.begin(); AVal != TheValues.end(); AVal++)
                {
                        *AVal = GiveVal(*AnHour++, TheArray);
                }
        }
        catch (...)
        {
		// Probably not the best idea...

                throw((string) "BMOS::GiveVals(): An invalid forecast hour");
        }

	return(TheValues);
}

vector <float> BMOS::GiveVals(const vector <int> &ForecastHours, const map<int, float> &TheArray) const
{
	vector <float> TheValues(ForecastHours.size());

        vector<int>::const_iterator AnHour = ForecastHours.begin();
        for (vector<float>::iterator AVal = TheValues.begin(); AVal != TheValues.end(); AVal++)
        {
                *AVal = GiveVal(*AnHour++, TheArray);
        }

        return(TheValues);
}

vector <string> BMOS::GiveVals(const vector <int> &ForecastHours, const map<int, string> &TheArray) const
{
	vector <string> TheValues(ForecastHours.size());

        vector<int>::const_iterator AnHour = ForecastHours.begin();
        for (vector<string>::iterator AVal = TheValues.begin(); AVal != TheValues.end(); AVal++)
        {
                *AVal = GiveVal(*AnHour++, TheArray);
        }

        return(TheValues);
}



string BMOS::EncodeTime() const
{
	char TimeStr[16];
	memset(TimeStr, '\0', 16);

	strftime(TimeStr, 16, "%m/%d/%Y %H%M", &myTime);
	return(TimeStr);
}

string BMOS::EncodeHours() const
{
	if (myHours.empty())
	{
		return("");
	}

	char LineStr[74];
        memset(LineStr, '\0', 74);

        size_t CharPos = snprintf(LineStr, 74, "%*s", -((int) myTableStartPos), myHourLabel.c_str());

	for (vector<int>::const_iterator AnHour = myHours.begin(); AnHour != myHours.end(); AnHour++)
	{
		CharPos += snprintf(LineStr + CharPos, 71 - CharPos, "%3.2d", (*AnHour + myTime.tm_hour) % 24);
	}

	return(LineStr);
}

string BMOS::EncodeExtremeTemps(const int ErrNum) const
{
	if (myMinTemps.empty() || myMaxTemps.empty())
	{
		return("");
	}

	string LineLabel;
	// Checking to see which map has the first temperature
	// with a lower hour offset, indicating that it is first.
	if (myMinTemps.begin()->first < myMaxTemps.begin()->first)
	{
		LineLabel = myMinMaxLabel;
	}
	else
	{
		LineLabel = myMaxMinLabel;
	}

	char LineStr[74];
	memset(LineStr, ' ', 73);
	*(LineStr + 73) = '\0';

	snprintf(LineStr, 74, "%*s", -((int) myTableStartPos), LineLabel.c_str());
	*(LineStr + myTableStartPos) = ' ';

	for (map<int, float>::const_iterator ATemp = myMinTemps.begin(); ATemp != myMinTemps.end(); ATemp++)
	{
		const size_t Pos = ((lower_bound(myHours.begin(), myHours.end(), ATemp->first) - myHours.begin()) * 3) + myTableStartPos;

		if (Pos > 70)
		{
			continue;
		}

		if (isnan(ATemp->second))
		{
			snprintf(LineStr + Pos, 74 - Pos, "%3.2d", ErrNum);
		}
		else
		{
			snprintf(LineStr + Pos, 74 - Pos, "%3.2d", (int) ATemp->second);
		}

		*(LineStr + Pos + 3) = ' ';
	}

	for (map<int, float>::const_iterator ATemp = myMaxTemps.begin(); ATemp != myMaxTemps.end(); ATemp++)
	{
		const size_t Pos = ((lower_bound(myHours.begin(), myHours.end(), ATemp->first) - myHours.begin()) * 3) + myTableStartPos;

		if (Pos > 70)
		{
			continue;
		}

                if (isnan(ATemp->second))
                {
                        snprintf(LineStr + Pos, 74 - Pos, "%3.2d", ErrNum);
                }
                else
                {
                        snprintf(LineStr + Pos, 74 - Pos, "%3.2d", (int) ATemp->second);
                }

		*(LineStr + Pos + 3) = ' ';
	}

	return(LineStr);
}

string BMOS::EncodeTableRow(const string &RowLabel, const map<int, int> &TheVals) const
{
	if (myHours.empty())
	{
		return("");
	}

	char LineStr[74];
	memset(LineStr, ' ', 73);
	*(LineStr + 73) = '\0';

	snprintf(LineStr, 74, "%*s", -((int) myTableStartPos), RowLabel.c_str());
	*(LineStr + myTableStartPos) = ' ';
	
	for (map<int, int>::const_iterator AVal = TheVals.begin(); AVal != TheVals.end(); AVal++)
        {
                const size_t Pos = ((lower_bound(myHours.begin(), myHours.end(), AVal->first) - myHours.begin()) * 3) + myTableStartPos;

                if (Pos > 70)
                {
                        continue;
                }
		
		snprintf(LineStr + Pos, 74 - Pos, "%3d", AVal->second);
		*(LineStr + Pos + 3) = ' ';
        }

	return(LineStr);
}


string BMOS::EncodeTableRow(const string &RowLabel, const map<int, float> &TheVals, const int ErrNum) const
{
	if (myHours.empty())
	{
		return("");
	}

	char LineStr[74];
        memset(LineStr, ' ', 73);
        *(LineStr + 73) = '\0';

        snprintf(LineStr, 74, "%*s", -((int) myTableStartPos), RowLabel.c_str());
	*(LineStr + myTableStartPos) = ' ';

        for (map<int, float>::const_iterator AVal = TheVals.begin(); AVal != TheVals.end(); AVal++)
        {
                const size_t Pos = ((lower_bound(myHours.begin(), myHours.end(), AVal->first) - myHours.begin()) * 3) + myTableStartPos;

                if (Pos > 70)
                {
                        continue;
                }

		if (isnan(AVal->second))
		{
			snprintf(LineStr + Pos, 74 - Pos, "%3.2d", ErrNum);
		}
		else
		{
                	snprintf(LineStr + Pos, 74 - Pos, "%3.2d", (int) AVal->second);
		}

		*(LineStr + Pos + 3) = ' ';
        }

        return(LineStr);
}


string BMOS::EncodeTableRow(const string &RowLabel, const map<int, string> &TheVals) const
{
	if (myHours.empty())
        {
                return("");
        }

        char LineStr[74];
        memset(LineStr, ' ', 73);
        *(LineStr + 73) = '\0';

        snprintf(LineStr, 74, "%*s", -((int) myTableStartPos), RowLabel.c_str());
	*(LineStr + myTableStartPos) = ' ';

        for (map<int, string>::const_iterator AVal = TheVals.begin(); AVal != TheVals.end(); AVal++)
        {
                const size_t Pos = ((lower_bound(myHours.begin(), myHours.end(), AVal->first) - myHours.begin()) * 3) + myTableStartPos;

                if (Pos > 70)
                {
                        continue;
                }

                snprintf(LineStr + Pos, 74 - Pos, "%3.3s", AVal->second.c_str());

		*(LineStr + Pos + 3) = ' ';
        }

        return(LineStr);
}


bool operator < (const BMOS &LeftMOS, const BMOS &RightMOS)
{
	struct tm LeftTime(LeftMOS.myTime), RightTime(RightMOS.myTime);

	return(timegm(&LeftTime) < timegm(&RightTime)
           || (timegm(&LeftTime) == timegm(&RightTime)
               && (LeftMOS.myStationID < RightMOS.myStationID
               || (LeftMOS.myStationID == RightMOS.myStationID
                   && LeftMOS.myModelType < RightMOS.myModelType
		  ))
              ));
}


bool operator > (const BMOS &LeftMOS, const BMOS &RightMOS)
{
	struct tm LeftTime(LeftMOS.myTime), RightTime(RightMOS.myTime);

        return(timegm(&LeftTime) > timegm(&RightTime)
           || (timegm(&LeftTime) == timegm(&RightTime)
               && (LeftMOS.myStationID > RightMOS.myStationID
               || (LeftMOS.myStationID == RightMOS.myStationID
                   && LeftMOS.myModelType > RightMOS.myModelType
                  ))
              ));
}



#endif
