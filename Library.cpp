#include "./Service.h"
#include "./DFAEngine.h"

#include <iostream>




struct LibraryDFALoginContext {
    int id;
    char pass[50];
};
struct LibraryDFALogin {
    static MemberVerificationService memSer;
    static DFAEngine engine;
    static LibraryDFALoginContext ctx;
    static const char* q0() {
        std::cout << "\n[STATE]: q0 (Giris Ekrani)\n";
        std::cout << "ID: ";
        std::cin >> ctx.id;

        std::cout << "--> [OUTPUT]: \"id\" firlatildi, q1'e geciliyor.\n";
        return "id";
    }

    static const char* q1() {
        std::cout << "\n[STATE]: q1 (Kullanici Kontrolu)\n";
        bool realUser = memSer.haveUser(ctx.id);
        if (!realUser) {
            std::cout << "--> [OUTPUT]: \"wrong\" (Kullanici Bulunamadi). q10'a (ERR) gidiliyor.\n";
            return "wrong";
        }
        std::cout << "--> [OUTPUT]: \"user\" (Kullanici Var). q2'ye geciliyor.\n";
        return "user";
    }

    static const char* q2() {
        std::cout << "\n[STATE]: q2 (Sifre Giris - 1. Hak)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" girildi, dogrulama (q3) yapilacak.\n";
        return "pw";
    }

    static const char* q3() {
        std::cout << "\n[STATE]: q3 (Sifre Dogrulama - 1. Hak)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Sifre Dogru). Uyelik kontrolune (q5) geciliyor.\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (Sifre Yanlis!). 2. Hak icin q4'e gidiliyor.\n";
        return "invalid";
    }

    static const char* q4() {
        std::cout << "\n[STATE]: q4 (Sifre Giris - 2. Hak)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" girildi, dogrulama (q7) yapilacak.\n";
        return "pw";
    }

    static const char* q5() {
        std::cout << "\n[STATE]: q5 (Uyelik Aktiflik Kontrolu)\n";
        bool active = memSer.isActiveUser();
        if (active) {
            std::cout << "--> [OUTPUT]: \"active\" (Uyelik Aktif). q6'ya (ACC) geciliyor.\n";
            return "active";
        }
        std::cout << "--> [OUTPUT]: \"inactive\" (Uyelik Pasif!). q10'a (ERR) gidiliyor.\n";
        return "inactive";
    }

    static const char* q6() {
        std::cout << "\n[STATE]: q6 (ACCEPT STATE - GIRIS BASARILI)\n";
        return "ACC";
    }

    static const char* q7() {
        std::cout << "\n[STATE]: q7 (Sifre Dogrulama - 2. Hak)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Sifre Dogru). Uyelik kontrolune (q5) geciliyor.\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (Sifre Yine Yanlis!). Son hak icin q8'e gidiliyor.\n";
        return "invalid";
    }

    static const char* q8() {
        std::cout << "\n[STATE]: q8 (Sifre Giris - Son Hak)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" girildi, son dogrulama (q9) yapilacak.\n";
        return "pw";
    }

    static const char* q9() {
        std::cout << "\n[STATE]: q9 (Sifre Dogrulama - Son Hak)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Sifre Dogru). Uyelik kontrolune (q5) geciliyor.\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (Haklar Tukendi!). q10'a (ERR) gidiliyor.\n";
        return "invalid";
    }

    static const char* q10() {
        std::cout << "\n[STATE]: q10 (TRAP/ERROR STATE - ERISIM ENGELLENDI)\n";
        return "ERR";
    }
    static void Init() {

        engine.AddTransition("q0", "id", "q1");
        engine.AddTransition("q1", "wrong", "q10");
        engine.AddTransition("q1", "user", "q2");
        engine.AddTransition("q2", "pw", "q3");
        engine.AddTransition("q3", "valid", "q5");
        engine.AddTransition("q5", "active", "q6");
        engine.AddTransition("q5", "inactive", "q10");
        engine.AddTransition("q3", "invalid", "q4");
        engine.AddTransition("q4", "pw", "q7");
        engine.AddTransition("q7", "invalid", "q8");
        engine.AddTransition("q7", "valid", "q5");
        engine.AddTransition("q8", "pw", "q9");
        engine.AddTransition("q9", "valid", "q5");
        engine.AddTransition("q9", "invalid", "q10");
        engine.AddFunction("q0", q0);
        engine.AddFunction("q1", q1);
        engine.AddFunction("q2", q2);
        engine.AddFunction("q3", q3);
        engine.AddFunction("q4", q4);
        engine.AddFunction("q5", q5);
        engine.AddFunction("q6", q6);
        engine.AddFunction("q7", q7);
        engine.AddFunction("q8", q8);
        engine.AddFunction("q9", q9);
        engine.AddFunction("q10", q10);

    }


    static  void Login() {
        engine.EngineStart();
    }
};

struct LibraryDFABorrowContext {
    char search[50];
    Book* book;
};
struct LibraryDFABorrow {
    static LibraryDFABorrowContext ctx;
    static BookService bookSer;
    static DFAEngine engine;
    static const char* q8() {
        
        std::cout << "Title/ISBN: ";
        std::cin >> ctx.search;


        return "search";
    }
    static const char* q9() {
        
        Book* found = bookSer.Search(ctx.search);
        if (found !=nullptr) {
            ctx.book = found;
            return "found";
        }

        return "notfound";
    }
    static const char* q10() {
        bool available = bookSer.isAvailable(ctx.book);
        if (available) {
            return "available";
        }
        return "unavailable";
    }
    static const char* q11() {
        bookSer.Borrow(ctx.book, LibraryDFALogin::ctx.id);
        return "borrow";
    }
    static const char* q12() {

        char con[10];
        std::cout << "Search/Accept?";
        std::cin >> con;
        if (Compare<const char*>(con, "Accept")) {
            return"ACC";
        }
        return "search";
    }
    static const char* q13() {
        return"ERR";
    }
    static const char* q14() {
        bookSer.Reserve(ctx.book, LibraryDFALogin::ctx.id);
        return "reserve";
    }
    static void Init() {

        engine.AddTransition("q8", "search", "q9");
        engine.AddTransition("q9", "found", "q10");
        engine.AddTransition("q9", "notfound", "q13");
        engine.AddTransition("q10", "available", "q11");
        engine.AddTransition("q10", "unavailable", "q14");
        engine.AddTransition("q11", "borrow", "q12");
        engine.AddTransition("q12", "search", "q9");
        engine.AddTransition("q14", "reserve", "q12");

        engine.AddFunction("q8", q8);
        engine.AddFunction("q9", q9);
        engine.AddFunction("q10", q10);
        engine.AddFunction("q11", q11);
        engine.AddFunction("q12", q12);
        engine.AddFunction("q13", q13);
        engine.AddFunction("q14", q14);
    }
    static void Borrow() {
        engine.EngineStart();
    }
};
struct LibraryDFARenewContext {
    char search[50];
    Book* book;
    int late;
};
struct LibraryDFARenewal {
    static BookService bookSer;
    static DFAEngine engine;
    static LibraryDFARenewContext ctx;
    static const char* q15() {

        std::cout << "Title/ISBN: ";
        std::cin >> ctx.search;

        ctx.book = bookSer.Search(ctx.search);

        return "book";
    }
    static const char* q16() {


        char con[10];
        std::cout << "Due/Lost?";
        std::cin >> con;
        if (Compare<const char*>(con, "Lost")) {
            return"lost";
        }
        return "due";
       

    }
    static const char* q17() {

        int late  =bookSer.isOverDue(ctx.book, LibraryDFALogin::ctx.id);
        if (late > 0) {
            return "overdue";

        }
        return "renew";



    }
    static const char* q18() {


        bookSer.RemoveLateReserve(ctx.book,ctx.late);

        return "pay";



    }
    static const char* q19() {
        char con[10];
        std::cout << "Next/Accept?";
        std::cin >> con;
        if (Compare<const char*>(con, "Accept")) {
            return"ACC";
        }
        return "next";
    }
    static const char* q20() {
        bookSer.renew(ctx.book, LibraryDFALogin::ctx.id);
        return "ekstra";
    }
    static const char *q21() {
        bookSer.Replace(ctx.book);
        return "replace";
    }

    static void Init() {

        engine.AddTransition("q15", "select", "q16");
        engine.AddTransition("q16", "due", "q17");
        engine.AddTransition("q17", "overdue", "q18");
        engine.AddTransition("q18", "pay", "q19");
        engine.AddTransition("q17", "ekstra", "q20");
        engine.AddTransition("q16", "lost", "q21");
        engine.AddTransition("q21", "replace", "q20");
        engine.AddTransition("q20", "next", "q15");

        engine.AddFunction("q15", q15);
        engine.AddFunction("q16", q16);
        engine.AddFunction("q17", q17);
        engine.AddFunction("q18", q18);
        engine.AddFunction("q19", q19);
        engine.AddFunction("q20", q20);
        engine.AddFunction("q21", q21);

    }
    static void Renew() {
        engine.EngineStart();
    }
};