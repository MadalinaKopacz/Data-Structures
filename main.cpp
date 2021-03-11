#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <queue>
#include <random>

using namespace std;
using namespace std::chrono;

ifstream fin("file.in");

///Function for checking the correctness of the sorting algorithm
bool CheckCorrectness(int n, vector <int> v)
{
    for (int i = 0; i < n-1; ++i)
        if (v[i] > v[i+1]) return false;    ///if two elements on consecutive positions are
    return true;                           ///ordered in descending order, the algorithm did not order correctly

}

///Optimized version of Bubble Sort
///After each iteration, if there is no swapping taking place then, there is no need for performing further loops.
vector<int> BubbleSort(int n, vector<int>  v)
{
   bool swapped;
   for (int i = 0; i < n-1; ++i)
   {
     swapped = false;
     for (int j = 0; j < n-i-1; ++j)
     {
        if (v[j] > v[j+1])
        {
           swap(v[j], v[j+1]);
           swapped = true;
        }
     }

     /// If no two elements were swapped by inner loop, then break.
     if (swapped == false)
        break;
   }

   return v;
}


///Counting Sort
vector<int> CountingSort(int n, vector<int>  v, int Max)
{
   int f[Max+1] = {0};
   for(int i = 0; i < n; ++i) f[v[i]]++;  ///finding the frequency of each value in the vector

   vector<int> aux;
   for(int i = 0; i <= Max; ++i)
        while(f[i]--) aux.push_back(i);

   return aux;
}


///Radix Sort - base 16
void RadixSort_16(int n, vector <int> & v)
{
    queue<int> q[16];
    int k;
    for(int i = 0; i <= 24; i += 4)    ///2^4=16 => 4 bits in base 2 form 1 digit in base 16
    {
        for(int j=0; j < n; ++j)
            q[(v[j]>>i)&15].push(v[j]);   /// (v[j]>>i)&15 <=> we eliminate the last i = 4*k (k = 0,1,2,3,4,5,6) bits from the given number and
                                          ///with 15 = (1111)2 we find the 4 bits that form the digit in base 16 for the specific bucket
        ///putting the elements from the bucket queue back in v
        k = 0;
        for(int j = 0; j <= 15 && k < n; ++j)
            while(!q[j].empty())
            {
                v[k++] = q[j].front();
                q[j].pop();
            }
    }
}

///Radix Sort - base 256
void RadixSort_256(int n, vector <int> & v)
{
    queue<int> q[256];
    int k;
    for(int i = 0; i <= 24; i += 8)    ///2^8 = 256  => 8 bits in base 2 form 1 digit in base 256
    {
        for(int j=0; j < n; ++j)
            q[(v[j]>>i)&255].push(v[j]); /// (v[j]>>i)&255 <=> we eliminate the last i = 8*k (k = 0,1,2,3) bits from the given number and
                                         ///with 255 = (11111111)2 we find the 8 bits that form the digit in base 256 for the specific bucket
        ///putting the elements from the bucket queue back in v
        k = 0;
        for(int j = 0; j <= 255 && k < n; ++j)
            while(!q[j].empty())
            {
                v[k++] = q[j].front();
                q[j].pop();
            }
    }
}


///Merge Sort
void Merge(vector<int> &v, int l, int m, int r)
{
    int i = l, j = m+1;
    vector<int> aux;
    while(i <= m && j <= r)
        if(v[i] < v[j]) aux.push_back(v[i++]);  ///merge the two halves into temp vector
        else aux.push_back(v[j++]);
    while(i <= m) aux.push_back(v[i++]);  ///copy the remaining elements if there are any in the left
    while(j <= r) aux.push_back(v[j++]);  ///copy the remaining elements if there are any in the right
    for(i = l; i <= r; ++i) v[i] = aux[i-l];
}

void MergeSort(vector<int> &v, int l, int r)
{
    if(r > l)
    {
        int m = l + (r - l)/2; ///same as (l + r) / 2, but avoids overflow
        ///Sort first and second halves
        MergeSort(v, l, m);
        MergeSort(v, m+1, r);
        ///Merging
        Merge(v, l, m, r);
    }
}


///Merges two subvectors of v:
///first one is starting with l position and ending with m position
///second one is starting with m2 = m + 1 position and endinf with r position
void InPlaceMerge(vector<int> & v, int l, int m, int r)
{
    int m2 = m + 1;
    if (v[m] <= v[m2]) return; ///direct merge is already sorted
    while (l <= m && m2 <= r)
    {
        if (v[l] <= v[m2]) l++; ///if the element on the 1st position is in the right place
        else
        {
            int val = v[m2], i = m2;
            while (i != l) v[i] = v[i-1], i--;  ///shifting all the ellements between v[l] and v[m2], right by v[l]
            v[l] = val;
            l++, m++, m2++;
        }
    }

}

void InPlaceMergeSort(vector<int> & v, int l, int r)
{
    if (l < r)
    {
        int m = l + ( r - l)/2; ///same as (l + r) / 2, but avoids overflow
        ///Sort first and second halves
        InPlaceMergeSort(v, l, m);
        InPlaceMergeSort(v, m+1, r);
        ///Merging
        InPlaceMerge(v, l, m, r);
    }
}

///Quick Sort
void QuickSort(vector<int> &v, int l, int r)
{
    int i = l, j = r;
    int x = v[rand()% (r-l+1) + l];  ///choosing randomly the pivot
    while(i < j)
    {
        ///searching for two elements that are not in the correct order regarding to the pivot
        while(v[i] < x) i++;
        while(v[j] > x) j--;
        if(i <= j) swap(v[i++], v[j--]);
    }
    ///Divide
    if(j > l) QuickSort(v, l, j);
    if(i < r) QuickSort(v, i, r);
}

int main()
{
    vector<int> v;
    int TestsNumber;
    fin>>TestsNumber;
    for(int test = 1; test <= TestsNumber; ++test)
    {
        v.clear();
        int n, Max;
        fin >> n >> Max;
        for( int i = 0; i < n; ++i) v.push_back(rand() % Max + 1);
        vector<int> aux;

        cout << "TEST " << test << ": n = " << n << " & Max = " << Max << "\n";

        ///Testing Bubble Sort
        if(n > 10000) cout << "Bubble Sort - FAILED (n is too large)\n";
        else
            {
                auto Start = high_resolution_clock::now();
                aux = BubbleSort(n, v);
                auto Stop = high_resolution_clock::now();
                if(CheckCorrectness(n, aux))
                {
                    auto Duration = duration_cast<milliseconds>(Stop-Start);
                    cout << "Bubble Sort - SUCCEEDED - " << Duration.count() << " ms\n";

                }
                else cout << "Bubble Sort - FAILED (sorted incorrectly)\n";
            }

        ///Testing Counting Sort
             if(n >= 100000000) cout << "Counting Sort - FAILED (n is too large)\n";
             else if(Max >= 1000000) cout << "Counting Sort - FAILED (Max is too large)\n";
                   else
                 {auto Start = high_resolution_clock::now();
                 aux = CountingSort(n, v, Max);
                 auto Stop = high_resolution_clock::now();
                 if(CheckCorrectness(n, aux))
                 {
                     auto Duration = duration_cast<milliseconds>(Stop-Start);
                     cout << "Counting Sort - SUCCEEDED - " << Duration.count() << " ms\n";
                 }
                  else cout << "Counting Sort - FAILED (sorted incorrectly)\n";

                 }

        ///Testing Radix Sort
        if(n >= 100000000) cout << "Radix Sort - FAILED (n is too large)\n";
        else
        {
            /// Testing Radix Sort - base 16
            aux = v;
            auto Start = high_resolution_clock::now();
            RadixSort_16(n, aux);
            auto Stop = high_resolution_clock::now();
            if(CheckCorrectness(n, aux))
            {
                auto Duration = duration_cast<milliseconds>(Stop-Start);
                cout << "Radix Sort in base 16 - SUCCEEDED - " << Duration.count() << " ms\n";
            }
            else cout << "Radix Sort in base 16 - FAILED (sorted incorrectly)\n";

            ///Testing Radix Sort - base 256
            aux = v;
            Start = high_resolution_clock::now();
            RadixSort_256(n, aux);
            Stop = high_resolution_clock::now();
            if(CheckCorrectness(n, aux))
            {
                auto Duration = duration_cast<milliseconds>(Stop-Start);
                cout << "Radix Sort in base 256 - SUCCEEDED - " << Duration.count() << " ms\n";
            }
            else cout << "Radix Sort in base 256 - FAILED (sorted incorrectly)\n";
        }


        ///Testing Merge Sort
        if(n >= 100000000) cout << "Merge Sort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            auto Start = high_resolution_clock::now();
            MergeSort(aux, 0, n-1);
            auto Stop = high_resolution_clock::now();
            if(CheckCorrectness(n, aux))
            {
                auto Duration = duration_cast<milliseconds>(Stop-Start);
                cout << "Merge Sort - SUCCEEDED - " << Duration.count() << " ms\n";
            }
            else cout << "Merge Sort - FAILED (sorted incorrectly)\n";
        }

        ///Testing In-Place Merge Sort
        if(n >= 1000000) cout << "In-Place Merge Sort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            auto Start = high_resolution_clock::now();
            InPlaceMergeSort(aux, 0, n-1);
            auto Stop = high_resolution_clock::now();
            if(CheckCorrectness(n, aux))
            {
                auto Duration = duration_cast<milliseconds>(Stop-Start);
                cout << "In-Place Merge Sort - SUCCEEDED - " << Duration.count() << " ms\n";
            }
            else cout << "In-Place Merge Sort - FAILED (sorted incorrectly)\n";
        }

       ///Testing Quick Sort
        if(n > 100000000) cout << "Quick Sort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            auto Start = high_resolution_clock::now();
            QuickSort(aux, 0, n-1);
            auto Stop = high_resolution_clock::now();
            if(CheckCorrectness(n, aux))
            {
                auto Duration = duration_cast<milliseconds>(Stop-Start);
                cout << "Quick Sort - SUCCEEDED - " << Duration.count() << " ms\n";
            }
            else cout << "Quick Sort - FAILED (sorted incorrectly)\n";
        }

        ///Testing Native (STL) Sort
        if(n > 100000000) cout << "Native (STL) Sort - FAILED (n is too large)\n";
        else
        {
            aux = v;
            auto Start = high_resolution_clock::now();
            sort(aux.begin(),aux.end());
            auto Stop = high_resolution_clock::now();
            auto Duration = duration_cast<milliseconds>(Stop-Start);
            cout << "Native (STL) Sort - SUCCEEDED - " << Duration.count() << " ms\n";

        }
        
        cout << "______________________________________________\n\n";
    }
}

