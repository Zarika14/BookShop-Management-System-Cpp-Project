# BookShop Management System
Create a MySQL Database

DataBase Name : Management

And Tables : 

Books

    int id			// Primary Key
	string name
	string auth
	int price
	int qty

suppliers

	int id				//Primary Key
	string name
	string phn
	string addr_line1
	string addr_line2
	string addr_city
	string addr_state

purchases

	int ord_id			//Primary Key
	int book_id		    //FK ref (books)
	int sup_id			//FK ref (suppliers)
	int qty
	date dt_ordered
	int eta
	char received		// Check(T or C or F) def (F)
	

employees

	int id				//Primary Key
	string name
	string addr_line1
	string addr_line2
	string addr_city
	string addr_state
	long int phn
	date date_of_joining
	long int salary
	

members

    int id				//Primary Key
	string name
	string addr_line1
	string addr_line2
	string addr_city
	string addr_state
	string phn
	date beg_date
	date end_date
	

sales

	int invoice_id		//Primary key
    	int member_id		//FK ref member(id)
	int book_id		//FK ref books(id)
	int qty
	int amount
	date date_s
