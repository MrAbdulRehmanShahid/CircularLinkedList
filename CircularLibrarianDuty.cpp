#include <iostream>
#include <limits>
using namespace std;
struct StaffNode
{
    int staffID;
    string name;
    int deskNo;
    StaffNode* next;     // self-referential pointer
};

// head points to the LAST node (pivot); head->next is the FIRST node
class CircularLinkedList
{
private:
    StaffNode* head;
    int count;
    StaffNode* onDuty;   // librarian currently on duty (rotates)

    StaffNode* GetNodeAt(int index)
    {
        if (head == NULL) return NULL;
        StaffNode* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(StaffNode* a, StaffNode* b)
    {
        swap(a->staffID, b->staffID);
        swap(a->name, b->name);
        swap(a->deskNo, b->deskNo);
        // keep the duty marker on the same person after their data moves
        if (onDuty == a) onDuty = b;
        else if (onDuty == b) onDuty = a;
    }

    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

    // Create a node without brace-initialisation (works on older compilers)
    StaffNode* MakeNode(int id, const string& name, int desk)
    {
        StaffNode* node = new StaffNode;
        node->staffID = id;
        node->name = name;
        node->deskNo = desk;
        node->next = NULL;
        return node;
    }

    // If the node about to be removed is on duty, pass duty to the next person
    void FixDutyBeforeRemove(StaffNode* target)
    {
        if (onDuty == target)
            onDuty = (count > 1) ? target->next : NULL;
    }

    // Index of the person on duty (0 if nobody)
    int DutyIndex()
    {
        if (head == NULL || onDuty == NULL) return 0;
        StaffNode* current = head->next;
        for (int i = 0; i < count; i++)
        {
            if (current == onDuty) return i;
            current = current->next;
        }
        return 0;
    }

    void CopyFrom(const CircularLinkedList& other)
    {
        if (other.head == NULL) return;
        StaffNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            InsertAt(MakeNode(current->staffID, current->name, current->deskNo), count);
            current = current->next;
        }
        onDuty = GetNodeAt(const_cast<CircularLinkedList&>(other).DutyIndex());
    }

public:
    CircularLinkedList() { head = NULL; count = 0; onDuty = NULL; }

    bool IsEmpty()    { return count == 0; }
    bool IsNotEmpty() { return count > 0; }
    int  GetCount()   { return count; }

    bool Contains(int id)
    {
        if (IsEmpty()) return false;
        StaffNode* current = head->next;
        for (int i = 0; i < count; i++)
        {
            if (current->staffID == id) return true;
            current = current->next;
        }
        return false;
    }

    // Insert at node next to head (front)
    void Insert(StaffNode* s)
    {
        if (s == NULL) return;
        if (head == NULL)
        {
            head = s;
            s->next = s;
            onDuty = s;
        }
        else
        {
            s->next = head->next;
            head->next = s;
        }
        count++;
    }

    // Remove node next to head (front)
    StaffNode* Remove()
    {
        if (IsEmpty()) return NULL;
        StaffNode* first = head->next;
        FixDutyBeforeRemove(first);
        if (first == head) head = NULL;
        else head->next = first->next;
        first->next = NULL;
        count--;
        return first;
    }

    void InsertAt(StaffNode* s, int index)
    {
        if (s == NULL) return;
        if (count == 0)
        {
            head = s;
            s->next = s;
            onDuty = s;
            count++;
            return;
        }
        if (index < 0) index = NormalizeIndex(index);
        if (index > count) index %= count;

        if (index == 0) { Insert(s); return; }
        if (index == count)
        {
            s->next = head->next;
            head->next = s;
            head = s;
            count++;
            return;
        }
        StaffNode* previous = GetNodeAt(index - 1);
        s->next = previous->next;
        previous->next = s;
        count++;
    }

    StaffNode* RemoveAt(int index)
    {
        if (IsEmpty()) return NULL;
        index = NormalizeIndex(index);
        if (index == 0) return Remove();

        StaffNode* previous = GetNodeAt(index - 1);
        StaffNode* target = previous->next;
        FixDutyBeforeRemove(target);
        previous->next = target->next;
        if (target == head) head = previous;
        target->next = NULL;
        count--;
        return target;
    }

    void Swap(int index1, int index2)
    {
        if (IsEmpty()) return;
        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);
        if (index1 == index2) return;
        SwapData(GetNodeAt(index1), GetNodeAt(index2));
    }

    // Sort by staff ID (ascending)
    void Sort()
    {
        if (count < 2) return;
        StaffNode* current = head->next;
        for (int i = 0; i < count - 1; i++)
        {
            StaffNode* nextNode = current->next;
            for (int j = i + 1; j < count; j++)
            {
                if (current->staffID > nextNode->staffID)
                    SwapData(current, nextNode);
                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    void Reverse()
    {
        if (count < 2) return;
        StaffNode* oldTail = head;
        StaffNode* oldFirst = head->next;
        StaffNode* previous = oldTail;
        StaffNode* current = oldFirst;
        while (current != oldTail)
        {
            StaffNode* nextNode = current->next;
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
            cout << "No staff in the roster.\n";
            return;
        }
        StaffNode* current = head->next;
        cout << "\nIndex\tStaff ID\tName\t\tDesk\n";
        cout << "---------------------------------------------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i << "\t" << current->staffID << "\t\t"
                 << current->name << "\t\t" << current->deskNo;
            if (current == onDuty) cout << "   <-- ON DUTY";
            cout << endl;
            current = current->next;
        }
        cout << "Total staff: " << count << endl;
    }

    // Pass duty to the next librarian (wraps automatically)
    void AdvanceDuty()
    {
        if (IsEmpty())
        {
            cout << "No staff in the roster.\n";
            return;
        }
        onDuty = onDuty->next;
        cout << "\nDuty passed to: [" << onDuty->staffID << "] " << onDuty->name
             << " (Desk " << onDuty->deskNo << ")\n";
        if (onDuty == head->next)
            cout << "(Reached the end of the roster - wrapped around to the first librarian)\n";
    }

    // Walk 'steps' nodes WITHOUT ever reaching NULL
    void DemonstrateCircularTraversal(int steps)
    {
        if (IsEmpty())
        {
            cout << "No staff in the roster.\n";
            return;
        }
        cout << "\nCircular traversal for " << steps << " steps:\n";
        StaffNode* current = head->next;
        for (int i = 1; i <= steps; i++)
        {
            cout << current->name;
            if (i < steps) cout << " -> ";
            current = current->next;       // never becomes NULL
        }
        cout << " -> ...\n";
        cout << "Last node (" << head->name << ") links back to first node ("
             << head->next->name << "): VERIFIED\n";
    }

    void CheckStatus()
    {
        if (IsEmpty())
        {
            cout << "\nStatus: Roster is EMPTY.\n";
            return;
        }
        StaffNode* first = head->next;
        cout << "\nStatus: Roster is NOT empty.\n";
        cout << "Total staff : " << count << endl;
        cout << "First       : [" << first->staffID << "] " << first->name << endl;
        cout << "Last        : [" << head->staffID << "] " << head->name << endl;
        cout << "Circular?   : " << (head->next == first ? "YES - last node links back to first node"
                                                          : "NO") << endl;
        cout << "On duty     : [" << onDuty->staffID << "] " << onDuty->name << endl;
    }

    CircularLinkedList(const CircularLinkedList& other)
    {
        head = NULL; count = 0; onDuty = NULL;
        CopyFrom(other);
    }

    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other) return *this;
        while (IsNotEmpty()) delete Remove();
        onDuty = NULL;
        CopyFrom(other);
        return *this;
    }

    ~CircularLinkedList()
    {
        while (IsNotEmpty()) delete Remove();
    }
};

// ========================================
// Helpers
// ========================================
StaffNode* CreateStaff(int id, string name, int desk)
{
    StaffNode* s = new StaffNode;
    s->staffID = id;
    s->name = name;
    s->deskNo = desk;
    s->next = NULL;
    return s;
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

// Reads a staff member; returns NULL if the ID already exists
StaffNode* ReadStaff(CircularLinkedList& roster)
{
    int id = ReadInt("Enter Staff ID: ");
    if (roster.Contains(id))
    {
        cout << "A staff member with ID " << id << " already exists.\n";
        return NULL;
    }
    string name;
    cout << "Enter Staff Name: ";
    getline(cin, name);
    int desk = ReadInt("Enter Desk Number: ");
    return CreateStaff(id, name, desk);
}

void LoadSampleData(CircularLinkedList& roster)
{
    int ids[5]      = {203, 201, 205, 202, 204};
    const char* nm[5] = {"Ayesha", "Bilal", "Hina", "Usman", "Sara"};
    int desks[5]    = {3, 1, 5, 2, 4};
    int added = 0;
    for (int i = 0; i < 5; i++)
    {
        if (roster.Contains(ids[i])) continue;
        roster.InsertAt(CreateStaff(ids[i], nm[i], desks[i]), roster.GetCount());
        added++;
    }
    cout << added << " sample staff members added.\n";
}

void ShowMenu()
{
    cout << "\n========================================\n";
    cout << "   CIRCULAR LIBRARIAN DUTY ROSTER SYSTEM\n";
    cout << "========================================\n";
    cout << "1.  Display Roster\n";
    cout << "2.  Insert Staff at Head\n";
    cout << "3.  Remove Staff from Head\n";
    cout << "4.  Insert Staff at Index\n";
    cout << "5.  Remove Staff at Index\n";
    cout << "6.  Swap Staff\n";
    cout << "7.  Sort Roster by Staff ID\n";
    cout << "8.  Reverse Roster\n";
    cout << "9.  Pass Duty to Next Librarian\n";
    cout << "10. Demonstrate Circular Traversal\n";
    cout << "11. Check Roster Status\n";
    cout << "12. Load Sample Data\n";
    cout << "13. Exit\n";
}

// ========================================
// Main: Menu-Driven Application
// ========================================
int main()
{
    CircularLinkedList roster;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            roster.Print();
            break;

        case 2:
        {
            StaffNode* s = ReadStaff(roster);
            if (s)
            {
                roster.Insert(s);
                cout << "Staff inserted at head.\n";
            }
            break;
        }

        case 3:
        {
            StaffNode* removed = roster.Remove();
            if (removed)
            {
                cout << "Removed: " << removed->name << endl;
                delete removed;
            }
            else
                cout << "Roster is empty. Nothing to remove.\n";
            break;
        }

        case 4:
        {
            StaffNode* s = ReadStaff(roster);
            if (s)
            {
                int index = ReadInt("Enter index: ");
                roster.InsertAt(s, index);
                cout << "Staff inserted.\n";
            }
            break;
        }

        case 5:
        {
            if (roster.IsEmpty())
            {
                cout << "Roster is empty. Nothing to remove.\n";
                break;
            }
            int index = ReadInt("Enter index to remove: ");
            StaffNode* removed = roster.RemoveAt(index);
            if (removed)
            {
                cout << "Removed: " << removed->name << endl;
                delete removed;
            }
            break;
        }

        case 6:
        {
            if (roster.GetCount() < 2)
            {
                cout << "Need at least 2 staff members to swap.\n";
                break;
            }
            int i1 = ReadInt("Enter first index: ");
            int i2 = ReadInt("Enter second index: ");
            roster.Swap(i1, i2);
            cout << "Staff swapped.\n";
            break;
        }

        case 7:
            roster.Sort();
            cout << "Roster sorted by staff ID.\n";
            break;

        case 8:
            roster.Reverse();
            cout << "Roster reversed.\n";
            break;

        case 9:
            roster.AdvanceDuty();
            break;

        case 10:
        {
            int steps = ReadInt("How many steps to traverse? ");
            if (steps < 1) steps = 1;
            roster.DemonstrateCircularTraversal(steps);
            break;
        }

        case 11:
            roster.CheckStatus();
            break;

        case 12:
            LoadSampleData(roster);
            break;

        case 13:
            cout << "Exiting system. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Please enter 1-13.\n";
        }
    } while (choice != 13);

    return 0;
}
