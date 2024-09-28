
import java.net.*; 
import java.io.*; 

public class Servidor_punto3 {

    public static void main(String[] args) throws IOException {

        if (args.length != 1) {
            System.err.println("Uso desde consola:  <numero puerto>");
            System.exit(1);
        }

        int numeroPuerto = Integer.parseInt(args[0]);

        try (
            ServerSocket socketdelServidor = new ServerSocket(numeroPuerto);  // Escuchando peticiones en el puerto dado
            Socket socketdelCliente = socketdelServidor.accept();             // Acepta la conexión del cliente
            PrintWriter escritor = new PrintWriter(socketdelCliente.getOutputStream(), true); // Para enviar mensajes al cliente
            BufferedReader lector = new BufferedReader(new InputStreamReader(socketdelCliente.getInputStream())); // Para leer mensajes del cliente
            BufferedReader entradaTeclado = new BufferedReader(new InputStreamReader(System.in)); // Para leer desde el teclado
        ) {
            System.out.println("Cliente conectado");
            String mensajeRecibido;
            String mensaje;
            // Comunicación con el cliente
            while ((mensajeRecibido = lector.readLine()) != null) {
                // Imprime lo que recibe del cliente
                System.out.println("Cliente dice: " + mensajeRecibido);

                // Responde con un mensaje
                System.out.print("Escribe un mensaje para el cliente: ");
                mensaje = entradaTeclado.readLine();
                escritor.println(mensaje); // Enviar respuesta al cliente
            }

            System.out.println("Conexión cerrada por el cliente.");
        } catch (IOException e) {
            System.out.println("Ocurrió una excepción en el puerto " + numeroPuerto);
            System.out.println(e.getMessage());
        }
    }
}
