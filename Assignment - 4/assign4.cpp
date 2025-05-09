#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <limits>
#include <algorithm>
#include <iomanip> // Required for formatted output
using namespace std;

// === Custom Exception Hierarchy ===
class UniversitySystemException : public runtime_error {
public:
    UniversitySystemException(const string& msg) : runtime_error("University Error: " + msg) {}
};

class EnrollmentException : public UniversitySystemException {
public:
    EnrollmentException(const string& msg) : UniversitySystemException("Enrollment Error: " + msg) {}
};

class GradeException : public UniversitySystemException {
public:
    GradeException(const string& msg) : UniversitySystemException("Grade Error: " + msg) {}
};

class PaymentException : public UniversitySystemException {
public:
    PaymentException(const string& msg) : UniversitySystemException("Payment Error: " + msg) {}
};

// === Basic Struct ===
struct date {
    int day, month, year;
    date(int d, int m, int y) : day(d), month(m), year(y) {
        if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900)
            throw UniversitySystemException("Invalid date: " + to_string(d) + "/" + to_string(m) + "/" + to_string(y));
    }

    //Added output stream
    friend ostream& operator<<(ostream& os, const date& d) {
        os << d.day << "/" << d.month << "/" << d.year;
        return os;
    }
};

// === Person Base ===
class person {
protected:
    string name, id, contact;
    int age;

public:
    person(string n, int a, string i, string c) : name(n), age(a), id(i), contact(c) {
        if (n.empty()) throw UniversitySystemException("Name cannot be empty.");
        if (a <= 0 || a > 130) throw UniversitySystemException("Invalid age: " + to_string(a));
        if (i.empty()) throw UniversitySystemException("ID cannot be empty.");
        if (c.empty()) throw UniversitySystemException("Contact cannot be empty.");
    }

    virtual ~person() = default;
    virtual void display_details() const = 0;
    virtual double calculate_payment() const = 0;

    const string& get_id() const { return id; } // Added getter for ID
    const string& get_name() const { return name; }
};

// === Student (Base) ===
class student : public person {
protected:
    date enrollment_date;
    string program;
    float GPA;
    vector<string> enrolled_courses;

public:
    student(string n, int a, string i, string c, date d, string p, float g)
        : person(n, a, i, c), enrollment_date(d), program(p), GPA(g) {
        if (program.empty()) throw UniversitySystemException("Program cannot be empty.");
        if (GPA < 0 || GPA > 4.0) throw GradeException("GPA must be between 0 and 4.0.  Given value was: " + to_string(g));
    }

    void display_details() const override {
        cout << "Student: " << name << ", ID: " << id << ", Age: " << age
            << ", Enrollment Date: " << enrollment_date << ", Program: " << program
            << ", GPA: " << fixed << setprecision(2) << GPA << endl; //Use stringstream
    }

    void enroll_course(const string& course_code) {
        if (enrolled_courses.size() >= 5)
            throw EnrollmentException("Course limit reached for student: " + id);
        if (find(enrolled_courses.begin(), enrolled_courses.end(), course_code) != enrolled_courses.end())
            throw EnrollmentException("Student " + id + " is already enrolled in course " + course_code);
        enrolled_courses.push_back(course_code);
    }

    vector<string> get_courses() const { return enrolled_courses; }

    double calculate_payment() const override {
        return 5000.0;
    }
};

class GraduateStudent : public student {
    string advisor, thesis_title;

public:
    GraduateStudent(string n, int a, string i, string c, date d, string p, float g,
        string adv, string thesis)
        : student(n, a, i, c, d, p, g), advisor(adv), thesis_title(thesis) {
        if (adv.empty()) throw UniversitySystemException("Advisor cannot be empty.");
        if (thesis.empty()) throw UniversitySystemException("Thesis title cannot be empty.");
    }

    void display_details() const override {
        student::display_details();
        cout << "Graduate | Advisor: " << advisor << ", Thesis: " << thesis_title << endl;
    }

    double calculate_payment() const override {
        return 3500.0;
    }
};

class professor : public person {
protected:
    string specialization;
    date hire_date;
    double base_salary;

public:
    professor(string n, int a, string i, string c, string spec, date h, double salary)
        : person(n, a, i, c), specialization(spec), hire_date(h), base_salary(salary) {
        if (spec.empty()) throw UniversitySystemException("Specialization cannot be empty.");
        if (salary < 0) throw PaymentException("Salary cannot be negative. Given value was: " + to_string(salary));
    }

    void display_details() const override {
        cout << "Professor: " << name << ", ID: " << id << ", Specialization: " << specialization
            << ", Hire Date: " << hire_date << ", Salary: " << fixed << setprecision(2) << base_salary << endl;
    }

    double calculate_payment() const override {
        return base_salary;
    }
};

class course {
private:
    string code, title;
    float credits;
    string description;
    professor* instructor;

public:
    course(string code, string title, float credits, string desc, professor* prof)
        : code(code), title(title), credits(credits), description(desc), instructor(prof) {
        if (code.empty()) throw UniversitySystemException("Course code cannot be empty.");
        if (title.empty()) throw UniversitySystemException("Course title cannot be empty.");
        if (credits <= 0) throw UniversitySystemException("Credits must be positive. Given value was: " + to_string(credits));
        if (desc.empty()) throw UniversitySystemException("Course description cannot be empty.");
    }

    string get_code() const { return code; }
    string get_title() const{return title;}

    void display_course() const {
        cout << "Course: " << title << " (" << code << ") - " << fixed << setprecision(1) << credits << " credits\n";
        cout << "Description: " << description << endl;
        if (instructor) {
            cout << "Instructor: ";
            instructor->display_details();
        }
        else {
            cout << "No instructor assigned.\n";
        }
    }
};

class GradeBook {
private:
    map<string, float> grades;

public:
    void add_grade(string student_id, float grade) {
        if (grade < 0 || grade > 100)
            throw GradeException("Grade must be between 0 and 100. Given value was: " + to_string(grade));
        grades[student_id] = grade;
    }

    float get_grade(string student_id) const {
        if (grades.find(student_id) == grades.end())
            throw GradeException("Grade not found for student: " + student_id);
        return grades.at(student_id); // Use at() for bounds checking
    }

    float calculate_average() const {
        if (grades.empty()) return 0;
        float total = 0;
        for (const auto& pair : grades) total += pair.second;
        return total / grades.size();
    }

    void display_all_grades() const {
        if (grades.empty()) {
            cout << "No grades available.\n";
            return;
        }
        for (const auto& pair : grades)
            cout << "Student ID: " << pair.first << ", Grade: " << fixed << setprecision(2) << pair.second << endl;
    }
};

class EnrollmentManager {
private:
    map<string, vector<string>> course_students;

public:
    void enroll(string course_code, string student_id) {
        auto& students = course_students[course_code]; // Get the vector of students for the course
        if (find(students.begin(), students.end(), student_id) != students.end())
            throw EnrollmentException("Student " + student_id + " is already enrolled in course " + course_code);
        if (students.size() >= 50)
            throw EnrollmentException("Course " + course_code + " is full (Max 50 students).");
        students.push_back(student_id); // Add the student's ID to the vector
    }

    void drop(string course_code, string student_id) {
        auto& students = course_students[course_code];
        auto it = find(students.begin(), students.end(), student_id);
        if (it == students.end()) throw EnrollmentException("Student " + student_id + " not enrolled in course " + course_code);
        students.erase(it);
    }

    void display_enrollment(string course_code) const {
        cout << "Students enrolled in " << course_code << ": ";
        const auto& students = course_students.find(course_code); // Find using find()
        if (students == course_students.end() || students->second.empty()) {
            cout << "None";
        }
        else {
            for (const auto& id : students->second) {
                cout << id << " ";
            }
        }
        cout << endl;
    }
    vector<string> get_enrolled_students(const string& courseCode) const {
        auto it = course_students.find(courseCode);
        if (it != course_students.end()) {
            return it->second;
        }
        return {}; // Return an empty vector if the course doesn't exist or has no students.
    }
};

class UniversitySystem {
private:
    vector<student*> students;
    vector<professor*> professors;
    vector<course*> courses;
    GradeBook gradebook;
    EnrollmentManager enrollment_mgr;

public:
    ~UniversitySystem() {
        for (auto s : students) delete s;
        for (auto p : professors) delete p;
        for (auto c : courses) delete c;
        students.clear();
        professors.clear();
        courses.clear();
    }

    void add_student(student* s) {
         if (s == nullptr) {
            throw UniversitySystemException("Cannot add null student.");
         }
        for (const auto& studentPtr : students) {
            if (studentPtr->get_id() == s->get_id()) {
                throw UniversitySystemException("Student with ID " + s->get_id() + " already exists.");
            }
        }
        students.push_back(s);
    }
    void add_professor(professor* p) {
        if (p == nullptr) {
            throw UniversitySystemException("Cannot add null professor.");
        }
        for (const auto& profPtr : professors) {
            if (profPtr->get_id() == p->get_id()) {
                throw UniversitySystemException("Professor with ID " + p->get_id() + " already exists.");
            }
        }
        professors.push_back(p);
    }
    void add_course(course* c) {
        if (c == nullptr) {
            throw UniversitySystemException("Cannot add null course.");
        }
        for (const auto& coursePtr : courses) {
            if (coursePtr->get_code() == c->get_code()) {
                throw UniversitySystemException("Course with code " + c->get_code() + " already exists.");
            }
        }
        courses.push_back(c);
    }

    void enroll_student(const string& course_code, const string& student_id) {
        // Check if the course exists
        bool course_exists = false;
        for (const auto* c : courses) {
            if (c->get_code() == course_code) {
                course_exists = true;
                break;
            }
        }
        if (!course_exists) {
            throw EnrollmentException("Course with code " + course_code + " does not exist.");
        }

        // Check if the student exists
        bool student_exists = false;
        for (const auto* s : students) {
            if (s->get_id() == student_id) {
                student_exists = true;
                break;
            }
        }
        if (!student_exists) {
            throw EnrollmentException("Student with ID " + student_id + " does not exist.");
        }
        enrollment_mgr.enroll(course_code, student_id);
        for (auto* s : students) {
            if (s->get_id() == student_id) {
                s->enroll_course(course_code);
                break;
            }
        }
    }

    void assign_grade(const string& student_id, float grade) {
        // Check if the student exists before assigning a grade.
        bool student_exists = false;
         for (const auto* s : students) {
            if (s->get_id() == student_id) {
                student_exists = true;
                break;
            }
        }
        if (!student_exists) {
             throw GradeException("Student with ID: " + student_id + " does not exist.");
        }
        gradebook.add_grade(student_id, grade);
    }

    void report_all_students() const {
        if (students.empty()) {
            cout << "No students available.\n";
            return;
        }
        cout << "\n--- All Students ---\n";
        for (const auto* s : students) s->display_details();
    }

    void report_all_courses() const {
        if (courses.empty()) {
            cout << "No courses available.\n";
            return;
        }
        cout << "\n--- All Courses ---\n";
        for (const auto* c : courses) c->display_course();
    }

    void report_grades() const {
        cout << "\n--- All Grades ---\n";
        gradebook.display_all_grades();
    }
     void display_course_enrollment(const string& courseCode) const {
        enrollment_mgr.display_enrollment(courseCode);
    }

    void menu() {
        int choice;
        do {
            cout << "\n=== University System Menu ===\n";
            cout << "1. Show All Students\n";
            cout << "2. Show All Courses\n";
            cout << "3. Enroll Student in Course\n";
            cout << "4. Assign Grade to Student\n";
            cout << "5. Show All Grades\n";
            cout << "6. Show Course Enrollment\n";
            cout << "7. Exit\n";
            cout << "Choose: ";
            cin >> choice;

            try {
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw UniversitySystemException("Invalid input type. Please enter a number.");
                }
                string id, code;
                float grade;
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

                switch (choice) {
                case 1:
                    report_all_students();
                    break;
                case 2:
                    report_all_courses();
                    break;
                case 3:
                    cout << "Enter Course Code: ";
                    getline(cin, code);
                    cout << "Enter Student ID: ";
                    getline(cin, id);
                    enroll_student(code, id);
                    cout << "Enrollment successful.\n";
                    break;
                case 4:
                    cout << "Enter Student ID: ";
                    getline(cin, id);
                    cout << "Enter Grade: ";
                    cin >> grade;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw GradeException("Invalid grade input.  Please enter a number between 0 and 100.");
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    assign_grade(id, grade);
                    cout << "Grade assigned successfully.\n";
                    break;
                case 5:
                    report_grades();
                    break;
                case 6:
                    cout << "Enter Course Code: ";
                    getline(cin, code);
                    display_course_enrollment(code);
                    break;
                case 7:
                    cout << "Exiting...\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
                }
            }
            catch (const UniversitySystemException& e) {
                cerr << "Exception: " << e.what() << endl;
            }
        } while (choice != 7);
    }
};

int main() {
    try {
        UniversitySystem uni;

        date d1(1, 1, 2020);
        date d2(1, 6, 2021);
        date d3(15, 8, 2022);

        professor* prof1 = new professor("Dr. Smith", 50, "P001", "1234567890", "AI", d1, 12000.0);
        professor* prof2 = new professor("Dr. Johnson", 45, "P002", "9876543210", "Networks", d2, 11000.0);
        professor* prof3 = new professor("Dr. Brown", 60, "P003", "5555555555", "Security", d3, 13000.0);
        uni.add_professor(prof1);
        uni.add_professor(prof2);
        uni.add_professor(prof3);

        course* cs101 = new course("CS101", "Intro to AI", 3.0, "Fundamentals of AI", prof1);
        course* cs102 = new course("CS102", "Computer Networks", 3.5, "Networking basics", prof2);
        course* cs201 = new course("CS201", "Network Security", 4.0, "Advanced Network Security", prof3);
        uni.add_course(cs101);
        uni.add_course(cs102);
        uni.add_course(cs201);

        student* s1 = new GraduateStudent("Alice", 24, "S001", "9999999999", d1, "M.Tech", 3.8, "Dr. Smith", "AI & Ethics");
        student* s2 = new GraduateStudent("Bob", 25, "S002", "8888888888", d2, "M.Tech", 3.5, "Dr. Johnson", "Cyber Defense");
        student* s3 = new student("Charlie", 20, "S003", "7777777777", d1, "B.Sc", 3.2);
        uni.add_student(s1);
        uni.add_student(s2);
        uni.add_student(s3);

        uni.enroll_student("CS101", "S001");
        uni.enroll_student("CS101", "S002");
        uni.enroll_student("CS102", "S002");
        uni.enroll_student("CS201", "S003");

        uni.assign_grade("S001", 95.0);
        uni.assign_grade("S002", 88.0);
        uni.assign_grade("S003", 75.0);

        uni.menu();
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1; // Use a non-zero return code to indicate failure
    }
    return 0;
}
