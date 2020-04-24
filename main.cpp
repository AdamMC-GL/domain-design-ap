#include <stdio.h>
#include <iostream> 
#include <string>
#include <ctime>
#include <vector>
using namespace std;

struct Use_room{
	int room_nr;
	int open_seats;
	int military_time;
	int day;
	int month;
};

struct Items{
	string item_name;
	int stock;
	float price;
};

class Employees{
public:
	string name_employee;
	string function_employee;
	bool is_working;
	float salary_hour;
	int worked_hours;
	
	int start_shift;
	int end_shift;
	
	float get_salary(){
		return (salary_hour*worked_hours);
	}
	
	void set_working(bool arg){
		time_t t = time(0);   // get time now, https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
		tm* now = localtime(&t);
		
		is_working = arg;
		if(is_working){
			end_shift = 0;
			start_shift = now->tm_hour*60+now->tm_min;
		}
		
		if(!is_working && start_shift > 0){
			end_shift = now->tm_hour*60+now->tm_min;
			start_shift = 0;
			worked_hours += (end_shift-start_shift)/60;
		}
	}
	
	void set_function(string arg){
		function_employee = arg;
	}
};


class SnackShop{
public:
	Employees employee;
	float revenue;
	vector<Items> products;
	
	void set_employee(Employees arg){
		employee = arg;
		employee.function_employee = "Kassa shop";
		employee.set_working(true);
	}
	
	void add_products(Items arg){
		products.push_back(arg);
	}
	
	void remove_products(Items arg){
		for(unsigned int i = 0; i < products.size(); i++){
			if(products[i].item_name == arg.item_name){
				products.erase(products.begin() + i);
				break;
			}
		}
	}
	
	string toString(){
		string str = "Current employee: " + employee.name_employee + ".\nStock: ";
		for(unsigned int j = 0; j < products.size(); j++){
			str += products[j].item_name + ": " + to_string(products[j].stock) + "\n";
		}
		str += "Total earnings: " + to_string(revenue);
		return str;
	}
};


class Movies{
public:
	string movie_name;
	Use_room room;
	float ticket_price;
	int sales;
	
	void set_ticket_price(float price){
		ticket_price = price;
	}
	
	void set_room(Use_room arg){
		room = arg;
	}
	
	void sale(){
		sales++;
	}
	string toString(){
		string str = "Name: " + movie_name + ", in room: " + to_string(room.room_nr) + " at " + to_string(room.military_time) + ", " + to_string(room.day) + "/" + to_string(room.month);
		str += ". Sales: " + to_string(sales) + to_string(ticket_price) + "\n";
		return str;

	}
};


class Customers{
public:
	string name_customer;
	vector<Movies> bought_tickets;
	float balance;
	
	string buy_ticket(Movies & ticket){
		if(ticket.room.open_seats > 0 && balance > ticket.ticket_price){
			bought_tickets.push_back(ticket);
			ticket.sale();
			ticket.room.open_seats--;
			balance -= ticket.ticket_price;
			
			return "Purchase succesfull for: " + ticket.movie_name + ", Room number: " + to_string(ticket.room.room_nr) + ", at " + to_string(ticket.room.military_time) + ", " + to_string(ticket.room.day) + "/" + to_string(ticket.room.month)+"\n";;
		}
		return "Purchase failed.";
	}
	
	string return_ticket(Movies & ticket){
		time_t t = time(0);   // get time now, https://www.tutorialspoint.com/cplusplus/cpp_date_time.htm
		tm* now = localtime(&t);
		if(ticket.room.month < (now->tm_mon + 1) && ticket.room.day < (now->tm_mday)){ // the right of an && statement is only ever checked if the left = true
			for(unsigned int i = 0; i < bought_tickets.size(); i++){
				if(bought_tickets[i].movie_name == ticket.movie_name && bought_tickets[i].room.military_time == ticket.room.military_time && bought_tickets[i].room.day == ticket.room.day && bought_tickets[i].room.month == ticket.room.month && bought_tickets[i].room.room_nr == ticket.room.room_nr){
					
					balance += ticket.ticket_price;
					ticket.room.open_seats++;
					ticket.sales--;
					bought_tickets.erase(bought_tickets.begin() + i);
					return "Refund succesfull.";
				}
			}
		}
		return "Refund denied.";
	}
	
	string buy_from_shop(SnackShop shop, Items product, int quantity){
		if(product.stock*quantity > 0 && balance > product.price*quantity){
			balance -= product.price*quantity;
			product.stock -= quantity;
			shop.revenue += product.price*quantity;
			return "Purchase succesfull";
		}
		return "Purchase failed";
	}
	
	string toString(){
		string str = name_customer + " has a balance of €" + to_string(balance).substr(0,5) + " and owns the following tickets: \n";
		for(unsigned int j = 0; j < bought_tickets.size(); j++){
			str += bought_tickets[j].movie_name + ", at " + to_string(bought_tickets[j].room.military_time) + ", " + to_string(bought_tickets[j].room.day) + "/" + to_string(bought_tickets[j].room.month)+"\n";
		}
		return str;
	}
};


class Theather{
public:
	string name_theather;
	SnackShop shop;
	vector<Movies> movies;
	vector<Employees> employees;
	vector<int> rooms_numbers;
	
	float net_earnings;
	
	string add_movie(Movies arg){
		for(unsigned int k = 0; k < movies.size(); k++){
			if(movies[k].room.military_time == arg.room.military_time && movies[k].room.day == arg.room.day && movies[k].room.month == arg.room.month && movies[k].room.room_nr == arg.room.room_nr){
				return "Add movie failed, there already is a movie playing then.";
			}
		}
		movies.push_back(arg);
		return "Movie added";
	}
	
	string remove_movie(Movies arg){
		for(unsigned int j = 0; j < movies.size(); j++){
			if(movies[j].movie_name == arg.movie_name && movies[j].room.military_time == arg.room.military_time && movies[j].room.day == arg.room.day && movies[j].room.month == arg.room.month && movies[j].room.room_nr == arg.room.room_nr){
				movies.erase(movies.begin() + j);
				return "Removal succesfull.";
			}
		}
		return "Removal failed.";
	}
	
	void hire_employee(Employees arg){
		employees.push_back(arg);
	}
	
	void fire_employee(Employees arg){
		for(unsigned int i = 0; i < employees.size(); i++){
			if(employees[i].name_employee == arg.name_employee){
				employees[i].is_working = false;
				employees.erase(employees.begin() + i);
				break;
			}
		}
	}
	
	void set_earnings(){
		for(unsigned int j = 0; j < movies.size(); j++){
			net_earnings += movies[j].sales * movies[j].ticket_price;
		}
		
		for(unsigned int k = 0; k < employees.size(); k++){
			net_earnings -= employees[k].get_salary();
		}
		net_earnings += shop.revenue;
	}
	
	float get_earnings(){
		set_earnings();
		return net_earnings;
	}
	
	string toString(){
		string str = name_theather + " has the following movies: \n";
		for(unsigned int i = 0; i < movies.size(); i++){
			str += movies[i].movie_name+", in room: " + to_string(movies[i].room.room_nr) + " at " + to_string(movies[i].room.military_time) + ", " + to_string(movies[i].room.day) + "/" + to_string(movies[i].room.month) + "\n";
		}
		str += "\nHas the following employees: \n";
		for(unsigned int j = 0; j < employees.size(); j++){
			str += employees[j].name_employee+", function: " + employees[j].function_employee + "\n";
		}
		string temp = to_string(get_earnings());
		for(unsigned int k = 0; k < temp.size(); k++){
			if(temp[k] == '.'){
				str += "Earnings: €" + temp.substr(0,k+3) + "\n";
				return str;
			}
		}
		str += "Earnings: " + to_string(get_earnings()).substr(0,7);
		return str;
	}
	
	string get_unused_rooms(){
		string str = "Unused rooms: ";
		bool unused;
		vector<int> Temp = rooms_numbers;
		for(unsigned int l = 0; l < Temp.size(); l++){
			unused = true;
			for(unsigned int m = 0; m < movies.size(); m++){
				if(Temp[l] == movies[m].room.room_nr){
					unused = false;
				}
			}
			if(unused){
				str += to_string(Temp[l]) + ", ";
			}
		}
		return str;
	}
};




int main()
{	
	Employees e1 = {"Hans Scholl", "Kassa", false, 5.5, 7, 0, 0};
	Employees e2 = {"Sophie Scholl", "Ticket verkoop", false, 6.4, 0, 0, 0};
	Employees e3 = {"Kurt Huber", "Schoonmaak", false, 9.5, 21, 0, 0};
	
	Items popcorn = {"Popcorn", 15, 10.99};
	Items soda_coke = {"Coca-Cola", 20, 6.99};
	Items soda_sprite = {"Sprite", 30, 5.99};
	Items chips = {"Chips", 11, 12.99};
	
	Movies m1 = {"No war", {1, 29, 1900, 8, 11}, 19.43, 24};
	Movies m2 = {"Tunsie 230", {2, 15, 1300, 5, 11}, 22.01, 21};
	Movies m3 = {"The underground railroad", {3, 30, 1600, 4, 11}, 13.07, 49};
	
	SnackShop shop = {e1, 0, {popcorn, soda_coke, soda_sprite, chips}};
	Theather cinema = {"Cinema White Rose", shop, {m1, m2, m3}, {e1, e2, e3}, {1, 2, 3, 4, 5, 6, 7}};
	
	cout << cinema.toString();
	cout << "\n\n";
	
}
