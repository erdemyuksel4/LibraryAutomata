#pragma once
#include "collection.h"
struct MemberUser {
    int id;
    const char* pw;
    bool activeMembership = false;
    MemberUser(int uid, const char* psw, bool active) :id(uid), pw(psw), activeMembership(active) {

    }
};
struct ReserveBook {
    int userId;
    ReserveBook(int uid) :userId(uid) {}
};
struct Book {
    const char* title;
    const char* isbn;
    bool isAvailable=true;
    Map<int, int> reserves;
    Book(const char* name,const char* id):title(name),isbn(id){

    }
};

struct BorrowedBook {
    int userId;
    const char* isbn;
    int borrowTimeIndex;   
    bool isReturned;
};
