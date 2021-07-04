#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include <math.h>
using namespace std;

typedef struct {
	string c;
	int number;
}state;//ex. s0的話  c=s;number=0;


typedef struct {
	string e_input;
	state current;
	state next;
	string e_output;
}element;//一開始讀入的input

typedef struct {
	state s_state;
	vector<string> s_input;
	vector<string> s_output;
	vector<state> s_next;
}sort_state;//為了合併(merge)，把同個currnet state的狀態整理過後比較，一樣的可以合併

vector<element> v_element;
vector<sort_state>v_sort;
state reset_state;
int terms;//#term
int s_terms;//#state


bool sort_in(vector<sort_state> v, state s) {
	bool B = false;
	for (int i = 0; i < v.size(); i++) {
		if (s.c == v[i].s_state.c && s.number == v[i].s_state.number)B = true;
	}
	return B;
}

int find_state(vector<element> v, state s, int start) {
	for (int i = start; i < v.size(); i++) {
		if (v[i].current.c == s.c && v[i].current.number == s.number)return i;
	}
	return -1;
}

vector<sort_state> sort(vector<element> v) {
	vector<sort_state> sum;
	sort_state temp;
	for (int i = 0; i < v.size(); i++) {
		if (i == 0) {
			temp.s_state.c = v[0].current.c;
			temp.s_state.number = v[0].current.number;
			sum.push_back(temp);
		}
		else {
			if (!sort_in(sum,v[i].current)) {
				temp.s_state.c = v[i].current.c;
				temp.s_state.number = v[i].current.number;
				sum.push_back(temp);
			}
		}
	}//先找出不同的state

	for (int i = 0; i < sum.size(); i++) {
		int pos = 0;
		while (find_state(v, sum[i].s_state, pos) != -1) {
			pos = find_state(v, sum[i].s_state, pos);
			sum[i].s_input.push_back(v[pos].e_input);
			sum[i].s_output.push_back(v[pos].e_output);
			sum[i].s_next.push_back(v[pos].next);
			pos++;
		}
	}//一樣的current state放一起
	
	
	return sum;
}



//bool in(vector<string> c, string input) {
//	bool B = false;
//	for (int i = 0; i < c.size(); i++) {
//		if (input == c[i])B = true;
//	}
//	return B;
//}

bool can_merge(vector<string> input1, vector<string> input2) {
	for (int i = 0; i < input1.size(); i++) {
		for (int j = 0; j < input2.size(); j++) {
			if (input1[i].find('-', 0) != -1 || input2[j].find('-', 0) != -1) {
				bool b = true;
				for (int k = 0; k < input1[i].size(); k++) {
					if (input1[i][k] == '-' || input2[j][k] == '-')continue;
					else if (input1[i][k] == input2[j][k])b = false;
				}
				return b;
			}
			else if (input1[i] == input2[j])return false;
		}
	}
	return true;
}//input都不一樣可以合併

void merge() {
	sort_state temp;
	for (int i = 0; i < v_sort.size(); i++) {
		temp = v_sort[i];
		if (v_sort[i].s_input.size() == pow(2, v_sort[i].s_input[0].size()))continue;
		for (int j = v_sort.size() - 1; j > i; j--) {
			if (v_sort[i].s_input.size() == pow(2, v_sort[i].s_input[0].size()))continue;
			if (v_sort[j].s_input.size() == pow(2, v_sort[j].s_input[0].size()))continue;
			else if (can_merge(temp.s_input, v_sort[j].s_input)) {
				for (int k = 0; k < v_sort[j].s_input.size(); k++) {
					v_sort[i].s_input.push_back(v_sort[j].s_input[k]);
					v_sort[i].s_next.push_back(v_sort[j].s_next[k]);
					v_sort[i].s_output.push_back(v_sort[j].s_output[k]);
				}
				vector<sort_state>mid;
				for (int k = 0; k < v_sort.size(); k++) {
					if (k != j)mid.push_back(v_sort[k]);
				}//類似pop[j]
				v_sort = mid;
				//break;

			}
		}
	}
}

void reduce() {
	for (int i = 0; i < v_sort.size(); i++) {
		if (v_sort[i].s_input.size() != pow(2, v_sort[i].s_input[0].size())) continue;
		for (int j = v_sort.size() - 1; j > i; j--) {
			if (v_sort[j].s_input.size() != pow(2, v_sort[j].s_input[0].size()))continue;
			else if (!can_merge(v_sort[i].s_input, v_sort[j].s_input) && !can_merge(v_sort[i].s_output, v_sort[j].s_output)) {//都一樣可以消去其中

				vector<sort_state>mid;
				for (int k = 0; k < v_sort.size(); k++) {
					if (k != j)mid.push_back(v_sort[k]);
				}
				v_sort = mid;
				//break;



			}
		}
	}
}




vector<string> dont_care(string s) {
	vector<string> v;

	int pos = 0, counter = 0;
	while (s.find('-', pos) != -1) {
		pos = s.find('-', pos);
		pos++;
		counter++;
	}//算有幾個dont care  
	int num = pow(2, counter);
	
	v.push_back(s);
	for (int i = 0; i < num; i++) {
		pos = v[i].find('-', 0);
		if (pos != -1) {
			s = v[i];
			s[pos] = '0';
			v.push_back(s);
			s[pos] = '1';
			v.push_back(s);
			pos++;
		}
		else if (pos == -1)break;
	}

	vector<string> temp;
	for (int i = 0; i < v.size(); i++) {
		if (v[i].find('-', 0) == -1)temp.push_back(v[i]);
	}

	return temp;

	

}

void min() {
	vector<sort_state> v;
	for (int i = 0; i < v_sort.size(); i++) {
		sort_state temp;
		temp.s_state = v_sort[i].s_state;
		vector<bool>b(v_sort[i].s_output.size());
		for (int j = 0; j < v_sort[i].s_output.size(); j++) {
			b[j] = true;
		}
		for (int j = 0; j < v_sort[i].s_output.size(); j++) {
			if (b[j] == false)continue;
			temp.s_output.push_back(v_sort[i].s_output[j]);
			temp.s_next.push_back(v_sort[i].s_next[j]);

			string s = v_sort[i].s_input[j];
			for (int k = j + 1; k < v_sort[i].s_output.size(); k++) {
				if (v_sort[i].s_output[j] == v_sort[i].s_output[k] && v_sort[i].s_next[j].c == v_sort[i].s_next[k].c && v_sort[i].s_next[j].number == v_sort[i].s_next[k].number) {
					
					for (int m = 0; m < v_sort[i].s_input[j].size(); m++) {
						if (v_sort[i].s_input[j][m] != v_sort[i].s_input[k][m]) {
							s[m] = '-';
						}
					}
					b[k] = false;
				}
			}
			temp.s_input.push_back(s);
		}
		v.push_back(temp);
	}
	v_sort = v;
	v.clear();
}

int main() {
	int i, j;
	string input;
	ifstream fin;
	fin.open("t00.kiss");
	getline(fin, input);
	while (input[0] == '#')getline(fin, input);
	for (i = 0; i < 5; i++) {
		if (input[1] == 's') {
			stringstream ss;
			ss.str("");
			ss.clear();
			ss << input.substr(3);
			ss >> s_terms;
		}
		else if (input[1] == 'p') {
			stringstream ss;
			ss.str("");
			ss.clear();
			ss << input.substr(3);
			ss >> terms;
		}
		else if (input[1] == 'r') {
			input = input.substr(3);
			for (j = 0; j < input.size(); j++) {
				if (input[j] >= '0' && input[j] <= '9') break;
			}
			reset_state.c = input.substr(0, j);
			stringstream ss;
			ss.str("");
			ss.clear();
			ss << input.substr(j);
			ss >> reset_state.number;
		}
		getline(fin, input);
	}//處理前面五個規定

	do {
		element mid;
		string temp;
		stringstream ss;
		ss.str("");
		ss.clear();
		ss << input;
		int counter = 0;
		while (getline(ss, temp, ' ')) {
			if (counter == 0) {
				mid.e_input = temp;
			}//input
			else if (counter == 1) {
				for (j = 0; j < temp.size(); j++) {
					if (temp[j] >= '0' && temp[j] <= '9') break;
				}
				mid.current.c = temp.substr(0, j);
				stringstream ss;
				ss.str("");
				ss.clear();
				ss << temp.substr(j);
				ss >> mid.current.number;
			}//current
			else if (counter == 2) {
				for (j = 0; j < temp.size(); j++) {
					if (temp[j] >= '0' && temp[j] <= '9') break;
				}
				mid.next.c = temp.substr(0, j);
				stringstream ss;
				ss.str("");
				ss.clear();
				ss << temp.substr(j);
				ss >> mid.next.number;
			}//next
			else if (counter == 3) {
				mid.e_output = temp;
			}//output
			counter++;
		}
		v_element.push_back(mid);
	} while (getline(fin, input)&&(input!=".e"));

	

	
	vector<element> e_temp;
	for (i = 0; i < v_element.size(); i++) {
		int pos = 0;
		if (v_element[i].e_input.find('-', 0) == -1) {
			e_temp.push_back(v_element[i]);
			continue;
		}
		else {
			element temp;
			temp.current = v_element[i].current;
			temp.e_output = v_element[i].e_output;
			temp.next = v_element[i].next;
			vector<string> s_input_v;
			s_input_v = dont_care(v_element[i].e_input);
			for (j = 0; j < s_input_v.size(); j++) {
				temp.e_input = s_input_v[j];
				e_temp.push_back(temp);
			}
			
		}
		
	}//把dont care展開
	v_element = e_temp;
	e_temp.clear();
	for (i = 0; i < v_element.size(); i++) {
		element temp;
		
		for (j = i + 1; j < v_element.size(); j++)
		{
			if (v_element[i].current.c > v_element[j].current.c && v_element[i].current.number > v_element[j].current.number)
			{
				//變數交換
				temp = v_element[i];
				v_element[i] = v_element[j];
				v_element[j] = temp;
			}
		}
		

	}

	//for (i = 0; i < v_element.size(); i++) {
	//	cout << v_element[i].e_input << " " << v_element[i].current.c << v_element[i].current.number << " " << v_element[i].next.c << v_element[i].next.number << " " << v_element[i].e_output << endl;
	//}
	//cout <<endl<< "///////////////////////////////////////////////" << endl<<endl;
	v_sort = sort(v_element);
	merge();
	reduce();
	min();//有dont care的要用回去

	cout << ".i " << v_sort[0].s_input[0].size() << endl;
	cout << ".o " << v_sort[0].s_output[0].size() << endl;
	cout << ".s " << v_sort.size() << endl;
	int sum = 0;
	for ( i = 0; i < v_sort.size(); i++) {
		sum += v_sort[i].s_input.size();
	}
	cout << ".p " << sum << endl;
	cout << ".r " << reset_state.c << reset_state.number << endl;
	for ( i = 0; i < v_sort.size(); i++) {
		for (j = 0; j < v_sort[i].s_input.size(); j++) {
			cout << v_sort[i].s_input[j] << " " << v_sort[i].s_state.c << v_sort[i].s_state.number << " " << v_sort[i].s_next[j].c << v_sort[i].s_next[j].number << " " << v_sort[i].s_output[j] << endl;
		}
	}
	cout << ".e" << endl;

	v_element.clear();
	v_sort.clear();
	return 0;
}
