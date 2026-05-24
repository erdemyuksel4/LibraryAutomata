#include "./Dal.h"
#include "./Entities.h"

class MemberVerificationService {
private:
    MemberUser* user = nullptr;
    MemberVerificationDal memDal;
public:
    bool haveUser(int id) {
        user = memDal.findUser(id);
        return user != nullptr;
    }
    bool verifyUser(const char* pw) {
        return memDal.verifyUser(user, pw);
    }
    bool isActiveUser() {
        return user->activeMembership;
    }
};
class BookService {
    BookDal bookDal;
public:
    Book* Search(const char* search) {
        bool isISBN = StartsWith(search, "ISBN");
        if (isISBN) {
            return bookDal.findByISBN(search);
        }
        else {
            return bookDal.findByTitle(search);

        }
        

    }
    
    int isOverDue(Book* book,int uid) {
        
        return bookDal.isLate(book, uid);
    }
    void RemoveLateReserve(Book* book, int late) {
        bookDal.CancelReserve(book, TIME-late);
    }
    bool isAvailable(Book* book) {
        return book->isAvailable;
    }
    void renew(Book* book, int uid) {
        if (book->reserves[TIME] = uid) {
            if (book->reserves[TIME + 1] == 0) {
                book->reserves[TIME + 1] = uid;
            }
        }
    }
    void Borrow(Book* book, int userId) {
        book->isAvailable = false;
        Reserve(book,userId);
    }
    void Reserve(Book* book,int userId) {
        
        bookDal.Reserve(book, userId);
        
    }
    void Replace(Book* book) {
        bookDal.replace(book,new Book(book->title, book->isbn));
    }
};