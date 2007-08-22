#ifndef _CLOUDTYPE_H
#define _CLOUDTYPE_H

/*
 *  CloudType.h
 *  
 *
 *  Created by Benjamin Root on 1/2/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>

enum Coverage_t {clear = 1, few, scattered, broken, overcast, vertical_visibility, no_sig_cloud, no_clouds_detected, UNKNOWN_COVER};

class CloudType
{
	public:
		CloudType();

		string Encode() const;
		bool Decode(const string &CloudStr);

	private:
		Coverage_t myCoverage;
		string myCloudClass;
		int myHeight;

		string EncodeCloudCoverage() const;
		string EncodeCloudClass() const;

		bool DecodeCloudCoverage(const string &CloudStr, int &StrPos);
		void DecodeCloudHeight(const string &CloudStr, int &StrPos);
		void DecodeCloudClass(const string &CloudStr, int &StrPos);
};

#endif

	
