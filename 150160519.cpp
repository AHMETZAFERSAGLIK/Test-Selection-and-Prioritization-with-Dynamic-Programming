#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

void tokenizer(string s, string delim, vector<string> &tokens){
  size_t pos = 0;
  string token;

  while ( (pos = s.find(delim)) != string::npos ) {
      token = s.substr(0, pos);
      tokens.push_back(token);
      s.erase(0, pos + delim.length());
  }
  tokens.push_back(s);
}

void print_matrix(int **matrix, int row, int col){
  for(int i = 0; i < row; i++){
    for(int j = 0; j < col; j++){
      cout << matrix[i][j] << " "; 
    }
    cout << endl;
  }
}

int knap_mem(int capacity, vector<int> &times, vector<int> &bugs, int i, int **mem){
    if (i < 0) return 0; 
    if (mem[i][capacity] != -1) return mem[i][capacity]; 
    if (times[i] > capacity){
      mem[i][capacity] = knap_mem(capacity, times, bugs, i - 1, mem); 
      return mem[i][capacity]; 
    } 
    else{ 
      mem[i][capacity] = max(
        bugs[i] + knap_mem(capacity - times[i], times, bugs, i - 1, mem), 
        knap_mem(capacity, times, bugs, i - 1, mem)
      );
      return mem[i][capacity]; 
    } 
} 

int **mem;

int knapSack(int capacity, vector<int> &times, vector<int> &bugs, int n){ 
    mem = new int*[n];
    for (int i = 0; i < n; i++) mem[i] = new int[capacity + 1]; 

    for (int i = 0; i < n; i++) 
      for (int j = 0; j < capacity + 1; j++) 
        mem[i][j] = -1;
    return knap_mem(capacity, times, bugs, n-1, mem); 
} 
  
vector<int> get_items(int capacity, vector<int> &times, vector<int> &bugs, int n, int result){ 
    int i, w;
    vector<int> items;

    w = capacity; 
    for (i = n; i > 0 && result > 0; i--) { 
        if (result == mem[i - 1][w]) continue;         
        else {
            items.push_back(i);
            result = result - bugs[i - 1]; 
            w = w - times[i - 1]; 
        } 
    }

    return items;
}

int ** get_profile_coverages(vector<int **> &profile_vector, vector<int> &selected_cases, vector<int> &num_row_vector, vector<int> &num_col_vector){

  int **totals = new int*[selected_cases.size()];
  for(int i = 0; i < selected_cases.size(); i++){
    totals[i] = new int[num_row_vector[selected_cases[i]]];
    for(int j = 0; j < num_row_vector[selected_cases[i]]; j++){
      totals[i][j] = 0;
      for(int k = 0; k < num_col_vector[selected_cases[i]]; k++){
        if(profile_vector[selected_cases[i]][j][k])
          totals[i][j] += 1;
      }
    }
  }
  
  return totals;

}

// implemented with dp
int edit_dist(int *prof1, int *prof2, int n){
  
    int table[2][n+1]; 
    for(int i = 0; i < 2; i++){
      for(int j = 0; j < n+1; j++){
        table[i][j] = 0;
      }
    }

    for(int i = 1; i <= n; i++){ 
      for(int j = 0; j <= n; j++){ 
        if( !j ) table[i%2][j] = i; 
        else if( prof1[j-1] == prof2[i-1] ){ 
          table[i%2][j] = table[(i-1)%2][j-1]; 
        }
        else{ 
          table[i%2][j] = 1 + min(table[(i-1)%2][j], 
                                  min(
                                      table[i%2][j-1], 
                                      table[(i-1)%2][j-1]
                                     )
                                  ); 
        }
      } 
    } 

  return table[n%2][n]; 
}

int arg_max(int *arr, int size){
  int idx = -1;
  int max_val = -1;
  for(int i = 0; i < size; i++){
    if( arr[i] > max_val ){
      idx = i;
      max_val = arr[i];
    }
  }
  return idx;
}

int main(int argc, char **argv){

  string file_name = string(argv[1]);

  /* Below is a reading session, since the input
     is unorthadox it is a bit messy */
  ifstream f(file_name);

  int max_run_time;

  string line;
  for(int i = 0; i < 4; i++) f >> line;
  f >> max_run_time; // Read max run time
  getline(f, line);
  getline(f, line); // pass empty line
  getline(f, line); // pass headers line

  vector<int> bug_vector;
  vector<int> time_vector;
  vector<int **> profile_vector;
  vector<string> ID_vector;
  vector<int> num_row_vector;
  vector<int> num_col_vector;

  while( getline(f, line) ){

    vector<string> tokens;
    tokenizer(line, " ", tokens);

    string ID = tokens[0];
    int bugs = stoi(tokens[1]);
    int time = stoi(tokens[2]);
    vector<string> profiles = vector<string>(tokens.begin() + 3, tokens.end());

    int **profiles_matrix;
    int num_row;
    int num_col;
    profiles_matrix = new int*[ profiles.size() ];
    num_row = profiles.size();
    tokens.clear();
    for(int i = 0; i < profiles.size(); i++){
      tokenizer(profiles[i], "-", tokens);
      profiles_matrix[i] = new int[tokens.size()];
      num_col = tokens.size();
      for(int j = 0; j < tokens.size(); j++){
        profiles_matrix[i][j] = stoi(tokens[j]);
      }
      tokens.clear();
    }

    /* In the end, all the necessary values are stores in
       the relevant variables */
    bug_vector.push_back(bugs);
    time_vector.push_back(time);
    ID_vector.push_back(ID);
    profile_vector.push_back(profiles_matrix);
    num_row_vector.push_back(num_row);
    num_col_vector.push_back(num_col);

  }

  f.close();
  /* End of reading session */

  clock_t begin, end;

  begin = clock();

  /* Run knapsack which is implemented with memoization and DP */
  int result = knapSack(max_run_time, time_vector, bug_vector, ID_vector.size());

  /* Extract the items from the precalculated table */
  vector<int> items = get_items(max_run_time, time_vector, bug_vector, ID_vector.size(), result);

  end = clock();

  /* Free allocated memory for memoization table */
  for(int i = 0; i < ID_vector.size(); i++) delete[] mem[i];
  delete[] mem;
  /******************************************/

  /* Print selected cases */
  cout << "Selected cases are: ";
  for(int i = items.size() - 1; i >= 0; i--){
    cout << ID_vector[items[i]] << " ";
  } cout << "with a total of " << result << " bugs. Elapsed time: " << (double(end - begin) / CLOCKS_PER_SEC) << endl;

  int **coverages = get_profile_coverages(profile_vector, items, num_row_vector, num_col_vector); // covarages

  cout << "Order of profiles are:" << endl;

  for(int i = items.size() - 1; i >= 0 ; i--){
    begin = clock();
    int item = items[i];
    int most_cov = arg_max(coverages[i], num_row_vector[item]);
    int **profs = profile_vector[item];
    
    vector<int> order;
    int visited[num_row_vector[item]];
    for(int j = 0; j < num_row_vector[item]; j++) visited[j] = 0;
    visited[most_cov] = 1;

    order.push_back(most_cov);

    for(int j = 1; j < num_row_vector[item]; j++){
      int max_dist = -1;
      int idx = -1;
      for(int k = 0; k < num_row_vector[item]; k++){
        if( visited[k] ) continue;
        int dist = edit_dist(profs[order.back()], profs[k], num_col_vector[item]);
        if(dist > max_dist){
          max_dist = dist;
          idx = k;
        }
      }
      visited[idx] = 1;
      order.push_back(idx);
    }

    end = clock();

    cout << "For " << ID_vector[item] << ":";

    for(int j = 0; j < order.size(); j++){
      cout << " " << order[j] + 1;
      if(j != order.size() - 1) cout << ",";
    }
    cout << ". Elapsed time: " << (double(end - begin) / CLOCKS_PER_SEC) << endl;

  }

  /* Free the allocated memory */
  for(int i = 0; i < profile_vector.size(); i++){
    for(int j = 0; j < num_row_vector[i]; j++){
        delete[] profile_vector[i][j];
    }
    delete[] profile_vector[i];
  }

  for(int i = 0; i < items.size(); i++){
    delete[] coverages[i];
  }
  delete[] coverages;
  /******************************************/

  return 0;

}