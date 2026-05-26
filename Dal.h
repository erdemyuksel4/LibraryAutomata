#include "Entities.h"
#include "collection.h"
const int TIME = 0;
struct MemberVerificationDal {
    List<MemberUser*> users;
    MemberVerificationDal() {
        users.Add(new MemberUser(101, "pass101", true));
        users.Add(new MemberUser(102, "pass102", true));
        users.Add(new MemberUser(103, "pass103", true));

        // Son 2 kullanýcý PASÝF (false) - q5 durumunda "Account Suspended" hatasýný test etmek için
        users.Add(new MemberUser(104, "pass104", false));
        users.Add(new MemberUser(105, "pass105", false));
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
        // 1. Durum: Tertemiz, rafta duran kitaplar (isAvailable = true, reserves boþ)
        books.Add(new Book("The Lord of the Rings", "ISBN9780261103251"));
        books.Add(new Book("Harry Potter and the Philosopher's Stone", "ISBN9780747532699"));
        books.Add(new Book("To Kill a Mockingbird", "ISBN9780061120084"));

        // 2. Durum: Baþkasý tarafýndan ÖDÜNÇ ALINMIÞ kitap (Borrow Otomatý -> q15 Rezervasyona düþecek)
        Book* b1 = new Book("1984", "ISBN9780451524935");
        b1->isAvailable = false;
        b1->reserves.Add(TIME, 102); // Kitap þu an 102 nolu üyede ödünçlü
        books.Add(b1);

        Book* b2 = new Book("The Great Gatsby", "ISBN9780743273565");
        b2->isAvailable = false;
        b2->reserves.Add(TIME, 103); // Kitap þu an 103 nolu üyede ödünçlü
        books.Add(b2);

        // 3. Durum: Hem baþkasýnda olan hem de ileriye dönük REZERVASYON KUYRUÐU olan kitap
        Book* b3 = new Book("Crime and Punishment", "ISBN9780140449136");
        b3->isAvailable = false;
        b3->reserves.Add(TIME, 102);     // Kitap þu an 102'de
        b3->reserves.Add(TIME + 1, 103); // Sonraki hak 103'ün (Kuyrukta)
        books.Add(b3);

        // 4. Durum: GECÝKMÝÞ KÝTAP (Renewal Otomatý -> q22 Ceza Ödeme katmanýna düþecek)
        // Üye 101 bu kitabý uzatmak istediðinde sistem cezaya düþürecek
        Book* b4 = new Book("Dune", "ISBN9780441172719");
        b4->isAvailable = false;
        b4->reserves.Add(TIME, 101); // Kitap þu an login olan 101 nolu üyede
        books.Add(b4);

        // Geri kalan standart kitaplar
        books.Add(new Book("The Hobbit", "ISBN9780261102216"));
        books.Add(new Book("The Catcher in the Rye", "ISBN9780316769174"));
        books.Add(new Book("Brave New World", "ISBN9780060850524"));
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