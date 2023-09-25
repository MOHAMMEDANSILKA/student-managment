#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

struct Student {
    int rollNo;
    string name;
    string phoneNumber;
    double cgpa;
    int yearOfStudy;
    vector<string> subjectsEnrolled;
    int totalClasses;
    int classesAttended;
    Student* next;

    Student(int rollNo, string name, string phoneNumber, double cgpa, int yearOfStudy)
        : rollNo(rollNo), name(name), phoneNumber(phoneNumber), cgpa(cgpa), yearOfStudy(yearOfStudy), next(nullptr), totalClasses(0), classesAttended(0) {}

    void addSubject(const string& subject) {
        subjectsEnrolled.push_back(subject);
    }

    bool isEligibleForNextYear() const {
        // CGPA criteria: 2.0 or higher
        // Attendance criteria: 70% or more
        return cgpa >= 2.0 && (static_cast<double>(classesAttended) / totalClasses) >= 0.7;
    }
};

class StudentManagementSystem {
private:
    Student* head;
    stack<pair<int, int>> attendanceStack; // Pair of (Roll No, Attended Classes)

public:
    StudentManagementSystem() : head(nullptr) {}

    ~StudentManagementSystem() {
        clear();
    }

    void addStudent(int rollNo, string name, string phoneNumber, double cgpa, int yearOfStudy) {
        Student* newStudent = new Student(rollNo, name, phoneNumber, cgpa, yearOfStudy);
        if (!head) {
            head = newStudent;
        } else {
            Student* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newStudent;
        }
        cout << "Student added successfully." << endl;
    }

    Student* searchStudent(int rollNo) {
        Student* current = head;
        while (current) {
            if (current->rollNo == rollNo) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void markAttendance(int rollNo, int attendedClasses) {
        attendanceStack.push(make_pair(rollNo, attendedClasses));
    }

    void updateAttendance() {
        while (!attendanceStack.empty()) {
            pair<int, int> record = attendanceStack.top();
            Student* student = searchStudent(record.first);
            if (student) {
                student->classesAttended += record.second;
                attendanceStack.pop();
            } else {
                cout << "Student not found for attendance record." << endl;
                break;
            }
        }
    }

    void displayAllStudents() {
        Student* current = head;
        cout << "Student List:" << endl;
        while (current) {
            cout << "Roll No: " << current->rollNo << ", Name: " << current->name << ", Phone Number: " << current->phoneNumber << ", CGPA: " << current->cgpa << ", Year of Study: " << current->yearOfStudy << ", Subjects Enrolled: ";
            for (const string& subject : current->subjectsEnrolled) {
                cout << subject << ", ";
            }
            cout << "Attendance: " << current->classesAttended << " / " << current->totalClasses << ", Eligible for Next Year: " << (current->isEligibleForNextYear() ? "Yes" : "No") << endl;
            current = current->next;
        }
    }

    void clear() {
        while (head) {
            Student* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    StudentManagementSystem system;

    int choice;
    do {
        cout << "\nStudent Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Search Student\n";
        cout << "3. Mark Attendance\n";
        cout << "4. Update Attendance\n";
        cout << "5. Display All Students\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int rollNo;
                string name;
                string phoneNumber;
                double cgpa;
                int yearOfStudy;
                cout << "Enter Roll No: ";
                cin >> rollNo;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Phone Number: ";
                cin.ignore();
                getline(cin, phoneNumber);
                cout << "Enter CGPA: ";
                cin >> cgpa;
                cout << "Enter Year of Study: ";
                cin >> yearOfStudy;

                system.addStudent(rollNo, name, phoneNumber, cgpa, yearOfStudy);

                int subjects;
                cout << "Enter number of subjects enrolled: ";
                cin >> subjects;
                cin.ignore();
                for (int i = 0; i < subjects; ++i) {
                    cout << "Enter subject " << i + 1 << ": ";
                    string subject;
                    getline(cin, subject);
                    system.searchStudent(rollNo)->addSubject(subject);
                }
                break;
            }
            case 2: {
                int rollNo;
                cout << "Enter Roll No to search: ";
                cin >> rollNo;
                Student* foundStudent = system.searchStudent(rollNo);
                if (foundStudent) {
                    cout << "Student found:" << endl;
                    cout << "Roll No: " << foundStudent->rollNo << ", Name: " << foundStudent->name << ", Phone Number: " << foundStudent->phoneNumber << ", CGPA: " << foundStudent->cgpa << ", Year of Study: " << foundStudent->yearOfStudy << ", Subjects Enrolled: ";
                    for (const string& subject : foundStudent->subjectsEnrolled) {
                        cout << subject << ", ";
                    }
                    cout << "Attendance: " << foundStudent->classesAttended << " / " << foundStudent->totalClasses << ", Eligible for Next Year: " << (foundStudent->isEligibleForNextYear() ? "Yes" : "No") << endl;
                } else {
                    cout << "Student not found." << endl;
                }
                break;
            }
            case 3: {
                int rollNo, attendedClasses;
                cout << "Enter Roll No to mark attendance: ";
                cin >> rollNo;
                cout << "Enter attended classes: ";
                cin >> attendedClasses;
                system.markAttendance(rollNo, attendedClasses);
                break;
            }
            case 4:
                system.updateAttendance();
                cout << "Attendance updated successfully." << endl;
                break;
            case 5:
                system.displayAllStudents();
                break;
            case 6:
                system.clear();
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
