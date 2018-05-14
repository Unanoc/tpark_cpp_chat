#include "database_interaction.hpp"

// функция регистрации пользователя
// по username и password_hash
// врзвращает новый id.
int add_user(pqxx::connection &c, std::string &username, std::string &password_hash){
    pqxx::work w(c); // Start a transaction.
    try {
        pqxx::row r = w.exec1(
            "insert into Users ("
                "username, "
                "password_hash"
            ") values ("
                "'" + w.esc(username) + "', "
                "'" + w.esc(password_hash) + "'"
            ") returning id;"
        );
        w.commit();
        return r[0].as<int>();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

// функция проверки регистрации по паролю
// получает имя пользователя и хеш пароля,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_pasword(pqxx::connection &c, std::string &username, std::string &password_hash){
    pqxx::work w(c);
    try {
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
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

// функция проверки регистрации по ключу
// получает имя пользователя и хеш ключа,
// возвращает user_id или 0 в случае ошибки
int get_user_id_by_login_session_key(pqxx::connection &c, std::string &username, std::string &key_hash){
    pqxx::work w(c);
    try {
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
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

// функция установки нового ключа авторизации пользователю с данным user_id
bool set_new_autorised_key(pqxx::connection &c, int user_id, std::string &new_tocken) {
    pqxx::work w(c);
    try {
        pqxx::result r = w.exec(
            "insert into Sessions ("
                "user_id, "
                "session_key, "
                "entry_time"
            ") values ("
                + std::to_string(user_id) +", "
                "'" + w.esc(new_tocken) + "', "
                "now()"
            ");"
        );
        w.commit();
        return true;
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return false;
    }
}

// функция создания чата. Создавший становится админом.
// в случае конфликта имён чатов(они уникальны) возвращает 0
int add_chat(pqxx::connection &c, int admin_user_id, std::string &chat_title) {
    pqxx::work w(c);
    try {
        pqxx::row r = w.exec1(
            "with tmp as ("
                "insert into Chats("
                    "title, "
                    "admin_id"
                ") values ("
                    "'"+ w.esc(chat_title) + "', "
                    + std::to_string(admin_user_id) +
                ") returning id"
            ") insert into Chat_User("
                "user_id, "
                "chat_id"
            ") values ("
                + std::to_string(admin_user_id) + ", "
                "(select id from tmp)"
            ") returning chat_id;"
        );
        w.commit();
        return r[0].as<int>();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}

// функция получения chat_id по user_id и названию
// получает id пользователя и название группы,
// возвращает chat_id или 0 в случае ошибки
int get_chat_id_by_user_id_chat_title(pqxx::connection &c, int user_id, std::string &chat_title){
    pqxx::work w(c);
    try {
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
        return r[0][0].as<int>();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}


// функция отправки сообщения по user_id и chat_id
// получает id пользователя и id группы,
// НЕ проверяет принадлежность к группе(это проверяется при получении chat_id)
void set_message_by_user_id_chat_id(pqxx::connection &c, int user_id, int chat_id, std::string &text){
    pqxx::work w(c); // Start a transaction.
    try {
        pqxx::result r = w.exec(
            "insert into Messages ("
                "sender_id, chat_id, send_time, text"
            ") values ("+
                std::to_string(user_id)+","+std::to_string(chat_id)+", now(), '"+w.esc(text)+"');"
        );
        w.commit();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
    }
}


std::vector<MessageGetStruct> get_from_chats_by_user_id_time(pqxx::connection &c, int user_id, int unix_epoch) {
    pqxx::work w(c);
    try {
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
            "Users.id = Messages.sender_id and "
            "Messages.send_time > ("
                "select timestamp 'epoch' + " + std::to_string(unix_epoch) + " * INTERVAL '1 second'"
            ");"
        );
        w.commit();
        std::vector <MessageGetStruct>result;
        const int num_rows = r.size();
        for (int rownum = 0; rownum < num_rows; ++rownum) {
            const pqxx::row row = r[rownum];
            MessageGetStruct m;
            m.username = row[0].as<std::string>();
            m.send_timestamp = row[1].as<long int>();
            m.text = row[2].as<std::string>();
            result.push_back(m);
        }
        return result;
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return std::vector<MessageGetStruct>();
    }
}

// функция, которая просто разрещает имя в id без проверки пароля, в случае неудачи возвращает 0.
int get_user_id_by_login(pqxx::connection &c, std::string &username){
    pqxx::work w(c);
    try {
        pqxx::row r = w.exec1(
            "select id from Users where username = '" + w.esc(username) + "';"
        );
        w.commit();
        return r[0].as<int>();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}


// функция добавления пользователя в чат, не проверяет никаких прав.
int add_user_to_chat(pqxx::connection &c, int user_id, int chat_id){
    pqxx::work w(c);
    try {
        pqxx::row r = w.exec1(
            "insert into Chat_User ("
                "user_id, chat_id"
            ") values ("
                + std::to_string(user_id)+", "
                + std::to_string(chat_id)+
            ") returning id;"
        );
        w.commit();
        return r[0].as<int>();
    } catch (const std::exception &e) {
        w.abort();
        std::cerr << e.what() << std::endl;
        return 0;
    }
}
