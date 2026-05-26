#include "./Library.cpp"

MemberVerificationService LibraryDFALogin::memSer;
DFAEngine LibraryDFALogin::engine = DFAEngine("q0", "q6", "q10");
LibraryDFALoginContext LibraryDFALogin::ctx;

BookService LibraryDFABorrow::bookSer;
DFAEngine LibraryDFABorrow::engine = DFAEngine("q11", "q17", "q13");
LibraryDFABorrowContext LibraryDFABorrow::ctx;


BookService LibraryDFARenewal::bookSer;
DFAEngine LibraryDFARenewal::engine = DFAEngine("q18", "q23", "q26");
LibraryDFARenewContext LibraryDFARenewal::ctx;

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




int main() {

    BookService globalBookService;

    // 🚨 2. ADIM: İKİ OTOMATA DA BU ORTAK SERVİSİN ADRESİNİ (&) VERİYORUZ
    LibraryDFABorrow::bookSer = globalBookService;
    LibraryDFARenewal::bookSer = globalBookService;

    // Otomat geçiş haritalarını ayağa kaldırıyoruz
    LibraryDFALogin::Init();
    LibraryDFABorrow::Init();
    LibraryDFARenewal::Init();

    // 1. ADIM: ZORUNLU GİRİŞ SİSTEMİ
    // Kullanıcı giriş yapmadan ana menüyü göremez.
    LoginController login;
    BookController book;
    RenewalController renew;

    std::cout << "[SISTEM]: Otomat baslatiliyor, lutfen giris yapiniz...\n";
    login.Login();

    // NOT: Giriş otomatının q6 (ACCEPT) durumuna ulaştığından emin ol kanka.
    // Eğer şifreyi 3 kere yanlış girip q10 (TRAP) durumunda bittiyse, 
    // main'de direkt return 0 çekip programı kapatacak bir kontrol koyabilirsin.


    bool isRunning = true;

    // 2. ADIM: ANA MENÜ DÖNGÜSÜ (INFINITE LOOP UNTIL EXIT)
    while (isRunning) {
        std::cout << "\n============================================\n";
        std::cout << "                ANA MENU                    \n";
        std::cout << "============================================\n";
        std::cout << " [1] -> Kitap Odunc Alma Islemleri (Borrow)\n";
        std::cout << " [2] -> Kitap Sure Uzatma Islemleri (Renew)\n";
        std::cout << " [3] -> Sistemden Guvenli Cikis (Exit)\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Lutfen yapmak istediginiz islemi secin: ";

        int choice;
        std::cin >> choice;

        // input buffer'da kalan newline karakterlerini temizlemek için
        std::cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            std::cout << "\n[SISTEM]: Kitap Odunc Alma Otomati tetiklendi...\n";
            book.Borrow(); // Bizim az önce yazdığımız q8-q14 arası çalışan otomat
            break;

        case 2:
            std::cout << "\n[SISTEM]: Sure Uzatma (Renewal) Otomati tetiklendi...\n";
            // Kanka senin Renew fonksiyonunun adı neyse (örn: book.Renew() veya ne olarak açtıysan) 
            // onu buraya çağıracaksın:
            renew.Renew(); // q18-q24 arası çalışan otomat
            break;

        case 3:
            std::cout << "\n[SISTEM]: Oturum kapatiliyor. Guvenli cikis yapildi. Iyi gunler!\n";
            isRunning = false; // Döngüyü kırar ve programı bitirir
            break;

        default:
            std::cout << "\n[HATA]: Gecersiz bir secim yaptiniz! Lutfen 1, 2 veya 3 yazin.\n";
            break;
        }
    }

    std::cout << "\nProgram bitti. Kapatmak icin Enter'a basin...";
    std::cin.get();
    return 0;
}