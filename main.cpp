#include<bits/stdc++.h>
#include"data_func.h"
#include"pf_conversion.h"

using namespace std;

vector <vector <string> > gen_func(string s)  //main function.This function is recursively called for nested query processing.
{
	map <string,int > map1;
	
		if(s.substr(0,3)=="pro") //the query is a project query in the form pro(r3)(r4)
		{
			stack<char> st; //technique to extract table name and attribute names from the query syntax
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty() && i<r1.length())	
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2); //extracted table name or nested query
			string rn=r1.substr(i);
			string r4=rn.substr(1,rn.length()-2); //extracted attributes to be projected
			vector <vector <string> > table;
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table=gen_func(r3); //recursively calling the main general function to process the nested query
			}
			else
			{
				//cout<<r3<<endl;
				//cout<<r4<<endl;
				table=read_file(r3); //loading the table from harddisk into a vector of vector of strings
			}
			vector <int> project_attr; //this will contain the index of those attributes which are to be projected
			vector<string > attribute_tokens=tokenizer(r4);
			for(int i=0;i<attribute_tokens.size();i++)
			{
				int flag = 0;
				for(int j=0;j<table[0].size();j++)
				{

					if( attribute_tokens[i]==table[0][j])
					{
						project_attr.push_back(j);
						flag++;
					}
				}
				if(flag==0)
				{
					cout<<"Invalid Query"<<endl;
					exit(-1);
				}
			}
			return project(table,project_attr); 
			

		}

		else if(s.substr(0,3)=="ren") //the query is a rename query in the form ren(r3)(r4)
		{
			stack<char> st;	//technique to extract table name and attribute names from the query syntax
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty() && i<r1.length())
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2); //extracted table name or nested query
			string rn=r1.substr(i);
			string r4=rn.substr(1,rn.length()-2); //extracted renamed attributes
			vector <vector <string> > table;
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table=gen_func(r3); //recursively calling the main general function to process the nested query
			}
			else
			{
				table=read_file(r3); //loading the table from harddisk into a vector of vector of strings
			}
			vector <int> project_attr;
			vector<string > attribute_tokens=tokenizer(r4);
			if(attribute_tokens.size()!=table[0].size())
			{
				cout<<"incorrect query syntax for rename"<<endl;
				exit(-1);
			}
			for(int i=0;i<attribute_tokens.size();i++)
				table[0][i]=attribute_tokens[i];
			return table;
		}

		
		else if(s.substr(0,3)=="sel") //the query is a select query in the form sel(r3)(r4)
		{
			vector <vector <string> > answer;
			stack<char> st;		//technique to extract table name and conditions from the query syntax
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty()  && i<r1.length())
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2); //extracted table name or nested query
			string rn=r1.substr(i);
			string r4=rn.substr(1,rn.length()-2); //extracted conditions for the select query
			vector <vector <string> > table1;

			
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table1=gen_func(r3); //recursively calling the main general function to process the nested query
			}
			else
			{
				table1=read_file(r3); //loading the table from harddisk into a vector of vector of strings
			}
			if(r4.length()==0)
				return table1;
			answer.push_back(table1[0]);
			map <string,int > map0;
			for(int i=0;i<table1[0].size();i++)
				map0[table1[0][i]]=i;


			map<char,string> map1;
       set<char> check1; //a set which contains the allowed operations on select
       check1.insert('>');
       check1.insert('<');
       check1.insert('(');
       	check1.insert(')');
       	check1.insert('=');
       	check1.insert('!');
       	check1.insert('&');
       	check1.insert('|');
       				//r4 is of the form age > 22 & branch = "cse"
     string new_r4; // this is of the form A>B & C=D .From r4,a character is assigned to each element to make the postfix evaluation easier. 
     string string_current;
     char c='A';
     for(int i=0;i<r4.size();i++){
     	if(check1.find(r4[i])==check1.end()){
     		string_current+=r4[i]; 
     	}
     	else if(string_current.size()>0){
     		map1[c]=string_current;
     		new_r4+=c;
     		c++;
     		new_r4+=r4[i];
     		string_current.clear();
     	}
     	else{
     		new_r4+=r4[i];
     	}

     }
     if(string_current.size()>0){
     	map1[c]=string_current;
     	new_r4+=c;
     	c++;
     }
     char flag1=c;
     
     int t = new_r4.size();
     char temp[t+1];
     for(i=0;i<t;i++) {
     	temp[i] = new_r4[i];
     } 
     temp[i] = '\0';

   infixToPostfix(temp); //temp is a postfix expression of the string new_r4

   string new_q;
   for(int i=0;temp[i]!='\0';i++)
   		new_q+=temp[i];
   
   for(int d=1;d<table1.size();d++)
   {
   	int k=1;
   	stack<char> evaluator;
   	evaluator.push(new_q[0]);
   	

   while(not(evaluator.empty()) and k<new_q.size()) //evaluating the postfix expression 
   { 
   	char ch = new_q[k++];
   	if(check1.count(ch)==0) 
   		evaluator.push(ch);
   	else 
   	{
   		if(ch == '!')
   		 {
   			char chr1 = evaluator.top();
   			evaluator.pop();
			bool value1 = !(chr1 - 48);
   			evaluator.push('0'+value1);
   		}
   		else 
   		{
   			char g1=evaluator.top();
   			evaluator.pop();
   			char g2= evaluator.top();
   			evaluator.pop();
   			
   			string attr1;
   			attr1+=g2;
   			string value1;
   			value1+=g1;

   			if(map1.find(g2) != map1.end()) 
   			{
   				attr1=map1[g2];
   			}
   			if(map1.find(g1) != map1.end()) 
   			{
   				value1=map1[g1];
   			}
   			if(map0.find(attr1)!=map0.end())
   			{
   				attr1=table1[d][map0[attr1]];
   			}
			if(map0.find(value1)!=map0.end())
   			{
   				value1=table1[d][map0[value1]];
   			}
   			if(attr1[0]=='"') //removing "" from attr1[0]. for example, "abhiram" becomes abhiram
   				attr1=attr1.substr(1,attr1.size()-2);
   			
   			if(value1[0]=='"') 		//removing "" from attr1[0]. for example, "abhiram" becomes abhiram
   				value1=value1.substr(1,value1.size()-2);
   			
   			if(ch == '<') 
   			{
   				int a2=atoi(attr1.c_str());
   				int a1=atoi(value1.c_str());
   				if(a1==0)
   				{
   					cout<<"wrong comparison.Int cannot be compared with strings"<<endl;
   					exit(-1);
   				}
   				bool f=a2<a1;
   				evaluator.push('0'+f);
   			}
   			else if(ch == '>') 
   			{
   				int a1= atoi(value1.c_str());
   				int a2= atoi(attr1.c_str());
   				if(a1==0)
   				{
   					cout<<"wrong comparison.Int cannot be compared with strings"<<endl;
   					exit(-1);
   				}
   				bool f=a2>a1;
   				evaluator.push('0'+f);
   			}

   			else if(ch == '=')
   			{
   				if(value1 == attr1)
   					evaluator.push('1');
   				else 
   					evaluator.push('0');
   			}
   			else if(ch == '|') 
   				evaluator.push('0' + ((value1[0]-48) | (attr1[0]-48)));
   			
   			else if(ch == '&') 
   				evaluator.push('0' + ((value1[0]-48) & (attr1[0]-48)));
   		
   		}
   	}
   }
     if(evaluator.top() == '1')  //if the condition is satisfied,ie,it returns a bool value '1',then add the row to the answer matrix.
     { 
     	answer.push_back(table1[d]);
     }
     
     for(char i=flag1;i<c;i++)
     	map1.erase(i);
     
     c=flag1;
 }
 return answer;

 
}

		else if(s.substr(0,3)=="uni")  //the query is a union query in the form uni(r3)(r4)
		{
			stack<char> st;		//technique to extract table1 name and table2 name from the query syntax
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty() && i<r1.length())
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2); //extracted table name or nested query
			string rn=r1.substr(i); 
			string r4=rn.substr(1,rn.length()-2); //extracted table name or nested query
			vector <vector <string> > table1;
			vector <vector <string> > table2;
			
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table1=gen_func(r3);   //recursively calling the main general function to process the nested query
			}
			else
			{
				table1=read_file(r3);   //loading the table from harddisk into a vector of vector of strings
			}
			if(r4.substr(0,3)=="pro" || r4.substr(0,3)=="sel" || r4.substr(0,3)=="ren" || r4.substr(0,3)=="uni" || r4.substr(0,3)=="dif" || r4.substr(0,3)=="cro")
			{
				table2=gen_func(r4);    //recursively calling the main general function to process the nested query
			}
			else
			{
				table2=read_file(r4);   //loading the table from harddisk into a vector of vector of strings
			}
			
			return union_ra(table1,table2);  //returns a vector of vector of strings after taking union of table1 and table2
		}

		else if(s.substr(0,3)=="cro")   //the query is a cartesian product query in the form cro(r3)(r4)
		{
			stack<char> st;
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty() && i<r1.length())
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2); 	//extracted table name or nested query
			string rn=r1.substr(i);
			string r4=rn.substr(1,rn.length()-2); 	//extracted table name or nested query
			vector <vector <string> > table1;
			vector <vector <string> > table2;
			
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table1=gen_func(r3);	//recursively calling the main general function to process the nested query
			}
			else
			{
				table1=read_file(r3);	//loading the table from harddisk into a vector of vector of strings
			}
			if(r4.substr(0,3)=="pro" || r4.substr(0,3)=="sel" || r4.substr(0,3)=="ren" || r4.substr(0,3)=="uni" || r4.substr(0,3)=="dif" || r4.substr(0,3)=="cro")
			{
				table2=gen_func(r4);	//recursively calling the main general function to process the nested query
			}
			else
			{
				table2=read_file(r4);	//loading the table from harddisk into a vector of vector of strings
			}
			for(int i=0;i<table1[0].size();i++)
			{
				for(int j=0;j<table2[0].size();j++)
				{
					if(table1[0][i]==table2[0][j])
					{
						table2[0][j]=r4+"."+table2[0][j];
					}
				}
			}
			return product(table1,table2);		//returns a vector of vector of strings after taking cross product of table1 and table2

		}

		else if(s.substr(0,3)=="dif")	//the query is a set difference query in the form dif(r3)(r4)
		{
			stack<char> st;
			string r1=s.substr(3);
			if (r1[0]!='(')
			{
				cout<<"invalid query"<<endl;
				exit(-1);
			}
			st.push(r1[0]);
			string r2; 
			int i=1;
			while(!st.empty() && i<r1.length())
			{
				if(r1[i]=='(')
					st.push('(');
						else if(r1[i]==')')
							st.pop();
						i++;			
					}
					if(i==r1.length())
					{
						cout<<"error in query"<<endl;
						exit(-1);
					}
			string r3=r1.substr(1,i-2);	//extracted table name or nested query
			string rn=r1.substr(i);
			string r4=rn.substr(1,rn.length()-2); 	//extracted table name or nested query
			vector <vector <string> > table1;
			vector <vector <string> > table2;
			
			if(r3.substr(0,3)=="pro" || r3.substr(0,3)=="sel" || r3.substr(0,3)=="ren" || r3.substr(0,3)=="uni" || r3.substr(0,3)=="dif" || r3.substr(0,3)=="cro")
			{
				table1=gen_func(r3);	//recursively calling the main general function to process the nested query
			}
			else
			{
				table1=read_file(r3);	//loading the table from harddisk into a vector of vector of strings
			}
			if(r4.substr(0,3)=="pro" || r4.substr(0,3)=="sel" || r4.substr(0,3)=="ren" || r4.substr(0,3)=="uni" || r4.substr(0,3)=="dif" || r4.substr(0,3)=="cro")
			{
				table2=gen_func(r4);	//recursively calling the main general function to process the nested query
			}
			else
			{
				table2=read_file(r4);	//loading the table from harddisk into a vector of vector of strings
			}
			return difference_ra(table1,table2);	//returns a vector of vector of strings after taking set difference of table1 and table2
		}

		
		
	}

	int main()
	{
		
		string s;
		
		while(true)
		{
			cout<<"\n \n \n Please maintain the following syntax"<<endl;
			cout<<"1) pro(table name)(comma separated attributes) \n"<<endl;
	//cout<<rename newattribute_1 newattribute_2 newattribute_3.......newattribute_n \n"<<endl;
cout<<"2)sel(table)(condition) "<<endl;
cout<<"\n3)uni(table1)(table2) "<<endl;
cout<<"\n4)dif(table1)(table2) "<<endl;
cout<<"\n5)cro(table1)(table2) "<<endl;
cout<<"\n6)ren(table)(comma separated attribute names) "<<endl;
cout<< "\n7) type exit to exit the prompt\n\n"<<endl;

cout<<"please enter your query in appropriate syntax mentioned above.For nested queries,refer to the syntax in the readme file"<<endl;

getline(cin,s);
s=space_remover(s);

if(s=="exit")
{
	cout<<"bye"<<endl;
	break;
}
else if(!(s.substr(0,4)=="pro(" || s.substr(0,4)=="sel(" || s.substr(0,4)=="ren(" || s.substr(0,4)=="uni(" || s.substr(0,4)=="dif(" || s.substr(0,4)=="cro("))
{
	cout<<"error in query"<<endl;
	exit(-1);
}


else
{
	vector <vector <string> > answer=gen_func(s);

	if(answer.size()>1)
	{
		for(int i=0;i<answer.size();i++)
		{

			for(int j=0;j<answer[0].size();j++)
			{
				cout<<setw(15)<<left<<answer[i][j];
			}
			cout<<endl;
		}
	}
}
}

return 0;
}
