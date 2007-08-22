#ifndef _CLOUDTYPE_C
#define _CLOUDTYPE_C
using namespace std;

#include <iostream>
#include <string>
#include <cstdio>	// for snprintf(), sscanf()

#include "CloudType.h"


CloudType::CloudType()
	:       myCoverage(UNKNOWN_COVER),
                myCloudClass(""),
                myHeight(-1)
{
}




bool CloudType::Decode(const string &CloudStr)
{
        if (CloudStr.length() < 3)
        {
                return(false);
        }

	if (CloudStr == "NSC")
	{
		myCoverage = no_sig_cloud;
		return(true);
	}
	else if (CloudStr == "NCD" || CloudStr == "SKC")
	{
		myCoverage = no_clouds_detected;
		return(true);
	}

	int StrPos = 0;

	// The CloudCoverage must be stated for it to be valid.
	if (!DecodeCloudCoverage(CloudStr, StrPos))
	{
		return(false);
	}

	DecodeCloudHeight(CloudStr, StrPos);
	DecodeCloudClass(CloudStr, StrPos);

	return(StrPos == CloudStr.length());
}


bool CloudType::DecodeCloudCoverage(const string &CloudStr, int &StrPos)
{
	if (CloudStr.substr(StrPos, 2) == "VV")
        {
                myCoverage = vertical_visibility;
                StrPos += 2;
        }
        else
        {
                const string TempHold = CloudStr.substr(StrPos, 3);

                if (TempHold == "CLR")
                {
                        myCoverage = clear;
                }
                else if (TempHold == "FEW")
                {
                        myCoverage = few;
                }
                else if (TempHold == "SCT")
                {
                        myCoverage = scattered;
                }
                else if (TempHold == "BKN")
                {
                        myCoverage = broken;
                }
                else if (TempHold == "OVC")
                {
                        myCoverage = overcast;
                }
                else
                {
                        return(false);
                }

                StrPos += 3;
        }

	return(true);
}

void CloudType::DecodeCloudHeight(const string &CloudStr, int &StrPos)
{
        if (CloudStr.substr(StrPos, 3) == "///")
        {
                StrPos += 3;
        }
	else if (CloudStr.length() < 10)
        {
		const string Token = CloudStr.substr(StrPos);
                char* CharPoint;

		int CloudHeight = strtol(Token.c_str(), &CharPoint, 10);
		if (CharPoint > Token.c_str())
		{
			myHeight = CloudHeight;
			StrPos += (CharPoint - Token.c_str());
		}
        }
}


void CloudType::DecodeCloudClass(const string &CloudStr, int &StrPos)
{
	myCloudClass = CloudStr.substr(StrPos, 3);
	StrPos += myCloudClass.length();
}




string CloudType::EncodeCloudCoverage() const
{
	string TheWord = "";

        switch (myCoverage)
        {
        case clear:
        	TheWord = "CLR";
                break;
        case few:
                TheWord = "FEW";
                break;
        case scattered:
                TheWord = "SCT";
                break;
        case broken:
                TheWord = "BKN";
                break;
        case overcast:
                TheWord = "OVC";
                break;
        case vertical_visibility:
                TheWord = "VV";
                break;
	case no_sig_cloud:
		TheWord = "NSC";
		break;
	case no_clouds_detected:
		TheWord = "NCD";
		break;
        case UNKNOWN_COVER:
                break;
        default:
                cerr << "ERROR: Unknown cloud coverage...\n";
                break;
        }

        return(TheWord);
}


string CloudType::EncodeCloudClass() const
{
        return(myCloudClass);
}

string CloudType::Encode() const
{
	char CloudWord[11];
        memset(CloudWord, '\0', 11);

        int CharPos = snprintf(CloudWord, 4, "%s", EncodeCloudCoverage().c_str());

        if (myHeight != -1)
        {
                snprintf(CloudWord + CharPos, 4, "%.3d", myHeight);
        }
        else if (CharPos != 0 && !(EncodeCloudClass()).empty())
        {
                strcat(CloudWord, "///");
        }

        strcat(CloudWord, EncodeCloudClass().c_str());

        return(CloudWord);
}


#endif
