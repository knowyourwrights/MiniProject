// Zinovii Kava
// 19.03.2026
// lab8

#include <iostream>

using namespace std;

class Student {
private:
    string name;// private data member
    int age;// private data member
    double grade;



public:
    // default constructor
    Student() { name = "Unknown", age = 0, grade = 0.0; }

    // parameterised constructor
    Student(string n, int a, double g) { name = n, age = a, grade = g; }
 

    string getName () 
    {
        return name;
	}
    int getAge()
    {
        return age;
    }
    double getGrade()
    {
        return grade;
    }

     void getData() // member function
    {
        cout << "   "<< name << age << grade << endl;

    }


};

int main()
{
	Student s1("Alice Johnson", 20, 88.5);


	Student s2("Bob Smith", 19, 75.0);

	Student s;  

    cout << "-------------------------------------------------------------------------------- " << endl;
    cout << "| Name           		| Age 		| Grade		| " << endl;
    cout << "-------------------------------------------------------------------------------- " << endl;

    s1.getData() ;
    s2.getData() ;
    s.getData() ;

    cout << "-------------------------------------------------------------------------------- " << endl;



    return 0;
}






