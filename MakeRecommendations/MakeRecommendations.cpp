#include "Score.h"
#include "RecommendData.h"

#include <map>
#include <fstream>
#include <iostream>

using namespace std;

const char * g_pMovieData = "./million-ml-data/movies.dat";
const char * g_pUserData = "./million-ml-data/users.dat";
const char * g_pRatingData = "./million-ml-data/ratings.dat";

map<int, Movie> g_mapMovieInfo;
map<int, User> g_mapUserInfo;
map<int, map<int, Rating> > g_mapRatingInfo;
double ** g_szRatings = NULL;

int main(int argc, char ** argv)
{
	if (argc != 3)
	{
		cerr << "Usage: " << argv[0] << " personIndex recNum\n";
		return 1;
	}

	ifstream oFileIn;
	string strLine;

	///load movie info
	oFileIn.open(g_pMovieData, ios::in);
	Movie oMovie;
	while (getline(oFileIn, strLine))
	{
		Parser::ParseMovie(strLine, oMovie);
		g_mapMovieInfo[oMovie.GetID()] = oMovie;
	}
	oFileIn.clear();
	oFileIn.close();

	///load user info
	oFileIn.open(g_pUserData, ios::in);
	User oUser;
	while (getline(oFileIn, strLine))
	{
		Parser::ParseUser(strLine, oUser);
		g_mapUserInfo[oUser.GetID()] = oUser;
	}
	oFileIn.clear();
	oFileIn.close();

	///load rating
	oFileIn.open(g_pRatingData, ios::in);
	Rating oRating;
	while (getline(oFileIn, strLine))
	{
		Parser::ParseRating(strLine, oRating);
		g_mapRatingInfo[oRating.GetUserID()][oRating.GetMovieID()] = oRating;
	}
	oFileIn.close();

	size_t unMaxUserNum = g_mapUserInfo.rbegin()->first;
	size_t unMaxMovieNum = g_mapMovieInfo.rbegin()->first;
	///allocate memory
	g_szRatings = new double*[unMaxUserNum];	
	for (size_t unIdx = 0; unIdx < unMaxUserNum; ++ unIdx)
	{
		g_szRatings[unIdx] = new double[unMaxMovieNum];
		::memset(g_szRatings[unIdx], 0, sizeof(double)*unMaxMovieNum);
	}

	///get critics
	map<int, User>::iterator oItUser = g_mapUserInfo.begin();
	while (oItUser != g_mapUserInfo.end())
	{
		map<int, Rating>::iterator oItRating = g_mapRatingInfo[oItUser->first].begin();
		while (oItRating != g_mapRatingInfo[oItUser->first].end())
		{
			g_szRatings[oItUser->first - 1][oItRating->first - 1] = oItRating->second.GetRate();
			++ oItRating;
		}
		++ oItUser;
	}

	int nPersonIndex = ::atoi(argv[1]) - 1;
	int nRecNum = ::atoi(argv[2]);
	int * pRecItems = new int[nRecNum];
	double * pRecScores = new double[nRecNum];

	///Euclidean
	GetRecommendation(const_cast<const double **>(g_szRatings), unMaxUserNum, 
		unMaxMovieNum, nPersonIndex, GetEuclideanScore, nRecNum, pRecItems, pRecScores);
	cout << "---------------------Euclidean Recommendation: \n";
	for (int nIdx = 0; nIdx < nRecNum; ++ nIdx)
	{
		cout << nIdx << "\t" << pRecScores[nIdx] << "\t" 
			<< g_mapMovieInfo[pRecItems[nIdx]].GetTitle() << endl;
	}

	///Pearson
	GetRecommendation(const_cast<const double **>(g_szRatings), unMaxUserNum, 
		unMaxMovieNum, nPersonIndex, GetPearsonScore, nRecNum, pRecItems, pRecScores);
	cout << "\n---------------------Pearson Recommendation: \n";
	for (int nIdx = 0; nIdx < nRecNum; ++ nIdx)
	{
		cout << nIdx << "\t" << pRecScores[nIdx] << "\t" 
			<< g_mapMovieInfo[pRecItems[nIdx]].GetTitle() << endl;
	}

	///Tanimoto
	GetRecommendation(const_cast<const double **>(g_szRatings), unMaxUserNum, 
		unMaxMovieNum, nPersonIndex, GetTanimotoScore, nRecNum, pRecItems, pRecScores);
	cout << "\n---------------------Tanimoto Recommendation: \n";
	for (int nIdx = 0; nIdx < nRecNum; ++ nIdx)
	{
		cout << nIdx << "\t" << pRecScores[nIdx] << "\t" 
			<< g_mapMovieInfo[pRecItems[nIdx]].GetTitle() << endl;
	}

	///deallocate memory
	for (size_t unIdx = 0; unIdx < unMaxUserNum; ++ unIdx)
	{
		delete [] g_szRatings[unIdx];
	}
	delete [] g_szRatings;
}