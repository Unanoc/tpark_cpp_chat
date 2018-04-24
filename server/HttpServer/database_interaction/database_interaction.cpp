#include <iostream>
#include <pqxx/pqxx>
#include <vector>

// функция регистрации по паролю
// получает имя пользователя и хеш пароля,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_pasword(pqxx::connection &c, std::string username, std::string password_hash){
    try {
        pqxx::work w(c); // Start a transaction.
        pqxx::result r = w.exec(
            "select id "
            "from Users "
            "where "
                "Users.username = '"+w.esc(username)+"' and "
                "Users.password_hash = '"+w.esc(password_hash)+"';"
        );
        w.commit();
        if (r.empty()){
            return 0;
        } else {
            return r[0][0].as<int>();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

// функция регистрации по ключу
// получает имя пользователя и хеш ключа,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_session_key(pqxx::connection &c, std::string username, std::string key_hash){
    try {
        pqxx::work w(c); // Start a transaction.
        pqxx::result r = w.exec(
            "select "
                "users.id "
            "from "
                "users, sessions "
            "where "
                "users.username = '"+w.esc(username)+"' and "
                "sessions.user_id = users.id and "
                "sessions.session_key = '"+w.esc(key_hash)+"';"
        );
        w.commit();
        if (r.empty()){
            return 0;
        } else {
            return r[0][0].as<int>();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

// функция получения chat_id по user_id и названию
// получает id пользователя и название группы,
// возвращает chat_id или 0 в случае ошибки
int get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, std::string chat_title){
    try {
        pqxx::work w(c); // Start a transaction.
        pqxx::result r = w.exec(
            "select "
                "chats.id "
            "from "
                "chats, chat_user "
            "where "
                "chats.title = '"+w.esc(chat_title)+"' and "
                "chat_user.user_id = "+std::to_string(user_id)+" and "
                "chat_user.chat_id = chats.id;"
        );
        w.commit();
        if (r.empty()){
            return 0;
        } else {
            return r[0][0].as<int>();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}

// функция отправки сообщения по user_id и chat_id
// получает id пользователя и id группы,
// НЕ проверяет принадлежность к группе(это проверяется при получении chat_id)
void get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, int chat_id, std::string &text){
    try {
        pqxx::work w(c); // Start a transaction.
        pqxx::result r = w.exec(
            "insert into Messages ("
                "sender_id, chat_id, send_time, text"
            ") values ("+
                std::to_string(user_id)+","+std::to_string(chat_id)+", now(), '"+w.esc(text)+"');"
        );
        w.commit();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}


std::vector<Message> get_from_chat_by_time(pqxx::connection &c, int user_id, int unix_epoch) {
    try {
        pqxx::work w(c); // Start a transaction.
        pqxx::result r = w.exec(
        "select "
            "Users.username as sender_username, "
            "cast(extract(epoch from Messages.send_time) as integer) as send_time, "
            "Messages.text as message_text "
        "from "
            "Messages, Users, Chats, Chat_User "
        "where "
            "Chat_User.user_id = " + std::to_string(user_id) + " and "
            "Chat_User.chat_id = Messages.chat_id and "
            "Users.id = user_id and "
            "Messages.send_time > ("
                "select timestamp 'epoch' + " + std::to_string(unix_epoch) + " * INTERVAL '1 second'"
            ");"
        );
        w.commit();
        std::vector <Message>result;
        const int num_rows = r.size();
        for (int rownum = 0; rownum < num_rows; ++rownum) {
            const pqxx::row row = r[rownum];
            Message m;
            m.username = row[0].as<std::string>();
            m.send_timestamp = row[1].as<long int>();
            m.text = row[2].as<std::string>();
            result.push_back(m);
        }
        return result;
    } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            throw;
    }
}
