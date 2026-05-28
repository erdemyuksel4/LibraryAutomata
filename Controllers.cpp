#include "./Library.cpp"

MemberVerificationService LibraryDFALogin::memSer;
DFAEngine LibraryDFALogin::engine = DFAEngine("q0", "q6", "q10");
LibraryDFALoginContext LibraryDFALogin::ctx;
TestLoginContext LibraryDFALogin::testCtx;

BookService LibraryDFABorrow::bookSer;
DFAEngine LibraryDFABorrow::engine = DFAEngine("q11", "q17", "q13");
LibraryDFABorrowContext LibraryDFABorrow::ctx;
TestBorrowContext LibraryDFABorrow::testCtx;


BookService LibraryDFARenewal::bookSer;
DFAEngine LibraryDFARenewal::engine = DFAEngine("q18", "q23", "q26");
LibraryDFARenewContext LibraryDFARenewal::ctx;
RenewTestContext LibraryDFARenewal::testCtx;

bool TEST_MODE=false;

struct LoginController {

    void Login() {


        LibraryDFALogin::Login();
    }
};
struct BookController {
    void Borrow() {


        LibraryDFABorrow::Borrow();
    }
};
struct RenewalController {
    void Renew() {


        LibraryDFARenewal::Renew();
    }
};
struct Tester{
    static void WaitForUser(const char* scenarioName) {
        
        std::cout << "\n======================================================";
        std::cout << "\n>>> READY FOR: " << scenarioName;
        std::cout << "\n>>> Press Enter to execute this scenario...";
        std::cout << "\n======================================================\n";
        std::cin.get(); // Kullanıcının Enter'a basmasını bekler kanka
    }
    static void Test_Scenario1() {
        WaitForUser("Scenario 1: Seamless Active Member Login");
        std::cout << "\n------------------------------------------------------\n";
        std::cout << "[RUNNING]: Scenario 1: Seamless Active Member Login\n";
        TEST_MODE = true;
        LibraryDFALogin::testCtx.id = 101;         // Aktif ve kayıtlı kullanıcı
        LibraryDFALogin::testCtx.pass = "pass101"; // Doğru şifre

        LibraryDFALogin::Login();
        TEST_MODE = false;
    }

    static void Test_Scenario2() {
        WaitForUser("Scenario 2: Direct Book Borrowing");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 2: Direct Book Borrowing [cite: 5]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        // Değerleri sırasıyla kuyruğa ekliyoruz kanka
        LibraryDFABorrow::testCtx.searchQueue.Add("1984");      // q11 için 1. arama [cite: 6]
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");  // q17 için bitirme komutu [cite: 6]

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }

    static void Test_Scenario3() {
        WaitForUser("Scenario 3: Reserving an Unavailable Book");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 3: Reserving an Unavailable Book [cite: 7]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("Dune");      // q11 için arama (Mevcut olmayan kitap) [cite: 8, 10]
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");  // q17 için bitirme komutu [cite: 10]

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }

    static void Test_Scenario4() {
        WaitForUser("Scenario 4: Sequential Borrowing Loop (Two Books)");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 4: Sequential Borrowing (Loop Test) [cite: 11]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("The Lord of the Rings");
        LibraryDFABorrow::testCtx.decisionQueue.Add("next");

        LibraryDFABorrow::testCtx.searchQueue.Add("Nutuk");
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }

    static void Test_Scenario5() {
        WaitForUser("Scenario 5: Sequential Reservation Loop (Two Books)");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 5: Sequential Reservation (Loop Test) [cite: 14]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("Hamlet");
        LibraryDFABorrow::testCtx.decisionQueue.Add("next");  

        LibraryDFABorrow::testCtx.searchQueue.Add("Macbeth");
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }

    static void Test_Scenario6() {
        WaitForUser("Scenario 6: Reserving After Borrowing (Combined Loop)");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 6: Reserving After Borrowing (Combined Loop) [cite: 16]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("To Kill a Mockingbird");    // Önce ödünç al [cite: 16]
        LibraryDFABorrow::testCtx.decisionQueue.Add("next");  // Döngüye devam [cite: 16]

        LibraryDFABorrow::testCtx.searchQueue.Add("Odyssey");    // Sonra rezerve et [cite: 16]
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");// Bitir [cite: 16]

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }

    static void Test_Scenario7() {
        WaitForUser("Scenario 7: Clearing Liability via Overdue Penalty Payment");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 7: Clearing Liability via Overdue Penalty Payment [cite: 18]\n";
        TEST_MODE = true;
        LibraryDFARenewal::testCtx.searchQueue.Clear();
        LibraryDFARenewal::testCtx.decisionQueue.Clear();

        LibraryDFARenewal::testCtx.searchQueue.Add("Crime and Punishment"); // q15 araması [cite: 18]
        LibraryDFARenewal::testCtx.decisionQueue.Add("due");            // q16 cezayı ödeme sinyali [cite: 19, 20]
        LibraryDFARenewal::testCtx.decisionQueue.Add("ACC");            // q16 cezayı ödeme sinyali [cite: 19, 20]

        LibraryDFARenewal::Renew();
        TEST_MODE = false;
    }

    static void Test_Scenario8() {
        WaitForUser("Scenario 8: Clearing Liability via Lost Book Replacement");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 8: Clearing Liability via Lost Book Replacement [cite: 21]\n";
        TEST_MODE = true;
        LibraryDFARenewal::testCtx.searchQueue.Clear();
        LibraryDFARenewal::testCtx.decisionQueue.Clear();

        LibraryDFARenewal::testCtx.searchQueue.Add("The Hobbit"); // q15 araması [cite: 21]
        LibraryDFARenewal::testCtx.decisionQueue.Add("lost");     // q16 kitabı yenisiyle değiştirme [cite: 22, 23]
        LibraryDFARenewal::testCtx.decisionQueue.Add("ACC");     // q16 kitabı yenisiyle değiştirme [cite: 22, 23]

        LibraryDFARenewal::Renew();
        TEST_MODE = false;
    }

    static void Test_Scenario9() {
        WaitForUser("Scenario 9: Reporting a Lost Book After Penalty Payment (Loop)");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 9: Lost Book After Penalty Payment (Loop) [cite: 24]\n";
        TEST_MODE = true;
        LibraryDFARenewal::testCtx.searchQueue.Clear();
        LibraryDFARenewal::testCtx.decisionQueue.Clear();

        // Gecikme cezasından sonra döngüyle kayıp bildirimi [cite: 24, 25]
        LibraryDFARenewal::testCtx.searchQueue.Add("Les Miserables");
        LibraryDFARenewal::testCtx.decisionQueue.Add("due");
        LibraryDFARenewal::testCtx.decisionQueue.Add("next");

        LibraryDFARenewal::testCtx.searchQueue.Add("The Catcher in the Rye");
        LibraryDFARenewal::testCtx.decisionQueue.Add("lost");
        LibraryDFARenewal::testCtx.decisionQueue.Add("ACC");


        LibraryDFARenewal::Renew();
        TEST_MODE = false;
    }
    static void Test_Scenario10() {
        WaitForUser("Scenario 10: Paying an Overdue Penalty After Lost Report (Loop)");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Scenario 10: Penalty After Lost Report (Loop) [cite: 25]\n";
        TEST_MODE = true;
        LibraryDFARenewal::testCtx.searchQueue.Clear();
        LibraryDFARenewal::testCtx.decisionQueue.Clear();

        // Önce kayıp bildirimi, döngüyle sonra gecikme cezası ödeme [cite: 25, 26]
        LibraryDFARenewal::testCtx.searchQueue.Add("Faust");
        LibraryDFARenewal::testCtx.decisionQueue.Add("lost");
        LibraryDFARenewal::testCtx.decisionQueue.Add("next");

        LibraryDFARenewal::testCtx.searchQueue.Add("Brave New World");
        LibraryDFARenewal::testCtx.decisionQueue.Add("due");
        LibraryDFARenewal::testCtx.decisionQueue.Add("ACC");

        LibraryDFARenewal::Renew();
        TEST_MODE = false;
    }


    static void Test_Error1() {
        WaitForUser("Error Scenario 1: Login Attempt with an Inactive Account");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Error Scenario 1: Login Attempt with an Inactive Account [cite: 28]\n";
        TEST_MODE = true;
        LibraryDFALogin::testCtx.id = 104; // Veritabanında pasif/askıda olan üye ID'si [cite: 29]
        LibraryDFALogin::testCtx.pass = "pass104";
        LibraryDFALogin::Login();
        TEST_MODE = false;
    }
    static void Test_Error2() {
        WaitForUser("Error Scenario 2: Three Consecutive Incorrect Passwords");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Error Scenario 2: Three Consecutive Incorrect Passwords [cite: 32]\n";
        TEST_MODE = true;
        LibraryDFALogin::testCtx.id = 101;
        LibraryDFALogin::testCtx.pass = "wrong_password"; // Üst üste 3 kez yanlış sinyal tetikler [cite: 32, 33]
        LibraryDFALogin::Login();
        TEST_MODE = false;
    }
    static void Test_Error3() {
        WaitForUser("Error Scenario 3: Searching for a Book Not in the Catalog");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Error Scenario 3: Searching for a Book Not in the Catalog [cite: 35]\n";
        TEST_MODE = true;
        LibraryDFALogin::testCtx.id = 101;
        LibraryDFALogin::testCtx.pass = "pass101";
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.searchQueue.Add("Unknown_Random_Book");
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");
        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }
    static void Test_Error4() {
        WaitForUser("Error Scenario 4: Invalid Search After a Borrowing Operation");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Error Scenario 4: Invalid Search After Borrowing [cite: 38]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("The Divine Comedy");    // 1. Başarılı arama [cite: 38]
        LibraryDFABorrow::testCtx.decisionQueue.Add("next");  // Oturumu kapatma, devam et [cite: 39]
        LibraryDFABorrow::testCtx.searchQueue.Add("Unknown_Random_Book"); // 2. Hatalı arama! [cite: 39]
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }
    static void Test_Error5() {
        WaitForUser("Error Scenario 5: Invalid Search After a Reservation Operation");
        std::cout << "\n------------------------------------------------------\n";

        std::cout << "\n RUNNING: Error Scenario 5: Invalid Search After Reservation [cite: 42]\n";
        TEST_MODE = true;
        LibraryDFABorrow::testCtx.searchQueue.Clear();
        LibraryDFABorrow::testCtx.decisionQueue.Clear();

        LibraryDFABorrow::testCtx.searchQueue.Add("Ulysses");    // 1. Başarılı rezervasyon [cite: 42]
        LibraryDFABorrow::testCtx.decisionQueue.Add("next");  // Devam et [cite: 43]
        LibraryDFABorrow::testCtx.searchQueue.Add("Unknown_Random_Book"); // 2. Hatalı arama! [cite: 43]
        LibraryDFABorrow::testCtx.decisionQueue.Add("ACC");

        LibraryDFABorrow::Borrow();
        TEST_MODE = false;
    }
    static void RunAllTests() {
        std::cout << "\n======================================================\n";
        std::cout << "    STARTING AUTOMATED QUEUE-BASED DFA TEST SUITE     \n";
        std::cout << "======================================================\n";

        Test_Scenario1();  
        Test_Scenario2();  
        Test_Scenario3();
        Test_Scenario4();  
        Test_Scenario5();  
        Test_Scenario6();
        Test_Scenario7();  
        Test_Scenario8();  
        Test_Scenario9();
        Test_Scenario10(); 
        Test_Error1();     
        Test_Error2();
        Test_Error3();     
        Test_Error4();     
        Test_Error5();

        std::cout << "\n======================================================\n";
        std::cout << "    ALL 15 DFA SCENARIOS EXECUTED SUCCESSFULLY        \n";
        std::cout << "======================================================\n";
    }
};


int main() {

    BookService globalBookService;

    // Injecting the global database service reference into the automata layers
    LibraryDFABorrow::bookSer = globalBookService;
    LibraryDFARenewal::bookSer = globalBookService;

    // Initializing state charts and map vectors
    LibraryDFALogin::Init();
    LibraryDFABorrow::Init();
    LibraryDFARenewal::Init();

    LoginController login;
    BookController book;
    RenewalController renew;

    // ============================================================================
    // 🌟 INITIAL RUNTIME CONFIGURATION SELECTOR
    // ============================================================================
    std::cout << "======================================================\n";
    std::cout << "       LIBRARY FORMAL AUTOMATON VERIFICATION          \n";
    std::cout << "======================================================\n";
    std::cout << " [1] -> Run All Automated Test Pools (Queue-Driven)\n";
    std::cout << " [2] -> Launch Standard Interactive Mode (User Login Portal)\n";
    std::cout << "------------------------------------------------------\n";
    std::cout << "Please select your execution environment: ";

    int startChoice;
    std::cin >> startChoice;
    std::cin.ignore(1000, '\n'); // clear buffer

    if (startChoice == 1) {
        // Runs all 15 scenarios automatically and directly completes the execution
        Tester::RunAllTests();

        std::cout << "\n[SYSTEM]: Test suite completed. Press Enter to terminate process...";
        std::cin.get();
        return 0;
    }

    // ============================================================================
    // ROUTINE DEPLOYMENT: COMPULSORY ACCOUNT ACCESS LAYER
    // ============================================================================
    std::cout << "\n[SYSTEM]: Production environment selected. Initializing credentials challenge...\n";
    login.Login();

    bool isRunning = true;

    // INTERACTIVE RUNTIME SHELL (INFINITE LOOP UNTIL DISCONNECT ACTION)
    while (isRunning) {
        std::cout << "\n============================================\n";
        std::cout << "                MAIN MENU                   \n";
        std::cout << "============================================\n";
        std::cout << " [1] -> Check Out / Borrow Book Transactions (Borrow Automaton)\n";
        std::cout << " [2] -> Period Extension / Fine Transactions (Renew Automaton)\n";
        std::cout << " [3] -> Run Diagnostics & Test Protocols (DFA Verification Suite)\n";
        std::cout << " [4] -> Secure Session Termination (Exit Portal)\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Please input your command: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(1000, '\n'); // buffer flush

        switch (choice) {
        case 1:
            std::cout << "\n[SYSTEM]: Triggering Book Borrowing Automaton process...\n";
            book.Borrow(); // Handles standard interactive loop (awaits standard console reads)
            break;

        case 2:
            std::cout << "\n[SYSTEM]: Triggering Period Renewal Automaton process...\n";
            renew.Renew(); // Handles standard interactive loop (awaits standard console reads)
            break;

        case 3:
            // Allows re-triggering the test suite from inside the main terminal interface
            Tester::RunAllTests();
            break;

        case 4:
            std::cout << "\n[SYSTEM]: Closing session handles. Safe checkout completed. Have a nice day!\n";
            isRunning = false;
            break;

        default:
            std::cout << "\n[ERROR]: Unrecognized token value. Please input a valid selection code.\n";
            break;
        }
    }
    std::cout << "\nProcess complete. Press Enter to close application...";

    // 🌟 Buffer'ı tamamen kazıyoruz ki hafızada artık kalmasın
    std::cin.clear();
    std::cin.ignore(1000, '\n');

    std::cin.get(); // Şimdi temiz temiz Enter bekleyebilir

}