
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // Biblioteca de sockets para Windows
#include <ws2tcpip.h> // Para funciones adicionales como inet_pton

#pragma comment(lib, "Ws2_32.lib") // Vincula Winsock

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso desde consola: %s <dirección IP> <número de puerto>\n", argv[0]);
        exit(1);
    }

    char *direccionServidor = argv[1]; // Dirección IP del servidor
    int numeroPuerto = atoi(argv[2]); // Número de puerto

    // Inicializar Winsock
    WSADATA wsaData;
    int resultado = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (resultado != 0) {
        printf("Error al inicializar Winsock: %d\n", resultado);
        return 1;
    }

    // Crear el socket
    SOCKET socketdelCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketdelCliente == INVALID_SOCKET) {
        printf("Error al crear el socket: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in direccionServidorStruct;
    direccionServidorStruct.sin_family = AF_INET;
    direccionServidorStruct.sin_port = htons(numeroPuerto);
    inet_pton(AF_INET, direccionServidor, &direccionServidorStruct.sin_addr);

    // Conectar al servidor
    if (connect(socketdelCliente, (struct sockaddr *)&direccionServidorStruct, sizeof(direccionServidorStruct)) == SOCKET_ERROR) {
        printf("Error al conectar con el servidor: %d\n", WSAGetLastError());
        closesocket(socketdelCliente);
        WSACleanup();
        return 1;
    }

    printf("Conectado al servidor\n");

    char mensaje[256];
    char respuestaServidor[256];

    while (1) {
        // Leer mensaje desde el teclado
        printf("Escribe un mensaje para el servidor: ");
        fgets(mensaje, sizeof(mensaje), stdin);
        mensaje[strcspn(mensaje, "\n")] = 0; // Eliminar el salto de línea del teclado

        // Añadir un salto de línea al mensaje antes de enviarlo
        strcat(mensaje, "\n");

        // Enviar el mensaje al servidor
        send(socketdelCliente, mensaje, strlen(mensaje), 0);


        // Recibir la respuesta del servidor
        int bytesRecibidos = recv(socketdelCliente, respuestaServidor, sizeof(respuestaServidor) - 1, 0);
        if (bytesRecibidos > 0) {
            respuestaServidor[bytesRecibidos] = '\0'; // Asegurar el final de la cadena
            printf("Servidor dice: %s\n", respuestaServidor);
        }

        // Si se envía "Adiós", se cierra la conexión
        if (strcmp(mensaje, "Adiós") == 0) {
            printf("Desconectando...\n");
            break;
        }
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(socketdelCliente);
    WSACleanup();

    return 0;
}
