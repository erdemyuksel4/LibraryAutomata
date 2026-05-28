#include "Entities.h"
#include "collection.h"
const int TIME = 18;
struct MemberVerificationDal {
    List<MemberUser*> users;
    MemberVerificationDal() {
        users.Add(new MemberUser(101, "pass101", true));
        users.Add(new MemberUser(102, "pass102", true));
        users.Add(new MemberUser(103, "pass103", true));

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

        books.Add(new Book("1984", "ISBN9780451524935"));
        books.Add(new Book("Harry Potter and the Philosopher's Stone", "ISBN9780747532699"));

        Book* b_sc3_1 = new Book("Dune", "ISBN9780441172719");
        b_sc3_1->isAvailable = false;
        b_sc3_1->reserves.Add(TIME, 102); 
        books.Add(b_sc3_1);

        Book* b_sc3_2 = new Book("The Great Gatsby", "ISBN9780743273565");
        b_sc3_2->isAvailable = false;
        b_sc3_2->reserves.Add(TIME, 103);
        books.Add(b_sc3_2);

        books.Add(new Book("The Lord of the Rings", "ISBN9780261103251"));
        books.Add(new Book("Nutuk", "ISBN9780012312084"));

        
        Book* b_sc5_1 = new Book("Hamlet", "ISBN9780141019321");
        b_sc5_1->isAvailable = false;
        b_sc5_1->reserves.Add(TIME, 105);
        books.Add(b_sc5_1);

        Book* b_sc5_2 = new Book("Macbeth", "ISBN9780141014135");
        b_sc5_2->isAvailable = false;
        b_sc5_2->reserves.Add(TIME, 106);
        books.Add(b_sc5_2);

       
        books.Add(new Book("To Kill a Mockingbird", "ISBN9780061120084")); 
        Book* b_sc6 = new Book("Odyssey", "ISBN9780140449112");            
        b_sc6->isAvailable = false;
        b_sc6->reserves.Add(TIME, 107);
        books.Add(b_sc6);

        Book* b_sc7 = new Book("Crime and Punishment", "ISBN9780140449136");
        b_sc7->isAvailable = false;
        b_sc7->reserves.Add(TIME - 5, 101); 
        books.Add(b_sc7);

        Book* b_sc9_1 = new Book("Les Miserables", "ISBN9780140444308");
        b_sc9_1->isAvailable = false;
        b_sc9_1->reserves.Add(TIME - 10, 101);
        books.Add(b_sc9_1);

        Book* b_sc10_1 = new Book("Faust", "ISBN9780140449013");
        b_sc10_1->isAvailable = false;
        b_sc10_1->reserves.Add(TIME - 15, 101); 
        books.Add(b_sc10_1);

        Book* b_sc8 = new Book("The Hobbit", "ISBN9780261102216");
        b_sc8->isAvailable = false;
        b_sc8->reserves.Add(TIME, 101);
        books.Add(b_sc8);

        Book* b_sc9_2 = new Book("The Catcher in the Rye", "ISBN9780316769174");
        b_sc9_2->isAvailable = false;
        b_sc9_2->reserves.Add(TIME, 101);
        books.Add(b_sc9_2);

        Book* b_sc10_2 = new Book("Brave New World", "ISBN9780060850524");
        b_sc10_2->isAvailable = false;
        b_sc10_2->reserves.Add(TIME, 101);
        books.Add(b_sc10_2);

        books.Add(new Book("The Divine Comedy", "ISBN9780141197494"));
        books.Add(new Book("Pride and Prejudice", "ISBN9780141439517"));
        books.Add(new Book("The Odyssey", "ISBN9780140449112"));
        books.Add(new Book("The Iliad", "ISBN9780140447941"));
        books.Add(new Book("Moby Dick", "ISBN9780142437247"));
        books.Add(new Book("Ulysses", "ISBN9780141182803"));
        books.Add(new Book("The Republic", "ISBN9780140455113"));
        books.Add(new Book("The Old Man and the Sea", "ISBN9780684801223"));
        books.Add(new Book("The Stranger", "ISBN9780679720201"));
        books.Add(new Book("The Trial", "ISBN9780805209990"));
        books.Add(new Book("The Metamorphosis", "ISBN9780553213690"));
        books.Add(new Book("The Castle", "ISBN9780805210446"));
        books.Add(new Book("The Brothers Karamazov", "ISBN9780374528379"));
        books.Add(new Book("The Idiot", "ISBN9780375702921"));
        books.Add(new Book("The Possessed", "ISBN9780679734291"));
        books.Add(new Book("Notes from Underground", "ISBN9780679734536"));
        books.Add(new Book("War and Peace", "ISBN9781400079988"));
        books.Add(new Book("Anna Karenina", "ISBN9780143035008"));
        books.Add(new Book("The Death of Ivan Ilyich", "ISBN9780307951335"));
        books.Add(new Book("Resurrection", "ISBN9780140446340"));
        books.Add(new Book("Don Quixote", "ISBN9780060934347"));
        books.Add(new Book("Madame Bovary", "ISBN9780140449129"));
        books.Add(new Book("Wuthering Heights", "ISBN9780141439555"));
        books.Add(new Book("Jane Eyre", "ISBN9780141441145"));
        books.Add(new Book("Great Expectations", "ISBN9780141439562"));
        books.Add(new Book("David Copperfield", "ISBN9780141439418"));
        books.Add(new Book("A Tale of Two Cities", "ISBN9780141439609"));
        books.Add(new Book("Oliver Twist", "ISBN9780141439746"));
        books.Add(new Book("Frankenstein", "ISBN9780141439470"));
        books.Add(new Book("Dracula", "ISBN9780141439845"));
        books.Add(new Book("The Picture of Dorian Gray", "ISBN9780141439579"));
        books.Add(new Book("The Count of Monte Cristo", "ISBN9780140449266"));
        books.Add(new Book("The Three Musketeers", "ISBN9780140440256"));
        books.Add(new Book("The Grapes of Wrath", "ISBN9780143039433"));
        books.Add(new Book("East of Eden", "ISBN9780142004234"));
        books.Add(new Book("Of Mice and Men", "ISBN9780140177398"));
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