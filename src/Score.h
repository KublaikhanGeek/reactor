#ifndef __XIAO5GE_SCORE_H__
#define __XIAO5GE_SCORE_H__

#include "MathUtility.h"

#include <map>

/**
 * @brief  Euclidean��֣�ȡֵ(0, 1], Euclidean����Խ����������ֵԽ�ӽ�1
 * @param  dist, Euclidean����
 * @return ת����ĵ÷�
 */
double GetEuclideanScore(double dist)
{
	return (1 / (1 + dist));
}

/**
 * @brief  Pearson��֣�ȡֵ[-1, 1], Խ����������ֵԽ�ӽ�1
 * @param  coef, the Pearson���ϵ��ֵ
 * @return Person���ֵ
 */
double GetPearsonScore(double coef)
{
	return coef;
}

/**
 * @brief  Tanimoto��֣�ȡֵ[0, 1], Խ����������ֵԽ�ӽ�1
 * @param  coef, the Tanimotoϵ��ֵ
 * @return Tanimoto���ֵ
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
	const double ** allCritics,    //[in]  �����˵Ĵ�ֱ�
	size_t personNum,              //[in]  �����˵ĸ���
	size_t size,                   //[in]  ��ֱ��С
	size_t myIndex,                //[in]  ���ڴ�ֱ����±�
	ScoreFunc scorer,              //[in]  ��ֺ���
	size_t recNum,                 //[out] ���Ƽ������
	int * recItems,                //[out] ���Ƽ����б�
	double * recScores             //[out] ���Ƽ���÷�
	)
 {
	double * allRels = new double[personNum];
	::memset(allRels, 0, sizeof(double)*personNum);
	///�������е���ض�
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
		///��ȡ��Ч����ضȺͶ�Ӧ������
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

		///�����Ȩ���
		double score = GetWeightedMead(critics, critics + personNum, rels, rels + personNum);
		mapScores.insert(std::make_pair(score, itemIdx));
	}

	///��ȡ���յ��Ƽ��б�Ͷ�Ӧ�Ĵ��
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