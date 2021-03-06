#ifndef DB_INTERSECTION_HPP
#define DB_INTERSECTION_HPP

#include <pqxx/pqxx>
#include <vector>
#include <iostream>
#include "../json_converter.hpp"

// функция регистрации пользователя по паролю
// по username и password_hash
// врзвращает новый id.
int add_user(pqxx::connection &c, std::string &username, std::string &password_hash);

// функция проверки регистрации по паролю
// получает имя пользователя и хеш пароля,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_pasword(pqxx::connection &c, std::string &username, std::string &password_hash);

// функция проверки регистрации по ключу
// получает имя пользователя и хеш ключа,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_session_key(pqxx::connection &c, std::string &username, std::string &key_hash);

// функция установки нового ключа авторизации пользователю с данным user_id
bool set_new_autorised_key(pqxx::connection &c, int user_id, std::string &new_tocken);

// функцию создания чата.
// принимает id пользователя-администратора и название нового чата.
// возвращает chat_id или 0.
int add_chat(pqxx::connection &c, int admin_user_id, std::string &chat_title);

// функция получения chat_id по user_id и названию
// получает id пользователя и название группы,
// возвращает chat_id или 0 в случае ошибки
int get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, std::string &chat_title);

// функция отправки сообщения по user_id и chat_id
// получает id пользователя и id группы,
// НЕ проверяет принадлежность к группе(это проверяется при получении chat_id)
void set_message_by_user_id_chat_id(pqxx::connection &c, int user_id, int chat_id, std::string &text);

// функция разрешения имени пользователя в user_id, без проверки пароля.
int get_user_id_by_login(pqxx::connection &c, std::string &username);

// функция добавления пользователя в чат
// принимает id добавляемого пользователя и id чата.
// не осуществляет проверок авторизации.
int add_user_to_chat(pqxx::connection &c, int user_id, int chat_id);

// функция получения всех новых сообщений,
// начиная с переданного времени(Unix time - секунды с 00:00 01.01.1970).
std::vector<MessageGetStruct> get_from_chats_by_user_id_time(pqxx::connection &c, int user_id, int unix_epoch);


#endif //DB_INTERSECTION_HPP
