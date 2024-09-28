import java.net.*;
import java.io.*;

public class Cliente_puente32 {
    public static void main(String[] args) throws IOException {
        if (args.length != 2) {
            System.err.println("Uso desde consola: <dirección IP> <número de puerto>");
            System.exit(1);
        }

        String direccionServidor = args[0];
        int numeroPuerto = Integer.parseInt(args[1]);

        try (
            Socket socketdelCliente = new Socket(direccionServidor, numeroPuerto);
            DataOutputStream salida = new DataOutputStream(socketdelCliente.getOutputStream());
            DataInputStream entrada = new DataInputStream(socketdelCliente.getInputStream());
            BufferedReader entradaTeclado = new BufferedReader(new InputStreamReader(System.in));
        ) {
            System.out.println("Conectado al servidor en " + direccionServidor + " en el puerto " + numeroPuerto);

            int numero;
            int respuesta;

            while (true) {
                // Leer un número desde el teclado
                System.out.print("Escribe un número entero (0 para terminar): ");
                numero = Integer.parseInt(entradaTeclado.readLine());

                // Enviar el número al servidor
                salida.writeInt(numero);

                // Terminar si el número es 0
                if (numero == 0) {
                    System.out.println("Desconectando...");
                    break;
                }

                // Recibir el número incrementado del servidor
                respuesta = entrada.readInt();
                System.out.println("El servidor respondió: " + respuesta);
            }
        } catch (IOException e) {
            System.err.println("Ocurrió un error en la conexión: " + e.getMessage());
        }
    }
}
