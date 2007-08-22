#ifndef _WEATHERTYPE_H
#define _WEATHERTYPE_H
/*
 *  WeatherType.h
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <string>


enum Intensity_t {light = 1, moderate, heavy, in_vicinity};
enum Descriptor_t {NO_DESCRIPT = 0, shallow, partial, patches, low_drifting, blowing, shower, thunderstorm, freezing, no_sig_weather};
enum Precipitation_t {NO_PRECIP = 0, drizzle, rain, snow, snow_grains, ice_crystals, ice_pellets, hail, hail_snow_pellets, unknown_precip};
enum Obscuration_t {NO_OBSCURE = 0, mist, fog, smoke, volcanic_ash, dust, sand, haze, spray};
enum Misc_t {NO_MISC = 0, dust_whirls, squalls, tornado, sandstorm, duststorm};

class WeatherType
{
	public:
		WeatherType();

		string Encode() const;
		bool Decode(const string &WeatherStr);

		Intensity_t GiveIntensity() const;
		Descriptor_t GiveDescriptor() const;
		Precipitation_t GivePrecipType() const;
		Obscuration_t GiveObscureType() const;
		Misc_t GiveMiscType() const;

		void TakeIntensity(const Intensity_t TheIntense);
		void TakeDescriptor(const Descriptor_t TheDescript);
		void TakePrecipType(const Precipitation_t ThePrecip);
		void TakeObscureType(const Obscuration_t TheObscure);
		void TakeMiscType(const Misc_t TheMiscEventType);

	private:
		Intensity_t myIntensity;
		Descriptor_t myDescription;
		Precipitation_t myPrecip;
		Obscuration_t myObscure;
		Misc_t myMiscInfo;


		string EncodeIntensity() const;
		string EncodeDescription() const;
		string EncodePrecip() const;
		string EncodeObscure() const;
		string EncodeMisc() const;

		void DecodeIntensity(const string &WeatherStr, int &StrPos);
		void DecodeDescription(const string &WeatherStr, int &StrPos);
		void DecodePrecip(const string &WeatherStr, int &StrPos);
		void DecodeObscure(const string &WeatherStr, int &StrPos);
		void DecodeMisc(const string &WeatherStr, int &StrPos);
};

#endif
