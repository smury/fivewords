#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>    // std::unique,
#include <unordered_map>
//#include <string_view>

using namespace std;



struct letter_freq
{
    char c;//letter
    unsigned int f;//frequency
    unsigned int n;//bit representation of the letter
};

unsigned int getbits(std::string_view word)
{
    unsigned int r = 0;
    for (char c : word)
        r |= 1 << (c - 'a'); //shifts 1 to the left by c-'a' positions then used bitwise OR (|=) with r
    return r;
}



int main()
{
    std::ifstream in("words_alpha.txt");

    std::string word;
    std::vector<std::string> allwords;//all 5 letter words, each with distinct letters
    std::vector<unsigned int> wordbits;
    std::unordered_map<unsigned int, size_t> bitstoindex;

    //make array of 5 letter words with distinct letters, excluding anagrams and storing letter frequencies
    std::array<letter_freq,26> freq= {};
    const std::array<char,26> alphabet= {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    for(int i=0; i<26; i++){
            freq[i].c=alphabet[i];
            freq[i].n=(1 <<(freq[i].c-'a'));
    }


    while (std::getline(in, word))
    {
        if (word.size() !=5) continue;
        unsigned int bits = getbits(word);//bitwise representation
        if (std::popcount(bits) != 5) continue;//only continue when there are 5 distinct letters

        //exclude anagrams
        if (!bitstoindex.contains(bits))
        {
            bitstoindex[bits] = wordbits.size();
            wordbits.emplace_back(bits);
            allwords.emplace_back(word);

            for(char c: word)
                freq[c - 'a'].f++;
        }
    }
    std::cout << allwords.size() << " words" <<'\n';


    //sort in order of ascending frequency
    std::sort (freq.begin(),freq.end(), [](auto x, auto y)
    {
        return x.f<y.f;
    });
    //for(auto x:freq) std::cout << x.c << " : " << x.f<< '\n';
    //std::cout << '\n';


    //bin the words by their least frequent letter
    std::array<std::vector<std::string_view>,26> allwords_by_mfl;
    std::array<std::vector<unsigned int>,26> wordbits_by_mfl;

    for(size_t k=0; k<allwords.size(); k++)
    {
        std::string_view word=allwords[k];
        int  i=0;
        while(!word.contains(freq[i].c)) i++;//i is in the index of the least frequent letter
        allwords_by_mfl[i].emplace_back(word);
        wordbits_by_mfl[i].emplace_back(wordbits[k]);
    }

    //for(int i=0; i<26; i++) std::cout << freq[i].c << " : " << allwords_by_mfl[i].size()<< '\n';
    //std::cout << '\n';


    //arrays to store the solutions
    std::array<std::string_view,5> sol= {};
    std::array<unsigned int,5> solbits= {};

    std::ofstream out("solutions_without_anagrams.txt");


    int counter=0;

    bool skipped1=0;
    for(int bin1=0; bin1<26; bin1++)
    {
        for(size_t i=0; i<allwords_by_mfl[bin1].size(); i++)
        {
            solbits[0]=wordbits_by_mfl[bin1][i];

            for(int bin2=bin1+1; bin2<26; bin2++)
            {
                if(solbits[0] & freq[bin2].n) continue;
                bool skipped2=skipped1;
                for(size_t i2=0; i2<allwords_by_mfl[bin2].size(); i2++)
                {
                    solbits[1]=wordbits_by_mfl[bin2][i2];
                    unsigned int cumbits2=solbits[0]|solbits[1];
                    if(std::popcount(cumbits2)!=10) continue;

                    for(int bin3=bin2+1; bin3<26; bin3++)
                    {
                        if(cumbits2 & freq[bin3].n) continue;//bin3 is the index of the least frequent letter that is not in the first two words
                        bool skipped3=skipped2;
                        for(size_t i3=0; i3<allwords_by_mfl[bin3].size(); i3++)
                        {
                            solbits[2]=wordbits_by_mfl[bin3][i3];
                            unsigned int cumbits3=cumbits2 | solbits[2];
                            if(std::popcount(cumbits3)!=15) continue;

                            for(int bin4=bin3+1; bin4<26; bin4++)
                            {
                                if(cumbits3 & freq[bin4].n) continue;//bin4 is the index of the least frequent letter that is not in the first three words
                                bool skipped4=skipped3;
                                for(size_t i4=0; i4<allwords_by_mfl[bin4].size(); i4++)
                                {
                                    solbits[3]=wordbits_by_mfl[bin4][i4];
                                    unsigned int cumbits4=cumbits3|solbits[3];
                                    if(std::popcount(cumbits4)!=20) continue;

                                    for(int bin5=bin4+1; bin5<26; bin5++)
                                    {
                                        if(cumbits4 & freq[bin5].n) continue;//bin5 is the index of the least frequent letter that is not in the first four words
                                        bool skipped5=skipped4;

                                        for(size_t i5=0; i5<allwords_by_mfl[bin5].size(); i5++)
                                        {
                                            solbits[4]=wordbits_by_mfl[bin5][i5];
                                            unsigned int cumbits5=cumbits4|solbits[4];
                                            if(std::popcount(cumbits5)!=25) continue;
                                            counter++;
                                            sol[0]=allwords_by_mfl[bin1][i];
                                            sol[1]=allwords_by_mfl[bin2][i2];
                                            sol[2]=allwords_by_mfl[bin3][i3];
                                            sol[3]=allwords_by_mfl[bin4][i4];
                                            sol[4]=allwords_by_mfl[bin5][i5];
                                            //std::cout<< sol[0] << '\t'<< sol[1] << '\t' << sol[2] << '\t'<< sol[3] << '\t' << sol[4] << '\n';
                                            std::stringstream buf;
                                            for (auto word : sol)
                                                buf << word << "\t";
                                            buf << "\n";
                                            out << buf.str();

                                        }
                                        if(skipped5) break;
                                        skipped5=true;
                                    }
                                }
                                if(skipped4) break;
                                skipped4=true;
                            }
                        }
                        if(skipped3) break;
                        skipped3=true;
                    }
                }
                if(skipped2) break;
                skipped2=true;
            }
        }
        if(skipped1) break;
        skipped1=true;
    }


    std::cout << counter << " solutions written";





    return 0;
}
