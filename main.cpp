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

    //take out the letter bit representation for better locality
    unsigned int letterbitrep[26];
    for(int i=0; i<26; i++){
            letterbitrep[i]=freq[i].n;
    }


    //bin the words by their least frequent letter
    std::vector<unsigned int> wordbits_by_mfl[26];

    for(size_t k=0; k<allwords.size(); k++)
    {
        std::string_view word=allwords[k];
        int  i=0;
        while(!word.contains(freq[i].c)) i++;//i is in the index of the least frequent letter
        wordbits_by_mfl[i].emplace_back(wordbits[k]);
    }

    //for(int i=0; i<26; i++) std::cout << freq[i].c << " : " << allwords_by_mfl[i].size()<< '\n';
    //std::cout << '\n';


    //array to store the solutions
    std::array<unsigned int,5> solbits= {};

    std::ofstream out("solutions_without_anagrams.txt");
    std::stringstream buf;

    auto print_sols=[&](const std::array<unsigned int,5>& solbits){
                                                    for (auto x : solbits)
                                                        buf << allwords[bitstoindex[x]] << "\t";
                                                    buf << "\n";
    };

    int counter=0;

    bool skipped1=0;
    for(int bin1=0; bin1<26; bin1++)
    {
        for(auto w : wordbits_by_mfl[bin1])
        {
            solbits[0]=w;

            for(int bin2=bin1+1; bin2<26; bin2++)
            {
                if(solbits[0] & letterbitrep[bin2]) continue;
                bool skipped2=skipped1;
                for(auto w : wordbits_by_mfl[bin2])
                {
                    solbits[1]=w;
                    unsigned int cumbits2=solbits[0]|solbits[1];
                    if(std::popcount(cumbits2)!=10) continue;

                    for(int bin3=bin2+1; bin3<26; bin3++)
                    {
                        if(cumbits2 & letterbitrep[bin3]) continue;//bin3 is the index of the least frequent letter that is not in the first two words
                        bool skipped3=skipped2;
                        for(auto w : wordbits_by_mfl[bin3])
                        {
                            solbits[2]=w;
                            unsigned int cumbits3=cumbits2 | solbits[2];
                            if(std::popcount(cumbits3)!=15) continue;

                            for(int bin4=bin3+1; bin4<26; bin4++)
                            {
                                if(cumbits3 & letterbitrep[bin4]) continue;//bin4 is the index of the least frequent letter that is not in the first three words
                                bool skipped4=skipped3;
                                for(auto w : wordbits_by_mfl[bin4])
                                {
                                    solbits[3]=w;
                                    unsigned int cumbits4=cumbits3|solbits[3];
                                    if(std::popcount(cumbits4)!=20) continue;

                                    for(int bin5=bin4+1; bin5<26; bin5++)
                                    {
                                        if(cumbits4 & letterbitrep[bin5]) continue;//bin5 is the index of the least frequent letter that is not in the first four words
                                        bool skipped5=skipped4;

                                        for(auto w : wordbits_by_mfl[bin5])
                                        {
                                            solbits[4]=w;
                                            unsigned int cumbits5=cumbits4|solbits[4];
                                            if(std::popcount(cumbits5)!=25) continue;
                                            counter++;
                                            //std::cout<< solbits[0] << '\t'<< solbits[1] << '\t' << solbits[2] << '\t'<< solbits[3] << '\t' << solbits[4] << '\n';
                                            print_sols(solbits);

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

    out << buf.str();
    std::cout << counter << " solutions written";





    return 0;
}
