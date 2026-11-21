#include <iostream>
#include <string>
using namespace std;

struct Node {
    int number;
    string direction;
    string time;
    Node* next;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
    int count = 0;
};

int created = 0;
int freed = 0;

Node* createNode(int number) {
    Node* p = new Node{ number, "Маршрут " + to_string(number),
                       "12:00", nullptr };
    created++;
    return p;
}

void addEnd(List& l, int number) {
    Node* p = createNode(number);

    if (l.head == nullptr)
        l.head = p;
    else
        l.tail->next = p;

    l.tail = p;
    l.count++;
}

void clear(List& l) {
    Node* p = l.head;

    while (p != nullptr) {
        Node* next = p->next;
        delete p;
        freed++;
        p = next;
    }

    l.head = nullptr;
    l.tail = nullptr;
    l.count = 0;
}

void view(const List& l) {
    Node* p = l.head;
    int i = 0;

    cout << "Список: ";

    while (p != nullptr && i < l.count) {
        cout << p->number;
        if (p->next != nullptr)
            cout << " -> ";
        p = p->next;
        i++;
    }

    if (l.head == nullptr)
        cout << "порожній";

    cout << "\nКількість вузлів: " << l.count << '\n';
}

bool check(const List& l) {
    if (l.count < 0)
        return false;

    if (l.head == nullptr)
        return l.tail == nullptr && l.count == 0;

    if (l.tail == nullptr || l.count == 0)
        return false;

    Node* slow = l.head;
    Node* fast = l.head;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return false;
    }

    Node* p = l.head;
    Node* last = nullptr;
    int amount = 0;

    while (p != nullptr && amount <= l.count) {
        last = p;
        p = p->next;
        amount++;
    }

    return p == nullptr && amount == l.count &&
        last == l.tail && l.tail->next == nullptr;
}

int searchFirst(const List& l, int key, int& visited) {
    Node* p = l.head;
    int position = 0;
    visited = 0;

    while (p != nullptr) {
        visited++;

        if (p->number == key)
            return position;

        p = p->next;
        position++;
    }

    return -1;
}

int searchAll(const List& l, int key) {
    Node* p = l.head;
    int position = 0;
    int found = 0;

    cout << "Позиції знайдених вузлів: ";

    while (p != nullptr) {
        if (p->number == key) {
            cout << position << " ";
            found++;
        }

        p = p->next;
        position++;
    }

    if (found == 0)
        cout << "не знайдено";

    cout << "\nКількість збігів: " << found << '\n';
    return found;
}

bool insertAt(List& l, int position, int number) {
    if (position < 0 || position > l.count)
        return false;

    Node* p = createNode(number);

    if (position == 0) {
        p->next = l.head;
        l.head = p;

        if (l.count == 0)
            l.tail = p;
    }
    else {
        Node* prev = l.head;

        for (int i = 0; i < position - 1; i++)
            prev = prev->next;

        p->next = prev->next;
        prev->next = p;

        if (p->next == nullptr)
            l.tail = p;
    }

    l.count++;
    return true;
}

bool insertBefore(List& l, int key, int number) {
    Node* prev = nullptr;
    Node* p = l.head;

    while (p != nullptr && p->number != key) {
        prev = p;
        p = p->next;
    }

    if (p == nullptr)
        return false;

    Node* added = createNode(number);
    added->next = p;

    if (prev == nullptr)
        l.head = added;
    else
        prev->next = added;

    l.count++;
    return true;
}

bool insertAfter(List& l, int key, int number) {
    Node* p = l.head;

    while (p != nullptr && p->number != key)
        p = p->next;

    if (p == nullptr)
        return false;

    Node* added = createNode(number);
    added->next = p->next;
    p->next = added;

    if (l.tail == p)
        l.tail = added;

    l.count++;
    return true;
}

bool eraseAt(List& l, int position) {
    if (position < 0 || position >= l.count)
        return false;

    Node* p = l.head;

    if (position == 0) {
        l.head = p->next;

        if (l.count == 1)
            l.tail = nullptr;
    }
    else {
        Node* prev = l.head;

        for (int i = 0; i < position - 1; i++)
            prev = prev->next;

        p = prev->next;
        prev->next = p->next;

        if (p == l.tail)
            l.tail = prev;
    }

    delete p;
    freed++;
    l.count--;
    return true;
}

bool eraseFirst(List& l, int key) {
    Node* prev = nullptr;
    Node* p = l.head;

    while (p != nullptr && p->number != key) {
        prev = p;
        p = p->next;
    }

    if (p == nullptr)
        return false;

    if (prev == nullptr)
        l.head = p->next;
    else
        prev->next = p->next;

    if (p == l.tail)
        l.tail = prev;

    delete p;
    freed++;
    l.count--;
    return true;
}

int eraseAll(List& l, int key) {
    int removed = 0;

    while (l.head != nullptr && l.head->number == key) {
        Node* p = l.head;
        l.head = l.head->next;
        delete p;
        freed++;
        l.count--;
        removed++;
    }

    Node* p = l.head;

    while (p != nullptr && p->next != nullptr) {
        if (p->next->number == key) {
            Node* del = p->next;
            p->next = del->next;

            if (del == l.tail)
                l.tail = p;

            delete del;
            freed++;
            l.count--;
            removed++;
        }
        else {
            p = p->next;
        }
    }

    if (l.head == nullptr)
        l.tail = nullptr;

    return removed;
}

bool matches(const List& l, const int expected[], int n) {
    if (!check(l) || l.count != n)
        return false;

    Node* p = l.head;

    for (int i = 0; i < n; i++) {
        if (p == nullptr || p->number != expected[i])
            return false;
        p = p->next;
    }

    return p == nullptr; }

bool finishTest(List& l, const int expected[], int n) {
    bool result = matches(l, expected, n);
    clear(l);

    return result && check(l) &&
        created == freed;
}

template <typename F>
void test(const string& name, F action, int& passed) {
    bool ok = action();
    cout << name << ": " << (ok ? "ПРОЙДЕНО" : "ПОМИЛКА") << '\n';

    if (ok)
        passed++;
}

void runTests() {
    int passed = 0;

    cout << "\n=== 20 автоматизованих тестів ===\n";

    test("Тест 1", []() {
        List l;
        int visited;
        int pos = searchFirst(l, 5, visited);
        return pos == -1 && visited == 0 &&
            finishTest(l, nullptr, 0);
        }, passed);

    test("Тест 2", []() {
        List l;
        int a[] = { 5 };
        return insertAt(l, 0, 5) && finishTest(l, a, 1);
        }, passed);

    test("Тест 3", []() {
        List l;
        addEnd(l, 5);
        return eraseAt(l, 0) && finishTest(l, nullptr, 0);
        }, passed);

    test("Тест 4", []() {
        List l;
        addEnd(l, 1); addEnd(l, 3);
        int a[] = { 1, 2, 3 };
        return insertAt(l, 1, 2) && finishTest(l, a, 3);
        }, passed);

    test("Тест 5", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2);
        int a[] = { 0, 1, 2 };
        return insertAt(l, 0, 0) && finishTest(l, a, 3);
        }, passed);

    test("Тест 6", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2);
        int a[] = { 1, 2, 3 };
        return insertAt(l, 2, 3) && finishTest(l, a, 3);
        }, passed);

    test("Тест 7", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 2, 3 };
        return eraseAt(l, 0) && finishTest(l, a, 2);
        }, passed);

    test("Тест 8", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 3 };
        return eraseAt(l, 1) && finishTest(l, a, 2);
        }, passed);

    test("Тест 9", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 2 };
        return eraseAt(l, 2) && finishTest(l, a, 2);
        }, passed);

    test("Тест 10", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 2, 3 };
        return eraseFirst(l, 2) && finishTest(l, a, 3);
        }, passed);

    test("Тест 11", []() {
        List l;
        addEnd(l, 2); addEnd(l, 2); addEnd(l, 1); addEnd(l, 2);
        int a[] = { 1 };
        return eraseAll(l, 2) == 3 && finishTest(l, a, 1);
        }, passed);

    test("Тест 12", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 2, 3 };
        bool ok = !insertAt(l, 5, 9);
        return ok && finishTest(l, a, 3);
        }, passed);

    test("Тест 13", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 2, 3 };
        bool ok = !eraseAt(l, -1);
        return ok && finishTest(l, a, 3);
        }, passed);

    test("Тест 14", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 1, 2, 3, 9 };
        return insertAfter(l, 3, 9) && finishTest(l, a, 4);
        }, passed);

    test("Тест 15", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 3);
        int a[] = { 0, 1, 2, 3 };
        return insertBefore(l, 1, 0) && finishTest(l, a, 4);
        }, passed);

    test("Тест 16", []() {
        List l;
        addEnd(l, 4); addEnd(l, 6); addEnd(l, 6);
        int visited;
        int pos = searchFirst(l, 6, visited);
        int a[] = { 4, 6, 6 };
        return pos == 1 && visited == 2 &&
            finishTest(l, a, 3);
        }, passed);

    test("Тест 17", []() {
        List l;
        addEnd(l, 2); addEnd(l, 2); addEnd(l, 1);
        int found = searchAll(l, 2);
        int a[] = { 2, 2, 1 };
        return found == 2 && finishTest(l, a, 3);
        }, passed);

    test("Тест 18", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2);
        clear(l);
        addEnd(l, 7); addEnd(l, 8);
        int a[] = { 7, 8 };
        return finishTest(l, a, 2);
        }, passed);

    test("Тест 19", []() {
        List l;
        addEnd(l, 3); addEnd(l, 3); addEnd(l, 3);
        int a[] = { 1 };
        int removed = eraseAll(l, 3);
        addEnd(l, 1);
        return removed == 3 && finishTest(l, a, 1);
        }, passed);

    test("Тест 20", []() {
        List l;
        addEnd(l, 1); addEnd(l, 2); addEnd(l, 2);
        insertAt(l, 1, 5);
        eraseFirst(l, 2);
        insertAfter(l, 5, 8);
        int a[] = { 1, 5, 8, 2 };
        return finishTest(l, a, 4);
        }, passed);

    cout << "Успішних тестів: " << passed << "/20\n";
}

void logCommand(int number, const string& name, const List& l) {
    cout << "\nКоманда " << number << ": " << name << '\n';
    view(l);
    cout << "Перевірка: "
        << (check(l) ? "правильно" : "помилка") << '\n';
}

void runCommands() {
    List l;

    cout << "\n=== Журнал 20 команд ===\n";

    addEnd(l, 6);
    logCommand(1, "Додати 6", l);

    addEnd(l, 2);
    logCommand(2, "Додати 2", l);

    addEnd(l, 6);
    logCommand(3, "Додати 6", l);

    addEnd(l, 8);
    logCommand(4, "Додати 8", l);

    addEnd(l, 1);
    logCommand(5, "Додати 1", l);

    int visited;
    int pos = searchFirst(l, 6, visited);
    cout << "\nПерший ключ 6: позиція " << pos
        << ", переглянуто " << visited << '\n';
    logCommand(6, "Пошук першого 6", l);

    searchAll(l, 6);
    logCommand(7, "Пошук усіх 6", l);

    insertAt(l, 2, 9);
    logCommand(8, "Вставити 9 на позицію 2", l);

    insertBefore(l, 8, 7);
    logCommand(9, "Вставити 7 перед 8", l);

    insertAfter(l, 1, 4);
    logCommand(10, "Вставити 4 після 1", l);

    eraseAt(l, 0);
    logCommand(11, "Видалити позицію 0", l);

    insertAt(l, 0, 6);
    logCommand(12, "Вставити 6 на початок", l);

    eraseFirst(l, 6);
    logCommand(13, "Видалити перший 6", l);

    insertAt(l, l.count, 10);
    logCommand(14, "Додати 10 у кінець", l);

    eraseAt(l, l.count - 1);
    logCommand(15, "Видалити останній вузол", l);

    insertBefore(l, 2, 3);
    logCommand(16, "Вставити 3 перед 2", l);

    insertAfter(l, 3, 5);
    logCommand(17, "Вставити 5 після 3", l);

    eraseAll(l, 6);
    logCommand(18, "Видалити всі 6", l);

    insertAt(l, 0, 11);
    logCommand(19, "Повторно вставити 11", l);

    clear(l);
    logCommand(20, "Повністю очистити список", l);

    cout << "\nСписок після команд очищено.\n";
}


int main() {
    cout << "Практична робота 12\n";
    cout << "Варіант 7 - Рейс\n";

    List l;
    int initial[] = { 6, 2, 6, 8, 1 };

    for (int x : initial)
        addEnd(l, x);

    cout << "\n=== Стани списку за варіантом 7 ===\n";

    cout << "\n1. Початковий список:\n";
    view(l);

    int visited;
    int position = searchFirst(l, 6, visited);

    cout << "\n=== Завдання 1. Пошук першого збігу ===\n";
    cout << "Позиція: " << position
        << ", переглянуто вузлів: " << visited << '\n';

    cout << "\n=== Завдання 2. Пошук усіх збігів ===\n";
    searchAll(l, 6);

    cout << "\n2. Після вставлення 9 на позицію 2:\n";
    insertAt(l, 2, 9);
    view(l);

    cout << "\n3. Після видалення всіх елементів зі значенням 6:\n";
    int removed = eraseAll(l, 6);
    cout << "Видалено вузлів: " << removed << '\n';
    view(l);

    cout << "\nПеревірка структури: "
        << (check(l) ? "правильно" : "помилка") << '\n';

    clear(l);

    runTests();
    runCommands();

    cout << "\n=== Перевірка пам'яті ===\n";
    cout << "Створено вузлів: " << created << '\n';
    cout << "Звільнено вузлів: " << freed << '\n';
    cout << "Активних вузлів: " << created - freed << '\n';

    if (created == freed)
        cout << "Баланс пам'яті правильний\n";
    else
        cout << "Є незвільнені вузли\n";

    return 0;
}
