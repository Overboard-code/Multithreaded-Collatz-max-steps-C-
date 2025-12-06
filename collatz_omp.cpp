#include <iostream> // cout 
#include <vector> // to store results 
#include <thread> // for multiprocessing 
#include <locale> // for number commas
#include <cstdlib>   // Required for strtol
#include <fstream>
#include <sstream> // For std::stringstream
#include <string>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ctime>

// g++ -O4 -o testo collatz_omp.cpp  
// ./testo 10000000
// Starting Collatz calculations from 1 up to 10,000,000 with 8 threads.
// Longest chain found for number 8,400,511 with length 685
/*
less than 10 is 9, which has 19 steps,
less than 100 is 97, which has 118 steps,
less than 1,000 is 871, which has 178 steps,
less than 10,000 is 6,171, which has 261 steps,
less than 100,000 is 77,031, which has 350 steps,
less than 1 million is 837,799, which has 524 steps,
less than 10 million is 8,400,511, which has 685 steps,
less than 100 million is 63,728,127, which has 949 steps,
less than 1 billion is 670,617,279, which has 986 steps,
less than 10 billion is 9,780,657,631, which has 1132 steps,
less than 100 billion is 75,128,138,247, which has 1228 steps.
less than 1 Trillion is 989,345,275,647, which has 1,348 steps*/

using namespace std;
#define coll_t __int128
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

string coll2string(coll_t value) {
    if (value == 0) return "0";
    std::string result;
    bool isNegative = (value < 0);
    if (isNegative) value = -value;

    while (value > 0) {
        result += static_cast<char>((value % 10) + '0');
        value /= 10;
    }

    std::reverse(result.begin(), result.end());
    if (isNegative) result.insert(0, "-");

    return result;
}


string n_str(coll_t n)
{
    std::string s = coll2string(n);
    if (s.size() < 4) return s;
    else
    {
        std::string tack_on = "," + s.substr(s.size() - 3, s.size());
        return n_str(n / 1000) + tack_on;
    }
}
 
// logger NOT sycronous  mesages all over each other 
static void log(const string& message) {
        // Get current time
		thread::id th_id = this_thread::get_id();
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);
        ostringstream oss;
        oss << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S: TH-");
        oss << th_id; 
        // Output to console with timestamp and message
        cout << "[" << oss.str() << "] " << message << endl;
    }
string howLong(chrono::steady_clock::duration total_duration ) {
       	
 // Calculate total duration
    //auto total_duration = end - start;

    // Extract hours, minutes, and seconds
    auto hours = chrono::duration_cast<chrono::hours>(total_duration);
    auto minutes = chrono::duration_cast<chrono::minutes>(total_duration % chrono::hours(1));
    auto seconds = chrono::duration_cast<chrono::seconds>(total_duration % chrono::minutes(1));
   auto millis = chrono::duration_cast<chrono::milliseconds>(total_duration);
    return to_string(hours.count()) + ":" + to_string(minutes.count()) + ":" +
          to_string(seconds.count()) + "." + to_string(millis.count());
}

// Recursive function to calculate Collatz steps for a given number
int collatz_steps_rec(coll_t x) {
  if (x > TOO_BIG && x&1) {  // odds will be made as 3*n+1 
     cerr << "N is too large: "<<n_str(x)<<" Max is:" <<n_str(TOO_BIG)<<endl;
     exit(EXIT_FAILURE); // Return a non-zero value for error
  }
  if (x==1) return 0; // or 1 if you think Collatz works that way 
  if (x < CACHE_SIZE && cache[x]!=0) return cache[x]; 
  int c = 0;
  if (x&1) c = 2 + collatz_steps_rec((x<<1)-(x>>1));  // (x*3+1)/2
  else     c = 1 + collatz_steps_rec(x>>1);  // x*2
  if (x < CACHE_SIZE) cache[x] = c; 
  return c;
 }

// Function to calulate Collatz steps for a given number 
coll_t collatz_steps(coll_t n) {
    coll_t original_n = n;
    int c = 0; // Start counting from the initial number
    while (n != 1) { 
	    if (n > TOO_BIG && n&1) {  // odds will be made as 3*n+1 
	       cerr << "N is too large: "<<n_str(n)<<" Max is:" <<n_str(TOO_BIG)<<endl;
           exit(EXIT_FAILURE); // Return a non-zero value for error
	    }
        if (n < CACHE_SIZE && cache[n]!=0) {
            // If length is already cached, add it and break
            c += cache[n]; // add on for the current 'n'
            break;  // Done with n 
        }
        if (n&1) {
            // Check for potential overflow before 3*n + 1 (Why I use 128_t
            // A full implementation requires careful overflow handling or arbitrary precision integers
            n = (n<<1)-(n>>1);  // (3*n+1)/2
			c++;
        } else 
			n >>=1;
		c++;  
    }
    if (original_n < CACHE_SIZE) cache[original_n] = c;
    return c;
}

// Function executed by each thread
void find_max_collatz(coll_t start_n, coll_t end_n, coll_t& max_n_local, int& max_steps_local) {
	
    max_steps_local = 0;
    max_n_local = 0;
    log("Started Thread for "+n_str(start_n)+" to "+n_str(end_n)); 
    for (coll_t i = start_n+(1-start_n%2); i <= end_n; i+=2) { // cheating  Not checking evens. 
        int current_steps = collatz_steps(i);
        if (current_steps >= max_steps_local) {
            max_steps_local = current_steps;
			//log("New max:"+n_str(max_steps_local)+" num:"+n_str((uint64_t)i));
            max_n_local = i;
        }
    }
	log("Done. "+n_str(start_n)+" to "+n_str(end_n)+" was "+
	   n_str(max_n_local)+" steps:"+n_str(max_steps_local)); 
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
    coll_t  upper_limit = ((argc > 1) ? strtoll(argv[1],&endp,10) : 1000000); // max n defaults to 1,000,000
	
    int num_threads = thread::hardware_concurrency()+4; // Use all available hardware threads
	cout <<"Starting Collatz calculations from 1 up to "<<n_str(upper_limit)<<" with "<<num_threads<< " threads."<<endl;
	cout<<"Max for n to reach is "<<n_str(TOO_BIG)<<endl; 

    vector<thread> threads;
    vector<coll_t> local_max_ns(num_threads); // one bucket per thread 
    vector<int> local_max_steps(num_threads);
    coll_t chunk_size = upper_limit / num_threads;
    auto start_time = chrono::steady_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        coll_t start_n = i * chunk_size + 1;
        coll_t end_n = (i == num_threads - 1) ? upper_limit : (i + 1) * chunk_size;
        threads.emplace_back(find_max_collatz, start_n, end_n, ref(local_max_ns[i]), ref(local_max_steps[i]));
    }

    for (auto& t : threads) t.join(); // Wait for all threads to complete

    coll_t max_n = 0;
    int max_steps = 0;

    for (int i = 0; i < num_threads; ++i) {
        if (local_max_steps[i] > max_steps) {
            max_steps = local_max_steps[i];
            max_n = local_max_ns[i];
        }
    }
	

	cout<<"\nAll Done. Longest chain below "<<n_str(upper_limit)<<" was "<<n_str(max_n)<<" steps:"<<max_steps<<endl;
	cout << "Duration : "<< howLong(chrono::steady_clock::now()-start_time) << endl;
    return 0;
}
