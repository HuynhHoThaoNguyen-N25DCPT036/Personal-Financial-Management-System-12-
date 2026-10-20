#include <iostream>
#include <fstream>
#include <string>
 
using namespace std;
struct Transaction{
	int id;
	string type;
	double amount;
	string category;
};
class FinanceStorage{
public:
	void saveFile(Transaction transactions[], int n){
    	ofstream file("finance.csv");
        file<<"ID,Type,Amount,Category"<<endl;
    	for (int i = 0; i < n; i++){
            file<<transactions[i].id<<",";
            file<<transactions[i].type<<",";
            file<<transactions[i].amount<<",";
            file<<transactions[i].category<<endl;
    	};
        file.close();
     	cout << "Da luu du lieu vao file finance.csv!" << endl;
	};
	void loadFile(){
    	ifstream file("finance.csv");
    	string line;
        getline(file, line);
        cout<<"\nDu lieu trong file:"<< endl;
    	while (getline(file, line)) {
 cout << line << endl;
    };
  file.close();
	};
};
 int main(){
	Transaction transactions[3];
    transactions[0].id = 1;
        	transactions[0].type = "Income";
        	transactions[0].amount = 5000000;
        	transactions[0].category = "Salary";
 
        	transactions[1].id = 2;
        	transactions[1].type = "Expense";
        	transactions[1].amount = 50000;
        	transactions[1].category = "Food";
        	
        	transactions[2].id = 3;
        	transactions[2].type = "Expense";
        	transactions[2].amount = 100000;
        	transactions[2].category = "Transport";
 
    FinanceStorage storage;
    storage.saveFile(transactions, 3);
    storage.loadFile();
	return 0;
};
