#ifndef _BMETAR_H
#define _BMETAR_H
/*
 *  BMetar.h
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <vector>
#include <ctime>

#include "WeatherType.h"		// for WeatherType info
#include "CloudType.h"			// for CloudType info


class BMetar
{
	public:
		BMetar();
		BMetar(const BMetar &MetarCopy);
		explicit BMetar(const string &MetarReport);
		BMetar(const string &MetarReport, const struct tm &ATime, const string &CodeTypeGuess = "METAR");
		
		bool Decode(const string &MetarReport);
		string Encode() const;

		bool IsValid() const;

		
		struct tm GiveTime() const;
		int GiveWindDir() const;
		int GiveMinWindDir() const;
		int GiveMaxWindDir() const;
		int GiveRunwayNum() const;
		float GiveWindSpeed() const;
		float GiveWindGust() const;
		float GiveVisibility() const;
		float GiveRunwayVisualRange() const;
		float GiveTemperature() const;
		float GiveDewPoint() const;
		float GiveAltimeterSetting() const;
		float GiveStationPressure() const;

		void TakeTime(const struct tm &ATime);
		void TakeWindDir(const int WindDir);
		void TakeWindSpeed(const float &WindSpeed);
		void TakeWindGust(const float &WindGust);
		void TakeVisibility(const float &Visibility);
		void TakeTemperature(const float &Temperature);
		void TakeDewPoint(const float &DewPoint);
		void TakeAltimeterSetting(const float &AltimeterSet);
		void TakeStationPressure(const float &StationPress);

		bool IsNil() const;
		bool IsAuto() const;
		bool IsCorrection() const;
		bool IsDirVariable() const;
		bool IsCAVOK() const;
		bool IsRVRMissing() const;
		bool IsNoSig() const;

		void IsAuto(const bool SomeBool);
		void IsCorrection(const bool SomeBool);
		void IsDirVariable(const bool SomeBool);
		void IsCAVOK(const bool SomeBool);
		void IsNil(const bool SomeBool);

		vector <WeatherType> GiveRecentWeathers() const;
		vector <WeatherType> GiveWeatherTypes() const;
		vector <CloudType> GiveCloudTypes() const;
		vector <string> GiveRemarks() const;

		string EncodeWeathers() const;
                string EncodeClouds() const;
		string EncodeRecentWeathers() const;

		void TakeWeatherTypes(const vector <string> &WeatherStrs);
		void TakeCloudTypes(const vector <string> &CloudStrs);
		void TakeRemarks(const vector <string> &RemarkStrs);

		string GiveStationID() const;
		string GiveCodeType() const;

		bool TakeStationID(const string &StationID);
		bool TakeCodeType(const string &CodeType);
		
	private:
		bool myIsValid;
		struct tm myTime;
		int myWindFrom;
		int myMinWindDir;
		int myMaxWindDir;
		int myRunway;
		float myWindSpeed;
		float myWindGusts;
		float myVisibility;
		float myRunwayVisRange;
		float myTemperature;
		float myDewPoint;
		float myAltimeterSet;
		float myStationPressure;
		bool myIsNil;
		bool myIsAuto;
		bool myIsCorrection;
		bool myIsDirVariable;
		bool myIsCAVOK;
		bool myIsRVRMissing;
		bool myIsNoSig;
		vector <WeatherType> myRecentWeathers;
		vector <WeatherType> myWeathers;
		vector <CloudType> myClouds;
		vector <string> myRemarks;
		string myStationID;
		string myCodeType;


		
		bool IsCodeType(const string &CodeStr) const;
		bool IsStationID(const string &StationID) const;


		bool DecodeTime(const string &MetarTime);
		bool DecodeWind(const string &MetarWind);
		bool DecodeMinMaxWindDir(const string &MetarWind);
		bool DecodeVisibility(const string &MetarVis);
		bool DecodeRVR(const string &MetarRVR);
		bool DecodeWeather(const string &MetarWeather);
		bool DecodeCloud(const string &MetarCloud);
		bool DecodeTemperature_DewPoint(const string &MetarTemp);
		bool DecodePressure(const string &MetarPress);
		bool DecodeRemark(const string &MetarRemark);
		bool DecodeRecentWeather(const string &MetarWeather);
		bool DecodeWindShear(const string &MetarWindShear);


		string EncodeTime() const;
		string EncodeWind() const;
		string EncodeVisibility() const;
		string EncodeRVR() const;
		string EncodeTemperature_DewPoint() const;
		string EncodePressures() const;
		string EncodeRemarks() const;


		vector <string> SplitBySpaces(const string &FullStr) const;
		float ParseFraction(const string &FracStr) const;
		string GiveFraction(const float &FloatVal) const;
		struct tm GuessBestTime(struct tm GuessTime, const time_t &CompareTime) const;
};	
		
#endif
