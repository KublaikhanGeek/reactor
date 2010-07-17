#ifndef __XIAO5GE_RECOMMEND_DATA_H__
#define __XIAO5GE_RECOMMEND_DATA_H__

#include <vector>
#include <string>

class Movie
{
public:

	Movie()
	{}

	Movie(int nID, const std::string & strTitle, 
		const std::string & strGenres) :
		m_nID(nID),
		m_strTitle(strTitle),
		m_strGenres(strGenres)
	{}

	int GetID() const
	{
		return m_nID;
	}

	void SetID(int nID)
	{
		m_nID = nID;
	}

	const std::string & GetTitle() const
	{
		return m_strTitle;
	}

	void SetTitle(const std::string & strTitle)
	{
		m_strTitle = strTitle;
	}

	const std::string & GetGenres() const
	{
		return m_strGenres;
	}

	void SetGenres(const std::string & strGenres)
	{
		m_strGenres = strGenres;
	}

private:

	int          m_nID;
	std::string  m_strTitle;
	std::string  m_strGenres;
};

static const char * g_szOccupations[] =
{
	"other",
	"academic/educator",
	"artist",
	"clerical/admin",
	"college/grad student",
	"customer service",
	"doctor/health care",
	"executive/managerial",
	"farmer",
	"homemaker",
	"K-12 student",
	"lawyer",
	"programmer",
	"retired",
	"sales/marketing",
	"scientist",
	"self-employed",
	"technician/engineer",
	"tradesman/craftsman",
	"unemployed",
	"writer"
};

class User
{
public:

	User()
	{}

	User(int nID, unsigned char ucGender, unsigned short usAge,
		unsigned short usOccupation, unsigned short usZipCode) :
		m_nID(nID),
		m_usGender(ucGender == 'F' ? 1 : 0),
		m_usAge(usAge),
		m_usOccupation(usOccupation),
		m_usZipCode(usZipCode)
	{}

	int GetID() const
	{
		return m_nID;
	}

	void SetID(int nID)
	{
		m_nID = nID;
	}

	unsigned char GetGender() const
	{
		return m_usGender == 0 ? 'M' : 'F';
	}

	void SetGender(unsigned char ucGender)
	{
		m_usGender = (ucGender == 'F' ? 1 : 0);
	}

	const char * GetAge() const
	{
		switch (m_usAge)
		{
		case 1:
			return "Under 18";
		case 18:
			return "18-24";
		case 24:
			return "25-34";
		case 35:
			return "35-44";
		case 45:
			return "45-49";
		case 50:
			return "50-55";
		default:
			return "56+";
		}
	}

	void SetAge(unsigned usAge)
	{
		m_usAge = usAge;
	}

	const char * GetOccupation() const
	{
		return g_szOccupations[m_usOccupation];
	}

	void SetOccupation(unsigned short usOccupation)
	{
		m_usOccupation = usOccupation;
	}

	unsigned short GetZipCode() const
	{
		return m_usZipCode;
	}

	void SetZipCode(unsigned short usZipCode)
	{
		m_usZipCode = usZipCode;
	}

private:

	int             m_nID;
	unsigned short  m_usGender; //0: Male, 1: Female
	unsigned short  m_usAge;
	unsigned short  m_usOccupation;
	unsigned short  m_usZipCode;
};

class Rating
{
public:

	Rating()
	{}

	Rating(int nUserID, int nMovieID, unsigned short usRate, time_t tTime) :
		m_nUserID(nUserID),
		m_nMovieID(nMovieID),
		m_usRate(usRate),
		m_tTimeStamp(tTime)
	{}

	int GetUserID() const
	{
		return m_nUserID;
	}

	void SetUserID(int nID)
	{
		m_nUserID = nID;
	}

	int GetMovieID() const
	{
		return m_nMovieID;
	}
	
	void SetMovieID(int nID)
	{
		m_nMovieID = nID;
	}

	unsigned short GetRate() const
	{
		return m_usRate;
	}

	void SetRate(unsigned short usRate)
	{
		m_usRate = usRate;
	}

	time_t GetTimeStamp() const
	{
		return m_tTimeStamp;
	}

	void SetTimeStamp(time_t tTime)
	{
		m_tTimeStamp = tTime;
	}

private:

	int            m_nUserID;
	int            m_nMovieID;
	unsigned short m_usRate;
	time_t         m_tTimeStamp;
};

class Parser
{
public:

	static void ParseMovie(const std::string & strData, Movie & oMovie)
	{
		std::vector<std::string> vecSplitRes;
		SplitData(strData, "::", vecSplitRes);
		oMovie.SetID(::atoi(vecSplitRes[0].c_str()));
		oMovie.SetTitle(vecSplitRes[1]);
		oMovie.SetGenres(vecSplitRes[2]);
	}

	static void ParseUser(const std::string & strData, User & oUser)
	{
		std::vector<std::string> vecSplitRes;
		SplitData(strData, "::", vecSplitRes);
		oUser.SetID(::atoi(vecSplitRes[0].c_str()));
		oUser.SetGender(vecSplitRes[1].at(0));
		oUser.SetAge(::atoi(vecSplitRes[2].c_str()));
		oUser.SetOccupation(::atoi(vecSplitRes[3].c_str()));
		oUser.SetZipCode(::atoi(vecSplitRes[4].c_str()));
	}

	static void ParseRating(const std::string & strData, Rating & oRating)
	{
		std::vector<std::string> vecSplitRes;
		SplitData(strData, "::", vecSplitRes);
		oRating.SetUserID(::atoi(vecSplitRes[0].c_str()));
		oRating.SetMovieID(::atoi(vecSplitRes[1].c_str()));
		oRating.SetRate(::atoi(vecSplitRes[2].c_str()));
		oRating.SetTimeStamp(::atoi(vecSplitRes[3].c_str()));
	}

private:

	static void SplitData(const std::string & strData, 
		const std::string & strSeparator, 
		std::vector<std::string> & vecRes)
	{
		std::string strValue;
		size_t unSize = strData.size();
		size_t unLen = strSeparator.size();

		for (size_t unIdx = 0; unIdx < unSize;)
		{
			if (strData.substr(unIdx, unLen) == strSeparator)
			{
				vecRes.push_back(strValue);
				strValue.clear();
				unIdx += unLen;
			}
			else
			{
				strValue.push_back(strData[unIdx]);
				++ unIdx;
			}
		}
		vecRes.push_back(strValue);
	}
};

#endif //__XIAO5GE_RECOMMEND_DATA_H__
