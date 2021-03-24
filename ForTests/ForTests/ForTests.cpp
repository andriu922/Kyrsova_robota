// ForTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <vector>
#include <locale>
#include "windows.h"

using namespace std;

vector<string> Split(string strToSplit, char delimeter)
{
	stringstream ss(strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}
//Storage for medical items
class medical_storage {
public:
	medical_storage(){}
	medical_storage(int id, string name,string category ,double price,int total_amounght,int popularity) {
		this->id = id;
		this->name = name;
		this->category = category;
		this->price = price;
		this->total_amounght = total_amounght;
		this->popularity = popularity;
	}
	int id;
	string name;
	string category;
	double price;
	int total_amounght;//кількість
	int popularity;
};
class storage_manager {
public:
	vector<medical_storage*> get_storage() {
		vector<medical_storage*> storage;
		string line;
		ifstream stor_file("storage.txt");
		while (getline(stor_file, line)) {
			vector<string> parts = Split(line, '|');
			medical_storage* u = new medical_storage;
			u->id = stoi(parts[0]);
			u->name = parts[1];
			u->category = parts[2];
			u->price = stod(parts[3]);
u->total_amounght = stoi(parts[4]);
u->popularity = find_popularity(stoi(parts[0]));
storage.push_back(u);
		}
		stor_file.close();
		//write_file(storage);
		return storage;
	}
	void print_storage() {
		auto list = get_storage();
		for (size_t i = 0; i < list.size(); i++) {
			print_item(list[i]);
		}
	}

private:
	int find_popularity(int id) {
		int n = 0;
		string cust;
		ifstream customer_file("orders.txt");
		if (customer_file.is_open()) {
			while (getline(customer_file, cust)) {
				vector<string> f = Split(cust, '|');
				if (stoi(f[2]) == id) {
					n++;
				}

			}
		}
		else {
			cout << "cannot open" << endl;
		}
		customer_file.close();
		return n;
	}
	void write_file(vector<medical_storage*> storage) {
		ofstream stor_file("storage.txt");
		for (size_t i = 0; i < storage.size(); i++) {
			stor_file << storage[i]->id << "|" << storage[i]->name << "|" << storage[i]->category << "|" << storage[i]->price << "|" << storage[i]->total_amounght << "|" << storage[i]->popularity << endl;
		}

	}
	//допрацювати вивід ітемів у складі
	void print_item(medical_storage* storage) {
		cout << storage->id << "." << storage->name << ":" << storage->category << "|" << storage->price << "|" << storage->total_amounght << "|" << storage->popularity << endl;
	}
};
//
//oder, order info, order manage
class order {
public:
	order() {}
	order(int id, string name, int item_id) {
		this->id = id;
		this->name = name;
		this->item_id - item_id;
	}
	int id;
	string name;
	int item_id;
};
class order_info {
public:
	order_info() {}
	order_info(int id, string name, string order_name, string order_status, double order_price) {
		this->id = id;
		this->name = name;
		this->order_name = order_name;
		this->order_status = order_status;
		this->order_price = order_price;
	}
	int id;
	string name;
	string order_name;
	string order_status;
	double order_price;

};
class order_manage :public storage_manager {
public:
	//storage_manager& _storage_manager;
	//order_manage(storage_manager& SManager) : _storage_manager(SManager) {};
	//Агрегація
	//

	//aviable order statuses
	string status1 = "waiting for delivery to storage";
	string status2 = "waiting for customer";
	//
	vector<order_info*> get_info() {
		vector<order_info*> info_stor;
		string line;
		ifstream orders_file("orders.txt");
		while (getline(orders_file, line)) {
			vector<string> parts = Split(line, '|');
			order_info* n = new order_info;
			n->id = stoi(parts[0]);
			n->name = parts[1];
			n->order_name = ger_order_name(stoi(parts[2]));
			n->order_status = get_order_status(stoi(parts[2]));
			n->order_price = get_order_price(stoi(parts[2]));
			info_stor.push_back(n);
		}
		orders_file.close();
		return info_stor;
	}
	void print_order_info(){
		auto order_information = get_info();
		for (size_t i = 0; i < order_information.size(); i++) {
			print_item_OI(order_information[i]);
		}
	}
	void search_order_by_order_status(string text) {
		auto v_search = get_info();
		for (size_t i = 0; i < v_search.size();i++) {
			if (v_search[i]->order_status == text) {
				print_item_OI(v_search[i]);
			}
		}
	}
	void search_order_by_category_and_order_status(string status, string category) {
		string line;
		vector<string> total;
		auto v_storage = get_storage();
		auto v_orders = get_info();
		ifstream orders_file("orders.txt");
		while (getline(orders_file,line)) {
			vector<string> parts = Split(line, '|');//parts[2]
			if ((v_storage.at(stoi(parts[2])-1)->category == category) && (v_orders.at(stoi(parts[0])-1)->order_status == status)) {
				print_item_OI(v_orders[stoi(parts[0])-1]);
			}
			
		}
	}
private:
	void print_item_OI(order_info* info_stor) {
		cout << info_stor->id << "." << endl;
		cout << "Order :" << info_stor->order_name << endl;
		cout << "Order status :" << info_stor->order_status << endl;
		cout << "Order price :" << info_stor->order_price << endl;
		cout << "------------------" << endl;
	}
	string ger_order_name(int id) {
		auto v_storage = get_storage();
		for (size_t i = 0; i < v_storage.size();i++) {
			if (v_storage[i]->id==id) {
				return v_storage[i]->name;
				break;
			}
		}
	}
	string get_order_status(int id) {
		auto v_storage = get_storage();
		for (size_t i = 0; i < v_storage.size(); i++) {
			if (v_storage[i]->id==id) {
				if (v_storage[i]->total_amounght>0) {
					return status2;
					break;
				}
				else if (v_storage[i]->total_amounght == 0) {
					return status1;
					break;
				}
			}
		}
	}
	double get_order_price(int id) {
		auto v_storage = get_storage();
		for (size_t i = 0; i < v_storage.size();i++) {
			if (v_storage[i]->id == id) {
				return v_storage[i]->price;
				break;
			}
		}
	}
};
//
/*
void storage_search_menu(storage_manager SManager&) {

}*/

void orders_search_menu(storage_manager& SManager, order_manage& OManager) {
	int a, a2, max=2; 
	//aviable order statuses
	vector<string> status{ "waiting for delivery to storage", "waiting for customer" };
	//
	while (true) {
		cout << "Search menu options" << endl;
		cout << "1. Print orders that are waiting for delivery" << endl;
		cout << "2. Print orders that are ready and waiting for the customer" << endl;
		cout << "3. Exit." << endl;

		cout << "Input command index" << endl;
		cin >> a;
		if (a>max) {
			break;
		}
		cout << "------------";
		/*switch (a) {
		case 1:OManager.search_order(status[0]); break;
		case 2:OManager.search_order(status[1]); break;
		}*/
		cout << "1. Show all" << endl;
		cout << "2. Search by categoryes" << endl;
		cout << "Input command index" << endl;
		cin >> a2;
		if ((a2 == 1)&&(a == 1)) {
			OManager.search_order_by_order_status(status[0]);
		}else if ((a2 == 1) && (a == 2)) {
			OManager.search_order_by_order_status(status[1]);
		}
		else if (a2 == 2) {
			auto v_storage = SManager.get_storage();
			auto v_orders = OManager.get_info();
			string text;
			int index;
			cout << "Aviable categories :" << endl;
			for (size_t i = 0; i < v_storage.size();i++) {
				cout << i+1 << ". " << v_storage[i]->category << "\t";
			}
			cout << "Choose category by index : " << endl;
			cin >> index;
			if (a == 1) {
				OManager.search_order_by_category_and_order_status(status[0], v_storage[index - 1]->category);
			}
			else if (a == 2) {
				OManager.search_order_by_category_and_order_status(status[1], v_storage[index - 1]->category);
			}
		}


	}
}
int main() {
	storage_manager SManager;
	order_manage OManager;
	OManager.get_info();
	int n;
	while (true) {
		cout << "Main menu" << endl;
		cout << "1. Print list of medicaments " << endl;
		cout << "2. Storage search menu" << endl;
		cout << "3. Print orders" << endl;
		cout << "4. Orders search menu" << endl;
		
		cout << "Input comand number" << endl;
		cin >> n;
		switch (n) {
		case 1:SManager.print_storage(); break;
		case 2:OManager.print_order_info(); break;

		case 4:orders_search_menu(SManager, OManager); break;
		}
	}
	
}
	




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
