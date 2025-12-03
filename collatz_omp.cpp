#include <iostream>
#include <vector>
#include <thread>
#include <locale>
#include <cstdlib>   // Required for strtoll
#include <cerrno>    // Required for errno
#include <climits>   // Required for LLONG_MIN, LLONG_MAX
#include <algorithm> // For max
// g++ -O4 -o testo collatz_omp.cpp  
// ./testo 10000000
// Starting Collatz calculations from 1 up to 10,000,000 with 8 threads.
// Longest chain found for number 8,400,511 with length 685

using namespace std;
// Function to calculate Collatz steps for a given number
long long collatz_steps(long long x)
 {
  long long c=0;
 
  while (x!=1)
   {
    if (x&1) x=(x << 1) - (x >> 1), c+=2;
    while (!(x&1)) x>>=1, c++;
   }
  return c;
 }

// Function executed by each thread
void find_max_collatz(long long start_n, long long end_n, long long& max_n_local, long long& max_steps_local) {
    max_steps_local = 0;
    max_n_local = 0;

    for (long long i = start_n; i <= end_n; ++i) {
        long long current_steps = collatz_steps(i);
        if (current_steps > max_steps_local) {
            max_steps_local = current_steps;
            max_n_local = i;
        }
    }
}

int main(int argc, char** argv) { 
    char* endp;
    if (argc < 2) 
	   cout << "You can also use a number as an argument. Defaulting to 1,000,000"<<endl; 
    long long  upper_limit = ((argc > 1) ? strtoll(argv[1],&endp,10) : 1000000); // max n defaults to 1,000,000
    int num_threads = thread::hardware_concurrency(); // Use available hardware threads
	cout.imbue(locale("")); // add seperators to numbers 
    cout <<"Starting Collatz calculations from 1 up to "<<upper_limit<<" with "<<num_threads<< " threads."<<endl;
    vector<thread> threads;
    vector<long long> local_max_ns(num_threads); // one bucket per thread 
    vector<long long> local_max_steps(num_threads);
    long long chunk_size = upper_limit / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        long long start_n = i * chunk_size + 1;
        long long end_n = (i == num_threads - 1) ? upper_limit : (i + 1) * chunk_size;
        threads.emplace_back(find_max_collatz, start_n, end_n, ref(local_max_ns[i]), ref(local_max_steps[i]));
    }

    for (auto& t : threads) t.join(); // Wait for all threads to complete

    long long max_n = 0;
    long long max_steps = 0;

    for (int i = 0; i < num_threads; ++i) {
        if (local_max_steps[i] > max_steps) {
            max_steps = local_max_steps[i];
            max_n = local_max_ns[i];
        }
    }
    cout<<"Longest chain found for number "<<max_n<<" with length "<<max_steps<<endl;
    
    return 0;
}