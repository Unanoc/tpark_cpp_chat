#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>

#define SERVER_BACKLOG 128
#define BUF_LEN 34



const char RESPONCE[BUF_LEN] = "<h1>Hello, Pigeon Post</h1><br/>";
const char *SERVER_NAME = "Simple HTTP Server";


void on_request(struct evhttp_request *, void *);

int main(int argc, char **argv) {
	// ПРОВЕРКА НА КОЛИЧЕСТВО ПЕРЕДАННЫХ ПАРАМЕТРОВ
	if (argc < 3) {
		std::cout << "Start as:" << std::endl << argv[0] << " [host_address] [port]" << std::endl;
		return 1;
	}
	std::cout << "Проверка прошла"

	// СОЗДАНИЕ СОКЕТА
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == -1) {
		std::cout << "Error socket(): " << strerror(errno) << std::endl;
		return 1;
	}

	unsigned int port  = atol(argv[2]); // из строки в лонг инт
	const char * host = argv[1];

	sockaddr_in  sa;
	int on = 1;
	sa.sin_family = AF_INET; // семейство адресов
	// При указании IP-адреса и номера порта необходимо преобразовать число из порядка хоста в сетевой (Host TO Network Short)
	sa.sin_port = htons(port); // номер порта
	sa.sin_addr.s_addr = inet_addr(host); // sin_addr - это структура in_addr. inet_addr -  конвертирует строку хоста ipv4 в подходящий адрес для структуры in_addr

	// setsockopt - устанавливает опции сокета
	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
		std::cout << "Error setsockopt(): " << strerror(errno) << std::endl;
		return 1;
	}

	// Bind - связывает сокет с некоторым адресом (ip:port)
	if (bind(server_sock, (const sockaddr*)&sa, sizeof(sa)) == -1) {
		std::cout << "Error bind(): " << strerror(errno) << " on: " << host << ":" << port << std::endl;
		return 1;
	}

	// listen - создаётся очередь запросов на соединение. При этом сокет переводится в режим ожидания запросов со стороны клиентов.
	if (listen(server_sock, SERVER_BACKLOG) == -1) {
		std::cout << "Error listen(): " << strerror(errno) << std::endl;
		return 1;
	}

	// Инициализация базы событийного движка
	event_base *serv_base = (event_base *)event_init();
	// Инициализация http сервера
	evhttp *http_server = evhttp_new(serv_base);

	// Ignore SIGPIPE (рекомендовано)
	signal(SIGPIPE, SIG_IGN);

	if (evhttp_accept_socket(http_server, server_sock) == -1) { // указываем, на каком сокете слушать подключения:
		std::cout << "Error evhttp_accept_socket(): " << strerror(errno) << std::endl;
		return 1;
	}


	/*
		//TODO добавить обработчики на каждый /url
	*/


	// Выставление обработчика на остальные запросы:                             
	evhttp_set_gencb(http_server, on_request, NULL);
	// Запуск цикла обработки запросов
	event_base_dispatch(serv_base);

	return 0;
}

void on_request(struct evhttp_request * req, void * arg) {
	// создаем буфер для ответа - это непрозрачный тип данных для эффективной буферизации данных, которые должны быть отправлены или получены в сети
	struct evbuffer *evb = evbuffer_new(); // выделяем память для этого буффера
	if (!evb) { 
		return; 
	}

	// добавляем текст(форматированную строку) в конец буффера
	evbuffer_add_printf(evb, "<html><head><title>%s Page</title></head><body>\n", SERVER_NAME);
	// добавляем данные в конец буфера
	evbuffer_add(evb, RESPONCE, BUF_LEN);

	evbuffer_add_printf(evb, "Your request is <B>%s</B> from <b>%s</b>.<br/>Your user agent is '%s'\n", 
					req->uri, req->remote_host, evhttp_find_header(req->input_headers, "User-Agent"));

	evbuffer_add_printf(evb, "</body></html>");

	// Set HTTP headers
	evhttp_add_header(req->output_headers, "Server", SERVER_NAME);
	evhttp_add_header(req->output_headers, "Connection", "close");

	// Send reply
	evhttp_send_reply(req, HTTP_OK, "OK", evb);

	// Free memory
	evbuffer_free(evb);
}