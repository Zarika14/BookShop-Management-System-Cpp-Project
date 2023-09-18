// +----------------------------------+
// |          HEADER FILES            |
// +----------------------------------+

#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <String>
#include <conio.h>
#define HOST "localhost"
#define USER "root"
#define PASS ""
#define DATABASE "Management"
#define PORT 3306
#define PASSWORD 

using namespace std;

// +--------------------------------+
// |		Global Variable     |
// +--------------------------------+

MYSQL * conn;
MYSQL_RES *res_set;
MYSQL_ROW row;
stringstream stmt;
const char * q;
string query;
// my_bool result;

typedef struct
{
	int date;
	int month;
	int year;
}date;


// +----------------------------------+
// |	         Classes Made 		  |
// +----------------------------------+

class books
{
	int id;			// Primary Key
	string name;
	string auth;
	int price;
	int qty;
public:
	void add();
	void update_price();
	void search();
	void display();
};

class suppliers
{
	int id;				//Primary Key
	string name;
	string  phn;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
public:
	void add_sup();
	void remove_supplier();
	void search_id();
};

class purchases
{
	int ord_id;			//Primary Key
	int book_id;		//FK ref (books)
	int sup_id;			//FK ref (suppliers)
	int qty;
	date dt_ordered;
	int eta;
	char received;		// Check(T or C or F) def (F)
	int inv;
public:
	void new_ord();
	void view();
	void mar_cancel();
	void mark_reciv();
};

class employees
{
	int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	string phn;
	date date_of_joining;
	long int salary;
	string mgr_status;	//check(T or F) def f
public:
	void add_emp();
	void search_emp();
	void assign_mgr_stat();
	void display();
	void update_sal();
};

class members
{
    int id;				//Primary Key
	string name;
	string addr_line1;
	string addr_line2;
	string addr_city;
	string addr_state;
	string phn;
	date beg_date;
	date end_date;
	string valid;
public:
	void add_mem();
	void refresh();
	void search_mem();
};

class sales
{
	int invoice_id;		//Primary key
 	int member_id;		//FK ref member(id)
	int book_id;		//FK ref books(id)
	int qty;
	int amount;
	date date_s;
public:
	void add();
	void find_total_sales();
};

// +----------------------------------+
// |		Member Functions	  	  |
// +----------------------------------+

// ------------
// class books
// ------------

void books::add()
{
	cout << "Enter the name of the book : " ;
	cin >> name;
	cout << "Enter the name of the author : ";
	cin >> auth;
	cout << "Enter the Price : ";
	cin >> price;
	cout << "Enter the Qty Recived : ";
	cin >> qty;
	stmt.str("");
	stmt << "Insert into books(name,auth,price,qty) values('" << name << "','" << auth << "'," << price << "," << qty << ");";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if (!(res_set))
        cout << endl << endl << "Book Record Inserted Successfully" << endl << endl << endl;
    else
        cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
}

void books::update_price()
{
    char choice;
    cout << "Enter the id of the book for an update in price: ";
    cin >> id;
    stmt.str("");

    stmt << "SELECT name, price FROM books WHERE id = " << id << ";";
    query = stmt.str();
    q = query.c_str();
    
    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing SELECT query: " << mysql_error(conn) << endl;
        return; // Exit function on error
    }
    
    res_set = mysql_store_result(conn);
    
    if ((row = mysql_fetch_row(res_set)) != NULL)
    {
        cout << "The Name of the book is: " << row[0] << endl;
        cout << "The current price of the book is: " << row[1] << endl;
        cout << "Do you want to update the Price [y/n]: ";
        
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            cout << "Enter the new price: ";
            cin >> price;
            
            stmt.str("");
            stmt << "UPDATE books SET price = " << price << " WHERE id = " << id << ";";
            query = stmt.str();
            q = query.c_str();

            if (mysql_query(conn, q) != 0) {
                cerr << "Error executing UPDATE query: " << mysql_error(conn) << endl;
                return; // Exit function on error
            }

            cout << "Book Price Updated Successfully!" << endl;
        }
        else
        {
            cout << "No changes made!";
        }
    }
    else
    {
        cout << "No Book found!!!";
    }
    
    mysql_free_result(res_set); // Free the result set when done
}

void books::search()
{
	cout << "Enter book id for details : ";
	cin >> id;
    stmt.str("");
    stmt << "Select * from books where id = " << id << ";";
    query = stmt.str();
    q = query.c_str();
    mysql_query(conn,q);
    res_set = mysql_store_result(conn);
    if((row = mysql_fetch_row(res_set)) != NULL)
    {
		cout << "The Details of Book Id " << row[0] << endl;
        cout << "The Name of the book is : " << row[1] << endl ;
		cout << "THE Author of " << row[1] << " is " << row[2] << endl;
        cout << "The Price of the book is : " << row[3] << endl ;
		cout << "The inventory count is " << row[4] << endl;
	}
	else
	{
		cout << "No record Found" << endl;
	}
}



void books::display()
{
	int i = 0;
	query = "Select * from books;";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Name for book " << ++i << " : " << row[1] << endl;
		cout << "Name of Author : " << row[2] << endl;
		cout << "Price : " << row[3] << endl;
		cout << "Quantity : " << row[4] << endl;
		cout << endl << endl << endl << endl ;
	}
}



// ---------------
// class suppliers
// ---------------
bool isValidPhoneNumber(const std::string &phn) {
    // Check if the string is exactly 10 characters long
    if (phn.length() != 10) {
        return false;
    }

    // Check if all characters are digits
    for (char c : phn) {
        if (!isdigit(c)) {
            return false;
        }
    }

    return true;
}
void suppliers::add_sup()
{
	
	cout << "Enter the Supplier Name : ";
	cin >> name;
 //Input the phone number from the user
    std::cout << "Enter a 10-digit phone number: ";
    std::cin >> phn;

    // Validate the phone number
    if (isValidPhoneNumber(phn)) {
        std::cout << "Phone number entered: " << phn << std::endl;
    } else {
        std::cout << "Invalid phone number. Please enter a 10-digit number." << std::endl;
    }
	
	cout<<endl;
	cout << "Enter the address (in 3 lines) : "<<endl;
	cout<<"Enter street name : ";
	cin>>addr_line1;
	
	cout<<endl;
	
	cout<<"Enter landmark : ";
	cin>>addr_line2;
	
	cout<<endl;
	
	cout<<"Enter city name: ";
	cin>>addr_city;
	
	cout<<endl;
	
	
	
	cout << "Enter State : ";
	cin >> addr_state;
	stmt.str("");
	stmt << "Insert into suppliers(name,phn,addr_line1,addr_line2,addr_city,addr_state) values('"  << name << "','" << phn << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "');";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	 
	if (!(res_set))
        cout << endl << endl << "Supplier Record Inserted Successfully" << endl << endl << endl;
    else
        cout << endl << endl << "Entry ERROR !" << endl << "Contact Technical Team " << endl << endl << endl;
}

void suppliers::remove_supplier()
{
	cout << "Enter the supplier id to remove the Supplier : ";
	cin >>  id;
	stmt.str("");
	stmt << "Delete from suppliers where id = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << "Supplier Removed.";
}

void suppliers::search_id()
{
	cout << "Enter the supplier id to find the Supplier details : ";
	cin >>  id;
	stmt.str("");
	stmt << "Select * from suppliers where id = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Details of Supplier Id : " << row[0] << endl;
		cout << "Name : " << row[1] << endl;
		cout << "Phone no. : " << row[2] << endl;
		cout << "Address Line 1 : " << row[3] << endl;
		cout << "Address Line 2 : " << row[4] << endl;
		cout << "City : " << row[5] << endl;
		cout << "State : " << row[6] << endl;
	}
	else
	{
		cout << "No Supplier Found!!";
	}
}



// ------------------
// class Purchases
// ------------------

void purchases::new_ord()
{
    cout << "Enter the book Id : ";
    cin >> book_id;
    cout << "Enter Supplier Id : ";
    cin >> sup_id;
    cout << "Enter the Quantity : ";
    cin >> qty;
    cout << "Estimated expected Delivery (in days) : ";
    cin >> eta;
    
    stmt.str("");
    stmt << "INSERT INTO purchases (book_id, sup_id, qty, dt_ordered, eta) VALUES ("
         << book_id << "," << sup_id << "," << qty << ",CURDATE(),DATE_ADD(CURDATE(), INTERVAL " << eta << " DAY));";
    
    query = stmt.str();
    q = query.c_str();
    
    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing INSERT query: " << mysql_error(conn) << endl;
        return; // Exit function on error
    }
    
    cout << "New order Added!!" << endl;
}


void purchases::mark_reciv()
{
    cout << "Enter the order id for order received: ";
    cin >> ord_id;

    // Construct the SQL query with placeholders
    stmt.str("");
    stmt << "UPDATE purchases SET recieved = 'T' WHERE ord_id = " << ord_id << ";";
    query = stmt.str();
    q = query.c_str();

    // Prepare the SQL statement
    MYSQL_STMT* prepared_stmt = mysql_stmt_init(conn);
    if (!prepared_stmt) {
        cerr << "Error initializing prepared statement: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(prepared_stmt, q, strlen(q)) != 0) {
        cerr << "Error preparing statement: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Bind the parameter for the order ID
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    int orderID = ord_id;
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &orderID;

    if (mysql_stmt_bind_param(prepared_stmt, bind) != 0) {
        cerr << "Error binding parameters: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Execute the SQL statement
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        cerr << "Error executing statement: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Check the number of affected rows
    if (mysql_stmt_affected_rows(prepared_stmt) > 0) {
        cout << "Received Marked successfully" << endl;
    } else {
        cout << "No matching order found for the given order ID." << endl;
    }

    mysql_stmt_close(prepared_stmt);
}

void purchases::mar_cancel()
{
    cout << "Enter the order id for order canceled: ";
    cin >> ord_id;

    // Construct the SQL query with placeholders
    stmt.str("");
    stmt << "UPDATE purchases SET recieved = 'C' WHERE ord_id = " << ord_id << ";";
    query = stmt.str();
    q = query.c_str();

    // Prepare the SQL statement
    MYSQL_STMT* prepared_stmt = mysql_stmt_init(conn);
    if (!prepared_stmt) {
        cerr << "Error initializing prepared statement: " << mysql_error(conn) << endl;
        return;
    }

    if (mysql_stmt_prepare(prepared_stmt, q, strlen(q)) != 0) {
        cerr << "Error preparing statement: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Bind the parameter for the order ID
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    int orderID = ord_id;
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &orderID;

    if (mysql_stmt_bind_param(prepared_stmt, bind) != 0) {
        cerr << "Error binding parameters: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Execute the SQL statement
    if (mysql_stmt_execute(prepared_stmt) != 0) {
        cerr << "Error executing statement: " << mysql_stmt_error(prepared_stmt) << endl;
        mysql_stmt_close(prepared_stmt);
        return;
    }

    // Check the number of affected rows
    if (mysql_stmt_affected_rows(prepared_stmt) > 0) {
        cout << "Canceled Marked successfully" << endl;
    } else {
        cout << "No matching order found for the given order ID." << endl;
    }

    mysql_stmt_close(prepared_stmt);
}


void purchases::view()
{
	int c;
	cout << "Select an Option" << endl;
	cout << "1. View orders not Recieved" << endl;
	cout << "2. View orders Cancelled" << endl;
	cout << "3. View orders Recieved" << endl;
	cout << "Enter Your choice : ";
	cin >> c;
	if (c == 1)
		received = 'F';
	else if (c == 2)
		received = 'C';
	else if (c == 3)
		received = 'T';
	else
		return;
	stmt.str("");
	stmt << "Select * from purchases where recieved = '" << received << "';";
	query  = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if (c == 1)
		cout << endl << "Orders not recieved are" << endl;
	else if (c == 2)
		cout << endl << "Orders Cancelled are" << endl;
	else if (c == 3)
		cout << endl << "Orders recieved are" << endl;
	while((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << endl;
		cout << "Order Id : " << row[0] << endl;
		cout << "Book Id : " << row[1] << endl;
		cout << "Supplier Id :" << row[2] << endl ;
		cout << "Quantity : " << row[3] << endl;
		cout << "Date Ordered : " << row[4] << endl;
		cout << "Estimated Delivery date : " << row[5] << endl;
		cout << endl << endl << endl;
	}
	cout <<endl << endl << endl <<endl << endl;
}


// ----------------
// class employees
// ----------------

void employees::add_emp()
{
	cout << "Enter Your Id for verification : ";
	cin >> id ;
	
//	stmt.str("");
//	stmt << "Select mgr_status from employees where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
//	if ((row = mysql_fetch_row(res_set)) == NULL)
//	{
//		cout << "Employee Not Found!!" << endl << endl << endl;
//		return;
//	}
//	else
//	{
//		mgr_status = (char*) row[0];
//		if (mgr_status == "T")
//		{
//			cout << "You Do Not have Manager Rights!!!" << endl << endl ;
//			return;
//		}
//	}
	
	cout << "Enter The details of new employees!!";
	cout<<endl;
	cout << "Enter The name of the employee : ";
	cin >> name;
	cout << "Enter the Address (in 3 lines) : " << endl;
	cin >> addr_line1;
	cin >> addr_line2;
	cin >> addr_city;
	cout << "Enter State : ";
	cin >> addr_state;
	cout << "Enter phone no.";
	cin >> phn;
	cout << "Enter the salary : ";
	cin >> salary;
	stmt.str("");
	stmt << "Insert into employees (name,addr_line1,addr_line2,addr_city,addr_state,phn,date_of_joining,salary) values ('" << name << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "'," << phn << ",curdate()," << salary << ");";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << endl << endl << "Employee Added Succesfully!" << endl << endl << endl;
}




void employees::search_emp()
{
	cout << "Enter the id for searching an employee : ";
	cin >> id ;
	stmt.str("");
	stmt << "Select * from employees where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Employees Details" << endl ;
		cout << "Name : " << row[1] <<endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Date of Joining : " << row[7] << endl;
		cout << "Salary : " << row[8] << endl << endl << endl;
	}
	else
	{
		cout << "No Employee Found!!" << endl << endl << endl;
	}
}

void employees::display()
{
	int i = 0;
	query = "Select * from employees;";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
    {
       do
       {
    	cout << "Employees Details of Emp no." << ++i << endl ;
        cout << "Name : " << row[1] <<endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Date of Joining" << row[7] << endl;
		cout << "Salary : " << row[8] << endl;
		cout << endl << endl << endl << endl;
       }while((row = mysql_fetch_row(res_set)) != NULL);
    }
	else
	{
		cout << "Employees Not found!" << endl;
	}
}

void employees::update_sal()
{
	cout << "Enter the id to update the salary of an employee : ";
	cin >> id ;
	cout << "Enter the updated salary : ";
	cin >> salary;
	stmt.str("");
	stmt << "Update employees set salary = " << salary << " where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	cout << endl << endl << endl << endl;
	cout << "Salary update Successfully";
	cout << endl << endl << endl << endl;
}

// ---------------------
// class members
// ---------------------

void members::add_mem()
{
    cout << "Enter the name of the member: ";
    cin >> name;
    cout << "Enter phone no.: ";
    cin >> phn;
    cout << "Enter address (in 3 lines):" << endl;
    cin >> addr_line1;
    cin >> addr_line2;
    cin >> addr_city;
    cout << "Enter the name of the state: ";
    cin >> addr_state;

    // Check if the connection is valid
    if (!conn) {
        cerr << "MySQL connection is not valid." << endl;
        return;
    }

    stmt.str("");
    stmt << "INSERT INTO members(name, addr_line1, addr_line2, addr_city, addr_state, phn, beg_date, end_date) VALUES ('" << name << "','" << addr_line1 << "','" << addr_line2 << "','" << addr_city << "','" << addr_state << "'," << phn << ",CURDATE(),DATE_ADD(CURDATE(), INTERVAL 1 YEAR));";
    query = stmt.str();
    q = query.c_str();

    // Execute the INSERT INTO query
    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing INSERT INTO query: " << mysql_error(conn) << endl;
        return;
    }

    // Fetch the member ID
    stmt.str("");
    stmt << "SELECT id FROM members WHERE phn = " << phn << ";";
    query = stmt.str();
    q = query.c_str();

    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing SELECT query: " << mysql_error(conn) << endl;
        return;
    }

    res_set = mysql_store_result(conn);

    if ((row = mysql_fetch_row(res_set)) != NULL) {
        cout << endl << "Member Added successfully" << endl << "Member Id: " << row[0] << endl;
    } else {
        cerr << "Member ID not found." << endl;
    }

    mysql_free_result(res_set);
}


void members::refresh()
{
	query = "Update members set valid = 'invalid' where end_date <= curdate();";
	q = query.c_str();
	mysql_query(conn,q);
}

void members::search_mem()
{
	cout << "Enter member id : ";
	cin >> id;
	stmt.str("");
	stmt << "Select * from members where id  = " << id << ";";
	query = stmt.str();
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if ((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Member Details" << endl ;
		cout << "Name : " << row[1] << endl;
		cout << "Address : " << endl << row[2] << endl << row[3] << endl << row[4] << endl;
		cout << "State : " << row[5] << endl;
		cout << "Contact no. : " << row[6] << endl;
		cout << "Membership begin date : " << row[7] << endl;
		cout << "Membership end date  : " << row[8] << endl;
		cout << "Membership Status : " << row[9] << endl << endl << endl;
	}
	else
	{
		cout << "No Member Found!!" << endl << endl << endl;
	}
}

// ------------------
// Class Sales
// ------------------

void sales::add()
{
    cout << "Enter Member ID: ";
    cin >> member_id;
    cout << "Enter the book ID: ";
    cin >> book_id;
    cout << "Enter the quantity: ";
    cin >> qty;

    // Check if the connection is valid
    if (!conn) {
        cerr << "MySQL connection is not valid." << endl;
        return;
    }

    stmt.str("");
    stmt << "SELECT price * " << qty << " FROM books WHERE id = " << book_id << ";";
    query = stmt.str();
    q = query.c_str();

    // Execute the SELECT query to calculate the bill amount
    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing SELECT query: " << mysql_error(conn) << endl;
        return;
    }

    res_set = mysql_store_result(conn);

    if ((row = mysql_fetch_row(res_set)) != NULL) {
        cout << "The bill amount: " << row[0] << endl << endl;
        amount = stoi(row[0]);
    } else {
        cout << "Book ID invalid!!" << endl;
        return;
    }

    stmt.str("");
    stmt << "INSERT INTO sales(member_id, book_id, qty, amount, date_s) VALUES (" << member_id << "," << book_id << "," << qty << "," << amount << ",CURDATE());";
    query = stmt.str();
    q = query.c_str();

    // Execute the INSERT INTO query to add the sales record
    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing INSERT INTO query: " << mysql_error(conn) << endl;
        return;
    }

    // Fetch the invoice ID
    stmt.str("");
    stmt << "SELECT invoice_id FROM sales WHERE member_id = " << member_id << " AND book_id = " << book_id << " AND qty = " << qty << " AND date_s = CURDATE();";
    query = stmt.str();
    q = query.c_str();

    if (mysql_query(conn, q) != 0) {
        cerr << "Error executing SELECT query for invoice ID: " << mysql_error(conn) << endl;
        return;
    }

    res_set = mysql_store_result(conn);

    if ((row = mysql_fetch_row(res_set)) != NULL) {
        cout << "The Invoice ID for the bill is " << row[0] << endl << endl << endl;
    } else {
        cout << "The entered details may be wrong." << endl << "Please recheck and enter again." << endl << endl << endl;
    }
}

void sales::find_total_sales()
{
	query = "select sum(amount) from sales where year(date_s) = year(curdate());";
	q = query.c_str();
	mysql_query(conn,q);
	res_set = mysql_store_result(conn);
	if((row = mysql_fetch_row(res_set)) != NULL)
	{
		cout << "Total sales this year : " << row[0] << endl << endl << endl << endl;
	}
}

// ----------------
// Functions
// ----------------

void book_menu();
void sup_menu();
void pur_menu();
void emp_menu();
void mem_menu();
void sal_menu();

// main menu

void main_menu()
{
	int c;
	cout << "*************************************************" << endl;
	cout << "         BOOKSHOP MANGEMENT SYSTEM" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. BOOKS" << endl;
	cout << "   2. SUPPLIERS" << endl;
	cout << "   3. PURCHASES" << endl;
	cout << "   4. EMPLOYEES" << endl;
	cout << "   5. MEMBERS" << endl;
	cout << "   6. SALES" << endl;
	cout << "   7. EXIT" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			system("cls");
			book_menu();
			getch();
			break;
		case 2:
			system("cls");
			sup_menu();
			getch();
			break;
		case 3:
			system("cls");
			pur_menu();
			getch();
			break;
		case 4:
			system("cls");
			emp_menu();
			getch();
			break;
		case 5:
			system("cls");
			mem_menu();
			getch();
			break;
		case 6:
			system("cls");
			sal_menu();
			getch();
			break;
		case 7:
			exit(1);
		default:
			system("cls");
			cout << "Wrong Input" << endl << endl << "Invalid input";
			getch();
			break;
	}
	return;
}

// book menu

void book_menu()
{
	int c;
	books b;
	cout << "*************************************************" << endl;
	cout << "                  BOOK MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. UPDATE PRICE" << endl;
	cout << "   3. SEARCH" << endl;
	cout << "   4. DISPLAY ALL" << endl;
	cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			b.add();
			break;
		case 2:
			b.update_price();
			break;
		case 3:
			b.search();
			break;
		
		case 4:
			b.display();
			break;
		case 5:
			return;
			break;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
	return;
}

// supp menu

void sup_menu()
{
	int c;
	suppliers s;
	cout << "*************************************************" << endl;
	cout << "                SUPPLIER MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. ADD" << endl;
	cout << "   2. REMOVE" << endl;
	cout << "   3. SEARCH" << endl;
	cout << "   4. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			s.add_sup();
			break;
		case 2:
			s.remove_supplier();
			break;
		case 3:
			s.search_id();
			break;
		case 4:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// purchase menu

void pur_menu()
{
	int c;
	purchases p;
	cout << "*************************************************" << endl;
	cout << "                PURCHASES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Order" << endl;
	cout << "   2. View All" << endl;
	cout << "   3. Cancel Order" << endl;
	cout << "   4. Recieved Order" << endl;
	cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			p.new_ord();
			break;
		case 2:
			p.view();
			break;
		case 3:
			p.mar_cancel();
			break;
		case 4:
			p.mark_reciv();
			break;
		case 5:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// emp_menu

void emp_menu()
{
	int c;
	employees e;
	cout << "*************************************************" << endl;
	cout << "                 EMPLOYEE MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Employee" << endl;
	cout << "   2. Search Employee" << endl;

	cout << "   3. View All" << endl;
	cout << "   4. Update Salary" << endl;
	cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			e.add_emp();
			break;
		case 2:
			e.search_emp();
			break;
		
		case 3:
			e.display();
			break;
		case 4:
			e.update_sal();
			break;
		case 6:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}


// mem menu

void mem_menu()
{
	int c;
	members m;
	m.refresh();
	cout << "*************************************************" << endl;
	cout << "                 MEMBERS MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. New Member" << endl;
	cout << "   2. Search Member" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			m.add_mem();
			break;
		case 2:
			m.search_mem();
			break;
		case 3:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}

// sal_menu

void sal_menu()
{
	int c;
	sales s;
	cout << "*************************************************" << endl;
	cout << "                 SALES MENU" << endl;
	cout << "*************************************************" << endl;
	cout << "   1. Add New Bill" << endl;
	cout << "   2. Total Sales Of The Year" << endl;
	cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
	cout << "Enter Your Choice : ";
	cin >> c;
	switch (c)
	{
		case 1:
			s.add();
			break;
		case 2:
			s.find_total_sales();
			break;
		case 3:
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
	}
}


// Password Function

void pass()
{
	int num = 0 ;
	cout << "Enter password : ";
	for(int i = 0; i < 4; i++)
	{
		num=num*10+(getch()-48);
		cout<<"*";
	}
	if(num == PASSWORD)
	{
		cout << endl << endl << "Correct Password" << endl << endl;
		cout << "Press any key...";
		getch();
	}
	else
	{
		cout << endl << endl << "Incorrect Password" << endl << endl;
		cout << "Press any key...";
		getch();
		exit(1);
	}
	return;
}

// +----------------------------------+
// |		Main Function 	      |
// +----------------------------------+

int main()
{
	pass();
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, HOST, USER, PASS, DATABASE, PORT, NULL, 0);
	int choice;
	if(conn)
    {
		while(1)
		{
			system("cls");
			main_menu();
		}
    }
	else
	{
	    system("cls");
		cout << "Error While connection to database." << endl << "Contact Tech Expert." << endl;
		getch();
	}
	return 0;
}

