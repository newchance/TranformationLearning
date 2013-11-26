/*
 * TBL.h
 * 
 * The core file for trainging.
 *
 * The implementation of transformation-based learning algorithm
 *
 * @author:  zhang haibo
 * @time:    2013-11-20
 * @version: 1.0
 * 
 * The class provides the following functions:
 *  1)count the word-tags of the corpus
 *  2)init the annotator
 *  3)extract the tranformation rules
 *  4)update the training corpus
 *  5)calculate the error counts
 *  6)update the ordered list
 */
#ifndef TBL_H
#define TBL_H

#include "WordTags.h"
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

/*
 *define the sentence data structure
 */
class Sentence
{
public:
    vector<string> wordSequence;//word sequence
    vector<string> refTags;//the gold tags
    vector<string> currentTags;//the current tags
};

/*
 *define the total corpus
 */
typedef vector<Sentence> Corpus;

/*
 *define one tranformation rule
 */
struct Rule
{
    string fromTag;
    string toTag;
    string featureName;
    string featureValue;
};

/*
 *the comparator of two rules
 */
struct RuleComp
{
    bool operator()(const Rule& a, const Rule& b)
    {
         if(a.fromTag == b.fromTag && a.toTag == b.toTag 
              && a.featureName == b.featureName && a.featureValue == b.featureValue)
             return 0;
         else if(a.fromTag == b.fromTag)
             return 1;
         else
             return -1;
    }
};

/*
 *define the total transformation rules
 */
typedef set<Rule, RuleComp> TRules;

/*
 *define function pointer
 */
typedef string (*FeatFuncPtr)(const Sentence&,const int&);

/*
 *define the Transformation-based learning algorithm
 */
class TBL
{
public:
    TBL();
    ~TBL();
    void Train(const string&);//the main function of training algorithm
    void OutputBestRules(const string&);//output the order-rules into file 
private:
//function member
    bool UploadRecource(const string&);
    void InitAnnotator();
    void LearnRules();
    int CalErrorCount();//calculate the errors of corpus
    void ExtractRules(TRules&);
    void CalBestRule(const TRules&, Rule&, int&);//get the best rule in the middle of all transformation rules
    void UpdateCorpus(const Rule&);//update the training corpus
    int CalCorpusSize();//calcualte the total words in the corpus
//data member
    WordTags*  _word_tags;
    Corpus _corpus;
    map<string, FeatFuncPtr> _mapFeatFunc;
    vector<Rule> _order_rules; 
};

#endif
