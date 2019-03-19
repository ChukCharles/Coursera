#include "phone_number.h"
#include<stdexcept>
#include<sstream>

PhoneNumber::PhoneNumber(const string &international_number) {
	//checking
	int i = 0;
	if( international_number[i] != '+' ) {
		throw invalid_argument("plus");
	}
	if( ++i == international_number.size() ) {
		throw invalid_argument("no city code");
	}

	string country = "";
	while( international_number[i] != '-' ) {
		country += international_number[i];
		++i;
		if( i == international_number.size() ) {
			throw invalid_argument("wrong country code");
		}
	}

	if( ++i == international_number.size() ) //skipping '-'
	{
		throw invalid_argument("no city code");
	}
	string city = "";
	while( international_number[i] != '-' ) {
		city += international_number[i];
		if( ++i == international_number.size() ) {
			throw invalid_argument("wrong city code");
		}
	}
	if( ++i == international_number.size() ) {
		throw invalid_argument("no local number");
	}
	string number = international_number.substr( i );
	//
	  country_code_ = country;
	  city_code_ = city;
	  local_number_ = number;
}

string PhoneNumber::GetCountryCode() const {
	return country_code_;
}
string PhoneNumber::GetCityCode() const {
	return city_code_;
}
string PhoneNumber::GetLocalNumber() const {
	return local_number_;
}
string PhoneNumber::GetInternationalNumber() const {
	string result = "+" + country_code_ + "-" + city_code_ + "-" + local_number_;
	return result;

}
