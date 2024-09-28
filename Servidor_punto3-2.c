#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso desde consola: %s <número de puerto>\n", argv[0]);
        exit(1);
    }

    int numeroPuerto = atoi(argv[1]);

    // Inicializar Winsock
    WSADATA wsaData;
    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (resultado != 0) {
        printf("Error al inicializar Winsock: %d\n", resultado);
        return 1;
    }

    // Crear el socket del servidor
    SOCKET socketdelServidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketdelServidor == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in direccionServidor;
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(numeroPuerto);
    direccionServidor.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket
    if (bind(socketdelServidor, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) == SOCKET_ERROR) {
        printf("Error al enlazar el socket: %d\n", WSAGetLastError());
        closesocket(socketdelServidor);
        WSACleanup();
        return 1;
    }

    // Escuchar peticiones
    if (listen(socketdelServidor, 1) == SOCKET_ERROR) {
        printf("Error al escuchar: %d\n", WSAGetLastError());
        closesocket(socketdelServidor);
        WSACleanup();
        return 1;
    }

    printf("Esperando conexiOn en el puerto %d...\n", numeroPuerto);

    // Aceptar la conexión del cliente
    SOCKET socketdelCliente = accept(socketdelServidor, NULL, NULL);
    if (socketdelCliente == INVALID_SOCKET) {
        printf("Error al aceptar la conexión: %d\n", WSAGetLastError());
        closesocket(socketdelServidor);
        WSACleanup();
        return 1;
    }

    printf("Cliente conectado.\n");

    int numeroRecibido;
    int numeroIncrementado;

    while (1) {
        // Recibir un número del cliente
        int bytesRecibidos = recv(socketdelCliente, (char *)&numeroRecibido, sizeof(numeroRecibido), 0);
        if (bytesRecibidos == SOCKET_ERROR || bytesRecibidos == 0) {
            printf("Error al recibir datos o conexión cerrada.\n");
            break;
        }

        // Convertir el número recibido a entero
        numeroRecibido = ntohl(numeroRecibido); // Convertir de formato de red a formato de host
        printf("Cliente enviO: %d\n", numeroRecibido);

        // Terminar si el cliente envía un "0"
        if (numeroRecibido == 0) {
            printf("Cliente enviO 0, cerrando conexión...\n");
            break;
        }

        // Incrementar el número
        numeroIncrementado = numeroRecibido + 1;
        printf("Enviando al cliente: %d\n", numeroIncrementado);

        // Enviar el número incrementado al cliente
        numeroIncrementado = htonl(numeroIncrementado); // Convertir a formato de red
        send(socketdelCliente, (char *)&numeroIncrementado, sizeof(numeroIncrementado), 0);
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(socketdelCliente);
    closesocket(socketdelServidor);
    WSACleanup();

    return 0;
}
