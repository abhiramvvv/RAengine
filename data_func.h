#include<bits/stdc++.h>
using namespace std;

vector<string > tokenizer(string& s) // tokenize the given string int a vector of strings according to ',' positions.Similar to what  
{									//split function does in python
	vector<string > result;
	string token;
	stringstream ss(s);
	while( getline(ss,token, ',') )
	{
		result.push_back(token);	
	}
	return result;
}

vector <vector <string> > read_file(string s) //reads the table with name 's' from the hard disk into a vector of vector of strings and returns it
{
	string path=s+".csv";
	//cout<<path<<endl;
	ifstream file;
	file.open(path.c_str());
	if(!file)
	{
		cerr<<"Unable to open file"<<endl;
	}
	string def1;
	getline(file,def1);
	vector<string > result = tokenizer(def1);
	
	int column = result.size();
	
	vector <vector <string> > answer;
	answer.push_back(result);

	//reading the rows
	while(file.good())
	{
		getline(file,def1);
		if(file.eof())
		{
			//return;
			break;
		}
			
		vector<string> tuple = tokenizer(def1);
		if(tuple.size()!=column)
		{
			cout<<"Error in tuple"<<endl;
			
		}
		else
		{
			answer.push_back(tuple);
		}
		
	}
	
	return answer;
}


string space_remover(string& str) //removes all the spaces from the query string and returns the new string
{
	vector <string> tokens;
	string token;
	stringstream ss(str);
	while( getline(ss,token, ' ') )
	{
		tokens.push_back(token);	
	}
	string s1;
	for(int i=0;i<tokens.size();i++)
	{
		s1+=tokens[i];
	}
	return s1;
}


//returns the table matrix containing columns corresponding to the project attribute vector
vector <vector <string> > project(vector <vector <string> > &table,vector<int> &project_attr)
{
	cout<<endl;
	// cout<<"in project"<<endl;
	set< vector <string> > project_result;
	
	cout<<endl;
	vector <string> temp;
	for(int j=0;j<project_attr.size();j++)
		{
			temp.push_back(table[0][project_attr[j]]);
		}
	for(int i=1;i<table.size();i++)
	{
		vector <string> tuple;
		for(int j=0;j<project_attr.size();j++)
		{
			tuple.push_back(table[i][project_attr[j]]);
		}
	
		project_result.insert(tuple);

	}
	vector <vector <string> > answer;
	answer.push_back(temp);
	set< vector <string> >::iterator i;
	for(i=project_result.begin();i!=project_result.end();++i)
	{
		vector < string > tuple = *i;
		answer.push_back(tuple);
	}
	return answer;
}

//returns a vector of vector of strings after taking union of table1 and table2
vector <vector <string> > union_ra(vector <vector <string> > &table1,vector <vector <string> > &table2) 
{																										
	set< vector <string> > union_result;
	if(table1[0].size()!=table2[0].size())
	{
		cout<<"union not possible"<<endl;
		exit(-1);
	}
	
	for(int i=0;i<table1[0].size();i++)
	{
		if(table1[0][i]!=table2[0][i])
		{
			cout<<"union not possible"<<endl;
			exit(-1);						
		}
	}
		

		for(int i=1;i<table1.size();i++)
		{
			union_result.insert(table1[i]);
		}
		for(int i=1;i<table2.size();i++)
		{
			union_result.insert(table2[i]);
		}
		vector <string> temp;
		for(int j=0;j<table1[0].size();j++)
		{
			temp.push_back(table1[0][j]);
		}
			
			vector <vector <string> > answer;
			answer.push_back(temp);
			set< vector <string> >::iterator i;
			for(i=union_result.begin();i!=union_result.end();++i)
			{
				vector < string > tuple = *i;
				answer.push_back(tuple);
			}
			return answer;
	
}


//returns a vector of vector of strings after taking cross product of table1 and table2
vector <vector <string> > product(vector <vector <string> > &table1,vector <vector <string> > &table2) 
{																										
	
	vector <vector <string> > answer;
	answer.push_back(table1[0]);
	answer[answer.size()-1].insert(answer[answer.size()-1].end(),table2[0].begin(),table2[0].end()); //to append the attribute column
	for(int i=1;i<table1.size();i++)
	{
		
		for(int j=1;j<table2.size();j++)
		{
			answer.push_back(table1[i]);
			answer[answer.size()-1].insert(answer[answer.size()-1].end(),table2[j].begin(),table2[j].end());
		}
	}

	return answer;
}



//returns a vector of vector of strings after taking set difference of table1 and table2
vector < vector <string> > difference_ra(vector <vector <string> > &table1,vector <vector <string> > &table2) //returns a vector of vector of strings after taking set
{																											//difference of table1 and table2
	vector < vector <string> > difference_result;
	if(table1[0].size()!=table2[0].size())
	{
		cout<<"set difference  not possible"<<endl;
		exit(-1);
	}

	for(int i=0;i<table1[0].size();i++)
	{
		if(table1[0][i]!=table2[0][i])
		{
			cout<<"set difference not possible"<<endl;
			exit(-1);
		}
	}

	difference_result.push_back(table1[0]);
	for(int i=1;i<table1.size();i++)
	{
		for(int j=1;j<table2.size();j++)
		{
			if(table1[i]==table2[j])
			{
				difference_result.push_back(table1[i]);
				break;
			}
		}
	}
	return difference_result;
}