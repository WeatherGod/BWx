#ifndef _WEATHERTYPE_C
#define _WEATHERTYPE_C
using namespace std;

#include <iostream>
#include <string>

#include "WeatherType.h"


WeatherType::WeatherType()
	:       myIntensity(moderate),
                myDescription(NO_DESCRIPT),
                myPrecip(NO_PRECIP),
                myObscure(NO_OBSCURE),
                myMiscInfo(NO_MISC)
{
}



bool WeatherType::Decode(const string &MetarWeather)
{
        if (MetarWeather.length() < 2)
        {
                return(false);
        }

	if (MetarWeather == "NSW")
	{
		myDescription = no_sig_weather;
		return(true);
	}

	int StrPos = 0;
	
	DecodeIntensity(MetarWeather, StrPos);
	DecodeIntensity(MetarWeather, StrPos);
	DecodeDescription(MetarWeather, StrPos);

	// Due to standards, if there is a mix of precipitation,
	// they can be reported together, with the most predominate
	// type first.  Since I can't store more than one precip, I 
	// will just keep the first one.

	int LastPos = StrPos;
	DecodePrecip(MetarWeather, StrPos);

	Precipitation_t TempHoldPrecip = myPrecip;
	while (LastPos != StrPos)
	{
		LastPos = StrPos;
		DecodePrecip(MetarWeather, StrPos);
	}

	myPrecip = TempHoldPrecip;
	
	DecodeDescription(MetarWeather, StrPos);

	DecodeObscure(MetarWeather, StrPos);
	DecodeMisc(MetarWeather, StrPos);

	DecodeIntensity(MetarWeather, StrPos);

	return(StrPos == MetarWeather.length());
}



void WeatherType::DecodeIntensity(const string &MetarWeather, int &StrPos)
{
	if (StrPos == MetarWeather.length())
	{
		return;
	}

        if (MetarWeather[StrPos] == '+')
        {
                myIntensity = heavy;
                StrPos++;
        }
        else if (MetarWeather[StrPos] == '-')
        {
                myIntensity = light;
                StrPos++;
        }
        else if (MetarWeather.substr(StrPos, 2) == "VC")
        {
                myIntensity = in_vicinity;
                StrPos += 2;
        }
}

void WeatherType::DecodeDescription(const string &MetarWeather, int &StrPos)
{
        const string TempHold = MetarWeather.substr(StrPos, 2);

        if (TempHold == "MI")
        {
                myDescription = shallow;
		StrPos += 2;
        }
        else if (TempHold == "PR")
        {
                myDescription = partial;
		StrPos += 2;
        }
        else if (TempHold == "BC")
        {
                myDescription = patches;
		StrPos += 2;
        }
        else if (TempHold == "DR")
        {
                myDescription = low_drifting;
		StrPos += 2;
        }
        else if (TempHold == "BL")
        {
                myDescription = blowing;
		StrPos += 2;
	}
        else if (TempHold == "SH")
        {
                myDescription = shower;
		StrPos += 2;
        }
        else if (TempHold == "TS")
        {
                myDescription = thunderstorm;
		StrPos += 2;
        }
        else if (TempHold == "FZ")
        {
                myDescription = freezing;
		StrPos += 2;
        }
}


void WeatherType::DecodePrecip(const string &MetarWeather, int &StrPos)
{
        const string TempHold = MetarWeather.substr(StrPos, 2);

        if (TempHold == "DZ")
        {
                myPrecip = drizzle;
		StrPos += 2;
        }
        else if (TempHold == "RA")
        {
                myPrecip = rain;
		StrPos += 2;
        }
        else if (TempHold == "SN")
        {
                myPrecip = snow;
		StrPos += 2;
        }
        else if (TempHold == "SG")
        {
                myPrecip = snow_grains;
		StrPos += 2;
        }
        else if (TempHold == "IC")
        {
                myPrecip = ice_crystals;
		StrPos += 2;
        }
        else if (TempHold == "PE")
        {
                myPrecip = ice_pellets;
		StrPos += 2;
        }
	else if (TempHold == "GR")
        {
                myPrecip = hail;
		StrPos += 2;
        }
        else if (TempHold == "GS")
        {
                myPrecip = hail_snow_pellets;
		StrPos += 2;
        }
        else if (TempHold == "UP")
        {
                myPrecip = unknown_precip;
		StrPos += 2;
        }
}


void WeatherType::DecodeObscure(const string &MetarWeather, int &StrPos)
{
        const string TempHold = MetarWeather.substr(StrPos, 2);

        if (TempHold == "BR")
        {
                myObscure = mist;
		StrPos += 2;
        }
        else if (TempHold == "FG")
        {
                myObscure = fog;
		StrPos += 2;
        }
        else if (TempHold == "FU")
        {
                myObscure = smoke;
		StrPos += 2;
        }
        else if (TempHold == "VA")
        {
                myObscure = volcanic_ash;
		StrPos += 2;
        }
        else if (TempHold == "DU")
        {
                myObscure = dust;
		StrPos += 2;
        }
        else if (TempHold == "SA")
        {
                myObscure = sand;
		StrPos += 2;
        }
        else if (TempHold == "HZ")
        {
		myObscure = haze;
		StrPos += 2;
        }
        else if (TempHold == "PY")
        {
                myObscure = spray;
		StrPos += 2;
        }
}


void WeatherType::DecodeMisc(const string &MetarWeather, int &StrPos)
{
        const string TempHold = MetarWeather.substr(StrPos, 2);

        if (TempHold == "PO")
        {
                myMiscInfo = dust_whirls;
		StrPos += 2;
        }
        else if (TempHold == "SQ")
        {
                myMiscInfo = squalls;
		StrPos += 2;
        }
        else if (TempHold == "FC")
        {
                myMiscInfo = tornado;
		StrPos += 2;
        }
        else if (TempHold == "SS")
        {
                myMiscInfo = sandstorm;
		StrPos += 2;
        }
	else if (TempHold == "DS")
	{
		myMiscInfo = duststorm;
		StrPos += 2;
	}
}



string WeatherType::EncodeIntensity() const
{
        string TheWord = "";

        switch (myIntensity)
        {
        case light:
                TheWord = "-";
                break;
        case heavy:
                TheWord = "+";
                break;
        case in_vicinity:
                TheWord = "VC";
                break;
        case moderate:
                break;
        default:
                cerr << "\nUnknown intensity...\n";
        	break;
        }

	return(TheWord);
}


string WeatherType::EncodeDescription() const
{
        string TheWord = "";

        switch (myDescription)
        {
        case shallow:
                TheWord = "MI";
                break;
        case partial:
                TheWord = "PR";
                break;
        case patches:
                TheWord = "BC";
                break;
        case low_drifting:
                TheWord = "DR";
                break;
        case blowing:
                TheWord = "BL";
                break;
        case shower:
                TheWord = "SH";
                break;
        case thunderstorm:
                TheWord = "TS";
                break;
        case freezing:
                TheWord = "FZ";
                break;
	case no_sig_weather:
		TheWord = "NSW";
		break;
        case NO_DESCRIPT:
                break;
        default:
        	cerr << "\nUnknown type..." << endl;
        }

	return(TheWord);
}



string WeatherType::EncodePrecip() const
{
        string TheWord = "";

        switch (myPrecip)
        {
        case drizzle:
                TheWord = "DZ";
                break;
        case rain:
                TheWord = "RA";
                break;
        case snow:
                TheWord = "SN";
                break;
        case snow_grains:
                TheWord = "SG";
                break;
        case ice_crystals:
                TheWord = "IC";
                break;
        case ice_pellets:
                TheWord = "PL";
                break;
        case hail:
                TheWord = "GR";
                break;
        case hail_snow_pellets:
                TheWord = "GS";
                break;
        case unknown_precip:
                TheWord = "UP";
                break;
        case NO_PRECIP:
                break;
        default:
                cerr << "\nUnknown precip type (besides UP)\n";
                break;
        }

	return(TheWord);
}


string WeatherType::EncodeObscure() const
{
        string TheWord = "";

        switch (myObscure)
        {
        case mist:
                TheWord = "BR";
                break;
        case fog:
                TheWord = "FG";
                break;
        case smoke:
                TheWord = "FU";
                break;
        case volcanic_ash:
                TheWord = "VA";
                break;
        case dust:
                TheWord = "DU";
                break;
        case sand:
                TheWord = "SA";
                break;
        case haze:
                TheWord = "HZ";
                break;
        case spray:
                TheWord = "PY";
                break;
        case NO_OBSCURE:
                break;
        default:
                cerr << "\nUnknown obscuration type...\n";
                break;
        }

	return(TheWord);
}


string WeatherType::EncodeMisc() const
{
        string TheWord = "";

        switch (myMiscInfo)
        {
        case dust_whirls:
                TheWord = "PO";
                break;
        case squalls:
                TheWord = "SQ";
                break;
        case tornado:
                TheWord = "FC";
                break;
        case sandstorm:
                TheWord = "SS";
                break;
        case duststorm:
                TheWord = "DS";
                break;
        case NO_MISC:
                break;
        default:
                cerr << "\nUnknown miscellaneous info...\n";
                break;
        }

	return(TheWord);
}


string WeatherType::Encode() const
{
	return(EncodeIntensity() + EncodeDescription() + EncodePrecip() 
	       + EncodeObscure() + EncodeMisc());
}




Intensity_t WeatherType::GiveIntensity() const
{
	return(myIntensity);
}

Descriptor_t WeatherType::GiveDescriptor() const
{
	return(myDescription);
}


Precipitation_t WeatherType::GivePrecipType() const
{
	return(myPrecip);
}

Obscuration_t WeatherType::GiveObscureType() const
{
	return(myObscure);
}


Misc_t WeatherType::GiveMiscType() const
{
	return(myMiscInfo);
}



void WeatherType::TakeIntensity(const Intensity_t TheIntense)
{
	myIntensity = TheIntense;
}

void WeatherType::TakeDescriptor(const Descriptor_t TheDescript)
{
	myDescription = TheDescript;
}

void WeatherType::TakePrecipType(const Precipitation_t ThePrecip)
{
	myPrecip = ThePrecip;
}

void WeatherType::TakeObscureType(const Obscuration_t TheObscure)
{
	myObscure = TheObscure;
}

void WeatherType::TakeMiscType(const Misc_t TheMiscEventType)
{
	myMiscInfo = TheMiscEventType;
}


#endif
