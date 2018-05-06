-- Аккаунт пользователя
create table Users (
  id            serial primary key,
  username      text not null unique,
  password_hash text not null
);

-- Чат, в который посылаются сообщения.
-- Количество пользователей от одного и не ограничено сверху.
-- Хотел бы я сделать шардирование, и ключом выбрать Chats.id, но не знаю, сколько времени это займёт.
create table Chats (
  id       serial primary key,
  title    text not null unique,
  -- is_public boolean,
  admin_id serial not null REFERENCES Users (id)
);

-- Сессии пользователей. По одному ключу авторизации на устройство.
create table Sessions (
  id          serial primary key,
  user_id     integer not null references Users (id),
  session_key text not null,
  entry_time  timestamp without time zone not null
);

-- Сообщения, самое важное.
create table Messages (
  id        serial primary key,
  sender_id integer                     not null references Users (id),
  chat_id   integer                     not null references Chats (id),
  send_time timestamp without time zone not null,
  text      text                        not null
);

-- Связь Chat`ов для каждого пользователя.
-- Служит для проверки прав пользователя читать/писать в этот чат.
create table Chat_User (
  id      serial primary key,
  user_id integer references Users (id),
  chat_id integer not null references Chats (id)
);

-- Ненормализованная таблица с информацией для профиля пользователя.
create table User_counters (
  id        serial primary key,
  user_id   integer not null references Users (id),
  last_view timestamp without time zone not null
);

-- при создании нового пользователя автоматически создаются счётчики для него
create function create_user_counter()
  returns trigger as $$
begin
  insert into User_counters (user_id, last_view) VALUES (
    new.id,
    now()
  );
  return new;
end;
$$
language 'plpgsql';

create trigger create_user_counters
  after insert
  on Users
  for each row
execute procedure create_user_counter();

-- ненормализованная таблица с информацией о чате.
create table Chat_counters (
  id              serial primary key,
  chat_id         integer not null references Chats (id),
  number_of_users integer not null
);

-- при создании нового чата автоматически создаются счётчики для него
create function create_chat_counter()
  returns trigger as $$
begin
  insert into Chat_counters (chat_id, number_of_users) values (
    new.id,
    1 /*admin уже есть*/
  );
  return new;
end;$$
language 'plpgsql';

create trigger create_chat_counters
  after insert
  on Chats
  for each row
execute procedure create_chat_counter();

