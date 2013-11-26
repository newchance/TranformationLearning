/*
 * WordTags.h
 * 
 * @author: zhang haibo
 * @time:   2013-11-20
 * @version:1.0
 *
 * The header provides the interfaces for word-tags.
 * Word-tags is composed of one word and its possible tags.
 * It has the functions:
 *   1)insert one word-tag
 *   2)get the most frequency tag of one word
 *   3)get all the possible tags of one word
 *   4)output the word-tags into file  
 */

#ifndef WORDTAGS_H
#define WORDTAGS_H

#include <string>
#include <vector>
#include <map>
using namespace std;

struct TagFrequency
{
    string tag;
    int frequency;

    TagFrequency(const string& _tag, const int& _frequency)
         :tag(_tag),frequency(_frequency)
    {}
    
    //desscend
    bool operator >(const TagFrequency& right) const
    {
        return frequency > right.frequency;
    }
   
    //asscend
    bool operator <(const TagFrequency& right) const
    {
        return frequency < right.frequency;
    }
};

typedef map<string, vector<TagFrequency> > MapWordTags;

class WordTags
{
public:
    WordTags(){}
    ~WordTags(){}
    void Insert(const string&, const string&); //insert word-tag into _wordTags
    void SortDesscend();//sort the tags of one word according to the frequency
    string GetMostFrequencyTag(const string&); //get the most frequency tag of one word
    vector<string> GetAllTags(const string&);  //get all tags of one word
    void Output(const string&); //output the word-tags into file
private:
    MapWordTags  _wordTags;//stores word-tags-frequency

};

#endif
