#include <iostream>
#include <string>
#include "database.h"
#define ANNOUNCEMENT_TEXT "command (list, add, get, del, exit): "

bool array2D_signal_1 = false;
bool array2D_signal_2 = false;
std::string command_Input;
std::string array_2value_type;
int array_small_size;
int array_big_size;
Database myDatabase;

Entry* create(Type type, std::string key, void* value) {
    Entry* newEntry = new Entry;
    newEntry->type = type;
    newEntry->key = key;
    if (type == Type::INT) {
        int* intValue = new int;
        *intValue = *static_cast<int*>(value);
        newEntry->value = intValue;
    } else if (type == Type::DOUBLE) {
        double* doubleValue = new double;
        *doubleValue = *static_cast<double*>(value);
        newEntry->value = doubleValue;
    } else if (type == Type::STRING) {
        std::string* stringValue = new std::string;
        *stringValue = *static_cast<std::string*>(value);
        newEntry->value = stringValue;
    } else if (type == Type::ARRAY) {
        int* arrayValue = static_cast<int*>(value); 
        int arraySize = array_small_size; 
        int* newArrayValue = new int[arraySize]; 
        
        for (int i = 0; i < arraySize; ++i) {
            newArrayValue[i] = arrayValue[i];
        }

        newEntry->value = newArrayValue;
    }

    return newEntry;
}
void init(Database &database) {
  database.size = 0;
  
}
void list(Database &database) {
    for (int i = 0; i < database.size; i++) {
        if (database.entry[i].type == Type::STRING) {
            std::cout << database.entry[i].key << " : \"" << *(static_cast<std::string*>(database.entry[i].value)) << "\"" << std::endl;
        } else if (database.entry[i].type == Type::DOUBLE) {
            std::cout << database.entry[i].key << " : " << *(static_cast<double*>(database.entry[i].value)) << std::endl;
        } else if (database.entry[i].type == Type::INT) {
            std::cout << database.entry[i].key << " : " << *(static_cast<int*>(database.entry[i].value)) << std::endl;
        } else if (database.entry[i].type == Type::ARRAY) {
            std::cout << database.entry[i].key << " : [";
            if (database.entry[i].value != nullptr) {
                if (database.entry[i].type == Type::INT) {
                    int* arr = static_cast<int*>(database.entry[i].value);
                    for (int j = 0; j < array_small_size; ++j) {
                        std::cout << arr[j];
                        if (j != array_small_size - 1) {
                            std::cout << ", ";
                        }
                    }
                } else if (database.entry[i].type == Type::DOUBLE) {
                    double* arr = static_cast<double*>(database.entry[i].value);
                    for (int j = 0; j < array_small_size; ++j) {
                        std::cout << arr[j];
                        if (j != array_small_size - 1) {
                            std::cout << ", ";
                        }
                    }
                } else if (database.entry[i].type == Type::STRING) {
                    std::string* arr = static_cast<std::string*>(database.entry[i].value);
                    for (int j = 0; j < array_small_size; ++j) {
                        std::cout << "\"" << arr[j] << "\"";
                        if (j != array_small_size - 1) {
                            std::cout << ", ";
                        }
                    }
                }
            }
            std::cout << "]" << std::endl;
        }
    }
}


void add(Database &database, Entry *entry) {
    for (int i = 0; i < database.size; ++i) {
        if (database.entry[i].key == entry->key) {
            database.entry[i].type = entry->type;
            database.entry[i].value = entry->value;
            return;
        }
    }
    database.entry[database.size].key = entry->key;
    database.entry[database.size].type = entry->type;
    database.entry[database.size].value = entry->value;
    database.size++;
}
Entry *get(Database &database, std::string &key) {
  for(int i=0;i<database.size;i++)
  {
    if(database.entry[i].key == key)
    {
      if(database.entry[i].type == Type::STRING)
      {
        std::cout << key << ": " << "\"" << *(static_cast<std::string*>(database.entry[i].value)) << "\"" << std::endl;
      }
      else if(database.entry[i].type == Type::ARRAY && !(array2D_signal_1) && !(array2D_signal_2))
      {
          int* array = static_cast<int*>(database.entry[i].value);
          std::cout << key << ": [";
          for (int j = 0; j < array_small_size; ++j) {
              std::cout << array[j];
              if (j != array_small_size - 1) {
                  std::cout << ", ";
              }
          }
          std::cout << "]" << std::endl;
      }
      else if(database.entry[i].type == Type::ARRAY && (array2D_signal_1) && (array2D_signal_2))
      {
          int* array = static_cast<int*>(database.entry[i].value);
          std::cout << key << ": [";
          for (int j = 0; j < array_big_size; ++j) {
              std::cout << array[j];
              if (j != array_big_size - 1) {
                  std::cout << ", ";
              }
          }
          std::cout << "]" << std::endl;
      }
      else if(database.entry[i].type == Type::INT)
      {
        std::cout << key << ": " << *(static_cast<int*>(database.entry[i].value)) << std::endl;
      }
      else if(database.entry[i].type == Type::DOUBLE)
      {
        std::cout << key << ": " << *(static_cast<double*>(database.entry[i].value)) << std::endl;
      }
    }
  }
}
void remove(Database &database, std::string &key) {
    
    for(int i = 0; i < database.size; ++i) {
        if(database.entry[i].key == key) {
            // 삭제된 항목 이후의 항목들을 하나씩 앞으로 이동
            for(int j = i; j < database.size - 1; ++j) {
                database.entry[j] = database.entry[j + 1];
            }
            // 배열 크기 줄이기
            database.size--;
            return; // 함수 종료
        }
    }
}
void destroy(Database &database) {
  
  exit(0);
}

int main() {

  while(true)
  {
    std::cout << ANNOUNCEMENT_TEXT;
    std::cin >> command_Input;
    if(command_Input == "add")
    {
      std::string key_input;
      std::cout << "key: ";
      std::cin >> key_input;

      std::cout << "type (int, double, string, array): ";
      std::string type_input;
      std::cin >> type_input;
      if(type_input == "int")
      {
        std::cout << "value: ";
        int int_input;
        std::cin >> int_input;
        add(myDatabase,create(Type::INT,key_input,&int_input));
      }
      else if(type_input == "double")
      {
        std::cout << "value: ";
        double double_input;
        std::cin >> double_input;
        add(myDatabase,create(Type::DOUBLE,key_input,&double_input));
      }
      else if(type_input == "string")
      {
        std::cout << "value: ";
        std::string string_input;
        std::cin >> string_input;
        add(myDatabase,create(Type::STRING,key_input,&string_input));
      }
      else if(type_input == "array")
      {
        std::cout << "value: type(int, double, string, array): ";
        std::string array_value_type;
        std::cin >> array_value_type;
        if(array_value_type == "int")
        {
          std::cout << "size: ";
          int array_small_size = 0;
          int *Array_int_small = new int[array_small_size];
          std::cin >> array_small_size;
          for (int i = 0; i < array_small_size; ++i) 
          {
            std::cout << "item[" << i << "]: ";
            std::cin >> Array_int_small[i];
          }
          add(myDatabase,create(Type::ARRAY,key_input,Array_int_small));
          //배열 썼으니 초기화 시키자 
          for(int i=0;i<array_small_size;i++)
          {
            Array_int_small[i] = 0;
          }
        }
        else if(array_value_type == "double")
        {
          std::cout << "size: ";
          int array_small_size = 0;
          double *Array_double_small = new double[array_small_size];
          std::cin >> array_small_size;
          for (int i = 0; i < array_small_size; ++i) 
          {
            std::cout << "item[" << i << "]: ";
            std::cin >> Array_double_small[i];
          }
          add(myDatabase,create(Type::ARRAY,key_input,Array_double_small));
          //배열 썼으니 초기화 시키자 
          for(int i=0;i<array_small_size;i++)
          {
            Array_double_small[i] = 0;
          }
        }
        else if(array_value_type == "string")
        {
          std::cout << "size: ";
          int array_small_size = 0;
          std::string *Array_string_small = new std::string[array_small_size];
          std::cin >> array_small_size;
          for (int i = 0; i < array_small_size; ++i) 
          {
            std::cout << "item[" << i << "]: ";
            std::getline(std::cin, Array_string_small[i]);
          }
          add(myDatabase,create(Type::ARRAY,key_input,Array_string_small));   
          //배열 썼으니 초기화 시키자 
          for(int i=0;i<array_small_size;i++)
          {
            Array_string_small[i] = "";
          }   
        }
        else if(array_value_type == "array")
        {
          std::cout << "size: ";
          int array_big_size = 0;
          std::cin >> array_big_size; 
          for(int i=0;i<array_big_size;i++)
          {
            std::cout << "item[" << i << "]: ";
            std::cout << "type (int, double, string, array): ";
            std::string array_2value_type;
            std::cin >> array_2value_type;
            if(array_2value_type == "int")
            {
              std::cout << "size: ";
              int array_small_size = 0;
              int *Array_int_small = new int[array_small_size];
              std::cin >> array_small_size;
              for(int k=0;k<array_small_size;k++)
              {
                std::cout << "item[" << k << "]: ";
                std::cin >> Array_int_small[k];
              }
              add(myDatabase,create(Type::ARRAY,key_input,Array_int_small));
              //배열 썼으니 초기화 시키자 
              for(int k=0;k<array_small_size;k++)
              {
                Array_int_small[k] = 0;
              }
              array2D_signal_1 = true;
            }
            else if(array_2value_type == "double")
            {
              std::cout << "size: ";
              int array_small_size = 0;
              double *Array_double_small = new double[array_small_size];
              std::cin >> array_small_size;
              for(int k=0;k<array_small_size;k++)
              {
                std::cout << "item[" << k << "]: ";
                std::cin >> Array_double_small[k];
              }
              add(myDatabase,create(Type::ARRAY,key_input,Array_double_small));
              //배열 썼으니 초기화 시키자 
              for(int k=0;k<array_small_size;k++)
              {
                Array_double_small[k] = 0;
              }
              array2D_signal_1 = true;
            }
            else if(array_2value_type == "string")
            {
              std::cout << "size: ";
              int array_small_size = 0;
              std::string *Array_string_small = new std::string[array_small_size];
              std::cin >> array_small_size;
              for(int k=0;k<array_small_size;k++)
              {
                std::cout << "item[" << k << "]: ";
                std::cin >> Array_string_small[k];
              }
              add(myDatabase,create(Type::ARRAY,key_input,Array_string_small));
              //배열 썼으니 초기화 시키자 
              for(int k=0;k<array_small_size;k++)
              {
                Array_string_small[k] = "";
              }
              array2D_signal_1 = true;
            }
          }
          array2D_signal_2 = true;
        }
      }


      
    }
    else if(command_Input == "get")
    {
      std::cout << "key: ";
      std::string get_input;
      std::cin >> get_input;
      get(myDatabase,get_input);
    }
    else if(command_Input == "del")
    { 
      std::cout << "key: ";
      std::string del_key;
      std::cin >> del_key;
      remove(myDatabase,del_key);
    }
    else if(command_Input == "list")
    {
      list(myDatabase);
    }
    else if(command_Input == "exit")
    {
      destroy(myDatabase);
    }

  }

  return 0;
}