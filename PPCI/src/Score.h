#ifndef __XIAO5GE_SCORE_H__
#define __XIAO5GE_SCORE_H__

#include "MathUtility.h"

#include <map>

/**
 * @brief  Euclidean打分，取值(0, 1], Euclidean距离越相近的情况分值越接近1
 * @param  dist, Euclidean距离
 * @return 转换后的得分
 */
double GetEuclideanScore(double dist)
{
	return (1 / (1 + dist));
}

/**
 * @brief  Pearson打分，取值[-1, 1], 越相近的情况分值越接近1
 * @param  coef, the Pearson相关系数值
 * @return Person打分值
 */
double GetPearsonScore(double coef)
{
	return coef;
}

/**
 * @brief  Tanimoto打分，取值[0, 1], 越相近的情况分值越接近1
 * @param  coef, the Tanimoto系数值
 * @return Tanimoto打分值
 */
double GetTanimotoScore(double coef)
{
	return coef;
}

double GetEuclideanScore(const double * myCritics, const double * hisCritics, size_t size)
{
	double dist = GetEuclideanDistance(myCritics, myCritics + size,
		hisCritics, hisCritics + size);
	
	return GetEuclideanScore(dist);
}

double GetPearsonScore(const double * myCritics, const double * hisCritics, size_t size)
{
	double coef = GetPearsonCorrelationCoefficient(myCritics, myCritics + size,
		hisCritics, hisCritics + size);

	return GetPearsonScore(coef);
}

double GetTanimotoScore(const double * myCritics, const double * hisCritics, size_t size)
{
	double coef = GetTanimotoCoefficient(myCritics, myCritics + size,
		hisCritics, hisCritics + size);

	return GetTanimotoScore(coef);
}

typedef double (*ScoreFunc)(const double *, const double *, size_t);

void GetRecommendation(	
	const double ** allCritics,    //[in]  所有人的打分表
	size_t personNum,              //[in]  所有人的个数
	size_t size,                   //[in]  打分表大小
	size_t myIndex,                //[in]  我在打分表中下标
	ScoreFunc scorer,              //[in]  打分函数
	size_t recNum,                 //[out] 被推荐项个数
	int * recItems,                //[out] 被推荐项列表
	double * recScores             //[out] 被推荐项得分
	)
 {
	double * allRels = new double[personNum];
	::memset(allRels, 0, sizeof(double)*personNum);
	///计算所有的相关度
	for (size_t idx = 0; idx < personNum; ++ idx)
	{
		if (idx == myIndex)
		{
			continue; //it's me, just continue
		}
		allRels[idx] = scorer(allCritics[myIndex], allCritics[idx], size);
	}

	double * rels = new double[personNum];
	double * critics = new double[personNum];
	std::multimap<double, size_t> mapScores;
	for (size_t itemIdx = 0; itemIdx < size; ++ itemIdx)
	{
		::memset(rels, 0, sizeof(double)*personNum);
		::memset(critics, 0, sizeof(double)*personNum);
		///获取有效的相关度和对应的评分
		for (size_t personIdx = 0; personIdx < personNum; ++ personIdx)
		{
			if (allCritics[personIdx][itemIdx] <= 0)//invalid score
			{
				rels[personIdx] = 0;
				critics[personIdx] = 0;
			}
			else
			{
				rels[personIdx] = allRels[personIdx];
				critics[personIdx] = allCritics[personIdx][itemIdx];
			}
		}

		///计算加权打分
		double score = GetWeightedMead(critics, critics + personNum, rels, rels + personNum);
		mapScores.insert(std::make_pair(score, itemIdx));
	}

	///获取最终的推荐列表和对应的打分
	std::multimap<double, size_t>::reverse_iterator oIt = mapScores.rbegin();
	for (size_t count = 0; count < recNum; ++ count)
	{
		recItems[count] = oIt->second;
		recScores[count] = oIt->first;
		++ oIt;
	}

	delete [] critics;
	delete [] rels;
	delete [] allRels;
}

#endif //__XIAO5GE_SCORE_H__