/*
 * WordTags.cpp
 *
 * The implementation of WordTags.h
 *
 * @author:  zhang haibo
 * @time  :  2013-11-20
 * @version: 1.0
 *
 * Refer to the header of the file.
 */

#include "WordTags.h"
#include <fstream>
#include <iostream>
#include <algorithm>

/*
 *@function: insert the word and tag into the wordTags
 *
 *@parameters:
 *   word: the word which will be inserted
 *   tag:  the tag of the input word
 *@return: void      
 */
void WordTags::Insert(const string& word, const string& tag)
{
   MapWordTags::iterator iter = _wordTags.find(word);
   if(iter == _wordTags.end())//not found
   {
        vector<TagFrequency> tagsVec;
        tagsVec.push_back(TagFrequency(tag,1));
        _wordTags[word] = tagsVec;
   }
   else//found
   {
        vector<TagFrequency> tagsVec = _wordTags[word];
        //check the tag whether is exist
        bool isExisted = false;
        int tagsSize = tagsVec.size();
        int i;
        for(i = 0; i < tagsSize; ++i)
        {
            if(tagsVec[i].tag == tag)
            {
                isExisted = true;
                break;  
            }
        }
        //existed
        if(isExisted)
        {
            ++_wordTags[word][i].frequency;
        }
        else//not existed
        {
            _wordTags[word].push_back(TagFrequency(tag,1));
        }
   }    
}

/*
 *@function: sort each word-tags according to the frequency of each tag
 *
 *@parameter:none
 *@return:void
 */
void WordTags::SortDesscend()
{
    MapWordTags::iterator iterator = _wordTags.begin();
    for(; iterator != _wordTags.end(); ++iterator)
    {
        sort(iterator->second.begin(), iterator->second.end(), greater<TagFrequency>());
    }
}

/*
 *@function: get the most frequency tag of one word
 *
 *@parameter:
 *   word: the word    
 *
 *@return:
 *   tag: the tag which is the most frequency
 */
string WordTags::GetMostFrequencyTag(const string& word)
{
    return _wordTags[word][0].tag;
}

/*
 *@function: get all the possible tags of one word
 *
 *@parameter:
 *   word: the word
 *@return:
 *   tags: the possible tags of the word
 */
vector<string> WordTags::GetAllTags(const string& word)
{
    vector<string> retTags;
    vector<TagFrequency> tagsVec = _wordTags[word];
    vector<TagFrequency>::iterator iter = tagsVec.begin();
    for(; iter != tagsVec.end(); ++iter)
    {
         retTags.push_back(iter->tag);
    }

    return retTags;
}

/*
 *@function:output the wordTags into file
 *
 *@parameter:
 *    fileName: the file name
 *@return: void
 */
void WordTags::Output(const string& fileName)
{
   ofstream fout(fileName.c_str());
   if(!fout)
   {
       cerr << "can not open the file " << fileName << endl;
       return;
   }
   
   MapWordTags::iterator iter = _wordTags.begin();
   for(; iter != _wordTags.end(); ++iter)
   {
       //the word
       fout << iter->first << " ";
       //the tags
       vector<string> tags = GetAllTags(iter->first);
       int tagsSize = tags.size();
       for(int i = 0; i < tagsSize; ++i)
           fout << tags[i] << " ";
       fout << endl;
   }

   //close file
   fout.close();
}
