#ifndef CALCULATE_H
#define CALCULATE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

struct Entry
{
    std::string course_name;
    std::string grade;
    double credits;
};

struct Semester
{
    std::vector<Entry> entries;
    double semester_hours;
    double quality_points;
    double gpa;
    double total_credits;
};

class CourseComp
{
private:
    std::string value;

public:
    CourseComp(std::string &value_in) : value(value_in) {}
    bool operator()(Entry a)
    {
        return a.course_name == value;
    }
};

class GradeComp
{
private:
    std::string value;

public:
    GradeComp(std::string &value_in) : value(value_in) {}
    bool operator()(Entry a)
    {
        return a.grade == value;
    }
};

class CreditComp
{
private:
    double value;

public:
    CreditComp(double &value_in) : value(value_in) {}
    bool operator()(Entry a)
    {
        return a.credits == value;
    }
};

class GPA
{
private:
    std::unordered_map<std::string, Semester> transcript;
    const std::unordered_map<std::string, double> grade_value =
        {{"A+", 4.0}, {"A", 4.0}, {"A-", 3.7}, {"B+", 3.3}, {"B", 3.0}, {"B-", 2.7}, {"C+", 2.3}, {"C", 2.0}, {"C-", 1.7}, {"D+", 1.3}, {"D", 1.0}, {"D-", 0.7}, {"E", 0.0}, {"P", 0.0}};

public:
    GPA();
    void readInput();
    bool exists(std::string &command, std::string &input, std::unordered_map<std::string, Semester>::iterator &it);
    void insert(std::string &command);
    void calculate_semester(Semester &semester);
    double printCumulative(std::string &command);
    void deleteEntry(std::string &command);
    ~GPA();
};

#endif