#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
using namespace std;

///////////////////////
// Rolodex class
///////////////////////
class Rolodex {
  //node class to hold information
  class Node {
    public: 
      string first_name;
      string last_name;
      string address;
      string phone_num;
  };
  
  private:
  vector<Node*> rolodex_info;
  
  // helper function to print a node's content
  void
  print_entry (Node* entry)
  {
    cout << "=========================================\n" << "First name: " 
         << entry->first_name << "\nLast name: "<< entry->last_name << "\nAddress: " 
         << entry->address << "\nContact number: " << entry->phone_num
         << "\n=========================================\n";
    return;
  }
  
  // helper function to remove the carriage character '\r'
  void
  remove_carriage (string & line)
  {
    if (*line.rbegin() == '\r')
    {
      line.erase(line.length()-1); //remove carriage character
    }
  }
  
  public:
  // constructor
  Rolodex()
  {}
  
  // function to prompt for name
  pair <string, string>
  set_name()
  {
    string first_name;
    string last_name;
    cout << "First name: ";
    cin >> first_name;
    cout << "Last name: ";
    cin >> last_name;
    
    pair <string, string> full_name(first_name, last_name);
    return full_name;
  }
  
  // function to prompt for address
  string
  set_address()
  {
    string address;
    cout << "Address: ";
    cin.ignore();
    getline(cin, address);
    return address;
  }
  
  // function to prompt for phone number
  string
  set_phone_number()
  {
    string p_num;
    cout << "Telephone number: ";
    getline(cin, p_num);
    return p_num;
  }
  
  // function for inserting a new entry
  void
  insert_new_address()
  {
    Node* n = new Node();
    // get the contact full name
    pair <string, string> name = set_name();
    n->first_name = name.first;
    n->last_name = name.second;
    
    //get the contact address
    string address = set_address();
    n->address = address;
    
    //get the contact phone number
    string phone_number = set_phone_number();
    n->phone_num = phone_number;
    
    //add completed contact info into the vector
    rolodex_info.push_back(n);
  }
  
  //function to find address info based on last name
  void
  find_address()
  {
    //if rolodex is empty then return
    if (rolodex_info.size() == 0)
    {
      cout << "Rolodex is currently empty." << endl;
      return;
    }
    string last_name;
    cout << "Last name: ";
    cin >> last_name;
    bool found = false;
    //traverse through rolodex and see if there is a match
    for (unsigned i = 0; i < rolodex_info.size(); i++)
    {
      if(rolodex_info[i] && rolodex_info[i]->last_name == last_name)
      {
          print_entry(rolodex_info[i]);
          found = true;
      }
    }
    if (!found)
    {
      cout << "Cannot find entry!" << endl;
    }
  }
  
  // function to print all entries in rolodex
  void
  print_all_entries()
  {
    //if rolodex is empty, print nothing
    if (rolodex_info.size() == 0)
    {
      cout << "Rolodex is currently empty." << endl;
      return;
    }
    
    for (unsigned i = 0; i < rolodex_info.size(); i++)
    {
      if (rolodex_info[i])
      {
        print_entry(rolodex_info[i]);
        cout << endl;
      }
    }
  }
  
  //delete an entry given last name
  void
  erase_entry()
  {
    // if rolodex is empty return
    if (rolodex_info.size() == 0)
    {
      cout << "Rolodex is currently empty." << endl;
      return;
    }
    
    bool found = false;
    string last_name;
    cout << "Last name: ";
    cin >> last_name;
    
    for (unsigned i = 0; i < rolodex_info.size(); i++)
    {
      if (rolodex_info[i] && rolodex_info[i]->last_name == last_name)
      {
        delete rolodex_info[i];
        rolodex_info[i] = NULL;
        found = true;
      }
    }
    if (found)
    {
      cout << "Successfully deleted contact to " << last_name << endl;
    }
    else
    {
      cout << "Cannot find entry to " << last_name << endl;
    }
  }
  
  // function to format the text for new text file
  string
  convert_to_formatted_text()
  {
    stringstream text_file;
    for (unsigned i = 0; i < rolodex_info.size(); i++)
    {
      if (rolodex_info[i])
      {
        text_file << "=========================================\n" << "First name: " 
         << (rolodex_info[i])->first_name << "\nLast name: "<< (rolodex_info[i])->last_name << "\nAddress: " 
         << (rolodex_info[i])->address << "\nContact number: " << (rolodex_info[i])->phone_num
         << "\n=========================================\n";
      }
    }
    return text_file.str();
  }
  // function to save rolodex contents into a file
  void
  save_rolodex_to_file (const string & file_name)
  {
    ofstream output_text (file_name.c_str());
    if (output_text)
    {
      output_text << convert_to_formatted_text();
      output_text.close();
      cout << file_name << " is saved!" << endl;
    }
    else
    {
      cout << "\n" << file_name << "was not saved!" << endl;
    }
  }
  
  //function to load addresses from a text file into the rolodex
  void
  load_from_text_into_rolodex()
  {
    string input_file;
    string first_name;
    string last_name;
    string address;
    string number;
    
    cout << "File to import: ";
    cin >> input_file;
    ifstream input(input_file.c_str());
    
    if (!input)
    {
      cout << "Cannot find file or there were issues opening the file!" << endl;
      return;
    }
    while (!input.eof())
    {
      Node* n = new Node();
      getline(input, first_name);
      remove_carriage(first_name);
      n->first_name = first_name;
      
      getline(input, last_name);
      remove_carriage(last_name);
      n->last_name = last_name;
      
      getline(input, address);
      remove_carriage(address);
      n->address = address;
      
      getline(input, number);
      remove_carriage(number);
      n->phone_num = number;
     
      rolodex_info.push_back(n); // add address information into rolodex
    }
      rolodex_info.pop_back(); //accounts for one over
    cout << "Successfully, imported contact from " << input_file << endl;
  }
};
// end of rolodex class

// function to display the menu
void
display_menu()
{
  cout << "+--------------------------------------------------------+"
       << "\n|   Enter one of the following to perform an action:     |\n"
       << "|   'i' - insert a new address                           |\n"
       << "|   'f' - find a given address                           |\n"
       << "|   'd' - delete a given address                         |\n"
       << "|   'p' - print all cards in the rolodex                 |\n"
       << "|   'l' - load address from a given rolodex text file    |\n"
       << "|   's' - save the addresses to a specified text file    |\n"
       << "|   'q' - quit the program                               |\n"
       << "+--------------------------------------------------------+"
       << "\nChoose an option: ";
}

//function that handles user's options
bool
perform_action (const char & choice, Rolodex & rolodex)
{
  switch(choice)
  {
    case 'i': //insert entry
      rolodex.insert_new_address();
      return true;
      break;
    case 'f': //find address
      rolodex.find_address();
      return true;
      break;
    case 'd': //delete address
      rolodex.erase_entry();
      return true;
      break;
    case 'p': //print all info
      rolodex.print_all_entries();
      return true;
      break;
    case 'l': // load addresses from file
      rolodex.load_from_text_into_rolodex();
      return true;
      break;
    case 's': // save address to file
      /*
        Curly braces here are necessary because switch case is essentially a "goto" statement.
      You are not allowed to goto a location in a block following the construction point of a local
      variable. Similarly, we cannot do so for a switch either. 
        The variable is still in scope for case 'q' and default. So at case 'q' or default, it would
      bring the variable into scope even though it has not been constructed. Using the curly braces
      we limit the variable scope to just case 's'.
      */
      {
        string file;
      
        cout << "Enter new file name: ";
        cin >> file;
        rolodex.save_rolodex_to_file(file);
      }
      return true;
      break;
    case 'q': //quit
      return false;
      break;
    default: //invalid option
      cout << "\nNot a valid option! Please pick another option." << endl;
      return true;
      break;
  }
}

int main()
{
  Rolodex rolodex; //initialize rolodex
  char user_choice;
  bool keep_running = true;
  while(keep_running)
  {
    display_menu();
    cin >> user_choice; //ask user for an option
    keep_running = perform_action(user_choice, rolodex);
  }
  // when we quit the rolodex, it would be nice for us to have a file containing all
  // the info in the current rolodex
  rolodex.save_rolodex_to_file("rolodex_info_storage.txt");
  
  cout << "\nGood bye!" << endl;
  return 0;
}
