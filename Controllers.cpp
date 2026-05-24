#include "./Library.cpp"

MemberVerificationService LibraryDFALogin::memSer;
DFAEngine LibraryDFALogin::engine = DFAEngine("q0", "q6", "q10");
LibraryDFALoginContext LibraryDFALogin::ctx;

BookService LibraryDFABorrow::bookSer;
DFAEngine LibraryDFABorrow::engine = DFAEngine("q8", "q12", "q13");
LibraryDFABorrowContext LibraryDFABorrow::ctx;

DFAEngine LibraryDFARenewal::engine = DFAEngine("q15", "q19", "");
LibraryDFARenewContext LibraryDFARenewal::ctx;

struct LoginController {

    void Login() {
        LibraryDFALogin::Init();

        LibraryDFALogin::Login();
    }
};
struct BookController {
    void Borrow() {
        LibraryDFABorrow::Init();

        LibraryDFABorrow::Borrow();
    }
};
struct RenewalController {
    void Renew() {
        LibraryDFARenewal::Init();

        LibraryDFARenewal::Renew();
    }
};


int main() {
    LoginController login;
    login.Login();

    BookController book;
    book.Borrow();

    std::cout << "\nProgram bitti. Cikis yapmak icin Enter'a basin...";
    std::cin.get(); 
    return 0;

    return 0;
}