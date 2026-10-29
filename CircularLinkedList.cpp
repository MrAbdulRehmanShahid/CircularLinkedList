#include <iostream>
#include <limits>
using namespace std;
// Book Node
struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};
// Circular Linked List Class
class CircularLinkedList
{
private:
    BookNode* head;
    int count;
    int currentIndex;   // position of the book currently on the display screen

    BookNode* GetNodeAt(int index)
    {
        if (head == NULL)
            return NULL;
        BookNode* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

public:
    CircularLinkedList()
    {
        head = NULL;
        count = 0;
        currentIndex = 0;
    }

    bool IsEmpty()    { return count == 0; }
    bool IsNotEmpty() { return count > 0; }
    int  GetCount()   { return count; }

    // Does a book with this ID already exist?
    bool Contains(int id)
    {
        if (IsEmpty()) return false;
        BookNode* current = head->next;
        for (int i = 0; i < count; i++)
        {
            if (current->bookID == id) return true;
            current = current->next;
        }
        return false;
    }

    // Insert at node next to head (front)
    void Insert(BookNode* book)
    {
        if (book == NULL)
            return;
        if (head == NULL)
        {
            head = book;
            book->next = book;
        }
        else
        {
            book->next = head->next;
            head->next = book;
        }
        count++;
    }

    // Remove node next to head (front)
    BookNode* Remove()
    {
        if (IsEmpty())
            return NULL;
        BookNode* first = head->next;
        if (first == head)
            head = NULL;
        else
            head->next = first->next;
        first->next = NULL;
        count--;
        return first;
    }

    void InsertAt(BookNode* book, int index)
    {
        if (book == NULL)
            return;
        if (count == 0)
        {
            head = book;
            book->next = book;
            count++;
            return;
        }
        if (index < 0)
            index = NormalizeIndex(index);
        if (index > count)
            index %= count;

        if (index == 0)
        {
            Insert(book);
            return;
        }
        if (index == count)
        {
            book->next = head->next;
            head->next = book;
            head = book;
            count++;
            return;
        }
        BookNode* previous = GetNodeAt(index - 1);
        book->next = previous->next;
        previous->next = book;
        count++;
    }

    BookNode* RemoveAt(int index)
    {
        if (IsEmpty())
            return NULL;
        index = NormalizeIndex(index);
        if (index == 0)
            return Remove();

        BookNode* previous = GetNodeAt(index - 1);
        BookNode* target = previous->next;
        previous->next = target->next;
        if (target == head)
            head = previous;
        target->next = NULL;
        count--;
        return target;
    }

    void Swap(int index1, int index2)
    {
        if (IsEmpty())
            return;
        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);
        if (index1 == index2)
            return;
        SwapData(GetNodeAt(index1), GetNodeAt(index2));
    }

    void Sort()
    {
        if (count < 2)
            return;
        BookNode* current = head->next;
        for (int i = 0; i < count - 1; i++)
        {
            BookNode* nextNode = current->next;
            for (int j = i + 1; j < count; j++)
            {
                if (current->bookID > nextNode->bookID)
                    SwapData(current, nextNode);
                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    void Reverse()
    {
        if (count < 2)
            return;
        BookNode* oldTail = head;
        BookNode* oldFirst = head->next;
        BookNode* previous = oldTail;
        BookNode* current = oldFirst;
        while (current != oldTail)
        {
            BookNode* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        oldTail->next = previous;
        head = oldFirst;
    }

    void Print()
    {
        if (IsEmpty())
        {
            cout << "No books in the collection.\n";
            return;
        }
        BookNode* current = head->next;
        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "-----------------------------------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i << "\t" << current->bookID << "\t" << current->title;
            if (i == currentIndex % count)
                cout << "   <-- now displaying";
            cout << endl;
            current = current->next;
        }
        cout << "Total books: " << count << endl;
    }

    // Move the display to the next book; wraps from last back to first
    void MoveToNext()
    {
        if (IsEmpty())
        {
            cout << "No books in the collection.\n";
            return;
        }
        currentIndex = (currentIndex % count + 1) % count;
        BookNode* book = GetNodeAt(currentIndex);
        cout << "\nNow displaying: [" << book->bookID << "] " << book->title << endl;
        if (currentIndex == 0)
            cout << "(Reached the end - wrapped around to the first book)\n";
    }

    // Report status and verify the list is really circular
    void CheckStatus()
    {
        if (IsEmpty())
        {
            cout << "\nStatus: Collection is EMPTY.\n";
            return;
        }
        BookNode* first = head->next;
        cout << "\nStatus: Collection is NOT empty.\n";
        cout << "Total books : " << count << endl;
        cout << "First book  : [" << first->bookID << "] " << first->title << endl;
        cout << "Last book   : [" << head->bookID << "] " << head->title << endl;
        cout << "Circular?   : " << (head->next == first ? "YES - last book links back to first book"
                                                          : "NO") << endl;
        BookNode* now = GetNodeAt(currentIndex % count);
        cout << "On display  : [" << now->bookID << "] " << now->title << endl;
    }

    // Copy constructor
    CircularLinkedList(const CircularLinkedList& other)
    {
        head = NULL;
        count = 0;
        currentIndex = other.currentIndex;
        if (other.head == NULL)
            return;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new BookNode{current->bookID, current->title, NULL}, count);
            current = current->next;
        }
    }

    // Assignment operator
    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other)
            return *this;
        while (IsNotEmpty())
            delete Remove();
        currentIndex = other.currentIndex;
        if (other.head == NULL)
            return *this;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(new BookNode{current->bookID, current->title, NULL}, count);
            current = current->next;
        }
        return *this;
    }

    ~CircularLinkedList()
    {
        while (IsNotEmpty())
            delete Remove();
    }
};
// Helpers
BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;
    book->bookID = id;
    book->title = title;
    book->next = NULL;
    return book;
}

int ReadInt(const string& prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

// Reads a book from the user; returns NULL if the ID already exists
BookNode* ReadBook(CircularLinkedList& library)
{
    int id = ReadInt("Enter Book ID: ");
    if (library.Contains(id))
    {
        cout << "A book with ID " << id << " already exists.\n";
        return NULL;
    }
    string title;
    cout << "Enter Book Title: ";
    getline(cin, title);
    return CreateBook(id, title);
}

void ShowMenu()
{
    cout << "\n========================================\n";
    cout << "   CIRCULAR LIBRARY BOOK DISPLAY SYSTEM\n";
    cout << "========================================\n";
    cout << "1.  Display All Books\n";
    cout << "2.  Insert Book at Head\n";
    cout << "3.  Remove Book from Head\n";
    cout << "4.  Insert Book at Index\n";
    cout << "5.  Remove Book at Index\n";
    cout << "6.  Swap Books\n";
    cout << "7.  Sort Books by ID\n";
    cout << "8.  Reverse Book Collection\n";
    cout << "9.  Move to Next Book\n";
    cout << "10. Check Collection Status\n";
    cout << "11. Exit\n";
}
// Main: Menu-Driven Application
int main()
{
    CircularLinkedList library;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            library.Print();
            break;

        case 2:
        {
            BookNode* book = ReadBook(library);
            if (book)
            {
                library.Insert(book);
                cout << "Book inserted at head.\n";
            }
            break;
        }

        case 3:
        {
            BookNode* removed = library.Remove();
            if (removed)
            {
                cout << "Removed: " << removed->title << endl;
                delete removed;
            }
            else
                cout << "Collection is empty. Nothing to remove.\n";
            break;
        }

        case 4:
        {
            BookNode* book = ReadBook(library);
            if (book)
            {
                int index = ReadInt("Enter index: ");
                library.InsertAt(book, index);
                cout << "Book inserted.\n";
            }
            break;
        }

        case 5:
        {
            if (library.IsEmpty())
            {
                cout << "Collection is empty. Nothing to remove.\n";
                break;
            }
            int index = ReadInt("Enter index to remove: ");
            BookNode* removed = library.RemoveAt(index);
            if (removed)
            {
                cout << "Removed: " << removed->title << endl;
                delete removed;
            }
            break;
        }

        case 6:
        {
            if (library.GetCount() < 2)
            {
                cout << "Need at least 2 books to swap.\n";
                break;
            }
            int i1 = ReadInt("Enter first index: ");
            int i2 = ReadInt("Enter second index: ");
            library.Swap(i1, i2);
            cout << "Books swapped.\n";
            break;
        }

        case 7:
            library.Sort();
            cout << "Books sorted by ID.\n";
            break;

        case 8:
            library.Reverse();
            cout << "Collection reversed.\n";
            break;

        case 9:
            library.MoveToNext();
            break;

        case 10:
            library.CheckStatus();
            break;

        case 11:
            cout << "Exiting system. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please enter 1-11.\n";
        }
    } while (choice != 11);

    return 0;
}
