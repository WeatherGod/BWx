#ifndef _BMOS_H
#define _BMOS_H
/*
 *  BMOS.h
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <vector>
#include <map>
#include <ctime>

class BMOS
{
	public:
		BMOS();
		BMOS(const BMOS &MOSCopy);
		explicit BMOS(const string &MOSReport);
		
		bool Decode(const string &MOSReport);
		string Encode() const;

		bool IsValid() const;

		
		struct tm GiveTime() const;

		void TakeTime(const struct tm &ATime);

		string GiveStationID() const;
		string GiveModelType() const;

		bool TakeStationID(const string &StationID);
		bool TakeModelType(const string &ModelType);


		vector <int> GiveHours() const;
		vector <float> GiveMinTemps(const vector <int> &ForecastHours) const;
		vector <float> GiveMaxTemps(const vector <int> &ForecastHours) const;
		vector <float> GiveTemperatures(const vector <int> &ForecastHours) const;
		vector <float> GiveDewPoints(const vector <int> &ForecastHours) const;
		vector <string> GiveCloudCovers(const vector <int> &ForecastHours) const;
		vector <float> GiveWindDirs(const vector <int> &ForecastHours) const;
		vector <float> GiveWindSpeeds(const vector <int> &ForecastHours) const;
		vector <int> GiveProbOfPrecips_6hr(const vector <int> &ForecastHours) const;
		vector <int> GiveProbOfPrecips_12hr(const vector <int> &ForecastHours) const;

		float GiveMinTemp(const int ForecastHour) const;
		float GiveMaxTemp(const int ForecastHour) const;
		float GiveTemperature(const int ForecastHour) const;
		float GiveDewPoint(const int ForecastHour) const;
		string GiveCloudCover(const int ForecastHour) const;
		float GiveWindDir(const int ForecastHour) const;
		float GiveWindSpeed(const int ForecastHour) const;
		int GiveProbOfPrecip_6hr(const int ForecastHour) const;
		int GiveProbOfPrecip_12hr(const int ForecastHour) const;
		
	private:
		bool myIsValid;
		size_t myTableStartPos;
		struct tm myTime;
		string myStationID;
		string myModelType;
		vector <int> myHours;
		map <int, float> myMinTemps;
		map <int, float> myMaxTemps;
		map <int, float> myTemperatures;
		map <int, float> myDewPoints;
		map <int, string> myCloudCovers;
		map <int, float> myWindDirs;
		map <int, float> myWindSpeeds;
		map <int, int> myProbOfPrecips_6hr;
		map <int, int> myProbOfPrecips_12hr;
		string myHourLabel;
		string myMinMaxLabel;
		string myMaxMinLabel;
		string myTemperatureLabel;
		string myDewPointLabel;
		string myCloudLabel;
		string myWindDirLabel;
		string myWindSpeedLabel;
		string myPOP6Label;
		string myPOP12Label;

		bool IsModelType(const string &CodeStr) const;
		bool IsStationID(const string &StationID) const;

		bool DecodeTime(const string &MOSTime);

		string EncodeTime() const;
		string EncodeHours() const;
		string EncodeExtremeTemps(const int ErrNum) const;

		string EncodeTableRow(const string &RowLabel, const map <int, int> &TheVals) const;
		string EncodeTableRow(const string &RowLabel, const map <int, float> &TheVals, const int ErrNum) const;
		string EncodeTableRow(const string &RowLabel, const map <int, string> &TheVals) const;

		void LoadHours(const string &Report, size_t &CharPos);
		void LoadExtremeTemps(const string &Report, size_t &CharPos, map <int, float> &Array_1, map <int, float> &Array_2, const int ErrNum);

		void LoadTableRow(const string &Report, size_t &CharPos, map <int, int> &TheArray);
		void LoadTableRow(const string &Report, size_t &CharPos, map <int, float> &TheArray, const int ErrNum);
		void LoadTableRow(const string &Report, size_t &CharPos, map <int, string> &TheArray);

		int GiveVal(const int ForecastHour, const map<int, int> &TheArray) const;
		float GiveVal(const int ForecastHour, const map<int, float> &TheArray) const;
		string GiveVal(const int ForecastHour, const map<int, string> &TheArray) const;
		
		vector <int> GiveVals(const vector <int> &ForecastHours, const map<int, int> &TheArray) const;
		vector <float> GiveVals(const vector <int> &ForecastHours, const map<int, float> &TheArray) const;
		vector <string> GiveVals(const vector <int> &ForecastHours, const map<int, string> &TheArray) const;

		friend bool operator < (const BMOS &LeftMOS, const BMOS &RightMOS);
		friend bool operator > (const BMOS &LeftMOS, const BMOS &RightMOS);
};	
		
#endif
