#include "Entities.h"
#include "collection.h"
const int TIME = 0;
struct MemberVerificationDal {
    List<MemberUser*> users;
    MemberVerificationDal() {
        users.Add(new MemberUser(1, "1", true));
        users.Add(new MemberUser(2, "2", true));
        users.Add(new MemberUser(3, "3", true));
        users.Add(new MemberUser(4, "4", true));
        users.Add(new MemberUser(5, "5", true));
    }
    MemberUser* findUser(int id) {
        MemberUser* user = users.FindFirst([id](MemberUser* x) {
            return x->id == id;
            });
        if (user != nullptr) {
            return user;
        }
        return nullptr;
    };

    bool verifyUser(MemberUser* user, const char* pw) {
        if (user == nullptr) return false;
        int i = 0;
        for (;pw[i] != '\0';i++) {
            if (user->pw[i] == '\0' || pw[i] != user->pw[i]) {
                return false;
            }
        }
        return user->pw[i] == '\0';
    }

};
struct BookDal {
    List<Book*> books;

    BookDal() {
        books.Add(new Book("A", "ISBN1"));
        books.Add(new Book("A1", "ISBN12"));
        books.Add(new Book("A2", "ISBN13"));
        books.Add(new Book("A3", "ISBN14"));
        books.Add(new Book("A4", "ISBN15"));
        books.Add(new Book("A5", "ISBN16"));
        books.Add(new Book("A6", "ISBN17"));
    }
    Book* findByTitle(const char* search) {
        Book* book = books.FindFirst([search](Book* x) {
            return Compare<const char*>(x->title, search);
            });

        return book;
    }
    Book* findByISBN(const char* search) {
        Book* book = books.FindFirst([search](Book* x) {
            return x->isbn == search;
            });
        if (book != nullptr) {
            return book;
        }
        return nullptr;
    }

    bool isAvailable(Book* book) {
        if (!book->isAvailable)return false;
        
        return !book->reserves.ContainsKey(TIME);
        
    }
    void Reserve(Book* book,int uid) {

        for (int i = TIME;i < 26;i++) {
            if (!book->reserves.ContainsKey(i)) {
                book->reserves.Add(i,uid);
            }
        }
    }
    void replace(Book* book,Book* nBook) {
        book = nullptr;
        book = nBook;
    }
    void CancelReserve(Book* book, int time) {
        book->reserves[time]=0;
        
    }
    int isLate(Book* book, int uid) {
        int i = TIME;
        for (;book->reserves[i] != uid;i--) {}
        return i;
    }
};