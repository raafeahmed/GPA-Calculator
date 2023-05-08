#include "calculate.h"

GPA::GPA(){}

//reads in the different queries that can be performed
void GPA::readInput(){
    std::string command;
    std::string input;

    do{
        std::cout << "% ";
        std::cin >> command;
        switch (command[0])
        {
        //creates a semester by first checkng that it doesn't already exist, then proceeds to insert in the transcript unordered map
        case 'C':{
            std::cin >> input;
            auto it = transcript.find(input);
            if(it != transcript.end()){
                std::cout << "Error during " << command << ": Cannot create already existing semester " << input << "\n";
                break;
            }
            Semester semester;
            transcript.insert({input, semester});
            std::cout << "New semester " << input << " created\n";
            break;
        }
        //inserting the list of entries into a semester
        case 'I':{
            insert(command);
            break;
        }
        //print
        case 'P':
            std::cin >> input;
            switch (input[0])
            {
            //prints a specified semester's number of credits, quality points, and gpa
            case 's':{
                std::cin >> input >> input >> input;
                auto it = transcript.find(input);
                if(!exists(command, input, it)){
                    break;
                }
                std::cout << input << " total:\n";
                std::cout << "Credits: " << it->second.total_credits << "\n";
                std::cout << "Quality points: " << it->second.quality_points << "\n";
                std::cout << "GPA: " << it->second.gpa << "\n";
                break;
            }
            //prints cumulative gpa
            case 'c':
                printCumulative(command);
                break;
            default:
                break;
            }
            break;
        //delete specified entries from a semester
        case 'D':
            deleteEntry(command);
            break;
        //removes an entire semester by first checking whether it exists, and then erasing it from the transcript
        case 'R':{
            std::cin >> input;
            auto it = transcript.find(input);
            if(!exists(command, input, it)){
                break;
            }
            transcript.erase(command);
            std::cout << "Removed " << input << "\n";
            break;
        }
        //quit program
        case 'Q':
            break;
        default:
            std::cout << "Error: unrecognized command\n";
            getline(std::cin, command);
        }
    }while(command != "QUIT");

    std::cout << "Keep up the great work!\n";
}

//helper function that checks whether a semester exists or not in the transcript
bool GPA::exists(std::string &command, std::string &input, std::unordered_map<std::string, Semester>::iterator &it){
    if(it == transcript.end()){
        std::cout << "Error during " << command << ": semester " << input << " does not exist\n";
        getline(std::cin, input);
        return false;
    }
    return true;
}

//reads in and adds each entry. Once all entries have been added, proceeds to calculate semester credits, quality points, and gpa
void GPA::insert(std::string &command){
    Entry entry;
    uint32_t num_entries;
    std::string input;

    std::cin >> input >> input;
    auto it = transcript.find(input);
    if(!exists(command, input, it)){
        return;
    }

    std::cin >> num_entries;
    std::cin >> input;
    it->second.entries.reserve(num_entries);

    for(uint32_t i = 0; i < num_entries; ++i){
        std::cin >> entry.course_name >> entry.grade >> entry.credits;
        it->second.entries.push_back(entry);
    }
    std::cout << "Inserted " << num_entries << " entries into " << it->first << "\n";
    calculate_semester(it->second);
}

//calculates and stores credits, quality points, and gpa for each semester
void GPA::calculate_semester(Semester &semester){
    semester.total_credits = 0;
    semester.quality_points = 0;
    semester.gpa = 0;

    for(uint32_t i = 0; i < semester.entries.size(); ++i){
        auto it = grade_value.find(semester.entries[i].grade); //grade_value maps letter grades to their point value
        if(it == grade_value.end()){
            continue;
        }
        semester.total_credits += semester.entries[i].credits;
        semester.quality_points += it->second * semester.entries[i].credits;   
    }
    semester.gpa = semester.quality_points / semester.total_credits;
}

//prints cumulative gpa for specified semesters or for all semesters contained in the transcript
void GPA::printCumulative(std::string &command){
    std::string input;
    double cumulative_points = 0;
    double cumulative_credits = 0;
    double cumulative_gpa = 0;

    std::cin >> input >> input;
    switch (input[0])
    {
    //specified semesters. Checks that it exists, then adds the semester's quality points and credits to their cumulative totals, respectively
    case 'F':{
        uint32_t num_semesters;
        std::vector<std::string> semester_names;
        
        std::cin >> num_semesters;
        std::cin >> input;
        semester_names.reserve(num_semesters);

        for(uint32_t i = 0; i < num_semesters; ++i){
            std::cin >> input;
            auto it = transcript.find(input);
            if(!exists(command, input, it)){
                return;
            }
            semester_names.push_back(input);
            cumulative_points += it->second.quality_points;
            cumulative_credits += it->second.total_credits;
        }
        cumulative_gpa = cumulative_points / cumulative_credits;

        std::cout << "Cumulative total for ";
        for(uint32_t i = 0; i < semester_names.size(); ++i){
            std::cout << semester_names[i] << " ";
        }
        std::cout << "\n";
        break;
    }
    //traverses all semesters in the transcript, performing the calculations
    case 'A':
        for(auto x : transcript){
            cumulative_points += x.second.quality_points;
            cumulative_credits += x.second.total_credits;
        }
        cumulative_gpa = cumulative_points / cumulative_credits;

        std::cout << "Cumulative total:\n";
        break;
    default:
        break;
    }

    std::cout << "Credits: " << cumulative_credits << "\n";
    std::cout << "Quality points: " << cumulative_points << "\n";
    std::cout << "GPA: " << cumulative_gpa << "\n"; 
} 

//deletes specific entries from a semester. Utilizes a specific functor based off what entry type(course, grade, credits) is read in, 
//and proceeds to erase the entries from the semester. Finally, recalculates credits, quality points, and gpa for that semester.
void GPA::deleteEntry(std::string &command){
    Entry entry;
    std::string input;
    uint32_t num_deleted = 0;
    char c;

    std::cin >> input >> input;
    auto it = transcript.find(input);
    if(!exists(command, input, it)){
        return;
    }
   
    num_deleted = uint32_t(it->second.entries.size());
    std::cin >> input >> input;

    if(input == "course"){
        std::cin >> c;
        std::cin >> entry.course_name;
        CourseComp course(entry.course_name);
        it->second.entries.erase(remove_if(it->second.entries.begin(), it->second.entries.end(), course), it->second.entries.end());
    }
    else if (input == "grade"){
        std::cin >> c;
        std::cin >> entry.grade;
        GradeComp grade(entry.grade);
        it->second.entries.erase(remove_if(it->second.entries.begin(), it->second.entries.end(), grade), it->second.entries.end());
    }
    else if (input == "credits"){
        std::cin >> c;
        std::cin >> entry.credits;
        CreditComp credit(entry.credits);
        it->second.entries.erase(remove_if(it->second.entries.begin(), it->second.entries.end(), credit), it->second.entries.end());
    }
    else{
        std::cout << "Error during " << command << ": " << input << " is not a valid entry type\n";
        getline(std::cin, input);
        return;
    }
    std::cout << "Deleted " << num_deleted - it->second.entries.size() << " entries from " << it->first << "\n";
    if(num_deleted - it->second.entries.size() > 0){
        calculate_semester(it->second);
    }
}

GPA::~GPA(){}

int main(){
    std::ios_base::sync_with_stdio(false);
    GPA gpa;
    std::cout.precision(4);
    gpa.readInput();
}