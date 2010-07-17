#ifndef __XIAO5GE_MATH_UTILITY_H__
#define __XIAO5GE_MATH_UTILITY_H__

#include <cmath>
#include <algorithm>

#define MATH_UTILITY_CHECK_DISTANCE(begin1, end1, begin2, end2) \
	do \
	{ \
		if (std::distance(begin1, end1) != std::distance(begin2, end2)) \
		{ \
			return 0.0; \
		} \
	} while(0)

template <typename ForwardIterator>
double GetEuclideanDistance(
	const ForwardIterator & firstBegin,	
	const ForwardIterator & firstEnd,
	const ForwardIterator & secondBegin,
	const ForwardIterator & secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);
	
	ForwardIterator iter1 = firstBegin;
	ForwardIterator iter2 = secondBegin;
	double res = 0.0;
	while (iter1 != firstEnd)
	{
		res += (*iter1 - *iter2)*(*iter1 - *iter2);
		++ iter1;
		++ iter2;
	}
	res = sqrt(res);

	return res;
}

template <typename T>
double GetEuclideanDistance(
	const T * firstBegin,	
	const T * firstEnd,
	const T * secondBegin,
	const T * secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);
	
	const T * iter1 = firstBegin;
	const T * iter2 = secondBegin;
	double res = 0.0;
	while (iter1 != firstEnd)
	{
		res += (*iter1 - *iter2)*(*iter1 - *iter2);
		++ iter1;
		++ iter2;
	}
	res = sqrt(res);

	return res;
}

template <typename ForwardIterator>
double GetPearsonCorrelationCoefficient(
	const ForwardIterator & firstBegin,	
	const ForwardIterator & firstEnd,
	const ForwardIterator & secondBegin,
	const ForwardIterator & secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);
	ForwardIterator iter1 = firstBegin;
	ForwardIterator iter2 = secondBegin;
	double res1 = 0.0; //sum(X*Y)
	double res2 = 0.0; //sum(X)
	double res3 = 0.0; //sum(Y)
	double res4 = 0.0; //sum(X^2)
	double res5 = 0.0; //sum(Y^2)
	size_t num = 0;
	while (iter1 != firstEnd)
	{
		res1 += (*iter1)*(*iter2);
		res2 += *iter1;
		res3 += *iter2;
		res4 += (*iter1)*(*iter1);
		res5 += (*iter2)*(*iter2);
		++ num;
		++ iter1;
		++ iter2;
	}
	double res6 = (res1 - (res2 * res3) / num);  //(sum(XY) - sum(X)sum(Y)/N
	double res7 = (res4 - res2 * res2 / num);    //sum(X^2)-sum(X)sum(X)/N
	double res8 = (res5 - res3 * res3 / num);    //sum(Y^2)-sum(Y)sum(Y)/N
	
	if (res7 * res8 == 0.0)
	{
		return 0.0;
	}
	return res6 / sqrt(res7 * res8);
}

template <typename T>
double GetPearsonCorrelationCoefficient(
	const T * firstBegin,	
	const T * firstEnd,
	const T * secondBegin,
	const T * secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);
	const T * iter1 = firstBegin;
	const T * iter2 = secondBegin;
	double res1 = 0.0; //sum(X*Y)
	double res2 = 0.0; //sum(X)
	double res3 = 0.0; //sum(Y)
	double res4 = 0.0; //sum(X^2)
	double res5 = 0.0; //sum(Y^2)
	size_t num = 0;
	while (iter1 != firstEnd)
	{
		res1 += (*iter1)*(*iter2);
		res2 += *iter1;
		res3 += *iter2;
		res4 += (*iter1)*(*iter1);
		res5 += (*iter2)*(*iter2);
		++ num;
		++ iter1;
		++ iter2;
	}
	double res6 = (res1 - (res2 * res3) / num);  //(sum(XY) - sum(X)sum(Y)/N
	double res7 = (res4 - res2 * res2 / num);    //sum(X^2)-sum(X)sum(X)/N
	double res8 = (res5 - res3 * res3 / num);    //sum(Y^2)-sum(Y)sum(Y)/N
	
	if (res7 * res8 == 0.0)
	{
		return 0.0;
	}
	return res6 / sqrt(res7 * res8);
}

template <typename ForwardIterator>
double GetWeightedMead(
	const ForwardIterator & firstBegin,	
	const ForwardIterator & firstEnd,
	const ForwardIterator & secondBegin,
	const ForwardIterator & secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);

	ForwardIterator iter1 = firstBegin;
	ForwardIterator iter2 = secondBegin;
	double res1 = 0.0;
	double res2 = 0.0;
	while (iter1 != firstEnd)
	{
		res1 += (*iter1) * (*iter2);
		res2 +=  *iter2;
		++ iter1;
		++ iter2;
	}

	if (res2 == 0.0)
	{
		return 0.0;
	}
	return res1 / res2;
}

template <typename T>
double GetWeightedMead(
	const T * firstBegin,	
	const T * firstEnd,
	const T * secondBegin,
	const T * secondEnd
	)
{
	MATH_UTILITY_CHECK_DISTANCE(firstBegin, firstEnd, secondBegin, secondEnd);

	const T * iter1 = firstBegin;
	const T * iter2 = secondBegin;
	double res1 = 0.0;
	double res2 = 0.0;
	while (iter1 != firstEnd)
	{
		res1 += (*iter1) * (*iter2);
		res2 +=  *iter2;
		++ iter1;
		++ iter2;
	}

	if (res2 == 0.0)
	{
		return 0.0;
	}
	return res1 / res2;
}

template <typename ForwardIterator>
double GetTanimotoCoefficient(
	const ForwardIterator & firstBegin,
	const ForwardIterator & firstEnd,
	const ForwardIterator & secondBegin,
	const ForwardIterator & secondEnd)
{
	typedef typename ForwardIterator::value_type T;
	size_t dist1 = std::distance(firstBegin, firstEnd);
	size_t dist2 = std::distance(secondBegin, secondEnd);
	size_t maxSize = std::min(dist1, dist2);
	T * interSet = new T[maxSize];
	size_t setSize = 0;
	ForwardIterator iter1 = firstBegin;
	ForwardIterator iter2 = secondBegin;

	for (size_t fIdx = 0;  fIdx < dist1; ++ fIdx)
	{
		for (size_t sIdx = 0; sIdx < dist2; ++ sIdx)
		{
			if (*iter1 == *iter2)
			{
				interSet[setSize ++] = *iter1;
				++ iter2;
			}
		}
		++ iter1;
	}

	return (setSize + 0.0) / (dist1 + dist2 - setSize);
}

template <typename T>
double GetTanimotoCoefficient(
	const T * firstBegin,
	const T * firstEnd,
	const T * secondBegin,
	const T * secondEnd)
{
	size_t dist1 = std::distance(firstBegin, firstEnd);
	size_t dist2 = std::distance(secondBegin, secondEnd);
	size_t maxSize = std::min(dist1, dist2);
	T * interSet = new T[maxSize];
	size_t setSize = 0;
	const T * iter1 = firstBegin;
	const T * iter2 = secondBegin;

	for (size_t fIdx = 0;  fIdx < dist1; ++ fIdx)
	{
		for (size_t sIdx = 0; sIdx < dist2; ++ sIdx)
		{
			if (*iter1 == *iter2)
			{
				interSet[setSize ++] = *iter1;
				++ iter2;
			}
		}
		++ iter1;
	}

	return (setSize + 0.0) / (dist1 + dist2 - setSize);
}

#endif //__XIAO5GE_MATH_UTILITY_H__