/**------------------------------------------
    Program 3: Typing Tutor

    Course: CS 141, Spring 2023.
    System: Linux x86_64 and C++
    Author: Soham Kaje, starter code provided by UIC Professors George Maratos and
    David Hayes
 ---------------------------------------------**/

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

const int NMISP = 5;
const int RNG_RANGE = 100;

void display_header(const int ndict, const int nmiss) {
  /* This function displays the information of how many words
  are in the dictionary and how many commonly misspelled words
  there are. The parameters are calling the counts of the
  dictionary words and the misspelled words. */

  cout << "*** Typing Tutor" << endl;
  cout << "There are " << ndict << " words in the dictionary." << endl;
  cout << "There are " << nmiss << " commonly misspelled words." << endl;
}

void display_menu_options() {
  /* This function prints out all the menu options, it is
  given to you as part of the starter code. If you change
  it,  you run the  risk of failing  an early test of the
  autograder.*/

  cout << endl
       << "*** Select a menu option:" << endl
       << "1. Spell commonly misspelled words" << endl
       << "2. Type random words from the full dictionary" << endl
       << "3. Display some words from one of the lists" << endl
       << "4. Use binary search to lookup a word in the dictionary" << endl
       << "5. Exit the program." << endl
       << "Your choice: ";
}

vector<int> randn(const int n, const int max) {
  /* This function creates and returns a vector of n length
  that holds different random numbers in the range from 0 to
  max. The parameters are n (how big the vector should be) and
  max (highest possible random number added to vector). */

  vector<int> temp;

  // for loop to append n amount of random numbers to the vector.
  for (int i = 0; i < n; i++) {
    int rnum = rand() % (max + 1);
    temp.push_back(rnum);
  }

  return temp;
}

// function to open given file in the pass-by-reference parameter (src_file).
ifstream open(const string &src_file) {
  ifstream src{src_file};
  // if statement to check whether file is open or not.
  if (!src.is_open()) {
    cout << "Could not find file " << src_file << "." << endl;
    exit(1);
  }
  return src;
}

void extract_dict(vector<string> &dst, const string &src_file) {
  /* This function reads data from the given file and separates each
  line to its own element in a pass-by-reference vector. The parameters
  are dst which is the vector that contains each line of the file as
  its own element in the end and const src_file which is the name of
  the file that is being read and extracted into the vector. */

  ifstream file;
  file = open(src_file); // open given file

  // for loop to read each line and append to dst vector.
  for (string line; getline(file, line);) {
    dst.push_back(line);
  }
}

void extract_misspelled(vector<string> &misspelled, vector<string> &correct,
                        const string &src_file) {
  /* This function reads data from the given file and separates each
  line to its own element in two pass-by-reference vectors. The parameters
  are misspelled which is the vector that contains each commonly misspelled
  word of the file as its own element in the end, correct which is the
  vector that contains the correct spelling of the misspelled word of the
  file in the same element, and const src_file which is the name of the
  file that is being read and extracted into the vector. */

  ifstream file;
  // calls open function to open the given file.
  file = open(src_file);
  string word = "";

  // nested for loop to add each commonly misspelled/correctly spelled word
  // to its respective vector.
  for (string line; getline(file, line);) {
    for (int i = 0; i < line.size(); i++) {
      if (line.at(i) != ' ') {
        word += line.at(i);
      } else if (line.at(i) == ' ') {
        misspelled.push_back(word);
        word = "";
      }
    }
    correct.push_back(word);
    word = "";
  }
}

void display_elems(const vector<string> &src, const vector<int> &positions) {
  /* This function prints out random words by using the random numbers
  from the pass-by-reference vector positions and printing that element
  in the pass-by-reference vector src with spaces in between each word.
  The parameters are a const pass-by-reference string vector src which holds
  the words that are to be randomly generated and a const pass-by-reference
  int vector that holds the indices for the elements that are to be
  printed in vector src. */

  // for loop to print out the randomly generated words.
  for (int i = 0; i < positions.size(); i++) {
    if (i < positions.size() - 1) {
      cout << src.at(positions.at(i)) << " ";
    } else if (i == positions.size() - 1) {
      cout << src.at(positions.at(i)) << endl;
    }
  }
}

void extract_spelling_answers(vector<string> &dst,
                              const vector<string> &correct,
                              const vector<int> positions) {
  /* This function appends the correct answers into a vector dst by
  accessing and adding the specific indices (positions) from a vector
  correct. The parameters are a pass-by-reference string vector dst
  which will copy the correct answers, a const pass-by-reference string
  vector correct which holds all the correct vectors, and a const pass-by-
  reference int vector positions which holds the specific indices
  that give access to the correct words. */

  // for loop that appends the correct words into vector dst
  for (int i = 0; i < positions.size(); i++) {
    dst.push_back(correct.at(positions.at(i)));
  }
}

void extract_spelling_mistakes(vector<string> &dst, const string &to_grade,
                               const vector<string> &correct) {
  /* This function compares whether the student answer is correct or incorrect.
  If the answer for a word is incorrect, the function will display the student's
  spelling of the word and the correct spelling of the word to show the student
  how they misspelled the word. The parameters are a pass-by-reference string
  vector dst which is filled with the spelling errors (in the format misspelled
  words -> correct spelling), a const pass-by-reference string to_grade
  which holds the answers that the student gave, and a const pass-by-reference
  string vector correct which holds the correct answers which you compare with
  the student answers from to_grade. */

  vector<string> student;
  string word;

  // for loop to separate the string of answers into a vector where each element
  // is a word.
  for (int i = 0; i < to_grade.size(); i++) {
    if (to_grade.at(i) != ' ') {
      word += to_grade.at(i);
    } else if (to_grade.at(i) == ' ') {
      student.push_back(word);
      word = "";
    }
  }
  student.push_back(word);

  // for loop to find misspelled words and add to vector dst in the format
  // misspelled word -> correctly spelled word.
  string add = "";
  for (int i = 0; i < student.size(); i++) {
    if (student.at(i) != correct.at(i)) {
      add = student.at(i) + " -> " + correct.at(i);
      dst.push_back(add);
      add = "";
    }
  }
}

void report_misspell_mistakes(const vector<string> &mistakes,
                              const int num_words) {
  /*  This function prints out the spelling mistakes to the user and
  lets the user know how many points they scored based on the points
  system for correctly spelled words versus misspelled words. The
  parameters are a const pass-by-reference string vector mistakes which
  holds the amount of misspelled words and which ones they are, and a
  const integer num_words which holds the total number of words that
  were tested. */

  // if statement to tell the user whether they made mistakes or not.
  if (mistakes.size() == 0) {
    cout << "No misspelled words!" << endl;
  } else {
    cout << "Misspelled words: " << endl;
  }

  // for loop to print out the mistakes that the user made (if any).
  for (int i = 0; i < mistakes.size(); i++) {
    cout << "    " << mistakes.at(i) << endl;
  }

  // declaring and initializing variables for outputing the points system
  // and the calculated score for the user.
  int rightwords = num_words - mistakes.size();
  int rightpoints = 3 * (num_words - mistakes.size());
  int wrongwords = mistakes.size();
  int wrongpoints = -6 * (mistakes.size());
  int wrongpointssentence = wrongpoints * -1;
  int score = rightpoints + wrongpoints;

  // prints the points system and the score that the user received.
  cout << rightpoints << " points: " << rightwords
       << " spelled correctly x 3 points for each." << endl;
  cout << wrongpointssentence << " point penalty: " << wrongwords
       << " spelled incorrectly x -6 points for each." << endl;
  cout << "Score: " << score << endl;
}

void evaluate_user_misspells(const vector<string> &src,
                             const vector<int> &positions) {
  /* This function reads the user input for the typing test and
  reports the mistakes using other functions. The parameters are
  a const pass-by-reference string vector src which holds the
  data that the user is tested on and a const pass-by-reference
  integer vector positions which holds the indices of the elements
  that are being compared to the user's input. */

  string input;
  getline(cin, input);
  vector<string> rightAnswers;
  // calls function to call the correct answers.
  extract_spelling_answers(rightAnswers, src, positions);
  vector<string> userAnswers;
  // calls function to find the mistakes from the user input.
  extract_spelling_mistakes(userAnswers, input, rightAnswers);
  // calls function to output the mistakes and score results.
  report_misspell_mistakes(userAnswers, NMISP);
}

void misspelled(const vector<string> &mspelled, const vector<string> &correct) {
  /* This function prints the structure of "Menu Option 1: Spelling
  Misspelled" by printing the starting message, displaying 5 random words,
  and evaluating the user's input. The parameters are a const pass-by-
  reference string vector mspelled that holds the commonly misspelled words in
  each index and aconst pass-by-reference string vector correct that
  holds the correct spelling for that word in the same index. */

  cout << endl;
  cout << "*** Using commonly misspelled words." << endl;
  // creates integer vector of 5 random numbers by calling randn function.
  vector<int> randomNumbers = randn(NMISP, mspelled.size() - 1);
  // calls function to display the commonly misspelled words.
  display_elems(mspelled, randomNumbers);
  cout << "Type the words above with the correct spelling:" << endl;
  // calls function to read user's input and evaluate a score using the points
  // system.
  evaluate_user_misspells(correct, randomNumbers);
  cout << endl;
}

void full_dict(const vector<string> &dict) {
  /* This function prints the structure of "Menu Option 2: Typing from
  Dictionary" by printing the starting message, displaying 5 random words
  from the dictionary, and evaluating the user's input. The parameter
  is a const pass-by-reference string vector dict that holds all of the
  words in the dictionary as its own element. */

  cout << endl;
  cout << "*** Using full dictionary." << endl;
  // creates integer vector of 5 random numbers by calling randn function.
  vector<int> randomNumbers = randn(NMISP, dict.size() - 1);
  // calls function to display the selected words from the dictionary.
  display_elems(dict, randomNumbers);
  cout << "Correctly type the words above:" << endl;
  // calls function to read user's input and evaluate a score using the points
  // system.
  evaluate_user_misspells(dict, randomNumbers);
  cout << endl;
}

void display_words_prompt() {
  // This function displays the data set choices for menu option 3
  cout << endl
       << "*** Displaying word list entries." << endl
       << "Which words do you want to display?" << endl
       << "A. Dictionary of all words" << endl
       << "B. Commonly misspelled words (wrongly spelled)" << endl
       << "C. Commonly misspelled words (correctly spelled)" << endl
       << "Your choice: ";
}

void print_range(const vector<string> &data) {
  /* This function displays the starting message for "Menu Option 3: Word
  Range", reads the user's range, makes sure that the range is valid, and
  prints the words in the requested range. The parameter is a const pass-
  by-reference string vector data that holds the words that will be printed
  based on the range given by the user. */

  int firstNumber;
  int secondNumber;
  // asks user for the range that they want to find in the vector.
  cout << "*** Enter the start and end indices between 0 and "
       << (data.size() - 1) << ": ";
  cin >> firstNumber >> secondNumber;

  // checks for valid range given.
  if (firstNumber > secondNumber) {
    cout << "The end index must come after the start, and the indices must be "
            "in range."
         << endl;
  } else if ((firstNumber < 0) || (secondNumber > data.size() - 1)) {
    cout << "The end index must come after the start, and the indices must be "
            "in range."
         << endl;
  } else {
    // for loop to print the words in the range.
    for (int i = firstNumber; i <= secondNumber; i++) {
      cout << i << ". " << data.at(i) << endl;
    }
  }
}

void display_in_range(const vector<string> &dict,
                      const vector<string> &mspelled,
                      const vector<string> &correct) {
  /* This function takes in an input from the user to see which vector
  they want to get the range of and calls the print_range function to
  print the range. The parameters are all the different vectors that
  hold the data for each list. */

  char choice;
  display_words_prompt();
  cin >> choice;
  choice = toupper(choice);
  // if statement to print the range in the chosen vector.
  if (choice == 'A') {
    print_range(dict);
  } else if (choice == 'B') {
    print_range(mspelled);
  } else if (choice == 'C') {
    print_range(correct);
  }
}

void bsearch(const vector<string> &data) {
  /* This function reads a word to search for and runs a Binary Search
  Algorithm to search for the word in the dictionary. At each attempt,
  the selected word is printed and is compared to the target word. The
  parameter is a const pass-by-reference string vector data which holds
  all the words from the dictionary. */

  string target;
  // asks user for the word that they want to find.
  cout << "*** Enter word to search: ";
  cin >> target;

  bool binary_search = false;
  int start = 0;
  int mid;
  string elem;
  int count = 1;
  int size = data.size();
  // do while loop for running the binary search algorithm.
  do {
    mid = (start + size) / 2;
    elem = data.at(mid);
    // if statement to fix the spacing before printing the selected word.
    if (count < 10) {
      cout << "    " << count << ". Comparing to: " << elem << endl;
    } else if (count >= 10) {
      cout << "   " << count << ". Comparing to: " << elem << endl;
    }
    // if statement to reset the variables based on whether the selected
    // word is less than, greater than, or equal to the word that the program
    // is trying to find.
    if (elem < target) {
      start = mid + 1;
    } else if (elem > target) {
      size = mid;
    } else {
      binary_search = true;
    }
    count += 1;
  } while ((start < size) && (elem != target));

  // if statement to inform the user whether the word was found or not.
  if (binary_search == true) {
    cout << target << " was found." << endl;
  } else {
    cout << target << " was NOT found." << endl;
  }
}

// This function changes each txt.file into vectors.
void extract_all_data(vector<string> &dict, vector<string> &mspelled,
                      vector<string> &correct) {
  extract_dict(
      dict, "dictionary.txt"); // change dictionary.txt file into a vector dict.
  extract_misspelled(mspelled, correct,
                     "misspelled.txt"); // change misspelled.txt file into two
                                        // vectors, mspelled and correct.
}

int main() {
  srand(1);
  vector<string> dict, mspelled, correct;
  extract_all_data(dict, mspelled, correct);
  display_header(dict.size(), correct.size());
  unsigned int choice;
  do { // do while loop to run program until the user chooses the exit.
    display_menu_options();
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    switch (choice) {
    case 1: // menu option 1
      misspelled(mspelled, correct);
      break;
    case 2: // menu option 2
      full_dict(dict);
      break;
    case 3: // menu option 3
      display_in_range(dict, mspelled, correct);
      break;
    case 4: // menu option 4
      bsearch(dict);
      break;
    case 5: // exit program
      cout << "Exiting." << endl;
      break;
    default:
      cout << "No menu option " << choice << ". "
           << "Please try again." << endl;
    }
  } while ((choice != 5) && !cin.eof());

  return 0;
}
