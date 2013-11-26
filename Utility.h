/*
 *the common functions
 *
 *@author:  zhang  haibo
 *@time:    2013-11-08
 *@version: 0.1
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <math.h>
#include <map>
using namespace std;


/*
 *split one sequence with the separator
 */
vector<string> split(const string& str, const string& separator = " ");


/*
 *calculate the total counts
 */
int GetTotalCounts(map<vector<int>, int>& grams);

/*
 *calculate the log 10 value of a/b
 */
double Log10(double a, double b);

/*
 *decide a whether or not in b
 */
bool IsInVec(const vector<string>&, const string&);

#endif



