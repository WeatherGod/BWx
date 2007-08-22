#ifndef _BMETAR_C
#define _BMETAR_C
using namespace std;

/*
 *  BMetar.C
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <ctime>		// for strftime(), strptime()
#include <cstdio>		// for sscanf(), snprintf()
#include <cmath>		// for nan() and isnan()

#include "WeatherType.h"	// for WeatherType info
#include "CloudType.h"		// for CloudType info

#include "BMetar.h"

BMetar::BMetar()
	:	myIsValid(false),
		myTime(),
		myWindFrom(-1),
		myMinWindDir(-1),
		myMaxWindDir(-1),
		myRunway(-1),
		myRecentWeathers(0),
		myWindSpeed(nan("NAN")),
		myWindGusts(nan("NAN")),
		myVisibility(nan("NAN")),
		myRunwayVisRange(nan("NAN")),
		myTemperature(nan("NAN")),
		myDewPoint(nan("NAN")),
		myStationPressure(nan("NAN")),
		myAltimeterSet(nan("NAN")),
		myIsNil(false),
		myIsAuto(false),
		myIsCorrection(false),
		myIsDirVariable(false),
		myIsCAVOK(false),
		myIsRVRMissing(false),
		myIsNoSig(false),
		myWeathers(0),
		myClouds(0),
		myRemarks(0),
		myStationID(""),
		myCodeType("")
{
	myTime.tm_year = 70;
        myTime.tm_mon = 0;
        myTime.tm_mday = 1;
        myTime.tm_hour = 0;
        myTime.tm_min = 0;
        myTime.tm_sec = 0;
}

BMetar::BMetar(const BMetar &MetarCopy)
	:	myIsValid( MetarCopy.myIsValid ),
		myTime( MetarCopy.myTime ),
		myWindFrom( MetarCopy.myWindFrom ),
		myMinWindDir( MetarCopy.myMinWindDir ),
		myMaxWindDir( MetarCopy.myMaxWindDir ),
		myRunway( MetarCopy.myRunway ),
		myRecentWeathers( MetarCopy.myRecentWeathers ),
		myWindSpeed( MetarCopy.myWindSpeed ),
		myWindGusts( MetarCopy.myWindGusts ),
		myVisibility( MetarCopy.myVisibility ),
		myRunwayVisRange( MetarCopy.myRunwayVisRange ),
		myTemperature( MetarCopy.myTemperature ),
		myDewPoint( MetarCopy.myDewPoint ),
		myAltimeterSet( MetarCopy.myAltimeterSet ),
		myStationPressure( MetarCopy.myStationPressure ),
		myIsNil( MetarCopy.myIsNil ),
		myIsAuto( MetarCopy.myIsAuto ),
		myIsCorrection( MetarCopy.myIsCorrection ),
		myIsDirVariable( MetarCopy.myIsDirVariable ),
		myIsCAVOK( MetarCopy.myIsCAVOK ),
		myIsRVRMissing( MetarCopy.myIsRVRMissing ),
		myIsNoSig( MetarCopy.myIsNoSig ),
		myWeathers( MetarCopy.myWeathers ),
		myClouds( MetarCopy.myClouds ),
		myRemarks( MetarCopy.myRemarks ),
		myStationID( MetarCopy.myStationID ),
		myCodeType( MetarCopy.myCodeType )
{
}

BMetar::BMetar(const string &MetarString)
	:	myIsValid(false),
		myTime(),
		myWindFrom(-1),
		myMinWindDir(-1),
		myMaxWindDir(-1),
		myRunway(-1),
		myRecentWeathers(0),
		myWindSpeed(nan("NAN")),
		myWindGusts(nan("NAN")),
		myVisibility(nan("NAN")),
		myRunwayVisRange(nan("NAN")),
		myTemperature(nan("NAN")),
		myDewPoint(nan("NAN")),
		myAltimeterSet(nan("NAN")),
		myStationPressure(nan("NAN")),
		myIsNil(false),
		myIsAuto(false),
		myIsCorrection(false),
		myIsDirVariable(false),
		myIsCAVOK(false),
		myIsRVRMissing(false),
		myIsNoSig(false),
		myWeathers(0),
		myClouds(0),
		myRemarks(0),
		myStationID(""),
		myCodeType("")
{
	myTime.tm_year = 70;
	myTime.tm_mon = 0;
	myTime.tm_mday = 1;
	myTime.tm_hour = 0;
	myTime.tm_min = 0;
	myTime.tm_sec = 0;
	Decode(MetarString);
}


BMetar::BMetar(const string &MetarString, const struct tm &ATime, const string &CodeTypeGuess)
        :       myIsValid(false),
		myTime(ATime),
                myWindFrom(-1),
                myMinWindDir(-1),
                myMaxWindDir(-1),
                myRunway(-1),
                myRecentWeathers(0),
                myWindSpeed(nan("NAN")),
                myWindGusts(nan("NAN")),
                myVisibility(nan("NAN")),
                myRunwayVisRange(nan("NAN")),
                myTemperature(nan("NAN")),
                myDewPoint(nan("NAN")),
                myAltimeterSet(nan("NAN")),
                myStationPressure(nan("NAN")),
                myIsNil(false),
                myIsAuto(false),
                myIsCorrection(false),
                myIsDirVariable(false),
                myIsCAVOK(false),
                myIsRVRMissing(false),
                myIsNoSig(false),
                myWeathers(0),
                myClouds(0),
                myRemarks(0),
                myStationID(""),
                myCodeType("")
{
	TakeCodeType(CodeTypeGuess);
        Decode(MetarString);
}


bool BMetar::IsCodeType(const string &CodeStr) const
{
	return(CodeStr == "METAR" || CodeStr == "SPECI");
}

bool BMetar::IsStationID(const string &StationID) const
{
	// must be 4 characters long...
	if (StationID.length() != 4)
	{
		return(false);
	}

	// First char must be a letter.
	if (isalpha(StationID[0]) == 0)
	{
		return(false);
	}

	// The last 3 characters must be an alphanumeric character.
	return(isalnum(StationID[1]) != 0 && isalnum(StationID[2]) != 0 && isalnum(StationID[3]) != 0);
}

bool BMetar::TakeCodeType(const string &CodeType)
{
	if (IsCodeType(CodeType))
	{
		myCodeType = CodeType;
		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMetar::TakeStationID(const string &StationID)
{
	if (IsStationID(StationID))
	{
		myStationID = StationID;
		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMetar::Decode(const string &MetarReport)
{
	const vector <string> MetarPieces = SplitBySpaces(MetarReport);
	
	if (MetarPieces.empty())
	{
		// failure in separating the MetarReport into enough pieces...
		//cerr << "ERROR: Empty split..." << endl;
		myIsValid = false;
		return(false);
	}
	
	vector<string>::const_iterator APiece = MetarPieces.begin();

	try
	{
	
	
	if (!TakeCodeType(*APiece))
	{
		// Assume METAR
		myCodeType = "METAR";
	}
	
	
	// Sometimes there are a bunch of junk at the beginning of a METAR, so
	// try multiple times to get the station ID.
	for (size_t TryCount = 0; TryCount < 4 && APiece != MetarPieces.end(); TryCount++)
	{
		if (TakeStationID(*APiece++))
		{
			break;
		}
	}

	if (myStationID.empty())
	{
		// not a valid station ID where expected.  This
		// might be a broken metar report...
		//cerr << "ERROR: Could not find valid stationID..." << endl;
		myIsValid = false;
		return(false);
	}
	
	if (APiece != MetarPieces.end())
	{
		if (*APiece == "NIL")
		{
			myIsValid = true;
			myIsNil = true;
			return(true);
		}
		else if (*APiece == "COR")
		{
			myIsCorrection = true;
			APiece++;
		}
	}
	
	if (APiece != MetarPieces.end())
	{
		if (DecodeTime(*APiece))
		{
			APiece++;
		}
		else
		{
			// since I didn't find the time info, which is required, return false...
			myIsValid = false;
			return(false);
		}
	}
	
	if (APiece != MetarPieces.end())
	{
		if (*APiece == "NIL")
		{
			myIsValid = true;
			myIsNil = true;
			return(true);
		}
		else if (*APiece == "AUTO")
		{
			myIsAuto = true;
			APiece++;
		}
	}

	
	if (APiece != MetarPieces.end())
	{
		// time for second check for the COR value...
		if (*APiece == "COR")
		{
			myIsCorrection = true;
			APiece++;
		}
	}

	if (APiece != MetarPieces.end())
	{
		// Have no clue what this is, oh well...
		if (*APiece == "RTD") APiece++;
	}
	
	if (APiece != MetarPieces.end())
	{
		if (DecodeWind(*APiece)) APiece++;
	}
	
	if (APiece != MetarPieces.end())
	{
		if (DecodeMinMaxWindDir(*APiece)) APiece++;
	}
	
	if (APiece != MetarPieces.end())
	{
		if (*APiece == "CAVOK")
		{
			myIsCAVOK = true;
			APiece++;
		}
	}
	
	if (APiece != MetarPieces.end())
	{
		// Sometimes, it takes two "words" for the visibility...
		// Stupid standards...
		
		if (DecodeVisibility(*APiece))
		{
			APiece++;
		}
		else if ((APiece + 1) != MetarPieces.end())
		{
			if (DecodeVisibility(*APiece + ' ' + *(APiece + 1)))
			{
				APiece += 2;
			}
		}
	}

	if (APiece != MetarPieces.end())
	{
		// Sometimes, there will be two visibilities given for different
		// directions.  This is international standard,
		// not the US, so I don't need to worry about fractions.

		// Note that the way the code is set up, I will not differentiate
		// between the two.  The last one will be considered as the
		// visibility.  This is temporary...

		if (DecodeVisibility(*APiece)) APiece++;
	}
	
	// I will just simply retain the last runway info...
	// this is also temporary...
	while (APiece != MetarPieces.end() && DecodeRVR(*APiece))
	{
		APiece++;
	}
	
	while (APiece != MetarPieces.end() && DecodeWeather(*APiece))
	{
		APiece++;
	}
	
	while (APiece != MetarPieces.end() && DecodeCloud(*APiece))
	{
		APiece++;
	}
	
	if (APiece != MetarPieces.end())
	{
		if (DecodeTemperature_DewPoint(*APiece)) APiece++;
	}
	
	if (APiece != MetarPieces.end())
	{
		if (DecodePressure(*APiece)) APiece++;
	}

	if (APiece != MetarPieces.end())
	{
		if (DecodePressure(*APiece)) APiece++;
	}

	while (APiece != MetarPieces.end() && DecodeRecentWeather(*APiece))
	{
		APiece++;
	}

	if (APiece != MetarPieces.end())
	{
		if (*APiece == "BLU" || *APiece == "WHT" || *APiece == "GRN" || 
		    *APiece == "YLO" || *APiece == "AMB" || *APiece == "RED" ||
		    *APiece == "BLACK")
		{
			// this is for British and American Air Force bases.
			// They sometimes report a color indicating the
			// "fitness" of the airfield.  Blue is best while 
			// Red is worst.  Black is for a closed airfield for 
			// non-weather reasons.
			// The "fitness" is based maily on the level of the lowest cloud base
			// I am not recording this info, for now...
			APiece++;
		}
	}

	while (APiece != MetarPieces.end() && DecodeWindShear(*APiece))
	{
		APiece++;
	}



	if (APiece != MetarPieces.end())
	{
		if (*APiece == "NOSIG" || *APiece == "NSG")
		{
			myIsNoSig = true;
			APiece++;
		}
		// I really don't like doing this, but it will help capture a few extra metar reports that have minor errors...
		else if (*APiece == "NO")
		{
			if ((APiece + 1) != MetarPieces.end() && *(APiece + 1) == "SIG")
			{
				myIsNoSig = true;
				APiece += 2;
			}
		}
	}

	if (APiece != MetarPieces.end())
	{
		if (*APiece == "NOSPECI") APiece++;
	}
	
	// Just skipping over some trend information
	if (APiece != MetarPieces.end())
	{
		if (*APiece == "BECMG" || *APiece == "TEMPO")
		{
			do
			{
				APiece++;
			} while (APiece != MetarPieces.end() && 
				 *APiece != "RMK" &&
				 *APiece != "RMKS" &&
				 *APiece != "REMARKS");
		}
	}

	if (APiece != MetarPieces.end() && (*APiece == "RMK" || *APiece == "RMKS" || *APiece == "REMARKS"))
	{
		do
		{
			APiece++;
		}
		while (APiece != MetarPieces.end() && DecodeRemark(*APiece));
	}

	}
	catch (...)
	{
		cerr << "ERROR: Exception Caught during decoding metar:\n";
		cerr << MetarReport << endl;
		myIsValid = false;
		throw;
	}
		
	
	return(myIsValid = (APiece == MetarPieces.end()));
}

string BMetar::Encode() const
{
	string MetarReport = "";
	string Token = "";

	if (!myCodeType.empty())
	{
		MetarReport += myCodeType;
	}
	
	if (!myStationID.empty())
	{
		if (!MetarReport.empty())
		{
			MetarReport += ' ';
		}

		MetarReport += myStationID;
	}

	// At this point on, assume that there has been something written, so give space delimination.

	if (myTime.tm_mday != 0)
	{
		MetarReport += ' ' + EncodeTime();
	}
	
	if (myIsNil)
	{
		MetarReport += " NIL";
		return(MetarReport);
	}
	
	if (myIsAuto)
	{
		MetarReport += " AUTO";
	}
	
	if (myIsCorrection)
	{
		MetarReport += " COR";
	}
	
	if (!(Token = EncodeWind()).empty())
	{
		Token = ' ' + Token;
	}

	MetarReport += Token;

	if (!(Token = EncodeVisibility()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;

	if (!(Token = EncodeRVR()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;

	if (!(Token = EncodeWeathers()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;

	if (myIsCAVOK)
	{
		MetarReport += " CAVOK";
	}

	if (!(Token = EncodeClouds()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;


	if (!(Token = EncodeTemperature_DewPoint()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;

	if (!(Token = EncodePressures()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;

	if (!(Token = EncodeRecentWeathers()).empty())
	{
		Token = ' ' + Token;
	}

	MetarReport += Token;

	if (myIsNoSig)
	{
		MetarReport += " NOSIG";
	}

	if (!(Token = EncodeRemarks()).empty())
        {
                Token = ' ' + Token;
        }

        MetarReport += Token;
	
	return(MetarReport);
}


void BMetar::TakeTime(const struct tm &MetarTime)
// need to be more advanced, so as to replace default values.
{
	myTime = MetarTime;
}

void BMetar::TakeWindDir(const int WindDir)
{
	if (WindDir >= -1 && WindDir < 360)
	{
		myWindFrom = WindDir;
	}
	else if (WindDir == 360)
	{
		myWindFrom = 0;
	}
}

void BMetar::TakeWindSpeed(const float &WindSpeed)
{
	if (isnan(WindSpeed) || WindSpeed >= 0.0)
	{
		myWindSpeed = WindSpeed;
	}
}


void BMetar::TakeWindGust(const float &WindGust)
{
	if (isnan(WindGust) || WindGust >= 0.0)
	{
		myWindGusts = WindGust;
	}
}

void BMetar::TakeVisibility(const float &Visibility)
{
	if (isnan(Visibility) || Visibility >= 0.0)
	{
		myVisibility = Visibility;
	}
}

void BMetar::TakeTemperature(const float &Temperature)
{
	myTemperature = Temperature;
}

void BMetar::TakeDewPoint(const float &DewPoint)
{
	myDewPoint = DewPoint;
}

void BMetar::TakeAltimeterSetting(const float &AltimeterSet)
{
	if (isnan(AltimeterSet) || AltimeterSet >= 0.0)
	{
		myAltimeterSet = AltimeterSet;
	}
}

void BMetar::TakeStationPressure(const float &StationPress)
{
	if (isnan(StationPress) || StationPress >= 0.0)
	{
		myStationPressure = StationPress;
	}
}

bool BMetar::DecodeTime(const string &MetarTime)
{
	char* CharPoint;
	struct tm NewTime;
	NewTime.tm_sec = 0;
	NewTime.tm_mday = 1;
	NewTime.tm_mon = 0;
	NewTime.tm_year = 70;
	
	if (MetarTime.length() == 4)
	{
		if ((CharPoint = strptime(MetarTime.c_str(), "%H%M", &NewTime)) == NULL)
		{
			return(false);
		}
	
		// This is because the Timeguessing algorithm expects that the
		// date is already known.
		NewTime.tm_mday = myTime.tm_mday;
	}
	else if (MetarTime.length() == 5)
	{
		if ((CharPoint = strptime(MetarTime.c_str(), "%H%MZ", &NewTime)) == NULL)
		{
			return(false);
		}

		// This is because the Timeguessing algorithm expects that the date is already known.
		NewTime.tm_mday = myTime.tm_mday;

	}
	else if (MetarTime.length() == 6)
	{
		if ((CharPoint = strptime(MetarTime.c_str(), "%d%H%M", &NewTime)) == NULL)
		{
			return(false);
		}
	}
	else if (MetarTime.length() == 7)
	{
		if ((CharPoint = strptime(MetarTime.c_str(), "%d%H%MZ", &NewTime)) == NULL)
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
	
	myTime = GuessBestTime(NewTime, timegm(&myTime));
	
	return(true);
}


bool BMetar::DecodeWind(const string &MetarWind)
{
	int WindDir = -1;
	int WindSpeed = -1;
	int WindGust = -1;
	
	int Count = sscanf(MetarWind.c_str(), "%3d%3dG%3d", &WindDir, &WindSpeed, &WindGust);
	
	if (Count <= 1 || Count > 3)
	{
		// didn't match enough.  See if it is VRB.
		if (sscanf(MetarWind.c_str(), "VRB%3dG%3d", &WindSpeed, &WindGust) == 0)
		{
			// no match here, either.  Oh, well!
			return(false);
		}
		else
		{
			myIsDirVariable = true;
		}
	}
	else
	{
		TakeWindDir(WindDir);
	}
	
	float ConversionFactor;		// Store wind data in meters per second.

	if (MetarWind.rfind("MPH") != string::npos)
	{
		// miles per hour
		ConversionFactor = 0.868976;
	}
	else if (MetarWind.rfind("KT") != string::npos)
	{
		// Knots
		ConversionFactor = 0.539957;
	}
	else if (MetarWind.rfind("KMH") != string::npos)
	{
		// Kilometers per hour
		ConversionFactor = 0.277778;
	}
	else if (MetarWind.rfind("MPS") != string::npos)
	{
		// Meters per second
		ConversionFactor = 1.0;
	}
	else
	{
		return(false);
	}
	
	
	TakeWindSpeed(ConversionFactor * (float) WindSpeed);
	TakeWindGust(ConversionFactor * (float) WindGust);
	
	return(true);
}

bool BMetar::DecodeMinMaxWindDir(const string &MetarWindDir)
{
	int MinDir = -1;
	int MaxDir = -1;
	
	if (sscanf(MetarWindDir.c_str(), "%3dV%3d", &MinDir, &MaxDir) == 2)
	{
		myMinWindDir = MinDir;
		myMaxWindDir = MaxDir;
		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMetar::DecodeVisibility(const string &MetarVisibility)
{
	if (MetarVisibility.empty())
	{
		return(false);
	}

	size_t UnitLocation;
	float ConversionFactor;		// store visibility in meters
	float Distance;
	size_t CharPos = 0;

	if (MetarVisibility[0] == 'P' || MetarVisibility[0] == 'M' ||
	    MetarVisibility[0] == '>' || MetarVisibility[0] == '<')
	{
		// the amount is above or below instrument threshold.  Not recording
		// this info, for now...
		CharPos++;
	}
	
	if ((UnitLocation = MetarVisibility.rfind("SM")) != string::npos)
	{
		ConversionFactor = 1609.344;

		// note that if something goes wrong, Distance will have a value of NaN.
		Distance = ParseFraction(MetarVisibility.substr(CharPos, UnitLocation));

		CharPos = UnitLocation + 2;
	}
	else if ((UnitLocation = MetarVisibility.rfind("KM")) != string::npos)
	{
		ConversionFactor = 1000.0;
		char* TempPoint;
		int Dist = strtol(MetarVisibility.c_str() + CharPos, &TempPoint, 10);

		if (UnitLocation == (TempPoint - MetarVisibility.c_str()))
		{
			CharPos = UnitLocation + 2;
			Distance = (float) Dist;
		}
	}
	else
	{
		if (MetarVisibility.length() < 4)
		{
			return(false);
		}

		int Dist;
		char* CharPoint;
				
		Dist = strtol(MetarVisibility.c_str(), &CharPoint, 10);

		if (CharPoint > MetarVisibility.c_str())
		{
			CharPos += (CharPoint - MetarVisibility.c_str());
		}
		else
		{
			// could not decode into a number...
			return(false);
		}


		if (CharPos + 3 <= MetarVisibility.length() && (strncmp(CharPoint, "NDV", 3) == 0))
		{
			CharPoint += 3;
			CharPos += 3;
		}
		else
		{
			
			/* There are 8 possible combinations of values for cardinal directions:
				N
				S
				NW
				NE
				SW
				SE
				W
				E

			  Note that North and South HAVE to be first, while West and East are either first (by themselves) or second.
			  So, there only needs to be two if statements like below for the most efficient coding practice.
			*/

			if (CharPos != MetarVisibility.length() && (*CharPoint == 'N' || *CharPoint == 'S'))
			{
				CharPoint++;
				CharPos++;
			}

			if (CharPos != MetarVisibility.length() && (*CharPoint == 'W' || *CharPoint == 'E'))
			{
				CharPoint++;
				CharPos++;
			}
		}

		if (Dist == 9999)
		{
			Dist = 10000;
		}

		Distance = (float) Dist;
		ConversionFactor = 1.0;
	}

	if (CharPos == MetarVisibility.length())
	{
		if (isnan(Distance) || Distance < 0.0)
		{
			return(false);
		}
		else
		{
			TakeVisibility(ConversionFactor * Distance);
			return(true);
		}
	}
	else
	{
		return(false);
	}
}


bool BMetar::DecodeRVR(const string &MetarRVR)
// Note, if the runway visibility is variable, I just take the mean.
// I know it is bad, but it is just for now...
// I also need to do some bound checking...
{
	if (MetarRVR == "RVRNO")
	{
		myIsRVRMissing = true;
		return(true);
	}

	int RunwayNum(-1);
	size_t CharPos = 0;
        float ConversionFactor;         // store in feet
	int RunwayVis = -1;
	string Token = "";

	if (MetarRVR.length() < 7)
	{
		// there has to be at least 7 characters, although 9 or 10 is more typical.
//		cerr << "Not long enough...\n";
		return(false);
	}

	// must start with an 'R'
	if (MetarRVR[0] != 'R')
	{
//		cerr << "Didn't start with an 'R'\n";
		return(false);
	}
	else
	{
		CharPos++;
	}

	// the units are assumed to be meters
	ConversionFactor = 3.28084;

	char* CharPoint;
	Token = MetarRVR.substr(CharPos);

	// gotta skip the first character.
	RunwayNum = strtoul(Token.c_str(), &CharPoint, 10);

	if (CharPoint == Token.c_str())
	{
		// didn't decode the string into a number...
//		cerr << "Couldn't decode the string into a number: " << Token << endl;
		return(false);
	}
	else
	{
		CharPos += (CharPoint - Token.c_str());
	}

	if (isalpha(*CharPoint))
	{
		// just a runway designation letter...
		CharPoint++;
		CharPos++;
	}

	if (*CharPoint != '/')
	{
		// the slash wasn't where it was expected...
//		cerr << "Didn't find the slash, found this instead: " << *CharPoint << endl;
		return(false);
	}
	else
	{
		CharPoint++;
		CharPos++;
	}

	if (*CharPoint == 'P' || *CharPoint == 'M')
	{
		// just a modifier indicating whether measurement is above
		// or below the instrument's threshold.  I am NOT storing this info internally...yet...
		CharPoint++;
		CharPos++;
	}

	Token = MetarRVR.substr(CharPos);

	RunwayVis = strtol(Token.c_str(), &CharPoint, 10);

	if (CharPoint == Token.c_str())
	{
		// could not convert the string into a number...
//		cerr << "Could not convert the string into a number: " << Token << endl;
		return(false);
	}
	else
	{
		CharPos += (CharPoint - Token.c_str());
	}

	if (*CharPoint == 'U' || *CharPoint == 'D' || *CharPoint == 'N')
	{
		// a modifier indicating increasing or decreasing visibility.
		// Don't know what the character 'N' means, but it is there...
		// Not storing this info...yet...
		CharPoint++;
		CharPos++;
	}

	if (*CharPoint == 'V')
	{
		CharPoint++;
		CharPos++;

		if (*CharPoint == 'P' || *CharPoint == 'M')
	        {
        	        // just a modifier indicating whether measurement is above
                	// or below the instrument's threshold.  I am NOT storing this info internally...yet...
	                CharPoint++;
        	        CharPos++;
	        }

        	Token = MetarRVR.substr(CharPos);

	        RunwayVis += strtol(Token.c_str(), &CharPoint, 10);

	        if (CharPoint == Token.c_str())
        	{
                	// could not convert the string into a number...
//			cerr << "Could not convert the string into a number: " << Token << endl;
                	return(false);
        	}
        	else
        	{
                	CharPos += (CharPoint - Token.c_str());

			// taking an average.  Yeah, I know this is bad, especially the integer division, but I really don't care...
			RunwayVis /= 2;
        	}

	        if (*CharPoint == 'U' || *CharPoint == 'D' || *CharPoint == 'N')
        	{
                	// a modifier indicating increasing or decreasing visibility.
			// don't know what the character 'N' means, though...
	                // Not storing this info...yet...
			CharPoint++;
        	        CharPos++;
        	}
	}
	
	if (MetarRVR.substr(CharPos, 2) == "FT")
	{
		ConversionFactor = 1.0;
		CharPoint += 2;
		CharPos += 2;
	
		if (MetarRVR.substr(CharPos) == "/D" || MetarRVR.substr(CharPos) == "/U" || MetarRVR.substr(CharPos) == "/N")
		{
			CharPoint += 2;
			CharPos += 2;
		}
	}	

	if (CharPos == MetarRVR.length())
	{
		myRunway = RunwayNum;
		myRunwayVisRange = (float) RunwayVis * ConversionFactor;
		return(true);
	}
	else
	{
//		cerr << "The char position didn't match up: " << CharPos << "  length: " << MetarRVR.length() << "\n";
		return(false);
	}
}



bool BMetar::DecodeWeather(const string &MetarWeatherStr)
{
	WeatherType TempWeather;
	
	if (TempWeather.Decode(MetarWeatherStr))
	{
		myWeathers.push_back(TempWeather);
		return(true);
	}
	else
	{
		return(false);
	}
}


bool BMetar::DecodeCloud(const string &MetarCloudStr)
{
	CloudType TempCloud;

	if (TempCloud.Decode(MetarCloudStr))
	{
		myClouds.push_back(TempCloud);
		return(true);
	}
	else
	{
		return(false);
	}
}

bool BMetar::DecodeTemperature_DewPoint(const string &MetarTemp)
{
	int Temperature;
	int DewPoint;
	size_t SlashPos;
	
	if ((SlashPos = MetarTemp.find('/')) == string::npos)
	{
		return(false);
	}

	
	const string TemperatureStr = MetarTemp.substr(0, SlashPos);
	const string DewPointStr = MetarTemp.substr(SlashPos + 1);		// get the rest of the string.
	
	
	if (!DewPointStr.empty())
	{
		if (sscanf(DewPointStr.c_str(), "M%3d", &DewPoint) == 1)
		{
			DewPoint *= -1;
		}
		else if (sscanf(DewPointStr.c_str(), "%3d", &DewPoint) != 1)
		{
			return(false);
		}
	}

		
	if (!TemperatureStr.empty())
	{
		if (sscanf(TemperatureStr.c_str(), "M%3d", &Temperature) == 1)
		{
			Temperature *= -1;
		}
		else if (sscanf(TemperatureStr.c_str(), "%3d", &Temperature) != 1)
		{
			return(false);
		}
	}
	else
	{
		return(false);
	}

	// Now that I know everything is successful...
	if (!DewPointStr.empty())
	{
		TakeDewPoint((float) DewPoint);
	}
		
	TakeTemperature((float) Temperature);
	
	return(true);
}

bool BMetar::DecodePressure(const string &MetarPressure)
{
	if (MetarPressure.length() < 4)
	{
		return(false);
	}

	if (MetarPressure == "Q////" || MetarPressure == "A////")
	{
		// it is a valid Pressure group, but it has no useful values.
		return(true);
	}

	int InstrumentVal;
	char* CharPoint;
	
	if (MetarPressure[0] == 'Q')
	{
		InstrumentVal = strtol(MetarPressure.c_str() + 1, &CharPoint, 10);

		if (CharPoint > (MetarPressure.c_str() + 1))
		{
			TakeStationPressure((float) InstrumentVal);
		}
		
		return(true);
	}
	else if (MetarPressure[0] == 'A')
	{
		InstrumentVal = strtol(MetarPressure.c_str() + 1, &CharPoint, 10);

                if (CharPoint > (MetarPressure.c_str() + 1))
                {
                        TakeAltimeterSetting((float) InstrumentVal);
                }

		return(true);
	}
	else
	{
		return(false);
	}
}

bool BMetar::DecodeRecentWeather(const string &MetarWeather)
{
	size_t CharPos = 0;

	if (MetarWeather.substr(CharPos, 2) == "RE")
	{
		if (MetarWeather == "REMARKS")
		{
			return(false);
		}

		CharPos += 2;
	}

	WeatherType TempWeather;
	if (!TempWeather.Decode(MetarWeather.substr(CharPos)))
	{
		return(false);
	}
	else
	{
		myRecentWeathers.push_back(TempWeather);
		return(true);
	}
}

bool BMetar::DecodeWindShear(const string &MetarWindShear)
// I am not storing any of this info, yet...
{
	if (MetarWindShear == "WS")
	{
		return(true);
	}

	if (MetarWindShear == "TKOF" || MetarWindShear  == "LDG")
	{
		return(true);
	}

	if (MetarWindShear.substr(0, 3) == "RWY")
	{
		return(true);
	}
	
	return(false);
}

bool BMetar::DecodeRemark(const string &MetarRemark)
{
	myRemarks.push_back(MetarRemark);

	return(true);
}



struct tm BMetar::GiveTime() const
{
	return(myTime);
}


int BMetar::GiveWindDir() const
{
	return(myWindFrom);
}

int BMetar::GiveMinWindDir() const
{
	return(myMinWindDir);
}

int BMetar::GiveMaxWindDir() const
{
	return(myMaxWindDir);
}

int BMetar::GiveRunwayNum() const
{
	return(myRunway);
}

float BMetar::GiveWindSpeed() const
{
	return(myWindSpeed);
}

float BMetar::GiveWindGust() const
{
	return(myWindGusts);
}

float BMetar::GiveVisibility() const
{
	return(myVisibility);
}

float BMetar::GiveRunwayVisualRange() const
{
	return(myRunwayVisRange);
}

float BMetar::GiveTemperature() const
{
	return(myTemperature);
}

float BMetar::GiveDewPoint() const
{
	return(myDewPoint);
}

float BMetar::GiveAltimeterSetting() const
{
	return(myAltimeterSet);
}

float BMetar::GiveStationPressure() const
{
	return(myStationPressure);
}

bool BMetar::IsValid() const
{
	return(myIsValid);
}

bool BMetar::IsNil() const
{
	return(myIsNil);
}

bool BMetar::IsAuto() const
{
	return(myIsAuto);
}

bool BMetar::IsDirVariable() const
{
	return(myIsDirVariable);
}

bool BMetar::IsCorrection() const
{
	return(myIsCorrection);
}

bool BMetar::IsCAVOK() const
{
	return(myIsCAVOK);
}

bool BMetar::IsNoSig() const
{
	return(myIsNoSig);
}


void BMetar::IsAuto(const bool SomeBool)
{
	myIsAuto = SomeBool;
}

void BMetar::IsCorrection(const bool SomeBool)
{
	myIsCorrection = SomeBool;
}

void BMetar::IsDirVariable(const bool SomeBool)
{
	myIsDirVariable = SomeBool;
}

void BMetar::IsCAVOK(const bool SomeBool)
{
	myIsCAVOK = SomeBool;
}

void BMetar::IsNil(const bool SomeBool)
{
	myIsNil = SomeBool;
}


vector <WeatherType> BMetar::GiveRecentWeathers() const
{
	return(myRecentWeathers);
}

vector <WeatherType> BMetar::GiveWeatherTypes() const
{
	return(myWeathers);
}

vector <CloudType> BMetar::GiveCloudTypes() const
{
	return(myClouds);
}

vector <string> BMetar::GiveRemarks() const
{
	return(myRemarks);
}

string BMetar::GiveStationID() const
{
	return(myStationID);
}

string BMetar::GiveCodeType() const
{
	return(myCodeType);
}


void BMetar::TakeWeatherTypes(const vector <string> &WeatherStrs)
{
	for (vector<string>::const_iterator AWeather = WeatherStrs.begin();
	     AWeather != WeatherStrs.end();
	     AWeather++)
	{
		WeatherType TempWeather;

		if (TempWeather.Decode(*AWeather))
		{
			myWeathers.push_back(TempWeather);
		}
	}
}

void BMetar::TakeCloudTypes(const vector <string> &CloudStrs)
{
	for (vector<string>::const_iterator ACloud = CloudStrs.begin();
             ACloud != CloudStrs.end();
             ACloud++)
        {
                CloudType TempCloud;

                if (TempCloud.Decode(*ACloud))
                {
                        myClouds.push_back(TempCloud);
                }
        }
}

void BMetar::TakeRemarks(const vector <string> &RemarkStrs)
{
	for (vector<string>::const_iterator ARemark = RemarkStrs.begin();
             ARemark != RemarkStrs.end();
             ARemark++)
        {
                if (!ARemark->empty())
		{
			myRemarks.push_back(*ARemark);
		}
        }
}



string BMetar::EncodeTime() const
{
	char TimeStr[8];
	memset(TimeStr, '\0', 8);

	strftime(TimeStr, 8, "%d%H%MZ", &myTime);
	return(TimeStr);
}

string BMetar::EncodeWind() const
{
	char WindStr[25];
	memset(WindStr, '\0', 25);
	int CharPos = 0;

	if (myIsDirVariable)
	{
		CharPos = snprintf(WindStr, 7, "VRB%.2d", (int) myWindSpeed);
	}
	else if (myWindFrom != -1)
	{
		CharPos = snprintf(WindStr, 7, "%.3d%.2d", myWindFrom, (int) myWindSpeed);
	}
	else
	{
		return("");
	}

	if (!isnan(myWindGusts))
	{
		CharPos += snprintf(WindStr + CharPos, 5, "G%.2d", (int) myWindGusts);
	}

	if (myMinWindDir != -1)
	{
		snprintf(WindStr + CharPos, 12, "MPS %.3dV%.3d", myMinWindDir, myMaxWindDir);
	}
	else
	{
		strcat(WindStr + CharPos, "MPS");
	}

	return(WindStr);
}

string BMetar::EncodeVisibility() const
{
	if (isnan(myVisibility))
	{
		return("");
	}

	if (myVisibility <= 402.34 && myIsAuto)
	{
		return("M1/4SM");
	}

	return(GiveFraction(myVisibility * 0.000621371) + "SM");
}

string BMetar::EncodeRVR() const
{
	if (myIsRVRMissing)
	{
		return("RVRNO");
	}

	if (myRunway == -1)
	{
		return("");
	}

	char RVRStr[13];
	memset(RVRStr, '\0', 13);

	snprintf(RVRStr, 12, "R%.2d/%.3dFT", myRunway, (int) myRunwayVisRange);

	return(RVRStr);
}


string BMetar::EncodeWeathers() const
{
	string WeatherStr = "";

	if (!myWeathers.empty())
	{
		WeatherStr += myWeathers[0].Encode();

		for (vector<WeatherType>::const_iterator AWeather = (myWeathers.begin() + 1);
		     AWeather != myWeathers.end();
		     AWeather++)
		{
			WeatherStr += ' ' + AWeather->Encode();
		}
	}
	
	return(WeatherStr);
}

string BMetar::EncodeClouds() const
{
	string CloudStr = "";

	if (!myClouds.empty())
	{
		CloudStr += myClouds[0].Encode();

		for (vector<CloudType>::const_iterator ACloud = (myClouds.begin() + 1);
		     ACloud != myClouds.end();
	     	     ACloud++)
		{
			CloudStr += ' ' + ACloud->Encode();
		}
	}

	return(CloudStr);
}

string BMetar::EncodeTemperature_DewPoint() const
{
	if (isnan(myTemperature))
	{
		return("");
	}

	char TempStr[8];
	memset(TempStr, '\0', 8);
	int CharPos = 0;

	if ((int) myTemperature >= 0)
	{
		CharPos = snprintf(TempStr, 5, "%.2d/", (int) myTemperature);
	}
	else
	{
		CharPos = snprintf(TempStr, 5, "M%.2d/", (int) (-1.0 * myTemperature));
	}

	if (!isnan(myDewPoint))
	{
		if ((int) myDewPoint >= 0)
		{
			snprintf(TempStr + CharPos, 4, "%.2d", (int) myDewPoint);
		}
		else
		{
			snprintf(TempStr + CharPos, 4, "M%.2d/", (int) (-1.0 * myDewPoint));
		}
	}

	return(TempStr);
}

string BMetar::EncodePressures() const
{
	char PressStr[12];
	char* CharPoint = PressStr;
	memset(PressStr, '\0', 12);

	if (!isnan(myAltimeterSet))
	{
		CharPoint += snprintf(CharPoint, 6, "A%.4d", (int) myAltimeterSet);

		if (!isnan(myStationPressure))
		{
			*CharPoint++ = ' ';
		}
	}

	if (!isnan(myStationPressure))
	{
		snprintf(CharPoint, 6, "Q%.4d", (int) myStationPressure);
	}

	return(PressStr);
}


string BMetar::EncodeRecentWeathers() const
{
	if (myRecentWeathers.empty())
	{
		return("");
	}

	string WeatherStr = "RE" + myRecentWeathers[0].Encode();
	
	for (vector<WeatherType>::const_iterator AWeather = (myRecentWeathers.begin() + 1);
	     AWeather != myRecentWeathers.end();
	     AWeather++)
	{
		WeatherStr += ' ' + AWeather->Encode();
	}

	return(WeatherStr);
}

string BMetar::EncodeRemarks() const
{
	if (myRemarks.empty())
	{
		return("");
	}

	string RemarkStr = "RMK " + myRemarks[0];

	for (vector<string>::const_iterator ARemark = (myRemarks.begin() + 1);
	     ARemark != myRemarks.end();
	     ARemark++)
	{
		RemarkStr += ' ' + *ARemark;
	}

	return(RemarkStr);
}





vector <string> BMetar::SplitBySpaces(const string &FullStr) const
{
	vector <string> Pieces(0);
	size_t StartPos = 0;

	for (size_t Index = 0; Index < FullStr.length(); Index++)
	{
		if (isspace(FullStr[Index]))
		{
			if (StartPos != Index)
			{
				Pieces.push_back(FullStr.substr(StartPos, Index - StartPos));
			}

			// Don't want the StartPos on a known space, so go to the next index.
			StartPos = Index + 1;
		}
	}

	if (StartPos < FullStr.length())
	{
		Pieces.push_back(FullStr.substr(StartPos));
	}

	return(Pieces);
}

float BMetar::ParseFraction(const string &FracStr) const
{
	int WholeNum, Numerator, Denominator;
	
	if (sscanf(FracStr.c_str(), "%d %d/%d", &WholeNum, &Numerator, &Denominator) == 3)
	{
		return((float) ((WholeNum * Denominator) + Numerator) / (float) Denominator);
	}
	else if (sscanf(FracStr.c_str(), "%d/%d", &Numerator, &Denominator) == 2)
	{
		return((float) Numerator / (float) Denominator);
	}
	else if (sscanf(FracStr.c_str(), "%d", &WholeNum) == 1)
	{
		return((float) WholeNum);
	}
	else
	{
		return(nan("nan"));
	}
}

string BMetar::GiveFraction(const float &FloatVal) const
/* This function will take a float value and produce a string
   representing a compound fraction best approximates that value,
   using denominators between 1 and 5, inclusive.
   Examples:
              Input             Output
              3.75              "3 3/4"
              0.5               "1/2"
              10.0              "10"
              21.3333           "21 1/3"
              9.2492            "9 1/4"
              17.9999           "18"

  Negative values have corresponding results, except in certain cases due to rounding.
*/
{
	float MinimumRemain = fabs(remainderf(FloatVal, 1.0 / (float) 1));
	int Denominator = 1;

	for (int Index = 2; Index <= 5; Index++)
	{
		float TestVal = fabs(remainderf(FloatVal, 1.0 / (float) Index));

		if (TestVal < MinimumRemain)
		{
			MinimumRemain = TestVal;
			Denominator = Index;
		}
	}

	int WholeNum = (int) FloatVal;
	int Numerator = (int) roundf(fabs(fmodf(FloatVal, 1.0) * (float) Denominator));

	char FracStr[32];		// Should be more than enough, right?
	memset(FracStr, '\0', 32);

	if (Numerator == Denominator)
	{
		// This fraction can be reduced, since it equals one.
		// Note that Denominator cannot equal zero, so I don't have to check for 0/0.

		// 'increment' due to Numerator/Denominator == 1.
		// Note that if the float is negative, then I want DECREASE
		// the value of WholeNum, hence copysignf().

		WholeNum += (int) copysignf(1.0, FloatVal);
		Numerator = 0;				// set to zero so I know not to print out the fraction.
	}

	

	if (WholeNum != 0)
        {
                if (Numerator != 0)
                {
                        // both WholeNum and Numerator has value, so print both parts.
                        snprintf(FracStr, 32, "%d %d/%d", WholeNum, Numerator, Denominator);
                }
                else
                {
                        // since Numerator is zero, don't bother printing fractional part.
                        snprintf(FracStr, 32, "%d", WholeNum);
                }
        }
        else
        {
                if (Numerator != 0)
                {
                        if (FloatVal < 0.0)
                        {
				// need to make the Numerator negative since I won't be printing WholeNum.
                                Numerator *= -1;
                        }

                        // Since WholeNum is zero, just print the fraction part...
                        snprintf(FracStr, 32, "%d/%d", Numerator, Denominator);
                }
                else
                {
                        // both the WholeNum and the Numerator == 0, so print '0'.
                        *FracStr = '0';
                }
        }


	return(FracStr);
}

struct tm BMetar::GuessBestTime(struct tm GuessTime, const time_t &CompareTime) const
// Tries to determine the year and the month by comparing several possibilities against CompareTime.
//
{
        GuessTime.tm_year = myTime.tm_year;
        GuessTime.tm_mon = myTime.tm_mon;
        const int OrigDay = GuessTime.tm_mday;          // need to store in case of renormalizing...
        GuessTime.tm_sec = 0;

        time_t CheckTime = timegm(&GuessTime);
        double SmallestDiff = fabs(difftime(CheckTime, CompareTime));
        struct tm BestTime = GuessTime;

        // remember that mktime() will renormalize the struct tm.
        // Therefore, some values in GuessTime may have changed, so just reassign them.
        GuessTime.tm_year = myTime.tm_year;
        GuessTime.tm_mon = myTime.tm_mon - 1;
        GuessTime.tm_mday = OrigDay;
        CheckTime = timegm(&GuessTime);

        if (SmallestDiff > fabs(difftime(CheckTime, CompareTime)))
        {
                SmallestDiff = fabs(difftime(CheckTime, CompareTime));
                BestTime = GuessTime;
        }

        GuessTime.tm_year = myTime.tm_year;
        GuessTime.tm_mon = myTime.tm_mon + 1;
        GuessTime.tm_mday = OrigDay;
        CheckTime = timegm(&GuessTime);

        if (SmallestDiff > fabs(difftime(CheckTime, CompareTime)))
        {
                // no need to reassign SmallestDiff since this is the last one...
                BestTime = GuessTime;
        }

        return(BestTime);
}


#endif
