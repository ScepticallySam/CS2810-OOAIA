#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <set>
using namespace std;

class Book{
    public:
        string title;
        string author;
        Book(string title = "UnknownTitle", string author = "UnknownAuthor", string isbn = "ISBN", int copiesAvailable = 0, int totalCopies = 5){
            this->title = title;
            this->author = author;
            this->isbn = isbn;
            this->copiesAvailable = copiesAvailable;
            this->totalCopies = totalCopies;
        }
        Book(Book &a, string new_isbn) {
            this->title = a.title;
            this->author = a.author;
            this->isbn = new_isbn;
            this->copiesAvailable = a.get_copiesAvailable();
            this->totalCopies = a.get_totalCopies();
        }
   
        ~Book() {}
   
        string get_isbn() {return isbn;}
        int get_copiesAvailable() {return copiesAvailable;}
        int get_totalCopies() {return totalCopies;}
   
        void updateCopies(int count) {
            if(this->copiesAvailable + count >= 0 && this->totalCopies + count >= 0) {
                this->copiesAvailable += count;
                this->totalCopies += count;
            }
            else cout << "Invalid request! Count becomes negative\n";
        }
   
        bool borrowBook() {
            if(copiesAvailable == 0) {
                cout << "Invalid request! Copy of book not available\n";
                return false;
            }
            copiesAvailable--;
            return true;
        }
   
        bool returnBook() {
            if(copiesAvailable == totalCopies) {
                cout << "Invalid request! Copy of book exceeds total copies\n";
                return false;
            }
            copiesAvailable++;
            return true;
        }
   
        void printDetails() {
            cout << title << " " << author << "\n";
        }    
   
    private:
        string isbn;
        int copiesAvailable;
        int totalCopies;
};

class Member {
    public:
        string name;
       
        Member() {}
        Member(string memberID, string name, int borrowLimit = 3) {
            this->memberID = memberID;
            this->name = name;
            this->borrowLimit = borrowLimit;
            this->borrowCount = 0;
        }
   
        bool borrowBook(string isbn) {
            if(borrowCount == borrowLimit) {
                cout << "Invalid request! Borrow limit exceeded\n";
                return false;
            }
            borrowedBooks[isbn]++;
            borrowCount++;
            return true;
        }
   
        bool returnBook(string isbn) {
            if(borrowedBooks[isbn] == 0) {
                cout << "Invalid request! Book not borrowed\n";
                return false;
            }
            borrowedBooks[isbn]--;
            borrowCount--;
            return true;
        }
   
        void printDetails() {
            for(auto book : borrowedBooks) {
                if(book.second != 0) cout << memberID << " " << name << " " << book.first << " " << book.second << "\n";          
            }
        }
   
        string get_ID() {return memberID;}
   
    private:
        map<string,int> borrowedBooks;
        int borrowLimit;
        int borrowCount;
        string memberID;
};

class Library {
    public:
        Library() {}
   
        bool addBook(Book& book) {
            if(isbns.find(book.get_isbn()) == isbns.end()) {
                books.push_back(book);
                isbns[book.get_isbn()] = books.size() - 1;
                return true;
            }
            cout << "Invalid request! Book with same isbn already exists\n";
            return false;
        }
   
        bool registerMember(Member& mem) {
            if(memberIDs.find(mem.get_ID()) == memberIDs.end()) {
                members.push_back(mem);
                memberIDs[mem.get_ID()] = members.size() - 1;
                return true;
            }
            cout << "Invalid request! Member with same id already exists\n";
            return false;
        }
   
        bool borrowBook(string memberID, string isbn) {
            Member& borrower = members[memberIDs[memberID]]; //finding the member, with reference
            Book& borrowedBook = books[isbns[isbn]]; //finding the book, with reference
            if(borrowedBook.borrowBook()) {
                if(borrower.borrowBook(isbn)) {
                    return true;
                }
                else {
                    borrowedBook.returnBook();
                }
            }
            return false;
        }
   
        bool returnBook(string memberID, string isbn) {
            Member& returner = members[memberIDs[memberID]]; //finding the member, with reference
            Book& returnedBook = books[isbns[isbn]]; //Finding the book, with reference
            if(returnedBook.returnBook()) {
                if(returner.returnBook(isbn)) {
                    return true;
                }
                else {
                    returnedBook.borrowBook();
                }
            }
            return false;
        }
   
        void printLibraryDetails() {
            for(auto elem : books) {
                cout << elem.title << " " << elem.author << " " << elem.get_copiesAvailable() << "\n";
            }
            for(auto elem : members) {
                cout << elem.get_ID() << " " << elem.name << "\n";
            }
        }
   
        Book& get_book(string isbn) {
            return books[isbns[isbn]];
        }
   
        Member& get_member(string mID) {
            return members[memberIDs[mID]];
        }
   
    private:
        vector<Book> books;
        vector<Member> members;
        map<string,int> isbns; //maps the isbns to the index in the vector of books
        map<string,int> memberIDs; //maps the memberID to the index in the members vector
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    Library myLibrary;
    string query;
    cin >> query;
    while(query != "Done") {
        if(query == "Book") {
            string first; //Taking first input since all the cases can be deduced after seeing the first input
            cin >> first;
            if(first == "None") {
                Book book;
                myLibrary.addBook(book);
            }
            else if(first == "ExistingBook") {
                string old_isbn, new_isbn;
                cin >> old_isbn >> new_isbn;
                Book copy = myLibrary.get_book(old_isbn); //finding the book, copy of the object is enough, no need of reference
                Book book(copy,new_isbn);
                myLibrary.addBook(book);
            }
            else {
                string author, isbn;
                int copiesAvailable,TotalCopies;
                cin >> author >> isbn >> copiesAvailable >> TotalCopies;
                Book book(first,author,isbn,copiesAvailable,TotalCopies);
                myLibrary.addBook(book);
            }
        }
        else if(query == "UpdateCopiesCount") {
            string isbn;
            int newCount;
            cin >> isbn >> newCount;
            Book& book = myLibrary.get_book(isbn); //first find the book, reference to the object is needed
            book.updateCopies(newCount);
        }
        else if(query == "Member") {
            string first;
            cin >> first;
            if(first == "NoBorrowLimit") {
                string mID, name;
                cin >> mID >> name;
                Member member(mID,name);
                myLibrary.registerMember(member);
            }
            else {
                string name;
                int borrowlimit;
                cin >> name >> borrowlimit;
                Member member(first,name,borrowlimit);
                myLibrary.registerMember(member);
            }
        }
        else if(query == "Borrow") {
            string mID, isbn;
            cin >> mID >> isbn;
            myLibrary.borrowBook(mID,isbn);
        }
        else if(query == "Return") {
            string mID, isbn;
            cin >> mID >> isbn;
            myLibrary.returnBook(mID,isbn);
        }
        else if(query == "PrintMember") {
            string memberId;
            cin >> memberId;
            myLibrary.get_member(memberId).printDetails();
        }
        else if(query == "PrintBook") {
            string isbn;
            cin >> isbn;
            myLibrary.get_book(isbn).printDetails();
        }
        else if(query == "PrintLibrary") {
            myLibrary.printLibraryDetails();
        }
        cin >> query;
    }
    return 0;
}
