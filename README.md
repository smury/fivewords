# fivewords

Motivated by this video on finding five five-letter words than cover 25 unique letters https://www.youtube.com/watch?v=_-AfhLQfb6w. The python code in that video took one month to run.
This was subsequently improved by viewers to under 1s https://www.youtube.com/watch?v=c33AZBnRHks.
Here is my own self-educational attempt at solving the problem using C++. Similar to Sylvester Hesp's implementation (https://github.com/oisyn/parkerwords), I use a bitwise representation for letter occurance. However, I use nested loops instead of recursive function calls. While I find this more readable/understandable, it is unfortunately an order of magnitude slower. 
