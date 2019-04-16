#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class gpaCalc
{
public:
    gpaCalc();

    gpaCalc(char, float, float, float, int, int);

    void setletterGrade(char);
    char getletterGrade();

    void settotalPoints(float);
    float gettotalPoints();         //get users GPA as entered from last class?

    void setgradePoints(float);
    float getgradePoints();

    void setGPA(float);
    float getGPA();

    void setnumClasses(int);
    int getnumClasses();

    void setcreditHours(int);
    int getcreditHours();

    void readFile2(string);

    void gpaCalculator();

    void addGradesToTotal();



private:
    char letterGrade;
    float totalPoints;
    float gradePoints;
    float GPA;
    int numClasses;
    int creditHours;
    float addGrades[10];
    int addGrades_idx;

};

class StudentID
{

public:
    //public methods that are accessible by the user.
    StudentID();

    StudentID(string, int, string, float);

    void setName(string);
    string getName();

    void setAge(int);
    int getAge();

    void setYear(string);
    string getYear();

    void setGPA(float);
    float getGPA();

    void readFile1(string);

    void userInterface();

    void searchFile(string);



private:
    //private data members used in StudentID class.
    string name;
    int age;
    string year;
    float GPA;

};

int main()
{
    //creating object for StudentID class to call and use my functions.
    StudentID object("Stephen", 3, "Cheese", 1.0);

    object.readFile1("FirstReadIn.txt");
    object.userInterface();
    object.searchFile("test.txt");


    //object for gpaCalc class.
    gpaCalc object2;
    object2.readFile2("2ndReadIn.txt");
    object2.addGradesToTotal();
    object2.gpaCalculator();

}


StudentID::StudentID()
{
    name = "";
    age = -1;
    year = "";
    GPA = -1.0;
}
StudentID::StudentID(string x, int b, string c, float d)
{
    name = x;
    age = b;
    year = c;
    GPA = d;
}
//below are appropriate getters and setters for the private data members of my class StudentID.
//they are used to retrieve or alter data members that would otherwise be unaccessible.
void StudentID::setName(string x)
{
    name = x;
}
string StudentID::getName()
{
    return name;
}
void StudentID::setYear(string c)
{
    year = c;
}
string StudentID::getYear()
{
    return year;
}
void StudentID::setAge(int b)
{
    age = b;
}
int StudentID::getAge()
{
    return age;
}
void StudentID::setGPA(float d)
{
    GPA = d;
}
float StudentID::getGPA()
{
    return GPA;
}
//This function simply reads a prompt for the user welcoming them to my project.
void StudentID::readFile1(string myFile)
{
    ifstream inFile;
    string line;
    inFile.open(myFile);
    while(!inFile.eof())
    {
        if (inFile.fail())
        {
            cout << "File Failed to open." << endl;
        }
        while (getline(inFile, line))
        {
            stringstream ss(line);
            getline(ss, line);
            cout << line << endl;

        }
    }
}
//The userInterface function retrieves and stores information that will later be written to a file. It serves to gain information from
//the user such as their name, age, year, and GPA. The user has the option to store multiple student's information.
void StudentID::userInterface()
{
    int numTimes;
    cout << "How many Student Profiles would you like to create?" << endl;
    cin >> numTimes;
    ofstream myFile;
    myFile.open("test.txt",ios::app);
    if(myFile.is_open())
    {
        for(int i = 0; i < numTimes; i++)
        {
            cout << "What is your name?" << endl;
            cin >> name;
            cout << "How old are you? (integer age value)" << endl;
            cin >> age;
            cout << "Got it. What grade in college are you? (freshman, sophomore...)" << endl;
            cin >> year;
            cout << "What is your current GPA?" << endl;
            cin >> GPA;
            cout << "...Storing Information..." << endl;
            myFile << "Name: " << name << " Age: " << age << " Year: " << year << " GPA: " << GPA << endl;
        }

    }
    else
    {
        cout << "Your File failed to Open" << endl;
    }
}
//this search function opens the file that user information is stored into. It searches for the users name and if it finds a match,
//it prints the whole line containing the students information for the user to see. This is helpful because it shows all of the students
//past GPA's and grade years.
void StudentID::searchFile(string myFile)
{
    ifstream inFile;
    inFile.open(myFile);
    string line;
    cout << "...Searching for Profile..." << endl;
    for (int i = 5; i > 0; i--)
    {
        cout << i << endl;
    }
    cout << "Search completed! Press any key to continue" << endl;
    cout << "*******************************************" << endl;
    char x;
    cin >> x;

    if(inFile.is_open())
    {
        while(getline(inFile, line))
        {
            if (line.find(name)!=string::npos)
            {
                cout << "We have found the following information on record: " << endl;
                cout << line << endl;
            }
        }
        inFile.close();
    }
}

gpaCalc::gpaCalc()
{
    letterGrade = 'z';
    totalPoints = 0;
    gradePoints = 0;
    GPA = -1.0;
    numClasses = -1;
    creditHours = -1;
    addGrades_idx=0;
}
gpaCalc::gpaCalc(char t, float y, float u, float i, int o, int p)
{
    letterGrade = t;
    totalPoints = y;
    gradePoints = u;
    GPA = i;
    numClasses = o;
    creditHours = p;
    addGrades_idx=0;
}
//appropriate getters/setters to access and mutate private data members within my class.
void gpaCalc::setletterGrade(char t)
{
    letterGrade = t;
}

char gpaCalc::getletterGrade()
{
    return letterGrade;
}

void gpaCalc::settotalPoints(float y)
{
    totalPoints = y;
}

float gpaCalc::gettotalPoints()
{
    return totalPoints;
}
void gpaCalc::setgradePoints(float u)
{
    gradePoints = u;
}
float gpaCalc::getgradePoints()
{
    return gradePoints;
}
void gpaCalc::setGPA(float i)
{
    GPA = i;
}
float gpaCalc::getGPA()
{
    return GPA;
}
void gpaCalc::setnumClasses(int o)
{
    numClasses = o;
}
int gpaCalc::getnumClasses()
{
    return numClasses;
}
void gpaCalc::setcreditHours(int p)
{
    creditHours = p;
}
int gpaCalc::getcreditHours()
{
    return creditHours;
}

//readFile2 reads in a file to prompt the user that the GPA calculation process has begun
void gpaCalc::readFile2(string myFile)
{
    char x;
    cout << "Press any character to continue" << endl;
    cout << "*******************************" << endl;
    cin >> x;
    string line;
    ifstream inFile;
    inFile.open(myFile);
    if(inFile.is_open())
    {
        while(getline(inFile, line))
        {
            cout << line << endl;
        }
    }
}

//This function is the one of the essential functions for GPA calculation. It takes in user input in the form of letter grades. When the
//user enters their grades, the according score, gradePoints, is summed using my user-defined array, addGrades, which collects the total
//score for later use. For example, if the user entered a, B, and C, their total points would be 9.0.
void gpaCalc::addGradesToTotal()
{
     cout << "Please enter the grades you would like to calculate and type # when you are done" << endl;
     cin >> letterGrade;
        while (letterGrade != '#')
        {
            if((letterGrade == 'A') || (letterGrade == 'a'))
            {
                gradePoints = 4.0;
                addGrades[addGrades_idx++] = gradePoints;

            }
            else if((letterGrade == 'B') || (letterGrade == 'b'))
            {
                gradePoints = 3.0;
                addGrades[addGrades_idx++] = gradePoints;
            }
            else if((letterGrade == 'C') || (letterGrade =='c'))
            {
                gradePoints = 2.0;
                addGrades[addGrades_idx++] = gradePoints;
            }
            else if((letterGrade == 'D') || (letterGrade == 'c'))
            {
                gradePoints = 1.0;
                addGrades[addGrades_idx++] = gradePoints;
            }
            else if((letterGrade == 'F') || (letterGrade == 'f'))
            {
                gradePoints = 0.0;
                addGrades[addGrades_idx++] = gradePoints;
            }
            cout << "Please enter in another grade! (or # to stop)" << endl;
            cin >> letterGrade;
    }
    return;
}

//gpaCalculator uses the information gathered from the last function. It also takes in the number of classes the user is taking as input.
//The values of addGrades are given to the float value total which is then divided by the number of classes that the user is taking in order to
//calculate an approximation of their GPA. The function lastly informs the user what their GPA would be if they were to receive a 4.0, 3.0, 2.0,
//etc in the next semester.
void gpaCalc::gpaCalculator()
{

    cout << "How many classes will you be taking next semester?" << endl;
    cin >> numClasses;
    float total=0.;

    for(int i=0;i<addGrades_idx;i++)
    {
        total+=addGrades[i];
    }

    GPA = total / numClasses;
    cout << "Your GPA has been calculated to be: " << GPA << endl;
    float grades_f[5]={4.0,3.0,2.0,1.0,0.0};
    float GPA_n=0.;

    for(int x = 0; x < 5; x++)
    {
        cout<<"If you get a GPA of "<<grades_f[x]<<" next semester" << endl;
        GPA_n = (GPA + grades_f[x])/2;
        cout << "Your new GPA would be: " << GPA_n << endl;
    }
}




