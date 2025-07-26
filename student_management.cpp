#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Student
{
    int rollNo;
    char name[50];
    char dept[50];
    float cgpa;
};

void printHeader()
{
    cout << setw(10) << "Roll No" << setw(25) << "Name " << setw(20) << " Department " << setw(10) << " CGPA " << "\n ";
    cout << string(65, '-') << "\n";
}

void printStudent(const Student &s)
{
    cout << setw(10) << s.rollNo << setw(25) << s.name << setw(20) << s.dept << fixed << setprecision(2) << setw(10) << s.cgpa << "\n";
}

void addStudent()
{
    ofstream outFile("students.dat", ios::binary | ios::app);
    Student s;
    cout << "Enter Roll Number: ";
    cin >> s.rollNo;
    cout << "Enter Name: ";
    cin.getline(s.name, 50);
    cout << "Enter Department: ";
    cin.getline(s.dept, 50);
    cout << "Enter CGPA: ";
    cin >> s.cgpa;

    outFile.write((char *)&s, sizeof(Student));
    outFile.close();
    cout << "Student record added successfully.\n";
}

// Displays all students
void displayAll()
{
    ifstream inFile("students.dat", ios::binary);
    Student s;
    bool found = false;

    printHeader();
    while (inFile.read((char *)(&s), sizeof(Student)))
    {
        printStudent(s);
        found = true;
    }
    inFile.close();

    if (!found)
        cout << "No records found.\n";
}

// To search a student record
void searchStudent()
{
    int roll;
    bool found = false;
    cout << "Enter Roll Number to search: ";
    cin >> roll;

    ifstream inFile("students.dat", ios::binary);
    Student s;
    while (inFile.read((char *)(&s), sizeof(Student)))
    {
        if (s.rollNo == roll)
        {
            cout << "\nRecord Found:\n";
            printStudent(s);
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found)
        cout << "Record not found.\n";
}

// To delete a student record
void deleteStudent()
{
    int roll;
    cout << "Enter Roll Number to delete: ";
    cin >> roll;

    ifstream inFile("students.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    Student s;
    bool found = false;

    while (inFile.read((char *)(&s), sizeof(Student)))
    {
        if (s.rollNo != roll)
            outFile.write((char *)(&s), sizeof(Student));
        else
            found = true;
    }

    inFile.close();
    outFile.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found)
        cout << "Record deleted successfully.\n";
    else
        cout << "Record not found.\n";
}

// To update a student record
void updateStudent()
{
    int roll;
    cout << "Enter Roll Number to update: ";
    cin >> roll;
    fstream file("students.dat", ios::binary | ios::in | ios::out);
    Student s;
    bool found = false;

    while (file.read((char *)(&s), sizeof(Student)))
    {
        if (s.rollNo == roll)
        {
            cout << "\nCurrent Details:\n";
            printHeader();
            printStudent(s);

            cout << "\nEnter New Department: ";
            cin.ignore();
            cin.getline(s.dept, 50);
            cout << "Enter New CGPA: ";
            cin >> s.cgpa;

            streampos pos = file.tellg();
            file.seekp(pos - static_cast<streamoff>(sizeof(Student)));
            file.write(reinterpret_cast<char *>(&s), sizeof(Student));
            found = true;
            cout << "Record updated successfully.\n";
            break;
        }
    }
    file.close();
    if (!found)
        cout << "Record not found.\n";
}

int main()
{
    int choice;
    do
    {
        cout << "\n*** Student Record Management System ***\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Update Student\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            displayAll();
            break;
        case 3:
            searchStudent();
            break;
        case 4:
            deleteStudent();
            break;
        case 5:
            updateStudent();
            break;
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);

    return 0;
}