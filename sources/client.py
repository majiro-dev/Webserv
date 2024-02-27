import socket
import threading
import time

def client_function(client_id):
    # Crear un socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Permitir reutilización de dirección
    client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    # Conectar al servidor
    server_address = ('localhost', 8080)  # Ajusta la dirección y puerto según tu configuración
    client_socket.connect(server_address)
    
    # Enviar una solicitud al servidor
    message = f"Cliente {client_id}: Hola desde el cliente {client_id}!"
    client_socket.sendall(message.encode('utf-8'))
    
    # Recibir la respuesta del servidor
    data = client_socket.recv(1024)
    print(f"Respuesta del servidor para Cliente {client_id}: {data.decode('utf-8')}")

    # Cerrar la conexión
    client_socket.close()

# Número de clientes simulados
num_clients = 1

# Crear y ejecutar threads para simular múltiples clientes
threads = []
for i in range(num_clients):
    thread = threading.Thread(target=client_function, args=(i+1,))
    threads.append(thread)
    thread.start()
    
    # Esperar un breve tiempo antes de crear el siguiente cliente
    time.sleep(0.1)

# Esperar a que todos los threads terminen
for thread in threads:
    thread.join()

print("Todos los clientes han finalizado.")
