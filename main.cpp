#include <iostream>
#include <mysql/mysql.h>
#include <sstream>
#include <string>

using namespace std;

const char* HOST="localhost";
const char* USER="root";
const char* PW=" ";
const char* DB="bank_management";


class Client {
private:
    string Id;
    string name;
    string surname;
    int age;
    string username;
    string password;
    double total_amount;
public:
    Client() {}

    void setId(const string& new_id) {
        Id = new_id;
    }

    void setName(const string& new_name) {
        name = new_name;
    }

    void setSurname(const string& new_surname) {
        surname = new_surname;
    }

    void setAge(int new_age) {
        age = new_age;
    }

    void setUsername(const string& new_username) {
        username = new_username;
    }

    void setPassword(const string& new_password) {
        password = new_password;
    }

    bool login(MYSQL* conn, const string& username, const string& password) {
        string exist = "SELECT * FROM client WHERE username = '" + username + "' AND password = '" + password + "'";
        if (mysql_query(conn, exist.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
            return false;
        } else {
            MYSQL_RES* res = mysql_store_result(conn);
            if (res) {
                bool loginSuccess = mysql_num_rows(res) > 0;
                mysql_free_result(res);
                return loginSuccess;
            } else {
                return false;
            }
        }
    }

    void Register(MYSQL* conn) {
        cout << "Enter name: ";
        cin >> name;

        cout << "Enter surname: ";
        cin >> surname;

        cout << "Enter age: ";
        cin >> age;

        cout << "Enter username: ";
        cin >> username;

        cout << "Enter password: ";
        cin >> password;

        stringstream ss;
        ss << age;

        string insert = "INSERT INTO client (name, surname, age, username, password) VALUES('" + name + "','" + surname + "','" + ss.str() + "','" + username + "','" + password + "')";
        if (mysql_query(conn, insert.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
        } else {
            cout << "Registered successfully" << endl;
        }
    }

    void CreateClientTable(MYSQL* conn) {
        const char* clientTable = R"(
            CREATE TABLE IF NOT EXISTS client (
                Id INT AUTO_INCREMENT PRIMARY KEY,
                name VARCHAR(255) NOT NULL,
                surname VARCHAR(255) NOT NULL,
                age INT NOT NULL,
                username VARCHAR(255) NOT NULL,
                password VARCHAR(255) NOT NULL
            )
        )";

        if (mysql_query(conn, clientTable)) {
            cout << "Error creating client table: " << mysql_error(conn) << endl;
        } else {
            cout << "Client table verified/created successfully." << endl;
        }
    }

    void displayAllClients(MYSQL* conn) {
        string query = "SELECT * FROM client";
        if (mysql_query(conn, query.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
        } else {
            MYSQL_RES* res = mysql_store_result(conn);
            if (res) {
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) {
                    cout << "Id: " << row[0] << ", Name: " << row[1] << ", Surname: " << row[2] << ", Age: " << row[3] << ", Username: " << row[4] << endl;
                }
                mysql_free_result(res);
            }
        }
    }

    void displayClientById(MYSQL* conn, const string& clientId) {
        string query = "SELECT * FROM client WHERE Id = '" + clientId + "'";
        if (mysql_query(conn, query.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
        } else {
            MYSQL_RES* res = mysql_store_result(conn);
            if (res) {
                MYSQL_ROW row;
                if ((row = mysql_fetch_row(res))) {
                    cout << "Id: " << row[0] << ", Name: " << row[1] << ", Surname: " << row[2] << ", Age: " << row[3] << ", Username: " << row[4] << endl;
                } else {
                    cout << "Client not found" << endl;
                }
                mysql_free_result(res);
            }
        }
    }

    void updateClient(MYSQL* conn) {
        cout << "Enter client ID to update: ";
        cin >> Id;

        cout << "Enter new name: ";
        cin >> name;

        cout << "Enter new surname: ";
        cin >> surname;

        cout << "Enter new age: ";
        cin >> age;

        cout << "Enter new username: ";
        cin >> username;

        cout << "Enter new password: ";
        cin >> password;

        stringstream ss;
        ss << age;

        string update = "UPDATE client SET name = '" + name + "', surname = '" + surname + "', age = '" + ss.str() + "', username = '" + username + "', password = '" + password + "' WHERE Id = '" + Id + "'";
        if (mysql_query(conn, update.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
        } else {
            cout << "Client updated successfully" << endl;
        }
    }

    void deleteClient(MYSQL* conn) {
        cout << "Enter client ID to delete: ";
        cin >> Id;

        string del = "DELETE FROM client WHERE Id = '" + Id + "'";
        if (mysql_query(conn, del.c_str())) {
            cout << "Error: " << mysql_error(conn) << endl;
        } else {
            cout << "Client deleted successfully" << endl;
        }
    }

    void deposit(MYSQL* conn,double quantity){
        cout << "Enter client ID to for deposit ";
        cin >> Id;
        string query("SELCT total_amount FROM client WHERE='"+Id+"'");
        if(mysql_query(conn, query.c_str())){
            cout<<"Error:"<<mysql_error(conn)<<endl;

        }else{
            MYSQL_RES* res = mysql_store_result(conn);
            if(res){
                MYSQL_ROW row = mysql_fetch_row(res);
                if(row){
                    total_amount = atof(row[0])+quantity;
                    stringstream ss;
                    ss << total_amount;

                    string update ="UPDATE client SET total_amount='"+ss.str()+"' WHERE Id = '"+Id+"'";
                    if (mysql_query(conn, update.c_str())) {
                        cout << "Error: " << mysql_error(conn) << endl;
                    } else {
                        cout << "Deposit successful. New total amount: " << total_amount << endl;
                    }
                }
                mysql_free_result(res);

                }
                
            }


        }

        void withdraw(MYSQL* conn, double quantity){
            cout<<"Enter Id client fo withdraw";
            cin>>Id;

            string query ="SELECT total_amount FROM client WHERE Id ='"+Id+"'";
            if(mysql_query(conn, query.c_str())){
                cout<<"error"<<mysql_error(conn);
            }else{
                MYSQL_RES* res = mysql_store_result(conn);
                if(res){
                    MYSQL_ROW row = mysql_fetch_row(res);
                    if(row){
                        double current_amount = atof(row[0]);
                        if(current_amount >= quantity){
                            total_amount = current_amount -quantity;
                            stringstream ss;
                            ss<<total_amount;

                            string update="UPDATE client SET total_amount ='"+ss.str()+"' WHERE Id='"+Id+"'";
                            if(mysql_query(conn, update.c_str())){
                                cout<<"error"<<mysql_error(conn)<<endl;
                            }else{
                                cout << "Withdrawal successful. New total amount: " << total_amount << endl;
                            }

                        }else{
                            cout << "Insufficient funds for withdrawal." << endl;
                        }
                    }
                    mysql_free_result(res);
                }
            }
        }

    };


int main() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
        return 1;
    } else {
        cout << "Connected to database" << endl;
    }

    Client client;
    client.CreateClientTable(conn);
    bool login = false;
    string username;
    string password;


    cout<<"request login"<<endl;
    cout<<"insert username"<<endl;
    cin>>username;
    cout<<"insert password"<<endl;
    cin>>password;
    login=client.login(conn,username,password);
    if(login == false){
        cout<<"plese register before login"<<endl;
        client.Register(conn);
        cout<<"insert username"<<endl;
        cin>>username;
        cout<<"insert password"<<endl;
        cin>>password;
    }

    if(login==true){
    bool exit = false;
    while (!exit) {
        int choice;
        cout << "2. Display All Clients" << endl;
        cout << "3. Display Client By ID" << endl;
        cout << "4. Update Client" << endl;
        cout << "5. Delete Client" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                client.Register(conn);
                break;
            case 2:
                client.displayAllClients(conn);
                break;
            case 3: {
                string clientId;
                cout << "Enter client ID: ";
                cin >> clientId;
                client.displayClientById(conn, clientId);
                break;
            }
            case 4:
                client.updateClient(conn);
                break;
            case 5:
                client.deleteClient(conn);
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice" << endl;
        }
    }

    mysql_close(conn);
    }
    return 0;
}
