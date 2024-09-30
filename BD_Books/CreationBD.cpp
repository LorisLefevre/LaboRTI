#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>

typedef struct {
  int  id;
  char last_name[20];
  char first_name[20];
  char birth_date[20];
} AUTHOR;

typedef struct {
  int  id;
  char name[20];
} SUBJECT;

typedef struct {
  int   id;
  int   author_id;    // clé étrangère
  int   subject_id;   // clé étrangère
  char  title[100];
  char  isbn[20];
  int   page_count;
  int   stock_quantity;
  float price;
  int   publish_year;
} BOOK;

typedef struct {
    int id;
    char login[20];
    char password[20];
} EMPLOYEE;

typedef struct {
    int id;
    char last_name[20];
    char first_name[20];
    int client_number;
    char address[30];
} CLIENT;

typedef struct {
    int id;
    int client_id; // Clé étrangère
    char date[11]; // Format YYYY-MM-DD
    float amount;
    float payed;
} CADDY;

typedef struct {
    int id;
    int caddy_id; // Clé étrangère
    int book_id;  // Clé étrangère
    int quantity;
} CADDY_ITEM;

AUTHOR authors[] = {
  {-1,"Werber","Bernard","1961-09-18"},  // id = 1
  {-1,"Brown","Dan","1964-06-22"},       // id = 2
  {-1,"Herbert","Frank","1920-10-08"},   // id = 3
  {-1,"Cixin","Liu","1963-06-23"},       // id = 4
  {-1,"Delannoy","Claude","1946-01-06"}, // id = 5
  {-1,"Le Cun","Yan","1960-07-08"},      // id = 6
  {-1,"Verne","Jules","1828-02-08"},     // id = 7
  {-1,"Harris","Thomas","1940-09-022"}   // id = 8
};
int nbAuthors = 8;

SUBJECT subjects[] = {
  {-1,"Science-Fiction"},                // id = 1
  {-1,"Thriller"},                       // id = 2
  {-1,"Informatique"}                    // id = 3
};
int nbSubjects = 3;

BOOK books[] = {
  {-1,1,1,"Les Thanatonautes","978-2253139225",505,3,9.70f,1999},
  {-1,1,1,"Les Fourmis","978-2253063339",312,10,9.99f,1998},
  {-1,1,1,"La boite de Pandore","978-2253934332",576,1,9.70f,2020},
  {-1,2,2,"Da Vinci Code","978-2709624930",574,6,22.90f,2004},
  {-1,2,2,"Anges et demons","978-2253093008",720,2,10.40f,2015},
  {-1,3,1,"Dune","978-2266320481",928,13,11.95f,2021},
  {-1,4,1,"Le probleme a trois corps","978-2330181055",512,0,11.00f,2024},
  {-1,5,3,"Programmer en C++ moderne: De C++11 a C++20","978-2212678956",848,1,38.00f,2019},
  {-1,5,3,"Programmer en Java","978-2212118896",916,8,71.18f,2016},
  {-1,6,3,"Quand la machine apprend","978-2415006563",394,2,11.50f,2023},
  {-1,7,1,"Vingt mille lieues sous les mers","978-2253006329",606,11,6.70f,2001},
  {-1,3,1,"Les heretiques de Dune","978-2221264577",598,2,24.90f,2023},
  {-1,8,2,"Le silence des agneaux","978-2266208949",377,17,7.70f,2015}
};
int nbBooks = 13;

EMPLOYEE employees[] = {
  { -1, "jdoe", "password123" },   // id = 1
  { -1, "asmith", "password456" },  // id = 2
  { -1, "jroberts", "password789" } // id = 3
};
int nbEmployees = 3;

CLIENT clients[] = {
  { -1, "Doe", "John", 1, "123 Main St." }, // id = 1
  { -1, "Smith", "Alice", 2, "456 Elm St." }, // id = 2
  { -1, "Roberts", "Jane", 3, "789 Oak St." } // id = 3
};
int nbClients = 3;

CADDY caddies[] = {
  { -1, 1, "2024-09-01", 100.00f, 0.00f }, // id = 1
  { -1, 2, "2024-09-02", 150.00f, 50.00f }, // id = 2
  { -1, 3, "2024-09-03", 200.00f, 200.00f }  // id = 3
};
int nbCaddies = 3;

CADDY_ITEM caddy_items[] = {
  { -1, 1, 1, 2 }, // id = 1, 2 copies of book with id 1 in caddy id 1
  { -1, 1, 2, 1 }, // id = 2, 1 copy of book with id 2 in caddy id 1
  { -1, 2, 1, 3 }  // id = 3, 3 copies of book with id 1 in caddy id 2
};
int nbCaddy_items = 3;

void finish_with_error(MYSQL *con);

int main(int argc,char *argv[])
{
  // Connection a MySql
  printf("Connection a la BD...\n");
  MYSQL* connexion = mysql_init(NULL);
  mysql_real_connect(connexion,"localhost","Student","PassStudent1_","PourStudent",0,0,0);

  // Supprimer la table caddy_items si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS caddy_items;")) {
    finish_with_error(connexion);
}

// Supprimer la table caddies si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS caddies;")) {
    finish_with_error(connexion);
}

// Supprimer la table clients si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS clients;")) {
    finish_with_error(connexion);
}

// Supprimer la table employees si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS employees;")) {
    finish_with_error(connexion);
}

// Supprimer la table books si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS books;")) {
    finish_with_error(connexion);
}

// Supprimer la table subjects si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS subjects;")) {
    finish_with_error(connexion);
}

// Supprimer la table authors si elle existe déjà
if (mysql_query(connexion, "DROP TABLE IF EXISTS authors;")) {
    finish_with_error(connexion);
}


  // Creation de la table authors
  printf("Creation de la table authors...\n");
  if (mysql_query(connexion,"CREATE TABLE authors ("
                      "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                      "last_name VARCHAR(20), "
                      "first_name VARCHAR(20), "
                      "birth_date DATE);")) {
    finish_with_error(connexion);
  }

  // Creation de la table subjects
  printf("Creation de la table subjects...\n");
  if (mysql_query(connexion,"CREATE TABLE subjects ("
                      "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                      "name VARCHAR(20));")) {
    finish_with_error(connexion);
  }

  // Créer la table books
  printf("Creation de la table books...\n");
  if (mysql_query(connexion, "CREATE TABLE books ("
                       "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                       "author_id INT, "
                       "subject_id INT, "
                       "title VARCHAR(100), "
                       "isbn VARCHAR(20), "
                       "page_count INT, "
                       "stock_quantity INT, "
                       "price DECIMAL(10, 2), "
                       "publish_year INT, "
                       "FOREIGN KEY (author_id) REFERENCES authors(id), "
                       "FOREIGN KEY (subject_id) REFERENCES subjects(id));")) {
    finish_with_error(connexion);
  }

  // Création de la table employees
printf("Création de la table employees...\n");
if(mysql_query(connexion, "CREATE TABLE employees("
                    "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                    "login VARCHAR(20), "
                    "password VARCHAR(20));")) 
{
    finish_with_error(connexion);
}

// Création de la table clients
printf("Création de la table clients...\n");
if(mysql_query(connexion, "CREATE TABLE clients("
                    "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                    "nom VARCHAR(20), "
                    "prenom VARCHAR(20), "
                    "client_number INT(4) UNIQUE, "
                    "adresse VARCHAR(30));"))
{
    finish_with_error(connexion);
}

// Création de la table caddies
printf("Création de la table caddies...\n");
if(mysql_query(connexion, "CREATE TABLE caddies("
                    "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                    "client_id INT, "
                    "date DATE, "
                    "amount DECIMAL(10, 2), "
                    "payed DECIMAL(10, 2), "
                    "FOREIGN KEY (client_id) REFERENCES clients(id));"))
{
    finish_with_error(connexion);
}

// Création de la table caddy_items
printf("Création de la table caddy_items...\n");
if(mysql_query(connexion, "CREATE TABLE caddy_items("
                    "id INT(4) AUTO_INCREMENT PRIMARY KEY, "
                    "caddy_id INT, "
                    "book_id INT, "
                    "quantity INT(4), "
                    "FOREIGN KEY (caddy_id) REFERENCES caddies(id), "
                    "FOREIGN KEY (book_id) REFERENCES books(id));"))
{
    finish_with_error(connexion);
}

  // Ajout de tuples dans la table authors
  printf("Ajout de %d auteurs la table authors...\n",nbAuthors);
  char request[256];
  for (int i=0 ; i<nbAuthors ; i++) {
    sprintf(request,"insert into authors values (NULL,'%s','%s','%s');",
      authors[i].last_name,authors[i].first_name,authors[i].birth_date);
    if (mysql_query(connexion,request) ) {
      finish_with_error(connexion);   
    }
  }


  // Ajout de tuples dans la table subjects
  printf("Ajout de %d sujets la table subjects...\n",nbSubjects);
  for (int i=0 ; i<nbSubjects ; i++) {
    sprintf(request,"insert into subjects values (NULL,'%s');",subjects[i].name);
    if (mysql_query(connexion,request)) {
      finish_with_error(connexion);   
    }
  }


  // Ajout de tuples dans la table books
  printf("Ajout de %d livres la table books...\n",nbBooks);
  for (int i=0 ; i<nbBooks ; i++) {
    sprintf(request,"INSERT INTO books (author_id, subject_id, title, isbn, page_count, stock_quantity, price, publish_year) VALUES (%d, %d, '%s', '%s', %d, %d, %f, %d);", 
      books[i].author_id,books[i].subject_id,books[i].title,books[i].isbn,books[i].page_count,books[i].stock_quantity,books[i].price,books[i].publish_year);
    if (mysql_query(connexion,request)) {
      finish_with_error(connexion);   
    }
  }

// Ajout de tuples dans la table employees
printf("Ajout de %d employés dans la table employees...\n", nbEmployees);
for(int i = 0; i < nbEmployees; i++)
{
    sprintf(request, "INSERT INTO employees (login, password) VALUES ('%s', '%s');", 
            employees[i].login, employees[i].password);
    if(mysql_query(connexion, request))
    {
        finish_with_error(connexion);
    }
}

// Ajout de tuples dans la table clients
printf("Ajout de %d clients dans la table clients...\n", nbClients);
for(int i = 0; i < nbClients; i++)
{
    sprintf(request, "INSERT INTO clients (nom, prenom, client_number, adresse) VALUES ('%s', '%s', %d, '%s');", 
            clients[i].last_name, clients[i].first_name, clients[i].client_number, clients[i].address);
    if(mysql_query(connexion, request))
    {
        finish_with_error(connexion);
    }
}

// Ajout de tuples dans la table caddies
printf("Ajout de %d caddies dans la table caddies...\n", nbCaddies);
for(int i = 0; i < nbCaddies; i++)
{
    sprintf(request, "INSERT INTO caddies (client_id, date, amount, payed) VALUES (%d, '%s', %.2f, %.2f);", 
            caddies[i].client_id, caddies[i].date, caddies[i].amount, caddies[i].payed);
    if(mysql_query(connexion, request))
    {
        finish_with_error(connexion);
    }
}

// Ajout de tuples dans la table caddy_items
printf("Ajout de %d caddy_items dans la table caddy_items...\n", nbCaddy_items);
for(int i = 0; i < nbCaddy_items; i++)
{
    sprintf(request, "INSERT INTO caddy_items (caddy_id, book_id, quantity) VALUES (%d, %d, %d);", 
            caddy_items[i].caddy_id, caddy_items[i].book_id, caddy_items[i].quantity);
    if(mysql_query(connexion, request))
    {
        finish_with_error(connexion);
    }
}


  // Deconnection de la BD
  mysql_close(connexion);
  exit(0);
}

void finish_with_error(MYSQL *con) {
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}