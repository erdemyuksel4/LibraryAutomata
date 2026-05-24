#pragma once
template<typename T>
bool Compare(T a, T b) {
    return a == b;
}
template<>
inline bool Compare<const char*>(const char* a, const char* b) {
    int i = 0;
    for (;a[i] != '\0' && b[i] != '\0';i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    if (a[i] != b[i]) {
        return false;
    }
    return true;
};
inline const char* substring(const char* txt, int start, int finish) {
    int length = finish - start;
    char* n = new char[length + 1];
    for (int i = 0;start + i < finish;i++) {
        n[i] = txt[start + i];
    }
    n[length] = '\0';
    return n;
}

inline bool StartsWith(const char* txt, const char* check) {
    
    for (int i = 0;check[i] != '\0';i++) {
        if (txt[i] != check[i]) {
            return false;
        }
    }

    return true;
}
template<typename T>
struct List {
    int capacity;
    int length = 0;

    T* list;
    List() {
        capacity = 2;
        list = new T[capacity];
    }
    void Add(T val) {
        isCapacityEnough();
        list[length] = val;
        length++;
    }
    T& operator[](int index) {
        return list[index];
    }
    template<typename F>
    T FindFirst(F expr) {
        for (int i = 0;i < length;i++) {
            if (expr(list[i])) {
                return list[i];
            }
        }
        return nullptr;
    }

    void isCapacityEnough() {

        if (length >= capacity) {
            capacity *= 2;
            T* nlist = new T[capacity];
            for (int i = 0;i < length;i++) {
                nlist[i] = list[i];
            }
            delete[] list;
            list = nlist;
        }
    }
    ~List() {
        delete[] list;
    }
};

template<typename T, typename P>
struct KeyValuePair {
    T key;
    P value;
    KeyValuePair(T k, P v) :key(k), value(v) {

    }
};

template<typename T, typename P>
struct Map {
    List<KeyValuePair<T, P>*> pairs;
    int& Count;
    Map() : Count(pairs.length) {}
    void Add(T key, P val) {
        pairs.Add(new KeyValuePair<T, P>(key, val));
    }
    bool ContainsKey(T key) {
        auto* found = pairs.FindFirst([ key](auto* x) {
            return Compare<T>(x->key, key);
            });        
        return found != nullptr;
    }
    P& operator[](T key) {
        for (int i = 0;i < pairs.length;i++) {
            if (Compare<T>(pairs[i]->key, key)) {
                return pairs[i]->value;
            }
        }
        static P default_val = P();
        return default_val;
    }
};