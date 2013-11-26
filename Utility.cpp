/*
 *the common functions
 *
 *@author:  zhang  haibo
 *@time:    2013-11-08
 *@version: 0.1
 */

#include "Utility.h"

/*
 *split one sequence with the separator
 */
vector<string> split(const string& str, const string& separator)
{
    vector<string> result;
    if(str.empty() || str.size() == 0)
        return result;
    int index = 0;
    while(true)
    {
        int next;
        if((next = str.find(separator, index)) == string::npos)
        {
             string part = str.substr(index);
             if(!part.empty() && part.size() != 0)
                 result.push_back(part);
             break;
        }
        else
        {
             string part = str.substr(index, next-index);
             if(!part.empty() && part.size() != 0)
                 result.push_back(part);
             index = next + separator.size();
        }
    }

    return result;
}


/*
 *calculate the total counts
 */
int GetTotalCounts(map<vector<int>, int>& grams)
{
    int totalSize = 0;
    map<vector<int>, int>::iterator iter = grams.begin();
    for(; iter != grams.end(); ++iter)
        totalSize += iter->second;
    return totalSize;
}

/*
 *calculate the log 10 value of a/b
 */
double Log10(double a, double b)
{
    return log10(a/b);
}

/*
 *decide a whether or not in b
 */
bool IsInVec(const vector<string>& b, const string& a)
{
    if(b.empty())
       return false;
    int size = b.size();
    for(int i = 0; i < size; ++i)
    {
        if(b[i] == a)
            return true;
    }

    return false;
}
