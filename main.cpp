#include <iostream>
#include <string>
#include "database.h"
#define ANNOUNCEMENT_TEXT "command (list, add, get, del, exit): "

bool array2D_signal_1 = false;
bool array2D_signal_2 = false;
std::string command_Input;
std::string array_2value_type;
int array_small_size = 0;
int array_big_size = 0;
Database myDatabase;

Entry* create(Type type, std::string key, void* value) {
    Entry* newEntry = new Entry;
    newEntry->type = type;
    newEntry->key = key;

    if (type == Type::INT) 
    {
        int* intValue = new int;
        *intValue = *static_cast<int*>(value);
        newEntry->value = intValue;
    } 
    else if (type == Type::DOUBLE) 
    {
        double* doubleValue = new double;
        *doubleValue = *static_cast<double*>(value);
        newEntry->value = doubleValue;
    } 
    else if (type == Type::STRING) 
    {
        std::string* stringValue = new std::string;
        *stringValue = *static_cast<std::string*>(value);
        newEntry->value = stringValue;
    } 
     if (type == Type::ARRAY) 
     {
        Array* arrayValue = static_cast<Array*>(value);
        Array* newArrayValue = new Array;
        newArrayValue->size = arrayValue->size;
        newArrayValue->type = arrayValue->type;

        if (arrayValue->items != nullptr) 
        {
            int* items = static_cast<int*>(arrayValue->items);
            int* newArrayItems = new int[arrayValue->size];
            for (int i = 0; i < arrayValue->size; ++i) 
            {
                newArrayItems[i] = items[i];
            }
            newArrayValue->items = newArrayItems;
        } 
        else 
        {
            newArrayValue->items = nullptr;
        }

        newEntry->value = newArrayValue;
    } else {
        newEntry->value = value;
    }

    return newEntry;
}
void init(Database &database) {
  database.size = 0;
  destroy(database);
  
}
void list(Database &database) {
    for (int i = 0; i < database.size; i++) 
    {
        if (database.entry[i].type == Type::STRING) 
        {
            std::cout << database.entry[i].key << " : \"" << *(static_cast<std::string*>(database.entry[i].value)) << "\"" << std::endl;
        } 
        else if (database.entry[i].type == Type::DOUBLE) 
        {
            std::cout << database.entry[i].key << " : " << *(static_cast<double*>(database.entry[i].value)) << std::endl;
        } 
        else if (database.entry[i].type == Type::INT) 
        {
            std::cout << database.entry[i].key << " : " << *(static_cast<int*>(database.entry[i].value)) << std::endl;
        } 
        else if (database.entry[i].type == Type::ARRAY) 
        {
            Array* array = static_cast<Array*>(database.entry[i].value);
            std::cout << database.entry[i].key << " : [";
            if (array->items != nullptr) {
                int* items = static_cast<int*>(array->items);
                for (int j = 0; j < array->size; ++j) 
                {
                    std::cout << items[j];
                    if (j != array->size - 1) 
                    {
                        std::cout << ", ";
                    }
                }
            }
            std::cout << "]" << std::endl;
        }
    }
}
void add(Database &database, Entry *entry) {
    for (int i = 0; i < database.size; ++i) 
    {
        if (database.entry[i].key == entry->key) 
        {
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
      else if(database.entry[i].type == Type::ARRAY && (!(array2D_signal_1) || !(array2D_signal_2)))
      {
          int* array = static_cast<int*>(database.entry[i].value); //배열의 자료형에 따라 int형이 바뀌어야할 것 같음
          std::cout << key << ": [";
          for (int j = 0; j < array_small_size; ++j) {
              std::cout << array[j];
              if (j != array_small_size - 1) {
                  std::cout << ", ";
              }
          }
          std::cout << "]" << std::endl;
      }
      else if(database.entry[i].type == Type::ARRAY && (array2D_signal_1) && (array2D_signal_2)) // 2차원 배열일때
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
        if(database.entry[i].key == key) 
        {
            // 삭제된 항목 이후의 항목들을 하나씩 앞으로 이동
            for(int j = i; j < database.size - 1; ++j) 
            {
                database.entry[j] = database.entry[j + 1];
            }
            // 배열 크기 줄이기
            database.size--;
            return; // 함수 종료
        }
    }
}
void destroy(Database& database) {
    for (int i = 0; i < database.size; ++i) {
        if (database.entry[i].type == Type::ARRAY) //Array가 아닐때는 왜 구현이 안돼있지?
        {
            Array* array = static_cast<Array*>(database.entry[i].value);
            if (array->type == Type::INT) 
            {
                delete[] static_cast<int*>(array->items);
            } 
            else if (array->type == Type::DOUBLE) 
            {
                delete[] static_cast<double*>(array->items);
            } 
            else if (array->type == Type::STRING) 
            {
                delete[] static_cast<std::string*>(array->items);
            }
            delete array;
        }
    }
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
      else if (type_input == "array") 
      {
        std::cout << "size: ";
        int array_size;
        std::cin >> array_size;

        Array* arrayValue = new Array;
        arrayValue->size = array_size;
        arrayValue->type = Type::INT; // For simplicity, let's assume it's always int array

        int* items = new int[array_size];
        for (int i = 0; i < array_size; ++i) {
            std::cout << "item[" << i << "]: ";
            std::cin >> items[i];
        }
        arrayValue->items = items;

        add(myDatabase, create(Type::ARRAY, key_input, arrayValue));
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