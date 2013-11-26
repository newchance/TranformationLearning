/*
 * TBL.cpp
 * 
 * The implementation of TBL.h
 *
 * @author:   zhang haibo
 * @time:     2013-11-21
 * @version:  1.0
 */

#include "TBL.h"
#include "Utility.h"
#include <fstream>
#include <iostream>

//global data
const string BEGIN = "<s>";//the label for the begining
const string END = "</s>";//the label for the end

//threshold value for learning stops
const int LEARN_STOP = 2;

//feature function
string PrecedingWordTag(const Sentence&, const int&);//the preceding word tag
string FollowingWordTag(const Sentence&, const int&);//the following word tag
string TwoBeforeTag(const Sentence&, const int&);//the word two tag before the current word
string TwoAfterTag(const Sentence&, const int&);//the word two tag after the current word
string OneOfTwoPrecedingWordsTag(const Sentence&, const int&);//One of the two preceding words tag
string OneOfTwoFollowingWordsTag(const Sentence&, const int&);//One of the two following words tag
string OneOfThreePrecedingWordsTag(const Sentence&, const int&);//One of the three preceding words tag
string OneOfThreeFollowingWordsTag(const Sentence&, const int&);//One of the three following words tag
string PrecedingFollowingWordsTag(const Sentence&, const int&);//the preceding word tag and the following word tag
string PreviousTwoTag(const Sentence&, const int&);//the previous two tags
string AfterTwoTag(const Sentence&, const int&);//the after two tags

/*
 *@function:constructor
 */
TBL::TBL()
{
    _word_tags = new WordTags();
    //init feature functions
    _mapFeatFunc["PrecedingWordTag"] = &PrecedingWordTag;
    _mapFeatFunc["FollowingWordTag"] = &FollowingWordTag;
    _mapFeatFunc["TwoBeforeTag"] = &TwoBeforeTag;
    _mapFeatFunc["TwoAfterTag"] = &TwoAfterTag;
    _mapFeatFunc["OneOfTwoPrecedingWordsTag"] = &OneOfTwoPrecedingWordsTag;
    _mapFeatFunc["OneOfTwoFollowingWordsTag"] = &OneOfTwoFollowingWordsTag;
    _mapFeatFunc["OneOfThreePrecedingWordsTag"] = &OneOfThreePrecedingWordsTag;
    _mapFeatFunc["OneOfThreeFollowingWordsTag"] = &OneOfThreeFollowingWordsTag;
    _mapFeatFunc["PrecedingFollowingWordsTag"] = &PrecedingFollowingWordsTag;
    _mapFeatFunc["PreviousTwoTag"] = &PreviousTwoTag;
    _mapFeatFunc["AfterTwoTag"] = &AfterTwoTag;
}

/*
 *@function:deconstructor
 */
TBL::~TBL()
{
    OutputBestRules("model/rules");
    delete _word_tags;
}

/*
 *@function:the main function for training algorithm
 *
 *@parameter:
 *    fileName: the corpus file name
 *@return: void
 */
void TBL::Train(const string& fileName)
{
    cout << "training start.................." << endl;
    //upload recources
    UploadRecource(fileName);
    //apply initial-state annotator to corpus
    InitAnnotator();
    //learning transformation rules
    LearnRules();
    cout << "training success................" << endl;    
}

/*
 *@function:upload the corpus recource
 *
 *@parameter:
 *   fileName: the corpus name
 *@return:void
 */
bool TBL::UploadRecource(const string& fileName)
{
    cout << "starting uploading resources......." << endl;
    //open file
    ifstream fin(fileName.c_str());
    if(!fin)
    { 
        cerr << "can not open the file " << fileName << endl;
        return false;
    }
    
    string line;
    while(getline(fin,line))
    {
        vector<string> words_tags = split(line);
        //cout << words_tags.size() << endl;
        if(words_tags.empty())
            continue;
        vector<string> words, tags;
        int size = words_tags.size();
        for(int i = 0; i < size; ++i)
        {
            vector<string> word_tag = split(words_tags[i], "/");
            if(word_tag.empty() || word_tag.size() != 2)
                continue;
            words.push_back(word_tag[0]);
            tags.push_back(word_tag[1]);
            //insert into WordTags
            _word_tags->Insert(word_tag[0], word_tag[1]);
        }
        //cout << words.size() << " " << tags.size() << endl;
        //sentence
        Sentence st;
        st.wordSequence = words;
        st.refTags = tags;
        _corpus.push_back(st);
    }
 
    //close file
    fin.close();
     
    cout << "uploading resources......success." << endl;
    cout << "corpus size is " << _corpus.size() << endl;

    return true; 
}


/*
 *@function: initial-state annotator to corpus
 */
void TBL::InitAnnotator()
{
     cout << "init annotator............." << endl;
     int corpus_size = _corpus.size();
     for(int i = 0; i < corpus_size; ++i)
     {
         vector<string> bestTags;
         vector<string> words = _corpus[i].wordSequence;
         int words_size = words.size();
         for(int j = 0; j < words_size; ++j)
         {
             bestTags.push_back(_word_tags->GetMostFrequencyTag(words[j]));
         }
         _corpus[i].currentTags = bestTags;
     }
     cout << "init annotator.............success." << endl;
}


/*
 *@function:learning the transformation rules
 */
void TBL::LearnRules()
{
    cout << "Learning rules start............" << endl; 
    //calculate the total errors
    int loop = 1;
    int errorCount = CalErrorCount();
    int totalWords = CalCorpusSize();
    while(true)
    {
       cout << "*******************The " << loop++ << " iteration: error counts " << errorCount << "********************"<< endl;
       cout << "The precision is " << (double)(totalWords-errorCount)/totalWords << endl;

       //extract tranformation rules
       TRules rules;
       ExtractRules(rules);
       cout << "rules size is " << rules.size() << endl;
       //find the best rule
       Rule best;
       int minusErrorsCount;
       CalBestRule(rules,best,minusErrorsCount);
       cout << "The decrease error count is " << minusErrorsCount << endl;
       //check the counts of errors
       if(minusErrorsCount <= 0 || minusErrorsCount < LEARN_STOP)
           break;
       //add the best rule into ordered-rules list
       _order_rules.push_back(best);
       //update the errorCount
       errorCount -= minusErrorsCount;
       //apply best rule to training corpus
       UpdateCorpus(best);
    }
    cout << "Learning rules success.........." << endl;
}


/*
 *@function: calculate the number of errors between refTags and bestTags
 */
int TBL::CalErrorCount()
{
    int errorCount = 0;
    int corpus_size = _corpus.size();
    for(int i = 0; i < corpus_size; ++i)
    {
        vector<string> refTags = _corpus[i].refTags;
        vector<string> currentTags = _corpus[i].currentTags;
        if(refTags.size() != currentTags.size())
            continue;
        int size = refTags.size();
        for(int j = 0; j < size; ++j)
        {
            if(refTags[j] != currentTags[j])
                 ++errorCount;
        }
    }

    return errorCount;
}

/*
 *@function:extract transformation rules
 *
 *@parameter:
 *    TRules: the extracted rules
 */
void TBL::ExtractRules(TRules& rules)
{
    cout << "extracting rules ...........";
    rules.clear();

    int corpus_size = _corpus.size();
    for(int i = 0; i < corpus_size; ++i)
    {
        //cout << i+1 << endl;
        Sentence st = _corpus[i];
        int size = st.wordSequence.size();
        for(int j = 0; j < size; ++j)
        {
            //same
            if(st.refTags[j] == st.currentTags[j])
                continue;
            //different
            string fromTag = st.currentTags[j];
            string toTag = st.refTags[j];
            map<string, FeatFuncPtr>::iterator iterator = _mapFeatFunc.begin();
            for(; iterator != _mapFeatFunc.end(); ++iterator)
            {
                Rule rl;
                rl.fromTag = fromTag;
                rl.toTag = toTag;
                rl.featureName = iterator->first;
                //cout << rl.featureName << endl;
                string tag = _mapFeatFunc[rl.featureName](st, j);
                //cout << "tag " << tag << endl;
                vector<string> tags = split(tag, "|");
                //cout << tags.size() << endl; 
                //for each tag
                for(int t = 0; t < tags.size(); ++t)
                {
                    rl.featureValue = tags[t];
                    rules.insert(rl);
                }
            }
        }      
    }
    cout << "success" << endl;

}

/*
 *@function: search the best rule
 *
 *@parameter:
 *    rules: all the transformation rules
 *    best: keep the result
 *    errorCount: the decreasing error count
 *@return: void
 */
void TBL::CalBestRule(const TRules& rules, Rule& best, int& errorCount)
{
    cout << "find best rule..............";
    int maxErrorCount = 0;
    //cout << rules.size() << endl;
    TRules::iterator iter = rules.begin();
    for(; iter != rules.end(); ++iter)
    {
         int positive = 0, negative = 0;
         int corpusSize = _corpus.size();
         for(int i = 0; i < corpusSize; ++i)
         {
              Sentence st = _corpus[i];
              int stSize = st.wordSequence.size();
              for(int j = 0; j < stSize; ++j)
              {
                   if(st.currentTags[j] == iter->fromTag)
                   {
                        string targetTag = _mapFeatFunc[iter->featureName](st,j);
                        vector<string> tags = split(targetTag, "|");
                        //simulate circumtance
                        if(IsInVec(tags, iter->featureValue))
                        {
                            if(iter->toTag == st.refTags[j])
                                 ++positive;
                            else if(st.refTags[j] == iter->fromTag)
                                 ++negative;
                        }
                   }
              }
         }
  
         if(positive > maxErrorCount+negative)
         {
              maxErrorCount = positive - negative;
              best = *iter;
         }
    }
    
    errorCount = maxErrorCount;
    cout << "success" << endl;
}

/*
 *@function:update the training corpus
 *
 *@parameter:
 *    best: the best rule
 *@return:void
 */
void TBL::UpdateCorpus(const Rule& best)
{
    cout << "update training corpus............";
    int corpusSize = _corpus.size();
    for(int i = 0; i < corpusSize; ++i)
    {
        Sentence st = _corpus[i];
        int stSize = st.wordSequence.size();
        for(int j = 0; j < stSize; ++j)
        {
            if(best.fromTag == st.currentTags[j])
            {
                string tag = _mapFeatFunc[best.featureName](st, j);
                vector<string> tags = split(tag, "|");
                if(IsInVec(tags, best.featureValue))
                {
                     //simulate
                     _corpus[i].currentTags[j] = best.toTag;
                }
            }
        }
    }
    cout << "success" << endl;    
}

/*
 *@function: output the order-rules into file
 *
 *@parameter:
 *    fileName: the target file
 */
void TBL::OutputBestRules(const string& fileName)
{
    //open file
    ofstream fout(fileName.c_str());
    if(!fout)
    {
        cerr << "can not open the file " << fileName << endl;
        return;
    }

    //write to file
    vector<Rule>::iterator iter = _order_rules.begin();
    for(; iter != _order_rules.end(); ++iter)
    {
        fout << iter->fromTag << " " <<
                iter->toTag << " " <<
                iter->featureName << " " <<
                iter->featureValue << endl;
    }

    //close file
    fout.close();
}

/*
 *@function: calculate the total words in the corpus
 *
 */
int TBL::CalCorpusSize()
{
    int ret = 0;
    Corpus::iterator iter = _corpus.begin();
    for(; iter != _corpus.end(); ++iter)
        ret += iter->wordSequence.size();
    return ret;
}

/*
 *@function:the preceding word tag
 */
string PrecedingWordTag(const Sentence& sen, const int& pos)
{
    return pos > 0 ? sen.currentTags[pos-1] : BEGIN;
}


/*
 *@function:the following word tag
 */
string FollowingWordTag(const Sentence& sen, const int& pos)
{
    return pos+1 < sen.wordSequence.size() ? sen.currentTags[pos+1] : END;
}

/*
 *@function:the word two tag before the current word 
 */
string TwoBeforeTag(const Sentence& sen, const int& pos)
{
    return pos > 1 ? sen.currentTags[pos-2] : BEGIN;
}

/*
 *@function:the word two tag after the current word
 */
string TwoAfterTag(const Sentence& sen, const int& pos)
{
    return pos+2 < sen.wordSequence.size() ? sen.currentTags[pos+2] : END;
}

/*
 *@function:One of the two preceding words tag
 */   
string OneOfTwoPrecedingWordsTag(const Sentence& sen, const int& pos)
{
    return PrecedingWordTag(sen, pos) + "|" + TwoBeforeTag(sen, pos);
}

/*
 *@function:One of the two following words tag
 */
string OneOfTwoFollowingWordsTag(const Sentence& sen, const int& pos)
{
    return FollowingWordTag(sen, pos) + "|" + TwoAfterTag(sen, pos);
}

/*
 *@function:One of the three preceding words tag
 */
string OneOfThreePrecedingWordsTag(const Sentence& sen, const int& pos)
{
    string thirdBeforeTag = (pos > 2 ? sen.currentTags[pos-3] : BEGIN);
    return thirdBeforeTag + "|" + OneOfTwoPrecedingWordsTag(sen,pos);
}

/*
 *@function:One of the three following words tag
 */
string OneOfThreeFollowingWordsTag(const Sentence& sen, const int& pos)
{
    string thirdAfterTag = (pos+3 < sen.wordSequence.size() ? sen.currentTags[pos+3] : END);
    return thirdAfterTag + "|" + OneOfTwoFollowingWordsTag(sen, pos);
}
  
/*
 *@function:the preceding word tag and the following word tag
 */  
string PrecedingFollowingWordsTag(const Sentence& sen, const int& pos)
{
    return PrecedingWordTag(sen, pos) + "&" + FollowingWordTag(sen, pos);
}

/*
 *@function:the previous tags
 */
string PreviousTwoTag(const Sentence& sen, const int& pos)
{
    return PrecedingWordTag(sen, pos) + "&" + TwoBeforeTag(sen, pos);
}

/*
 *@function: the after tags
 */
string AfterTwoTag(const Sentence& sen, const int& pos)
{
    return FollowingWordTag(sen,pos) + "&" + TwoAfterTag(sen, pos);
}
