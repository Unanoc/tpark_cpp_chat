#ifndef DB_INTERSECTION_H
#define DB_INTERSECTION_H

#include "../json_converter.h"
// функция регистрации по паролю
// получает имя пользователя и хеш пароля,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_pasword(pqxx::connection &c, std::string &username, std::string &password_hash);

// функция регистрации по ключу
// получает имя пользователя и хеш ключа,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_session_key(pqxx::connection &c, std::string &username, std::string &key_hash);

// функция получения chat_id по user_id и названию
// получает id пользователя и название группы,
// возвращает chat_id или 0 в случае ошибки
int get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, std::string &chat_title);

// функция отправки сообщения по user_id и chat_id
// получает id пользователя и id группы,
// НЕ проверяет принадлежность к группе(это проверяется при получении chat_id)
void get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, int chat_id, std::string &text);

// функция получения всех новых сообщений,
// начиная с переданного времени(Unix time - секунды с 00:00 01.01.1970).
std::vector<MessageGet> get_from_chat_by_time(pqxx::connection &c, int user_id, int unix_epoch);


#endif 