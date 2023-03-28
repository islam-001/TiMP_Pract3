#include <iostream>
#include <cstdlib>  // exit()-для закрытия программы
#include <cstring>  // strcpy()-для копирования строки, strlen()-для определения длины строки
#include <unistd.h> // close()-для закрытия сокета
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

void errHandler(const char * why, const int exitCode = 1) {
    cerr << why << endl;
    exit(exitCode);
}

int main() {
    // структура с адресом(клиент)
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET; 
    selfAddr->sin_port = 0;
    selfAddr->sin_addr.s_addr = 0;
    
    // структура с адресом(сервер)
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(7);
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12");
    
    // Буфер обмена для передачи и приема данных
    char *buf = new char [256];
    strcpy(buf, "Вингардиум Левиоса\n"); // Копируем строку
    int msgLen = strlen(buf);                                    // Вычисляем длину строки
    
    // Создание сокета
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket == -1) {
        errHandler("Error open socket", 11);
    }
    
    // Связывание сокета с адресом
    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error bind socket local address", 12);
    }
    
    // Установка соединения
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error connect socket with remote server", 13);
    }
    
    // Передача данных
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1){
        close(mySocket);
        errHandler("Error send message", 14);
    }
    cout << "Мы сказали: " << buf << endl;
    
    // Прием данных
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Error recevie answer", 15);
    }
    buf[rc] = '\0';
    cout << "Нам ответили: " << buf << endl;
    
    
    // Закрытие сокета
    close(mySocket);
    
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}