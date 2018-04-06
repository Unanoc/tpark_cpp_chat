-- Аккаунт пользователя
create table Users(
  id serial primary key,
  username text not null unique,
  password_hash text not null
);

-- Чат, в который посылаются сообщения.
-- Количество пользователей от одного и не ограничено сверху.
-- Хотел бы я сделать шардирование, и ключом выбрать Chats.id, но не знаю, сколько времени это займёт.
create table Chats(
  id serial primary key,
  title text not null unique,
  admin_id serial not null REFERENCES Users (id)
);

-- Сессии пользователей. По одному ключу авторизации на устройство.
create table Sessions(
  id serial primary key,
  user_id integer not null references Users (id),
  session_key text not null,
  entry_time timestamp without time zone not null
);

-- Сообщения, самое важное.
create table Meseges(
  id serial primary key,
  sender_id integer not null references Users (id),
  chat_id integer not null references Chats (id),
  send_time timestamp without time zone not null,
  text text not null
);

-- Связь Chat`ов для каждого пользователя.
-- Служит для проверки прав пользователя читать/писать в этот чат.
create table Chat_User(
  id serial primary key,
  user_id integer references Users (id),
  chat_id integer not null references Chats (id)
);


-- Ненормализованная таблица с информацией для профиля пользователя.
create table User_counters(
  id serial primary key,
  user_id integer not null references Users (id),
  last_view timestamp without time zone not null
);

-- при создании нового пользователя автоматически создаются счётчики для него
CREATE FUNCTION create_user_counter() RETURNS trigger AS $$
    BEGIN
        insert into User_counters (user_id, last_view) VALUES (
          new.id,
          now()
        );
    END;
$$ language postgresql;

create trigger create_user_counters
  after insert on Users
  for each row
  execute procedure create_user_counter();

-- ненормализованная таблица с информацией о чате.
create table Chat_counters(
  id serial primary key,
  chat_id integer not null references Chats (id),
  number_of_users integer not null
);


-- основные запросы

-- зарегистрировать пользователя по username

-- присоединить пользователя по названию чата
-- исключить пользователя по username, из чата, выбранного по названию

-- Послать сообщение в чат
-- удалить сообщение по id
-- получить по id чата 10 последних или начиная с указанной даты.

-- создать чат и стат его админом
-- удалить чат, будучи админом.

-- положить пользователю новый ключ авторизации
-- удалить у пользователя все ключи авторизации

insert into Users (username, password_hash) values (
    'Admin',
    '12345'
) returning id;