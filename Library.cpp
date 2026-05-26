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
        std::cout << "\n[STATE]: q0 (Login Screen)\n";
        std::cout << "Enter User ID: ";
        std::cin >> ctx.id;

        std::cout << "--> [OUTPUT]: \"id\" triggered. Transitioning to q1.\n";
        return "id";
    }

    static const char* q1() {
        std::cout << "\n[STATE]: q1 (User Verification)\n";
        bool realUser = memSer.haveUser(ctx.id);
        if (!realUser) {
            std::cout << "--> [OUTPUT]: \"wrong\" (User Not Found). Transitioning to q10 (ERR).\n";
            return "wrong";
        }
        std::cout << "--> [OUTPUT]: \"user\" (User Exists). Transitioning to q2.\n";
        return "user";
    }

    static const char* q2() {
        std::cout << "\n[STATE]: q2 (Password Input - Attempt 1)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" submitted. Verification (q3) will be processed.\n";
        return "pw";
    }

    static const char* q3() {
        std::cout << "\n[STATE]: q3 (Password Verification - Attempt 1)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Password Correct). Transitioning to membership check (q5).\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (Incorrect Password!). Transitioning to Attempt 2 (q4).\n";
        return "invalid";
    }

    static const char* q4() {
        std::cout << "\n[STATE]: q4 (Password Input - Attempt 2)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" submitted. Verification (q7) will be processed.\n";
        return "pw";
    }

    static const char* q5() {
        std::cout << "\n[STATE]: q5 (Membership Status Check)\n";
        bool active = memSer.isActiveUser();
        if (active) {
            std::cout << "--> [OUTPUT]: \"active\" (Account Active). Transitioning to q6 (ACC).\n";
            return "active";
        }
        std::cout << "--> [OUTPUT]: \"inactive\" (Account Suspended!). Transitioning to q10 (ERR).\n";
        return "inactive";
    }

    static const char* q6() {
        std::cout << "\n[STATE]: q6 (ACCEPT STATE - LOGIN SUCCESSFUL)\n";
        return "ACC";
    }

    static const char* q7() {
        std::cout << "\n[STATE]: q7 (Password Verification - Attempt 2)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Password Correct). Transitioning to membership check (q5).\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (Incorrect Password!). Transitioning to Final Attempt (q8).\n";
        return "invalid";
    }

    static const char* q8() {
        std::cout << "\n[STATE]: q8 (Password Input - Final Attempt)\n";
        std::cout << "Password: ";
        std::cin >> ctx.pass;

        std::cout << "--> [OUTPUT]: \"pw\" submitted. Final verification (q9) will be processed.\n";
        return "pw";
    }

    static const char* q9() {
        std::cout << "\n[STATE]: q9 (Password Verification - Final Attempt)\n";
        bool verified = memSer.verifyUser(ctx.pass);
        if (verified) {
            std::cout << "--> [OUTPUT]: \"valid\" (Password Correct). Transitioning to membership check (q5).\n";
            return "valid";
        }
        std::cout << "--> [OUTPUT]: \"invalid\" (No Attempts Remaining!). Transitioning to q10 (ERR).\n";
        return "invalid";
    }

    static const char* q10() {
        std::cout << "\n[STATE]: q10 (TRAP / ERROR STATE - ACCESS DENIED)\n";
        return "ERR";
    }

    static void Init() {
        engine.AddTransition("q0", "id", "q1");
        engine.AddTransition("q1", "user", "q2");
        engine.AddTransition("q2", "pw", "q3");
        engine.AddTransition("q3", "valid", "q5");
        engine.AddTransition("q3", "invalid", "q4");
        engine.AddTransition("q5", "active", "q6");
        engine.AddTransition("q4", "pw", "q7");
        engine.AddTransition("q7", "invalid", "q8");
        engine.AddTransition("q7", "valid", "q5");
        engine.AddTransition("q8", "pw", "q9");
        engine.AddTransition("q9", "valid", "q5");
        engine.AddTransition("q9", "invalid", "q10");
        engine.AddTransition("q1", "wrong", "q10");
        engine.AddTransition("q5", "inactive", "q10");

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

    static void Login() {
        engine.EngineStart();
    }
};
struct LibraryDFABorrowContext {
    const char* search;
    Book* book;
};

struct LibraryDFABorrow {
    static LibraryDFABorrowContext ctx;
    static BookService bookSer; // 🚨 Shared service için pointer yapıldı kanka!
    static DFAEngine engine;

    static const char* q11() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q11 (Book Search Screen)\n";
        std::cout << "============================================\n";

        char searchBuffer[100];
        std::cout << "Enter Book Title / ISBN: ";
        std::cin >> searchBuffer;
        ctx.search = searchBuffer;

        std::cout << "--> [OUTPUT]: \"search\" triggered. Moving to Database Search Layer (q12).\n";
        return "search";
    }

    static const char* q12() {
        std::cout << "\n[STATE]: q12 (Database Search Layer)\n";
        std::cout << "--> Searching for \"" << ctx.search << "\" in library repository...\n";

        // . yerine -> kullanıyoruz
        Book* found = bookSer.Search(ctx.search);
        if (found != nullptr) {
            ctx.book = found;
            std::cout << "--> [SUCCESS]: Book found! -> " << found->title << "\n";
            std::cout << "--> [OUTPUT]: \"found\" triggered. Moving to Availability Check (q14).\n";
            return "found";
        }

        std::cout << "--> [ERROR]: Book NOT found in library inventory!\n";
        std::cout << "--> [OUTPUT]: \"notfound\" triggered. Transitioning to Trap State (q13).\n";
        return "notfound";
    }

    static const char* q14() {
        std::cout << "\n[STATE]: q14 (Availability Check Layer)\n";
        std::cout << "--> Querying loan/reservation status for \"" << ctx.book->title << "\"...\n";

        // . yerine -> kullanıyoruz
        bool available = bookSer.isAvailable(ctx.book);
        if (available) {
            std::cout << "--> [STATUS]: Book is on the shelf and available.\n";
            std::cout << "--> [OUTPUT]: \"available\" triggered. Proceeding to Checkout (q16).\n";
            return "available";
        }

        std::cout << "--> [STATUS]: Book is currently borrowed or reserved by another member!\n";
        std::cout << "--> [OUTPUT]: \"unavailable\" triggered. Transitioning to Reservation Screen (q15).\n";
        return "unavailable";
    }

    static const char* q16() {
        std::cout << "\n[STATE]: q16 (Checkout Action Layer)\n";
        std::cout << "--> Processing Member ID: " << LibraryDFALogin::ctx.id << "\n";

        // . yerine -> kullanıyoruz
        bookSer.Borrow(ctx.book, LibraryDFALogin::ctx.id);

        std::cout << "--> [SUCCESS]: Book has been successfully checked out to the member!\n";
        std::cout << "--> [OUTPUT]: \"borrow\" triggered. Moving to Decision Screen (q17).\n";
        return "borrow";
    }

    static const char* q15() {
        std::cout << "\n[STATE]: q15 (Reservation Action Layer)\n";
        std::cout << "--> Creating a reservation queue for Member ID: " << LibraryDFALogin::ctx.id << "...\n";

        // . yerine -> kullanıyoruz
        bookSer.Reserve(ctx.book, LibraryDFALogin::ctx.id);

        std::cout << "--> [SUCCESS]: Reservation record created. You will be notified when the book is returned.\n";
        std::cout << "--> [OUTPUT]: \"reserve\" triggered. Moving to Decision Screen (q17).\n";
        return "reserve";
    }

    static const char* q17() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q17 (Decision / Session Screen)\n";
        std::cout << "============================================\n";

        char con[20];
        std::cout << "Type [Search] to search again, or [Accept] to finish: ";
        std::cin >> con;

        if (Compare<const char*>(con, "Accept")) {
            std::cout << "--> [OUTPUT]: \"ACC\" triggered. Automaton terminating SUCCESSFULLY.\n";
            return "ACC";
        }

        std::cout << "--> [OUTPUT]: \"next\" triggered. Returning to Book Search (q11).\n";
        return "next";
    }

    static const char* q13() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q13 (TRAP / ERROR STATE - TRANSACTION FAILED)\n";
        std::cout << "============================================\n";
        std::cout << "--> [ERROR]: Operation aborted because the requested book was not found.\n";
        std::cout << "--> [OUTPUT]: \"ERR\" triggered. Automaton terminated with errors.\n";
        return "ERR";
    }

    static void Init() {
        engine.AddTransition("q11", "search", "q12");
        engine.AddTransition("q12", "found", "q14");
        engine.AddTransition("q12", "notfound", "q13");
        engine.AddTransition("q14", "available", "q16");
        engine.AddTransition("q14", "unavailable", "q15");
        engine.AddTransition("q16", "borrow", "q17");
        engine.AddTransition("q17", "next", "q11");
        engine.AddTransition("q15", "reserve", "q17");

        engine.AddFunction("q11", q11);
        engine.AddFunction("q12", q12);
        engine.AddFunction("q13", q13);
        engine.AddFunction("q14", q14);
        engine.AddFunction("q15", q15);
        engine.AddFunction("q16", q16);
        engine.AddFunction("q17", q17);
    }

    static void Borrow() {
        engine.EngineStart();
    }
};
// ============================================================================
// 3. RENEWAL AUTOMATON
// ============================================================================
struct LibraryDFARenewContext {
    const char* search;
    Book* book;
    int late;
};

struct LibraryDFARenewal {
    static BookService bookSer;
    static DFAEngine engine;
    static LibraryDFARenewContext ctx;

    static const char* q18() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q18 (Renewal - Book Selection Screen)\n";
        std::cout << "============================================\n";

        char searchBuffer[100];
        std::cout << "Enter Book Title / ISBN to Renew: ";
        std::cin >> searchBuffer;
        ctx.search = searchBuffer;

        std::cout << "--> [OUTPUT]: \"select\" triggered. Moving to Database Search Query (q19).\n";
        return "select";
    }

    static const char* q19() {
        std::cout << "\n[STATE]: q19 (Book Search Query)\n";
        std::cout << "--> Verifying \"" << ctx.search << "\" in inventory...\n";

        ctx.book = bookSer.Search(ctx.search);

        if (ctx.book != nullptr) {
            std::cout << "--> [SUCCESS]: Book found! -> " << ctx.book->title << "\n";
            std::cout << "--> [OUTPUT]: \"found\" triggered. Proceeding to Condition Assessment (q20).\n";
            return "found";
        }

        // 🚨 Artik q18'e dönmüyor, q26 Error State'e tetikleniyor kanka!
        std::cout << "--> [ERROR]: The specified book could not be found!\n";
        std::cout << "--> [OUTPUT]: \"notfound\" triggered. Falling into Trap State (q26).\n";
        return "notfound";
    }

    static const char* q20() {
        std::cout << "\n[STATE]: q20 (Condition Assessment Layer)\n";
        std::cout << "--> Is \"" << ctx.book->title << "\" physically intact or reported lost?\n";

        char con[20];
        std::cout << "Select Status [Due] (Return/Renew) or [Lost] (Report Lost): ";
        std::cin >> con;

        if (Compare<const char*>(con, "Lost")) {
            std::cout << "--> [STATUS]: Book marked as lost.\n";
            std::cout << "--> [OUTPUT]: \"lost\" triggered. Proceeding to Replacement Billing (q25).\n";
            return "lost";
        }

        std::cout << "--> [STATUS]: Book is intact. Proceeding to overdue check.\n";
        std::cout << "--> [OUTPUT]: \"due\" triggered. Advancing to Overdue Assessment (q21).\n";
        return "due";
    }

    static const char* q21() {
        std::cout << "\n[STATE]: q21 (Overdue & Fine Assessment Layer)\n";
        std::cout << "--> Checking overdue logs for Member ID: " << LibraryDFALogin::ctx.id << "...\n";

        int late = bookSer.isOverDue(ctx.book, LibraryDFALogin::ctx.id);
        ctx.late = late;

        if (late > 0) {
            std::cout << "--> [WARNING]: ATTENTION! The return date has been exceeded by " << late << " seconds.\n";
            std::cout << "--> [OUTPUT]: \"overdue\" triggered. Proceeding to Fine Collection (q22).\n";
            return "overdue";
        }

        std::cout << "--> [STATUS]: Book returned on time. No outstanding fines.\n";
        std::cout << "--> [OUTPUT]: \"renew\" triggered. Advancing directly to Extension Process (q24).\n";
        return "renew";
    }

    static const char* q22() {
        std::cout << "\n[STATE]: q22 (Fine Collection Layer)\n";
        std::cout << "--> Total Overdue Period: " << ctx.late << " units.\n";
        std::cout << "--> Processing fine payments and lifting system blocks...\n";

        bookSer.RemoveLateReserve(ctx.book, ctx.late);

        std::cout << "--> [SUCCESS]: Fine paid successfully. Penalties cleared!\n";
        std::cout << "--> [OUTPUT]: \"pay\" triggered. Moving to Confirmation Screen (q23).\n";
        return "pay";
    }

    static const char* q24() {
        std::cout << "\n[STATE]: q24 (Extension Action Layer)\n";
        std::cout << "--> Extending the due date of \"" << ctx.book->title << "\" by 14 days...\n";

        bookSer.renew(ctx.book, LibraryDFALogin::ctx.id);

        std::cout << "--> [SUCCESS]: Book return period successfully renewed!\n";
        std::cout << "--> [OUTPUT]: \"renewed\" triggered. Moving to Confirmation Screen (q23).\n";
        return "renewed";
    }

    static const char* q25() {
        std::cout << "\n[STATE]: q25 (Lost Book Billing Layer)\n";
        std::cout << "--> Removing \"" << ctx.book->title << "\" from inventory and charging fees to member account...\n";

        bookSer.Replace(ctx.book);

        std::cout << "--> [SUCCESS]: Book replacement fee collected. Inventory logs updated.\n";
        std::cout << "--> [OUTPUT]: \"replace\" triggered. Moving to Confirmation Screen (q23).\n";
        return "replace";
    }

    static const char* q23() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q23 (End of Process / Confirmation Screen)\n";
        std::cout << "============================================\n";

        char con[20];
        std::cout << "Type [Next] for another action, or [Accept] to quit: ";
        std::cin >> con;

        if (Compare<const char*>(con, "Accept")) {
            std::cout << "--> [OUTPUT]: \"ACC\" triggered. Renewal automaton completed SUCCESSFULLY.\n";
            return "ACC";
        }

        std::cout << "--> [OUTPUT]: \"next\" triggered. Returning to Book Selection (q18).\n";
        return "next";
    }

    // 🚨 YENI EKLENEN TRAP STATE (HATA ÇIKIŞI)
    static const char* q26() {
        std::cout << "\n============================================\n";
        std::cout << "[STATE]: q26 (TRAP / ERROR STATE - RENEWAL FAILED)\n";
        std::cout << "============================================\n";
        std::cout << "--> [ERROR]: Operation aborted because the requested book does not exist.\n";
        std::cout << "--> [OUTPUT]: \"ERR\" triggered. Automaton terminated with errors.\n";
        return "ERR";
    }

    static void Init() {
        engine.AddTransition("q18", "select", "q19");

        // 🚨 Değişen Geçiş Ayarı: notfound artık q26'ya gidiyor
        engine.AddTransition("q19", "found", "q20");
        engine.AddTransition("q19", "notfound", "q26");

        engine.AddTransition("q20", "due", "q21");
        engine.AddTransition("q21", "overdue", "q22");
        engine.AddTransition("q22", "pay", "q23");

        engine.AddTransition("q21", "renew", "q24");
        engine.AddTransition("q24", "renewed", "q23");

        engine.AddTransition("q20", "lost", "q25");
        engine.AddTransition("q25", "replace", "q23");
        engine.AddTransition("q23", "next", "q18");

        // Fonksiyon bağlamaları
        engine.AddFunction("q18", q18);
        engine.AddFunction("q19", q19);
        engine.AddFunction("q20", q20);
        engine.AddFunction("q21", q21);
        engine.AddFunction("q22", q22);
        engine.AddFunction("q23", q23);
        engine.AddFunction("q24", q24);
        engine.AddFunction("q25", q25);
        engine.AddFunction("q26", q26); // 🚨 Yeni fonksiyon motora bağlandı
    }

    static void Renew() {
        engine.EngineStart();
    }
};