GPA Calculator
==============
GPA calculation tool designed to efficiently calculate and manage semester and cumulative GPAs.

Functionalities
=============
The program reads in various commands through an interactive shell. The following is the specific syntax needed for the program to run correctly:
- CREATE **semester name*
    - The program supports the creation of a semester table, which organizes and manages course information effectively. Each entry in the table consists of the course name, grade, and credit hours, facilitating convenient data entry and retrieval.
- INSERT INTO **semester name* **number of entries* entries
    - Inserts specified number of entries into the specific semester
    - An entry is defined as: **course name* **letter grade* **credits*
- PRINT semester gpa FROM **semester name*
- PRINT cumulative gpa <ALL | FROM > **number of semesters* semesters: **semester name* **semester name*...
    - The GPA Calculator offers flexible reporting options. Users can obtain detailed information, such as the semester GPA, total credits, quality points, and cumulative GPA for specified semesters or for all semesters stored in the transcript.
- DELETE FROM **semester name* WHERE <course | grade | credits> = **specified entry type*
    - To provide efficient data management, users can delete specific entries based on course name, grade, or credit hours. The program ensures accurate recalculation of semester credits, quality points, and GPA after the deletion.
- REMOVE **semester name*
    - Removes all semester data from the transcript
- QUIT
    - exits the program
