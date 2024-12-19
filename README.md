# Webserv

Webserv is a simple web server implementation project. It aims to provide a functional web server with various features and configurations.

## Features

The Webserv project includes the following features:

1. **Hostnames**: Setup multiple servers with different hostnames.
2. **Error Pages**: Configure custom error pages for different HTTP status codes.
3. **Client Body Size Limit**: Limit the size of the client request body.
4. **CGI Support**: Execute CGI scripts with support for different interpreters.
5. **Request Methods**: Support for GET, POST, and DELETE requests.
6. **Redirections**: Configure URL redirections.
7. **Directory Listing**: Enable or disable directory listing.

## Configuration

The server configuration is done through `.conf` files. You will find example files in the `conf/` folder.

## Prerequisites

Before you can compile and run the Webserv project, ensure you have the following installed on your system:

1. **C++ Compiler**
2. **Make**

## Compilation

To compile the Webserv project, follow these steps:

1. Run the `make` command to compile the project:
    ```sh
    make
    ```

    This will compile the source files and generate the `webserv` executable.

2. To clean the object files generated during compilation, run:
    ```sh
    make clean
    ```

3. To remove the executable and object files, run:
    ```sh
    make fclean
    ```

4. To recompile the project from scratch, run:
    ```sh
    make re
    ```

5. To run the server with the default configuration, use:
    ```sh
    ./webserv conf/server.conf
    ```

## Usage

Once the server is running, you can access it through various methods:

1. **Web Browser**: Open your preferred web browser and navigate to `http://localhost:8080` (or the port specified in your configuration file, this is only meant to serve as an example).

2. **cURL**: Use the cURL command-line tool to interact with the server. For example:
    ```sh
    curl http://localhost:8080
    ```

3. **Postman**: Use Postman to send requests to the server. Set the request URL to `http://localhost:8080` and configure the request method, headers, and body as needed.
