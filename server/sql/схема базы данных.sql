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


-- служебная функция, проверяющая правильность логина / пароля.
-- возвращает либо id пользователя, либо NULL
create or replace function is_authenticated_by_password(username text, password_hash text)
  returns integer as $$
begin
  return (select id
          from Users
          where Users.username = $1 and Users.password_hash = $2);
end;
$$
language 'plpgsql';

-- служебная функция, проверяющая правильность логина / ключа авторизации.
-- возвращает либо id пользователя, либо NULL
create or replace function is_authenticated_by_key(username text, session_key text)
  returns integer as $$
declare
  verifiable_user_id integer;
begin
  verifiable_user_id := (select id
                         from Users
                         where Users.username = $1);
  case
    when (verifiable_user_id notnull) and
         (select count(*)
          from Sessions
          where Sessions.user_id = verifiable_user_id and
                Sessions.session_key = $2) > 0
    then
      return verifiable_user_id;
  else
    return null;
  end case;
end;
$$
language 'plpgsql';

-- основные запросы

-- Послать сообщение в чат
-- удалить сообщение по id
-- получить по id чата 10 последних или начиная с указанной даты.

-- положить пользователю новый ключ авторизации
-- удалить у пользователя все ключи авторизации

-- зарегистрировать пользователя по username
create function add_user(new_username text, password_hash text)
  returns boolean as $$
declare
  new_user_id integer;
begin
  insert into Users (username, password_hash) values (
    $1,
    $2
  ) on conflict do nothing returning id into new_user_id;
  return new_user_id notnull;
end;
$$ language 'plpgsql';

-- положить пользователю новый ключ авторизации, когда он приходит с логином и хешем пароля
create or replace function insert_new_session(username text, password_hash text, new_tocken text)
  returns boolean as $$
declare
  verifaed_user_id integer;
begin
  verifaed_user_id := is_authenticated_by_password($1, $2);
  if verifaed_user_id notnull
  then
    insert into Sessions (
      user_id,
      session_key,
      entry_time
    ) values (
      verifaed_user_id,
      $3,
      now()
    );
    return true;
  else
    return false;
  end if;
end;
$$
language 'plpgsql';

-- пример вызова
-- select insert_new_session('Admin', '123456', '1234567890');
-- возвращает успешность: true или false


-- удалить у пользователя, ds выбранного по нику и хешу пароля, все ключи авторизации
create or replace function delete_all_sessions(username text, password_hash text)
  returns boolean as $$
declare
  id_cleaned_user integer;
begin
  id_cleaned_user := is_authenticated_by_key($1, $2);
  if id_cleaned_user notnull
  then
    delete from Sessions
    where Sessions.user_id = id_cleaned_user;
    return true;
  else
    return false;
  end if;
end;$$
language 'plpgsql';
-- возвращает true, если пользователь с логином и паролем были в таблице, даже если у него небыло сессий.

-- создать чат и стать его админом.
create or replace function create_chat(username text, key text, chat_title text)
  returns boolean as $$
declare
  verified_user_id integer;
  new_chat_id      integer;
begin
  verified_user_id := is_authenticated_by_key($1, $2);
  if verified_user_id notnull
  then
    insert into Chats (title, admin_id)
    values ($3, verified_user_id)
    returning id
      into new_chat_id;
    insert into Chat_User (user_id, chat_id) values (verified_user_id, new_chat_id);
    return true;
  else
    return false;
  end if;
end;
$$
language 'plpgsql';

-- добавить кого-то в чат, будучи админом.
create or replace function add_user_to_chat_as_admin(
  admin_username text, admin_key text, chat_title text, added_username text
)
  returns boolean as $$
declare
  admin_user_id integer;
  added_user_id integer;
  added_chat_id integer;
begin
  admin_user_id := is_authenticated_by_key($1, $2);
  if admin_user_id notnull
  then
    added_user_id := (select id
                      from Users
                      where username = $4);
    if added_user_id notnull
    then
      added_chat_id := (select id
                        from Chats
                        where title = $3 and admin_id = admin_user_id);
      if added_chat_id notnull
      then
        insert into Chat_User (user_id, chat_id) values (added_user_id, added_chat_id);
        return true;
      end if;
    end if;
  end if;
  return false;
end;
$$
language 'plpgsql';



-- удалить пользователя, будучи админом.
-- удалить чат, будучи админом.

-- присоединить пользователя c именем к чату с именем

-- Послать сообщение в чат, учавствуя в чате
-- получить последние 10
-- получить все, начиная с указанного 
