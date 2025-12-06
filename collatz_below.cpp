#include <iostream>  // cout 
#include <locale>    // for number commas
#include <cstdlib>   // Required for strtol
#include <climits> // Required for ULLONG_MAX
// sinlge treaded version of collatz_omp.cpp 
//g++ -O4 -o collatz collatz_below.cpp
//collatz 10000000
//Starting.   Find longest Collatz chain below 10,000,000
//All Done. Longest chain below 10,000,000 was 8,400,511 steps:685

using namespace std;
#define coll_t unsigned long long 
#define BITS sizeof(coll_t)*8-1
#define COL_MAX ~((coll_t)1 << BITS)
#define TOO_BIG COL_MAX/3-1 // (TOO_BIG+1)*3+1 > COL_MAX_MAX 
#define CACHE_SIZE 500000000
int *cache; 

#if defined(__MINGW32__)  // as of 2025 MINGW does not support locale for numbers 
struct comma_out : numpunct<char> {
    char do_thousands_sep()   const { return ','; }  // separate with spaces
    string do_grouping() const { return "\3"; } // groups of 3 digit
};
#endif

// Function to calculate Collatz steps for a given number
int collatz_steps(coll_t x) {
  if (x > TOO_BIG) {
	  cerr<<"N too large: "<<x<<" Max is "<<TOO_BIG<<endl; 
	  exit(EXIT_FAILURE);
  }
  if (x==1) return 0; // or 1 if you think Collatz works that way 
  if (x < CACHE_SIZE && cache[x]!=0) return cache[x]; 
  int c = 0;
  if (x&1) c = 2 + collatz_steps((x<<1)-(x>>1));  // (x*3+1)/2
  else     c = 1 + collatz_steps(x>>1);  // x*2
  if (x < CACHE_SIZE) cache[x] = c; 
  return c;
 }

void coll_below(long long n) { 
   coll_t max_n = 0;
   int max_s = 0;
   for (coll_t i = 1;i<n;i+=2) {
       int t = collatz_steps(i); 
       if (t > max_s) {
           max_s = t;
           max_n = i; 
       }
    }	   
    cout<<"\nAll Done. Longest chain below "<<n<<" was "<<max_n<<" steps:"<<max_s<<endl; 
}

int main(int argc, char** argv) { 
    char* endp;
#if defined(__MINGW32__) // MINGW does not support locales  Roll your own above
	cout.imbue(locale(cout.getloc(), new comma_out)); // For Windows MINGW 
	cerr.imbue(locale(cout.getloc(), new comma_out)); // For Windows MINGW 
#else
	cout.imbue(locale("")); // for Linux g++
    cerr.imbue(locale("")); // for Linux g++
#endif
if (argc < 2) 
       cout << "You can also use a number as an argument. Defaulting to 1,000,000"<<endl; 
    cache = (int*)calloc((CACHE_SIZE+4) * sizeof(int),1);
    coll_t  upper_limit = ((argc > 1) ? strtoull(argv[1],&endp,10) : 1000000); // max n defaults to 1,000,000
    cout<<"Starting.   Find longest Collatz chain below "<<upper_limit<<endl; 
	cout<<"Max for n to reach is "<<TOO_BIG<<endl; 
    coll_below(upper_limit);
}
